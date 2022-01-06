#include <iostream>
#include "BitBoard.h"
#include <bitset>
using namespace std;

int main() {
    //Tests only


    
    BitBoard s;
    int len;
    BitBoard* y = s.getLegalBoards(0, &len);
    BitBoard* z = y[7].getLegalBoards(1, &len);
    delete[] y;
    BitBoard* f = z[16].getLegalBoards(0, &len);
    delete[] z;
    
    
    return 0;
}