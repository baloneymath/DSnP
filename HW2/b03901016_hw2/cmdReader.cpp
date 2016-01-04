/****************************************************************************
  FileName     [ cmdReader.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Define command line reader member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cassert>
#include <cstring>
#include "cmdParser.h"

using namespace std;

//----------------------------------------------------------------------
//    Extrenal funcitons
//----------------------------------------------------------------------
void mybeep();
char mygetc(istream&);
ParseChar getChar(char, istream&);


//----------------------------------------------------------------------
//    Member Function for class Parser
//----------------------------------------------------------------------
void
CmdParser::readCmd()
{
   if (_dofile.is_open()) {
      readCmdInt(_dofile);
      _dofile.close();
   }
   else
      readCmdInt(cin);
}

void
CmdParser::readCmdInt(istream& istr)
{
   resetBufAndPrintPrompt();

   while (1) {
      char ch = mygetc(istr);
      ParseChar pch = getChar(ch, istr);
      if (pch == INPUT_END_KEY) break;
      switch (pch) {
         case LINE_BEGIN_KEY :
         case HOME_KEY       : moveBufPtr(_readBuf); break;
         case LINE_END_KEY   :
         case END_KEY        : moveBufPtr(_readBufEnd); break;
         case BACK_SPACE_KEY : /* TODO */ 
                               if(_readBufPtr > _readBuf){
                                 moveBufPtr(_readBufPtr - 1);
                                 deleteChar();
                               }
                               break;
         case DELETE_KEY     : deleteChar(); break;
         case NEWLINE_KEY    : addHistory();
                               cout << char(NEWLINE_KEY);
                               resetBufAndPrintPrompt();break;
         case ARROW_UP_KEY   : moveToHistory(_historyIdx - 1); break;
         case ARROW_DOWN_KEY : moveToHistory(_historyIdx + 1); break;
         case ARROW_RIGHT_KEY: /* TODO */moveBufPtr(_readBufPtr + 1); break;
         case ARROW_LEFT_KEY : /* TODO */moveBufPtr(_readBufPtr - 1); break;
         case PG_UP_KEY      : moveToHistory(_historyIdx - PG_OFFSET); break;
         case PG_DOWN_KEY    : moveToHistory(_historyIdx + PG_OFFSET); break;
         case TAB_KEY        : /* TODO */
                               insertChar(' ',8 - (int)(_readBufPtr - _readBuf) % 8);
                               break;
         case INSERT_KEY     : // not yet supported; fall through to UNDEFINE
         case UNDEFINED_KEY:   mybeep(); break;
//       case 'D':deleteLine();
         default:  // printable character
            insertChar(char(pch)); break;
      }
      #ifdef TA_KB_SETTING
      taTestOnly();
      #endif
   }
}


// This function moves _readBufPtr to the "ptr" pointer
// It is used by left/right arrowkeys, home/end, etc.
//
// Suggested steps:
// 1. Make sure ptr is within [_readBuf, _readBufEnd].
//    If not, make a beep sound and return false. (DON'T MOVE)
// 2. Move the cursor to the left or right, depending on ptr
// 3. Update _readBufPtr accordingly. The content of the _readBuf[] will
//    not be changed
//
// [Note] This function can also be called by other member functions below
//        to move the _readBufPtr to proper position.
bool
CmdParser::moveBufPtr(char* const ptr)
{
   // TODO...
   if(_readBuf <= ptr && ptr <= _readBufEnd){
     if(ptr >= _readBufPtr)
       for(int i=0;i<(int)(ptr - _readBufPtr);i++) cout << *(_readBufPtr + i);
     else
       for(int i=0;i<(int)(_readBufPtr - ptr);i++) cout << char(8);
     _readBufPtr = ptr;
   }
   else{
     mybeep();
     return false;
   }
   return true;
}


// [Notes]
// 1. Delete the char at _readBufPtr
// 2. mybeep() and return false if at _readBufEnd
// 3. Move the remaining string left for one character
// 4. The cursor should stay at the same position
// 5. Remember to update _readBufEnd accordingly.
// 6. Don't leave the tailing character.
// 7. Call "moveBufPtr(...)" if needed.
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteChar()---
//
// cmd> This is he command
//              ^
//
bool
CmdParser::deleteChar()
{
   // TODO...
   if(_readBufPtr == _readBufEnd){
     mybeep();
     return false;
   }
   else{
     for(int i=0;i <(int)(_readBufEnd - _readBufPtr);i++)
       *(_readBufPtr + i) = *(_readBufPtr+ i + 1);
     char* ptr = _readBufPtr;
     moveBufPtr(_readBufEnd);
     cout<<' ';
     moveBufPtr(ptr);
     _readBufEnd--;
     *_readBufEnd = 0;
     return true;
   }
}

