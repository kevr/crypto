/*
* Project: bitmanip
* Author: kevr <kevr@nixcode.us>
* License: MIT
* Description: Some binary manipulation
*/
#include <iostream>
#include <bitset>
using namespace std;

template<typename T>
bitset<sizeof(T) * 8> bits(const T& n)
{
    return bitset<sizeof(T) * 8>(n);
}

int main(int argc, char *argv[])
{
    int n = 0;
    unsigned char *p = (unsigned char *)(&n);

    for(unsigned int i = 0; i < sizeof(n) - 1; ++i) {
        *(p++) = ~(0x00);
    }
    *p = 0x7F;

    cout << bits(n) << endl << n << endl;

    n = 0x80000000;

    cout << bits(n) << endl << n << endl;

    return 0;
}

