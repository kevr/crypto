/*
* Project: hash
* Author: kevr <kevr@nixcode.us>
* License: MIT
* Description: Hashing program
*/
#include <type_traits>
#include <utility>
#include <cstdint>
#include <bitset>
#include <array>
#include <vector>
#include <iostream>
#include <cstring>
#include <cstdio>
using namespace std;

typedef unsigned char uchar;

struct SHA256_CTX
{
    array<uchar, 64>   data;
    array<uint32_t, 8> hash;

    template<size_t N>
    SHA256_CTX(array<uchar, N>&& arr)
        : data(move(arr))

    {
        static_assert(N <= 64, "SHA256_CTX(data[N]) takes size N = 64");

        // Initial hash values setup in constructor
        hash[0] = 0x6a09e667;
        hash[1] = 0xbb67ae85;
        hash[2] = 0x3c6ef372;
        hash[3] = 0xa54ff53a;
        hash[4] = 0x510e527f;
        hash[5] = 0x9b05688c;
        hash[6] = 0x1f83d9ab;
        hash[7] = 0x5be0cd19;

    }
};

static const uint32_t key[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
    0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
    0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
    0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
    0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
    0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

template<typename T>
T rotate(const T& x, const int& n)
{
    return (x >> n) | (x << (sizeof(T) * 8 - n));
}

void transform(SHA256_CTX& ctx, vector<uint32_t>& w)
{
    uint32_t s0, s1;

    for(uint32_t i = 0, j = 0; i < 16; ++i, j += 4) {
        w[i] = (ctx.data[j] << 24) | (ctx.data[j+1] << 16)
             | (ctx.data[j+2] << 8) | (ctx.data[j+3]);
    }

    for(uint32_t i = 16; i < 64; ++i) {
        s0 = (rotate(w[i-15], 7)) ^ (rotate(w[i-15], 18)) ^ (w[i-15] >> 3);
        s1 = (rotate(w[i-2], 17)) ^ (rotate(w[i-2], 19)) ^ ((w[i-2] >> 10));
        w[i] = w[i-16] + s0 + w[i-7] + s1;
    }

    uint32_t a, b, c, d, e, f, g, h;

    a = ctx.hash[0];
    b = ctx.hash[1];
    c = ctx.hash[2];
    d = ctx.hash[3];
    e = ctx.hash[4];
    f = ctx.hash[5];
    g = ctx.hash[6];
    h = ctx.hash[7];

    uint32_t ch, maj, t1, t2;

    for(uint32_t i = 0; i < 64; ++i) {
        s1 = rotate(e, 6) ^ rotate(e, 11) ^ rotate(e, 25);
        ch = (e & f) ^ (~(e) & g);
        t1 = h + s1 + ch + key[i] + w[i];

        s0 = rotate(a, 2) ^ rotate(a, 13) ^ rotate(a, 22);
        maj = (a & b) ^ (a & c) ^ (b & c);
        t2 = s0 + maj;

        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    ctx.hash[0] += a;
    ctx.hash[1] += b;
    ctx.hash[2] += c;
    ctx.hash[3] += d;
    ctx.hash[4] += e;
    ctx.hash[5] += f;
    ctx.hash[6] += g;
    ctx.hash[7] += h;

}

template<typename T>
void print(T& t)
{
    uint32_t i = 0;
    uchar *p = nullptr;
    for(auto& e : t) {
        p = (uchar *)(&e);
        for(uint32_t k = 0; k < 4; ++k, ++p) {
            printf(++i % 32 == 0 ? "%02x\n" : "%02x ", *p);
        }
    }
}

template<typename T>
void print(T& t, const char *msg)
{
    cout << msg << endl;
    uint32_t i = 0;
    uchar *p = nullptr;
    for(auto& e : t) {
        p = (uchar *)(&e);
        for(uint32_t k = 0; k < 4; ++k, ++p) {
            printf(++i % 32 == 0 ? "%02x\n" : "%02x ", *p);
        }
    }
}

template<>
void print<std::array<uchar,64>>(std::array<uchar,64>& t, const char *msg)
{
    cout << msg << endl;
    uint32_t i = 0;
    for(auto& e : t) {
        printf(++i % 32 == 0 ? "%02x\n" : "%02x ", e);
    }
}

int main(int argc, char *argv[])
{
    if(argc != 2) {
        cerr << "usage: " << argv[0] << " 'passphrase'" << endl;
        return 1;
    }

    // our std::array to copy to context
    array<uchar, 64> data;

    uint32_t len = strlen(argv[1]);
    uint32_t i = 0; // a reused loop variable

    // copy argv[1] into leading bits
    *(argv[1] + len) = 0x80;
    memcpy(&data[0], argv[1], strlen(argv[1]));
    //data[len] = 0x80;

    // Zero all bits
    for(i = len + 1; i < 63; ++i)
        data[i] = 0x00;
    data[i] = (len << 3); // then add our bit length to the end

    // construct a context
    SHA256_CTX ctx(move(data));

    print(ctx.data, "\nPrinting Data");

    // Message Schedule Array, size 64 initialized all to 0x00
    vector<uint32_t> sd(64, 0x00);
    uchar *p = nullptr;

    // copy over data to first 64 bytes
    transform(ctx, sd);

    print(sd, "\nPrinting Message Schedule Array");

    // now time to build message schedule array and hash

    vector<uchar> hash(32, 0x00);

    for (i = 0; i < 4; ++i) { 
        hash[i]    = (ctx.hash[0] >> (24 - (i << 3))) & 0x000000ff; 
        hash[i+4]  = (ctx.hash[1] >> (24 - (i << 3))) & 0x000000ff; 
        hash[i+8]  = (ctx.hash[2] >> (24 - (i << 3))) & 0x000000ff;
        hash[i+12] = (ctx.hash[3] >> (24 - (i << 3))) & 0x000000ff;
        hash[i+16] = (ctx.hash[4] >> (24 - (i << 3))) & 0x000000ff;
        hash[i+20] = (ctx.hash[5] >> (24 - (i << 3))) & 0x000000ff;
        hash[i+24] = (ctx.hash[6] >> (24 - (i << 3))) & 0x000000ff;
        hash[i+28] = (ctx.hash[7] >> (24 - (i << 3))) & 0x000000ff;
    }

    string digest;
    char buf[2];
    for(int j = 0, k = 0; j < 64; j += 2, ++k) {
        sprintf(buf, "%02x", hash[k]);
        digest.append(buf);
    }

    cout << "\nHash: " << digest << endl;

    cout << endl;
    return 0;
}


