#include "field25519.h"
#include<cstdlib>

// Arithmetic under modulo 2^255 - 19

i256::i256(i64 value) {
        data.resize(16);
        data[0] = value;
        carry();
}

i64& i256::operator[](int index) {
        return data[index];
}

const i64& i256::operator[](int index) const {
        return data[index];
}


i256 i256::operator+(const i256 &other) const {
        i256 result;

        for(int i = 0; i < 16; ++i) {
                result[i] = data[i] + other.data[i];
        }
        result.carry();
        return result;
}


i256 i256::operator+(const i64 &other) const {
        i256 result;
        i64 value = other;

        for (int i = 0; i < 16; ++i) {
                result[i] = data[i] + (value & 0xffff);
                value >>= 16;
        }
        result.carry();
        return result;
}


i256 i256::operator-(const i256 &other) const {
        i256 result;

        for(int i = 0; i < 16; ++i) {
                result[i] = data[i] - other.data[i];
        }
        result.carry();
        return result;
}


i256 i256::operator-(const i64 &other) const {
        i256 result;
        i64 value = other;

        for(int i = 0; i < 16; ++i) {
                result[i] = data[i] - (value & 0xffff);
                value >>= 16;
        }
        result.carry();
        return result;
}


i256 i256::operator*(const i256 &other) const {
        i256 result;
        std::vector<i64> temp(32);

        for(int i = 0; i < 16; ++i)
                for(int j = 0; j < 16; j++)
                        temp[i+j] += data[i] * other[j];

        for(int i = 0; i < 16; i++) {
                result[i] = temp[i];
                result[i] += 38 * temp[i+16];
        }
        
        result.carry();
        result.carry();
        result.carry();

        return result;
}


bool i256::operator==(const i256 &other) const {
        for(int i = 0; i < 16; i++)
                if(data[i] != other[i])
                        return false;
        return true;
}


i256 i256::operator=(const i256 &other) {
        if(this != &other)
                for(int i = 0; i < 16; i++)
                        data[i] = other[i];

        return *this;
}


void i256::carry() {
        i64 c;
        for(int i = 0; i < 16; ++i) {
                c = data[i] >> 16;
                data[i] -= c << 16;
                if(i < 15)      data[i+1] += c;
                else            data[0] += 38*c;
        }
}


i256 power(i256 &a, i256 &e) {
        i256 r; r[0] |= 1;

        for(int i = 15; i >= 0; i--) {
                for(int j = 15; j >= 0; j--) {
                        r = r*r;
                        if((e[i] >> j) & 1)
                                r = r*a;
                }
        }

        return r;
}


i256 inverse(i256 &a) {
        // Define order of field minus 2;
        i256 q;
        for(int i = 0; i < 16; i++) q[i] = 0xffff;
        q[15] &= 0x7fff;         q = q - 20;

        return power(a, q);
}


void swap25519(i256 &a, i256 &b, i64 &bit) {
        i64 t, c = ~(bit - 1);
        for(int i = 0; i < 16; i++) {
                t = c & (a[i] ^ b[i]);
                a[i] ^= t;
                b[i] ^= t;
        }
}


i256 random() {
        i256 r;
        for(int i = 0; i < 16; i++)
                r[i] = rand() % 0x10000;
        return r;
}
