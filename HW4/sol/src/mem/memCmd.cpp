/****************************************************************************
  FileName     [ memCmd.cpp ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"

using namespace std;

extern MemTest mtest;  // defined in memTest.cpp

bool
initMemCmd()
{
   if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
         cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
         cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
         cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)
      )) {
      cerr << "Registering \"mem\" commands fails... exiting" << endl;
      return false;
   }
   return true;
}


//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------
CmdExecStatus
MTResetCmd::exec(const string& option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   if (token.size()) {
      int b;
      if (!myStr2Int(token, b) || b < toSizeT(sizeof(MemTestObj))) {
         cerr << "Illegal block size (" << token << ")!!" << endl;
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
      #ifdef MEM_MGR_H
      mtest.reset(toSizeT(b));
      #else
      mtest.reset();
      #endif // MEM_MGR_H
   }
   else
      mtest.reset();
   return CMD_EXEC_DONE;
}

void
MTResetCmd::usage(ostream& os) const
{  
   os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void
MTResetCmd::help() const
{  
   cout << setw(15) << left << "MTReset: " 
        << "(memory test) reset memory manager" << endl;
}


//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------
CmdExecStatus
MTNewCmd::exec(const string& option)
{
   // TODO
   // check option
   vector<string> options;
   if (!CmdExec::lexOptions(option, options))
      return CMD_EXEC_ERROR;
   bool doArray = false;
   int nObjs = -1, aSize;
   for (size_t i = 0, n = options.size(); i < n; ++i) {
      if (myStrNCmp("-Array", options[i], 2) == 0) {
         if (doArray) return CmdExec::errorOption(CMD_OPT_EXTRA,options[i]);
         if (++i == n)
            return CmdExec::errorOption(CMD_OPT_MISSING, options[i - 1]);
         if (!myStr2Int(options[i], aSize) || (aSize <= 0))
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[i]);
         doArray = true;
      }
      else {
         if (nObjs > 0)
            return CmdExec::errorOption(CMD_OPT_EXTRA,options[i]);
         if (!myStr2Int(options[i], nObjs) || (nObjs <= 0))
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[i]);
      }
   }
   if (nObjs <= 0)
      return CmdExec::errorOption(CMD_OPT_MISSING, "");

   try {
      if (doArray)
         mtest.newArrs(nObjs, aSize);
      else mtest.newObjs(nObjs);
   }
   catch (bad_alloc) {
      return CMD_EXEC_ERROR;
   }

   return CMD_EXEC_DONE;
}

void
MTNewCmd::usage(ostream& os) const
{  
   os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void
MTNewCmd::help() const
{  
   cout << setw(15) << left << "MTNew: " 
        << "(memory test) new objects" << endl;
}


//----------------------------------------------------------------------
//    MTDelete <-Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------
CmdExecStatus
MTDeleteCmd::exec(const string& option)
{
   // TODO
   // check option
   vector<string> options;
   if (!CmdExec::lexOptions(option, options))
      return CMD_EXEC_ERROR;
   bool doIdx = false, doRand = false, doArray = false;
   vector<int> objIds(1);
   int nRand;
   size_t objOpt = 0, randOpt = 0;
   for (size_t i = 0, n = options.size(); i < n; ++i) {
      if (myStrNCmp("-Array", options[i], 2) == 0) {
         if (doArray) return CmdExec::errorOption(CMD_OPT_EXTRA,options[i]);
         doArray = true;
      }
      else if (myStrNCmp("-Index", options[i], 2) == 0) {
         if (doIdx || doRand)
            return CmdExec::errorOption(CMD_OPT_EXTRA,options[i]);
         if (++i == n)
            return CmdExec::errorOption(CMD_OPT_MISSING, options[i - 1]);
         if (!myStr2Int(options[i], objIds[0]) || (objIds[0] < 0))
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[i]);
         objOpt = i;
         doIdx = true;
      }
      else if (myStrNCmp("-Random", options[i], 2) == 0) {
         if (doIdx || doRand)
            return CmdExec::errorOption(CMD_OPT_EXTRA,options[i]);
         randOpt = i;
         if (++i == n)
            return CmdExec::errorOption(CMD_OPT_MISSING, options[i - 1]);
         if (!myStr2Int(options[i], nRand) || (nRand <= 0))
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[i]);
         doRand = true;
      }
      else
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[i]);
   }

   int s = (doArray)? mtest.getArrListSize(): mtest.getObjListSize();
   if (doIdx) {
      if (objIds[0] >= s) {
         cerr << "Size of " << (doArray?"array":"object")
              << " list (" << s << ") is <= " << objIds[0] << "!!\n";
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[objOpt]);
      }
   }
   else if (doRand) {
      objIds.clear();
      if (s > 0)  // has something to delete
         for (int i = 0; i < nRand; ++i)
            objIds.push_back(rnGen(s));
      else {
         cerr << "Size of " << (doArray?"array":"object")
              << " list is 0!!" << endl;
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[randOpt]);
      }
   }
   else
      return CmdExec::errorOption(CMD_OPT_MISSING, "");

   if (doArray)
      for (size_t i = 0, n = objIds.size(); i < n; ++i)
         mtest.deleteArr(objIds[i]);
   else
      for (size_t i = 0, n = objIds.size(); i < n; ++i)
         mtest.deleteObj(objIds[i]);

   return CMD_EXEC_DONE;
}

void
MTDeleteCmd::usage(ostream& os) const
{  
   os << "Usage: MTDelete <-Index (size_t objId) | "
      << "-Random (size_t numRandId)> [-Array]" << endl;
}

void
MTDeleteCmd::help() const
{  
   cout << setw(15) << left << "MTDelete: " 
        << "(memory test) delete objects" << endl;
}


//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------
CmdExecStatus
MTPrintCmd::exec(const string& option)
{
   // check option
   if (option.size())
      return CmdExec::errorOption(CMD_OPT_EXTRA, option);
   mtest.print();

   return CMD_EXEC_DONE;
}

void
MTPrintCmd::usage(ostream& os) const
{  
   os << "Usage: MTPrint" << endl;
}

void
MTPrintCmd::help() const
{  
   cout << setw(15) << left << "MTPrint: " 
        << "(memory test) print memory manager info" << endl;
}