// 1. Insert character 'ch' for "repeat" times at _readBufPtr
// 2. Move the remaining string right for "repeat" characters
// 3. The cursor should move right for "repeats" positions afterwards
// 4. Default value for "repeat" is 1. You should assert that (repeat >= 1).
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling insertChar('k', 3) ---
//
// cmd> This is kkkthe command
//                 ^
//
void
CmdParser::insertChar(char ch, int repeat)
{
   // TODO...
   assert(repeat >= 1);
   for(int i =0;i <= (int)(_readBufEnd-_readBufPtr);i++)
     *(_readBufEnd + repeat - i) = *(_readBufEnd - i);
   for(int i=0;i < repeat;i++)
     *(_readBufPtr+i) = ch;
//   char* temp;
//   for(temp = _readBufEnd;temp >= _readBufPtr;temp--)
//     *(temp + repeat) = *temp;
//   for(temp = _readBufPtr;temp < _readBufPtr + repeat;temp++)
//     *temp = ch;
   cout << _readBufPtr;
   char* ptr = _readBufPtr + repeat;
   _readBufEnd += repeat;
   _readBufPtr = _readBufEnd;
   moveBufPtr(ptr);
}

// 1. Delete the line that is currently shown on the screen
// 2. Reset _readBufPtr and _readBufEnd to _readBuf
// 3. Make sure *_readBufEnd = 0
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteLine() ---
//
// cmd>
//      ^
//
void
CmdParser::deleteLine()
{
   // TODO...
   moveBufPtr(_readBuf);
   while(_readBufEnd != _readBuf) deleteChar();
   _readBufPtr = _readBufEnd = _readBuf;
   *_readBufEnd = 0;
}


// This functions moves _historyIdx to index and display _history[index]
// on the screen.
//
// Need to consider:
// If moving up... (i.e. index < _historyIdx)
// 1. If already at top (i.e. _historyIdx == 0), beep and do nothing.
// 2. If at bottom, temporarily record _readBuf to history.
//    (Do not remove spaces, and set _tempCmdStored to "true")
// 3. If index < 0, let index = 0.
//
// If moving down... (i.e. index > _historyIdx)
// 1. If already at bottom, beep and do nothing
// 2. If index >= _history.size(), let index = _history.size() - 1.
//
// Assign _historyIdx to index at the end.
//
// [Note] index should not = _historyIdx
//
void
CmdParser::moveToHistory(int index)
{
   // TODO...
   if(_historyIdx == index) return;
   //UP
   if(index < _historyIdx){
     if(index < 0) index = 0;
     if(_historyIdx == 0) {mybeep();return;}
     else if(_historyIdx == _history.size()){
       string str(_readBuf);
       _history.push_back(str);
       _tempCmdStored = true;
     }
     if(_tempCmdStored){
       string str(_readBuf);
       _history.pop_back();
       _history.push_back(str);
     }
   }
   //DOWN
   else{
     if(_historyIdx >= _history.size() - 1) {mybeep();return;}
     if(index >= _history.size()) index = _history.size() - 1;
   }
   deleteLine();
   strcpy(_readBuf, _history[index].c_str());
   _readBufEnd = _readBuf + strlen(_readBuf);
   moveBufPtr(_readBuf);
   moveBufPtr(_readBufEnd);
   _historyIdx = index;
}


// This function adds the string in _readBuf to the _history.
// The size of _history may or may not change. Depending on whether 
// there is a temp history string.
//
// 1. Remove ' ' at the beginning and end of _readBuf
// 2. If not a null string, add string to _history.
//    Be sure you are adding to the right entry of _history.
// 3. If it is a null string, don't add anything to _history.
// 4. Make sure to clean up "temp recorded string" (added earlier by up/pgUp,
//    and reset _tempCmdStored to false
// 5. Reset _historyIdx to _history.size() // for future insertion
//
//
string& trim(string&);
void
CmdParser::addHistory()
{
   // TODO...
   string str(_readBuf);
   if(str.empty() == true) return;
   if(_tempCmdStored == true){
     _history.pop_back();
     _tempCmdStored = false;
   }
   trim(str);
   _history.push_back(str);
   //*_readBuf = 0;
   //_readBufPtr = _readBufEnd = _readBuf;
   //*_readBufEnd = 0;
   _historyIdx = _history.size();
}
string& trim(string& str){
  if(str.empty()) return str;
  str.erase(0,str.find_first_not_of(' '));
  str.erase(str.find_last_not_of(' ') + 1);
  return str;
}


// 1. Replace current line with _history[_historyIdx] on the screen
// 2. Set _readBufPtr and _readBufEnd to end of line
//
// [Note] Do not change _history.size().
//
void
CmdParser::retrieveHistory()
{
   deleteLine();
   strcpy(_readBuf, _history[_historyIdx].c_str());
   cout << _readBuf;
   _readBufPtr = _readBufEnd = _readBuf + _history[_historyIdx].size();
}
