#include "field25519.h"
#include "curve25519.h"
// #include <iostream>
// using namespace std;

// y^2 = x^3 + Ax^2 + x


point::point() {
        x[0] |= 1;
}


point add(point &P1, point &P2, i256 &P) {
        i256 x2 = P2.z;

        P2.x = P1.x*P2.x - P1.z*P2.z;
        P2.x = P2.x*P2.x;
        P2.z = P1.x*P2.z - x2*P1.z;
        P2.z = P2.z*P2.z;
        P2.z = P2.z * P;
}


void doublee(point &P1) {
        i256 xz = P1.x*P1.z;

        P1.x = P1.x*P1.x;
        P1.z = P1.z*P1.z;
        P1.x = P1.x - P1.z;
        P1.z = P1.z*2 + P1.x;
        P1.z = P1.z + xz*A;
        P1.z = P1.z * xz * A;
        P1.x = P1.x * P1.x;
}


void swap(point &P1, point &P2, i64 &bit) {
        swap(P1.x, P2.x, bit);
        swap(P1.z, P2.z, bit);
}


i256 scalarMultiply(i256 &a, i256 &P) {
        point R; 

        a[0] &= 0xfff8;
        a[15] &= 0x7fff;
        a[15] |= 0x4000;

        point P1, P2;
        P2.x = P; P2.z = 1;

        for(int i = 15; i >= 0; i--)
                for(int j = 15; j >= 0; j--) {
                        i64 bit = (a[i*15] >> j) & 1;
                        swap(P1, P2, bit);
                        add(P1, P2, P);
                        doublee(P1);
                        swap(P1, P2, bit);
                }

        return P1.x * inverse(P1.z);
}


//delete main  AND iostream---------------------------------------------------------------
/*
void print(i256 &a) {
        for(int i = 15; i >= 0; i--)
                cout << (a[i] & 0xffff);
        cout << " ";
}
int main() {
        
        i256 s;
        s = s+3;
        print(s);
}
*/