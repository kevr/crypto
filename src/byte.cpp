#include <iostream>
#include <cstdio>
#include <bitset>
using namespace std;

typedef unsigned int uint;
typedef unsigned char uchar;

template<uint byte, uint n, char... c>
struct literal;

template<uint byte, uint n>
struct literal<byte, n>
{
    static constexpr uint value() { return byte; }
};

template<uint byte, uint n, char bit, char... c>
struct literal<byte, n, bit, c...>
{
    static constexpr uint value()
    {
        return literal<byte | ((bit == '0' ? 0 : 1) << n),
                       n - 1, c...>::value();
    }
};

template<char... c>
uint operator "" _b()
{
    return literal<0x00, sizeof...(c) - 1, c...>::value();
}

int main(int argc, char *argv[])
{
    uint b = 00110011_b;

    cout << b << endl;

    printf("0x%02x\n", b);

    cout << bitset<32>(b) << endl;

    return 0;
}

