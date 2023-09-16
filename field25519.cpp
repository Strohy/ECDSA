#include <vector>
#include <field25519.h>


struct i256
{
        std::vector<i64> data;


        // i256(void) {
        //         data.resize(16);
        //         for(int i = 0; i < 16; ++i) {
        //                 data[i] = 0;
        //         }
        // }

        i256(i64 value = 0) {
                data.resize(16);
                for(int i = 0; i < 16; ++i) {
                        data[i] = value;
                }
        }

        i64 &operator[](int index) {
                return data[index];
        }
        const i64 &operator[](int index) const{
                return data[index];
        }

        

        i256 operator+(const i256 &other) const {
                i256 result;

                for(int i = 0; i < 16; ++i) {
                        result[i] = data[i] + other.data[i];
                }

                return result;
        }


        i256 operator+(const i64 &other) const {
                i256 result;
                i64 value = other;

                for (int i = 0; i < 16; ++i) {
                        result[i] = data[i] + (value & 0xffff);
                        value >>= 16;
                }
                result.carry();
                return result;
        }
        

        i256 operator-(const i64 &other) const {
                i256 result;
                i64 value = other;

                for(int i = 0; i < 16; ++i) {
                        result[i] = data[i] - (value & 0xffff);
                        value >>= 16;
                }
                result.carry();
                return result;
        }


        i256 operator*(const i256 &other) const {
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
        

        void carry() {
                i64 c;
                for(int i = 0; i < 16; ++i) {
                        c = data[i] >> 16;
                        data[i] -= c << 16;
                        if(i < 15)      data[i+1] += c;
                        else            data[0] += 38*c;
                }
        }
};



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




// Delete main

// int main() {
//         i64 a = 5;
//         i256 V(12);
//         i256 q;
//         for(int i = 0; i < 16; ++i) q[i] = i+1;
//         // q[15] &= 0x0fff;
//         q = q+2;

//         for(int i = 0; i < 16; ++i) std::cout << q[i] << " ";
// }