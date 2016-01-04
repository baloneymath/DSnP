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
      if (!myStr2Int(token, b) || b < int(toSizeT(sizeof(MemTestObj)))) {
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
   vector<string> options;
   // check options
   if (!CmdExec::lexOptions(option, options, 0)) return CMD_EXEC_ERROR;
   if (options.size() == 0) return CmdExec::errorOption(CMD_OPT_MISSING, "");

   int num, ArrSize;
   bool doArr = false;

   for (size_t i = 0; i < options.size(); ++i)
     if (myStrNCmp("-Array", options[i], 2) == 0) doArr = true;
   // case do array
   if (doArr) {
     if (options.size() == 1) return CmdExec::errorOption(CMD_OPT_MISSING, options[0]);
     else if (options.size() == 2) {
       if (!myStr2Int(options[0], num)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
       else return CmdExec::errorOption(CMD_OPT_MISSING, options[1]);
     }
     else if (options.size() > 3) return CmdExec::errorOption(CMD_OPT_EXTRA, options[3]);
     else {
       if (myStrNCmp("-Array", options[0], 2) == 0) {
         for (size_t i = 1; i < 3; ++i)
           if (!myStr2Int(options[i], num)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[i]);
         myStr2Int(options[1], ArrSize);
         myStr2Int(options[2], num);
         if (ArrSize == 0) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
         try { mtest.newArrs(num, ArrSize); }
         catch (bad_alloc) { return CMD_EXEC_ERROR; }
       }
       else if (myStrNCmp("-Array", options[1], 2) == 0) {
         if (!myStr2Int(options[0], num)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
         else if(!myStr2Int(options[2], num)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
         myStr2Int(options[0], num);
         myStr2Int(options[2], ArrSize);
         if (ArrSize == 0) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
         try { mtest.newArrs(num, ArrSize); }
         catch (bad_alloc) { return CMD_EXEC_ERROR; }
       }
     }
   }
   // case default
   else {
     if (!myStr2Int(options[0], num)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
     else {
       if (options.size() > 1) return CmdExec::errorOption(CMD_OPT_EXTRA, options[1]);
       else {
         try { mtest.newObjs(num); }
         catch (bad_alloc) { return CMD_EXEC_ERROR; }
       }
     }
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
   int num;
   bool doRand = false, doArr = false, doIdx;

   vector<string> options;

   if(!CmdExec::lexOptions(option, options, 0)) return CMD_EXEC_ERROR;
   if(options.empty()) return CmdExec::errorOption(CMD_OPT_MISSING, "");

   for (size_t i = 0; i < options.size(); ++i) {
     if (myStrNCmp("-Array", options[i], 2) == 0) doArr = true;
     if (myStrNCmp("-Random", options[i], 2) == 0) doRand = true;
     if (myStrNCmp("-Index", options[i], 2) == 0) doIdx = true;
   }
   if (!myStrNCmp("-Array", options[0], 2) == 0 &&
       !myStrNCmp("-Random", options[0], 2) == 0 &&
       !myStrNCmp("-Index", options[0], 2) == 0)
     return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);

   const int s = (doArr)? mtest.getArrListSize(): mtest.getObjListSize();
   // case rand
   if (doRand) {
     if (options.size() == 1) return CmdExec::errorOption(CMD_OPT_MISSING,"");
     else if (options.size() == 2) {
       if (doArr) {
         if (myStrNCmp("-Random", options[0], 2) == 0) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
         else return CmdExec::errorOption(CMD_OPT_MISSING, options[1]);
       }
       else {
         if (myStrNCmp("-Random", options[0], 2) == 0) {
           if (!myStr2Int(options[1], num)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
           else {
             if (s >= num) {
               for (int i = 0; i < num; ++i) mtest.deleteObj(rnGen(s));
             }
             else {
               cerr << "Size of " << (doArr? "array":"object") << " list is 0!!" << endl;
               return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
             }
           }
         }
         else return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
       }
     }
     else if (options.size() > 3) return CmdExec::errorOption(CMD_OPT_EXTRA, options[3]);
     // case do array
     else {
       if (doArr) {
         if (myStrNCmp("-Random", options[0], 2) == 0) {
           if (!myStr2Int(options[1], num)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
           else {
             if (myStrNCmp("-Array", options[2], 2) != 0) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
             else {
               if (s >= num) {
                 for (int i = 0; i < num; ++i) mtest.deleteArr(rnGen(s));
               }
               else {
                 cerr << "Size of " << (doArr? "array":"object") << " list is 0!!" << endl;
                 return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
               }
             }
           }
         }
         else if (myStrNCmp("-Random", options[1], 2) == 0) {
           if (myStrNCmp("-Array", options[0], 2) == 0) {
             if (!myStr2Int(options[2], num)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
             else {
               if (s >= num) {
                 for (int i = 0; i < num; ++i) mtest.deleteArr(rnGen(s));
               }
               else {
                 cerr << "Size of " << (doArr? "array":"object") << " list is 0!!" << endl;
                 return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
               }
             }
           }
         }
         else {
           if (myStrNCmp("-Array", options[0], 2) == 0) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
           else return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
         }
       }
       else {
         if (myStrNCmp("-Random", options[0], 2) == 0) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
         else if (myStrNCmp("-Random", options[1], 2) == 0) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
         else return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
       }
     }
   }
   // case index
   if (doIdx) {
     if (options.size() == 1) return CmdExec::errorOption(CMD_OPT_MISSING, "");
     else if (options.size() == 2) {
       if (doArr) {
         if (myStrNCmp("-Index", options[0], 2) == 0) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
         else return CmdExec::errorOption(CMD_OPT_MISSING, options[1]);
       }
       else {
         if (myStrNCmp("-Index", options[0], 2) == 0) {
           if (!myStr2Int(options[1], num)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
           else {
             if (s >= num) for (int i = 0; i < num; ++i) mtest.deleteObj(num);
             else {
               cerr << "Size of " << (doArr? "array":"object") << " list (" << s << ") is <= " << options[1] << "!!\n";
               return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
             }
           }
         }
         else return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
       }
     }
     else if (options.size() > 3) return CmdExec::errorOption(CMD_OPT_EXTRA, options[3]);
     // case do array
     else {
       if (doArr) {
         if (myStrNCmp("-Index", options[0], 2) == 0) {
           if (!myStr2Int(options[1], num)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
           else {
             if (myStrNCmp("-Array", options[2], 2) != 0) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
             else {
               if (s >= num) for (int i = 0; i < num; ++i) mtest.deleteArr(num);
               else {
                 cerr << "Size of " << (doArr? "array":"object") << " list (" << s << ") is <= " << options[1] << "!!\n";
                 return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
               }
             }
           }
         }
         else if (myStrNCmp("-Index", options[1], 2) == 0) {
           if (myStrNCmp("-Array", options[0], 2) == 0) {
             if (!myStr2Int(options[2], num)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
             else {
               if (s >= num) for (int i = 0; i < num; ++i) mtest.deleteArr(num);
               else {
                 cerr << "Size of " << (doArr? "array":"object") << " list (" << s << ") is <= " << options[2] << "!!\n";
                 return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
               }
             }
           }
         }
         else {
           if (myStrNCmp("-Array", options[0], 2) == 0) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
           else return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
         }
       }
       else {
         if (myStrNCmp("-Index", options[0], 2) == 0) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
         else if (myStrNCmp("-Index", options[1], 2) == 0) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
         else return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
       }
     }
   }

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


