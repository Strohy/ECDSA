#include <vector>
#include "field25519.h"



i256::i256(i64 value) {
        data.resize(16);
        for(int i = 0; i < 16; ++i) {
                data[i] = value;
        }
}

i64& i256::operator[](int index) {
        return data[index];
}

const i64& i256::operator[](int index) const{
        return data[index];
}



i256 i256::operator+(const i256 &other) const {
        i256 result;

        for(int i = 0; i < 16; ++i) {
                result[i] = data[i] + other.data[i];
        }

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
                        temp[i+j] = data[i] * other[j];

        for(int i = 0; i < 16; i++)
                result[i] += 38 * temp[i+16];
        
        result.carry();
        result.carry();
        result.carry();

        return result;
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



i256 pow(i256 &a, i256 &e) {
        i256 r; r[0] |= 1;

        for(int i = 15; i >= 0; i--) {
                for(int j = 15; j >= 0; j--) {
                        r = r*r;
                        if((a[i*16] >> j) & 1)
                                r = r*a;
                }
        }

        return r;
}




i256 inverse(i256 a) {
        // Define order of field minus 2;
        i256 q(0xffff);
        q[15] &= 0xfff;         q = q - 20;

        return pow(a, q);
}
