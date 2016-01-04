#include <iostream>

using namespace std;

class MemTestObj
{
#ifdef MEM_MGR_H
   USE_MEM_MGR(MemTestObj);
#endif // MEM_MGR_H

public:
   MemTestObj() {}
   ~MemTestObj() {}

private:
   char   _data[37];
//   int  _dataI[9];
//   char _dataC;
};


int main()
{
   cout << sizeof(MemTestObj) << endl;
}
