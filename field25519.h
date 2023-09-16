#pragma once
#include<vector>
using i64 = long long;

struct i256 {

        std::vector<i64> data;

        i256(i64 value = 0);

        i64& operator[](int index);
        const i64& operator[](int index) const;

        i256 operator+(const i256 &other) const;
        i256 operator+(const i64 &other) const;
        i256 operator-(const i64 &other) const;
        i256 operator*(const i256 &other) const;
        void carry();
};

i256 pow(i256 &a, i256 &e);
i256 inverse(i256 a);
