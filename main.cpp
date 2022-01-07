#include <iostream>
#include "BitBoard.h"
#include <bitset>
using namespace std;

int test_depthtwo_movecount();

int main() {
    //Tests only
    int success = test_depthtwo_movecount();


    if (success != 0) {
        cout << "Test fail" << endl;
        return -1;
    }
    
    return 0;
}

int test_depthtwo_movecount() {
    /**
     * Checks that the number of legal positions is 400 after two moves
     * returns 0 if this works
     */
    BitBoard s;
    int movecount_depthtwo, len;
    movecount_depthtwo = 0;
    BitBoard* depthone = s.getLegalBoards(0, &len);
    for (int i = 0; i < len; i++) {
        int leafdepth;
        BitBoard* leaf = depthone[i].getLegalBoards(1, &leafdepth);
        movecount_depthtwo += leafdepth;
        delete[] leaf;
    }
    delete[] depthone;
    cout << "Depth 2 move count: " << movecount_depthtwo << endl;
    
    return movecount_depthtwo - 400;
}