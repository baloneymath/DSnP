#include <iostream>
#include "../myUsage.h"

using namespace std;

int main()
{
#define MM 26
    MyUsage mm;

    cout << "Hello" << endl;

    int **q = new int*[1<<MM];
    for (size_t i = 0; i < 1<<MM; ++i){
       q[i] = new int[10];
    }

    mm.report(true, true);

    while (1);
}

