/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"

using namespace std;

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
// TODO: Define your own data members and member functions, or classes
class CirGate
{
  friend class CirMgr;
public:
   CirGate() {}
   CirGate(GateType GT, GateList VC, vector<bool> II, GateList o, unsigned ID=0, unsigned LN=0,bool MK=false, string n=""):
     _gateType(GT), _faninList(VC), _faninIsInv(II), _fanoutList(o), _id(ID), _lineNo(LN),_isMarked(MK), _name(n) {}
   ~CirGate() {}

   // Basic access methods
   string getTypeStr() const {
     if (_gateType == UNDEF_GATE) return "UNDEF";
     else if (_gateType == PI_GATE) return "PI";
     else if (_gateType == PO_GATE) return "PO";
     else if (_gateType == AIG_GATE) return "AIG";
     else if (_gateType == CONST_GATE) return "CONST";
     else return "";
   }
   GateType     getType() const { return _gateType; }
   unsigned     getLineNo() const { return _lineNo; }
   unsigned     getId() const { return _id; }
   GateList     getFanin() const { return _faninList; }
   vector<bool> getIsInv() const { return _faninIsInv; }
   GateList     getFanout() const { return _fanoutList; }
   bool         getMark() const { return _isMarked; }
   string       getName() const { return _name; }
   void         setMark(bool i) { _isMarked = i; }
   void         setName(string s) { _name = s; }
   void         ResetInMark(CirGate*);
   void         ResetOutMark(CirGate*);

   // Printing functions
   void printGate() const;
   void reportGate() const;
   void __reportFanin(int, int, bool);
   void reportFanin(int level);
   void __reportFanout(int, int, bool);
   void reportFanout(int level);

private:
   GateType             _gateType;
   GateList            _faninList;
   vector<bool>       _faninIsInv;
   GateList           _fanoutList;
   unsigned                   _id;
   unsigned               _lineNo;
   bool                 _isMarked;
   string                   _name;
};

#endif // CIR_GATE_H
