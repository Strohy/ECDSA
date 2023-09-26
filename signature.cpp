#include<string>
#include "signature.h"


i256 hashtoi256(std::string digest) {
        i256 result;

        for(int i = 0; i < 64; i++) {
                i64 h;
                if(digest[i] >= 'a')
                        h = digest[i] - 'a' + 10;
                else
                        h = digest[i] - '0';
                result[i/4] += h << (i%4);
        }

        return result;
}


std::string i256tostring(i256 &a) {
        std::string result;

        for(int i = 0; i < 16; i++)
                for(int j = 0; j < 4; j++) {
                        int idx = (a[i] >> (4*j)) & 0xf;
                        result += "0123456789abcde"[idx];
                }

        return result;
}


std::vector<i256> sign(std::string &privateKey, std::string &message) {
        i256 s = hashtoi256(sha256(privateKey));
        i256 publicKey = generatePublicKey(privateKey);

        i256 r = random();
        i256 R = scalarMultiply(r, G);

        i256 k = hashtoi256(sha256(i256tostring(R) + message));
        i256 ks = k*s;
        i256 z = r - ks;

        return {z, k};
}


bool verify(i256 &z, i256 &k, i256 &publicKey, std::string message) {
        i256 R1 = scalarMultiply(z, G) + scalarMultiply(k, publicKey);

        return hashtoi256(sha256(i256tostring(R1) + message)) == k;
}


i256 generatePublicKey(std::string &privateKey) {
        i256 s = hashtoi256(sha256(privateKey));
        i256 A = scalarMultiply(s, G);

        return A;
}
