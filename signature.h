#pragma once
#include "curve25519.h"
#include "sha256.h"
#include<string>

i256 G = 9;

i256 hashtoi256(std::string digest);
std::string i256tostring(i256 &a);
std::vector<i256> sign(std::string &privateKey, std::string &message);
bool verify(i256 &z, i256 &k, i256 &publicKey, std::string message);
i256 generatePublicKey(std::string &privateKey);