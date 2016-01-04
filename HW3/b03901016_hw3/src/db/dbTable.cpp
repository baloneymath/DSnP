/****************************************************************************
  FileName     [ dbTable.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Table member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <string>
#include <cctype>
#include <cassert>
#include <set>
#include <algorithm>
#include "dbTable.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream& operator << (ostream& os, const DBRow& r)
{
   // TODO: to print out a row.
   // - Data are seperated by a space. No trailing space at the end.
   // - Null cells are printed as '.'
   for(size_t i=0;i < r.size();i++){
     if(r[i] != INT_MAX) os << r[i];
     else os << '.';
     if(i != r.size() - 1) os << ' ';
   }
   return os;
}

ostream& operator << (ostream& os, const DBTable& t)
{
   // TODO: to print out a table
   // - Data are seperated by setw(6) and aligned right.
   // - Null cells should be left blank (printed as ' ').
   for(size_t i=0;i < t.nRows();i++){
     for(size_t j=0;j < t.nCols();j++){
       if(t[i][j] != INT_MAX) os << setw(6) << t[i][j];
       else os << setw(6) << '.';
     }
     os << endl;
   }
   return os;
}

ifstream& operator >> (ifstream& ifs, DBTable& t)
{
   // TODO: to read in data from csv file and store them in a table
   // - You can assume all the data of the table are in a single line.
   char ch;
   string str;
   DBRow _D;
   while(ifs.get(ch)){
     if(ch != ',' && ch != '\r' && ch != '\n') str.push_back(ch);
     else if(ch == ','){
       if(str.empty()) _D.addData(INT_MAX);
       else{
         int a = atoi(str.c_str());
         _D.addData(a);
         str.clear();
       }
     }
     else if(ch == '\r' || ch == '\n'){
       if(ifs.peek() == '\r' || ifs.peek() == '\n'){
         if(str.empty()){
           _D.addData(INT_MAX);
           t.addRow(_D);
           _D.reset();
           return ifs;
         }
         else{
           int a = atoi(str.c_str());
           _D.addData(a);
           t.addRow(_D);
           str.clear();
           _D.reset();
           return ifs;
         }
       }
       if(str.empty()){
         _D.addData(INT_MAX);
         t.addRow(_D);
         _D.reset();
       }
       else{
         int a = atoi(str.c_str());
         _D.addData(a);
         t.addRow(_D);
         str.clear();
         _D.reset();
       }
     }
   }
   return ifs;
}

/*****************************************/
/*   Member Functions for class DBRow    */
/*****************************************/
void
DBRow::removeCell(size_t c)
{
   // TODO
   _data.erase(_data.begin() + c);
}

/*****************************************/
/*   Member Functions for struct DBSort  */
/*****************************************/
bool
DBSort::operator() (const DBRow& r1, const DBRow& r2) const
{
   // TODO: called as a functional object that compares the data in r1 and r2
   //       based on the order defined in _sortOrder
   for(size_t i=0;i < _sortOrder.size();i++){
     if(r1[_sortOrder[i]] < r2[_sortOrder[i]]) return true;
     else if(r1[_sortOrder[i]] > r2[_sortOrder[i]]) return false;
   }
   return false;
}

/*****************************************/
/*   Member Functions for class DBTable  */
/*****************************************/
void
DBTable::reset()
{
   // TODO
   vector<DBRow> t;
   t.swap(_table);
}

void
DBTable::addCol(const vector<int>& d)
{
   // TODO: add a column to the right of the table. Data are in 'd'.
   for(size_t i=0;i < _table.size();i++){
     _table[i].addData(d[i]);
   }
}

void
DBTable::delRow(int c)
{
   // TODO: delete row #c. Note #0 is the first row.
   _table.erase(_table.begin() + c);
}

void
DBTable::delCol(int c)
{
   // delete col #c. Note #0 is the first row.
   for (size_t i = 0, n = _table.size(); i < n; ++i)
      _table[i].removeCell(c);
}

// For the following getXXX() functions...  (except for getCount())
// - Ignore null cells
// - If all the cells in column #c are null, return NAN
// - Return "float" because NAN is a float.
float
DBTable::getMax(size_t c) const
{
   // TODO: get the max data in column #c
   bool flag = false;
   for(size_t i=0;i < _table.size();i++)
     if(_table[i][c] != INT_MAX) flag = true;
   if(flag == false) return NAN;

   float max = INT_MIN;
   for(size_t i=0;i < _table.size();i++)
     if(_table[i][c] > max) max = _table[i][c];
   return max;
}

float
DBTable::getMin(size_t c) const
{
   // TODO: get the min data in column #c
   bool flag = false;
   for(size_t i=0;i < _table.size();i++)
     if(_table[i][c] != INT_MAX) flag = true;
   if(flag == false) return NAN;
   
   float min = INT_MAX;
   for(size_t i=0;i < _table.size();i++)
     if(_table[i][c] < min) min = _table[i][c];
   return min;
}

float
DBTable::getSum(size_t c) const
{
   // TODO: compute the sum of data in column #c
   bool flag = false;
   for(size_t i=0;i < _table.size();i++)
     if(_table[i][c] != INT_MAX) flag = true;
   if(flag == false) return NAN;
   
   float sum = 0;
   for(size_t i=0;i < _table.size();i++)
     if(_table[i][c] != INT_MAX) sum += _table[i][c];
   return sum;
}

int
DBTable::getCount(size_t c) const
{
   // TODO: compute the number of distinct data in column #c
   // - Ignore null cells
   int temp[_table.size()];
   for(size_t i=0;i < _table.size();i++) temp[i] = _table[i][c];
   for(size_t i=0;i < _table.size();i++)
     for(size_t j=1;j < _table.size();j++)
       if(temp[j] < temp[i]){
         int _T = temp[i];
         temp[i] = temp[j];
         temp[j] = _T;
       }
   int counter = 1;
   bool if_exist_NULL = false;
   for(size_t i=0;i < _table.size() - 1;i++){
     if(temp[i] != temp[i + 1]) counter++;
     if(temp[i] == INT_MAX) if_exist_NULL = true;
   }
   if(if_exist_NULL == true) counter--;
   return counter;
}

float
DBTable::getAve(size_t c) const
{
   // TODO: compute the average of data in column #c
   bool flag = false;
   for(size_t i=0;i < _table.size();i++)
     if(_table[i][c] != INT_MAX) flag = true;
   if(flag == false) return NAN;

   float ave = 0;
   int num = _table.size();
   for(size_t i=0;i < _table.size();i++){
     if(_table[i][c] != INT_MAX) ave += _table[i][c];
     else num--;
   }
   ave = ave / num;
   return ave;
}

void
DBTable::sort(const struct DBSort& s)
{
   // TODO: sort the data according to the order of columns in 's'
  std::sort(_table.begin(), _table.end(), s);
}

void
DBTable::printCol(size_t c) const
{
   // TODO: to print out a column.
   // - Data are seperated by a space. No trailing space at the end.
   // - Null cells are printed as '.'
   for(size_t i=0;i < _table.size();i++){
     if(_table[i][c] != INT_MAX) cout << _table[i][c];
     else cout << '.';
     if(i != _table.size() - 1) cout << ' ';
   }
   cout << endl;
}

void
DBTable::printSummary() const
{
   size_t nr = nRows(), nc = nCols(), nv = 0;
   for (size_t i = 0; i < nr; ++i)
      for (size_t j = 0; j < nc; ++j)
         if (_table[i][j] != INT_MAX) ++nv;
   cout << "(#rows, #cols, #data) = (" << nr << ", " << nc << ", "
        << nv << ")" << endl;
}

