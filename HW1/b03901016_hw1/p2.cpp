#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

#ifndef DATA_H
#define DATA_H
class Data{
public:
  Data(int);
  const int operator[] (size_t i) const;
  int& operator[] (size_t i);
private:
  int *_cols;
};
#endif
Data::Data(int col_num){
  _cols = new int [col_num];
}
const int Data::operator[] (size_t i) const{
  return _cols[i];
}
int& Data::operator[] (size_t i){
  return _cols[i];
}

#ifndef SORTDATA_H
#define SORTDATA_H
struct SortData{
  bool operator() (const Data& d1, const Data& d2);
  void pushOrder(size_t);
  vector<size_t> _sortOrder;
};
#endif

bool SortData::operator() (const Data& d1, const Data& d2){
  for(size_t i=0;i<_sortOrder.size();i++){
    if(d1[_sortOrder[i]] < d2[_sortOrder[i]]) return true;
    else if(d1[_sortOrder[i]] > d2[_sortOrder[i]]) return false;
  }
  return false;
}
void SortData::pushOrder(size_t i){
  _sortOrder.push_back(i);
}

void PRINT(vector<Data>&, int, int);
void ADD(vector<Data>&, int&, int);
void SUM(vector<Data>&, int);
void AVE(vector<Data>&, int);
void MAX(vector<Data>&, int);
void MIN(vector<Data>&, int);
void COUNT(vector<Data>&, int);
void SORT(vector<Data>&);


int main()
{
  vector<Data> table;
  char file_name[100];
  int row = 0, col = 0;

  //input file
  cout <<"Please enter the file name: " << endl;
  cin >> file_name;
  cout << "Please enter the number of rows and columns: ";
  cin >> row >> col;
  fstream file;
  file.open(file_name, ios::in);

  char m[4];

  for(int i=0;i<row;i++){
    Data aa = Data(col);
    table.push_back(aa);
    for(int j=0;j<col;j++){
      if(j < col-1) file.getline(m,4,',');
      else file.getline(m,4,'\r');
      if(m[0] != '\0'){
        stringstream ss(m);
        ss >> table[i][j];
      }
      else table[i][j] = 9999;
    }
  }

  if(!file.good()){
    cerr << "Can't open file!!\n";
  }
  else cout << "File \"" << file_name << "\" was read  in successfully.\n";

  string command;
  while(1){
    cin >> command;
    if(command == "PRINT")      PRINT(table,row,col);
    else if(command == "ADD")   ADD(table,row,col);
    else if(command == "SUM")   SUM(table,row);
    else if(command == "AVE")   AVE(table,row);
    else if(command == "MAX")   MAX(table,row);
    else if(command == "MIN")   MIN(table,row);
    else if(command == "COUNT") COUNT(table,row);
    else if(command == "SORT")  SORT(table);
  }
  return 0;
}

void PRINT(vector<Data>& table, int row_num, int col_num){
  for(int i=0;i<row_num;i++){
    for(int j=0;j<col_num;j++){
      if(table[i][j] != 9999) cout << setw(4) << table[i][j];
      else cout << setw(4) << ' ';
    }
    cout << endl;
  }
}

void ADD(vector<Data>& table, int& row_num, int col_num){
  Data aa = Data(col_num);
  table.push_back(aa);
  string input;
  for(int i=0;i<col_num;i++){
    cin >> input;
    if(input == "-") table[row_num][i] = 9999;
    else{
      stringstream ss(input);
      ss >> table[row_num][i];
    }
  }
  row_num++;
}

void SUM(vector<Data>& table, int row_num){
  int ans = 0, COL;
  cin >> COL;
  for(int i=0;i<row_num;i++)
    if(table[i][COL] < 9999) ans += table[i][COL];
  cout << "The summation of data in column #" << COL << " is " << ans << '.' << endl;
}

void AVE(vector<Data>& table, int row_num){
  int COL, num = row_num;
  double ans = 0;
  cin >> COL;
  for(int i=0;i<row_num;i++){
    if(table[i][COL] < 9999) ans += table[i][COL];
    else if(table[i][COL] == 9999) num--;
  }
  ans = ans / num;
  cout << "The average of data in column #" << COL << " is " << ans << '.' << endl;
}

void MAX(vector<Data>& table, int row_num){
  int max = -99, COL;
  cin >> COL;
  for(int i=0;i<row_num;i++)
      if(table[i][COL] >= max && table[i][COL] < 9999) max = table[i][COL];
  cout << "The maximum of data in column #" << COL << " is " << max << '.' << endl;
}

void MIN(vector<Data>& table, int row_num){
  int min = 999, COL;
  cin >> COL;
  for(int i=0;i<row_num;i++)
    if(table[i][COL] <= min && table[i][COL] < 9999) min = table[i][COL];
  cout << "The minimum of data in column #" << COL << " is " << min << '.' << endl;
}

void COUNT(vector<Data>& table, int row_num){
  int counter = 1, COL, temp[row_num];
  cin >> COL;
  for(int i=0;i<row_num;i++) temp[i] = table[i][COL];
  for(int i=0;i<row_num;i++)
    for(int j=1;j<row_num;j++)
      if(temp[j] < temp[i]){
        int _T = temp[i];
        temp[i] = temp[j];
        temp[j] = _T;
      }
  bool flag = false;
  for(int i=0;i<row_num-1;i++){
    if(temp[i] != temp[i+1]) counter++;
    if(temp[i] == 9999) flag = true;
  }
  if(flag == true) counter--;
      cout << "The distinct count of data in column #" << COL << " is " << counter << '.' << endl;
}

void SORT(vector<Data>& table){
  SortData s;
  string m;
  getline(cin,m);
  stringstream ss(m);
  while(1){
    int a;
    ss >> a;
    s.pushOrder(a);
    if(ss.eof()) break;
  }
  sort(table.begin(),table.end(),s);
}
