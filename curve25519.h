#pragma once
#include "field25519.h"
#include<string>

const i64 A = 486662;

struct point {

        i256 x, z;

        point();
};

point add(point &P1, point &P2, i256 &P);
void doublee(point &P1);
void swap(point &P1, point &P2, i64 &bit);
i256 scalarMultiply(i256 &a, i256 &P);