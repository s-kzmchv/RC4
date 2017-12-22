//
// Created by skzmc on 02.12.2017.
//

#include "RC4algorithm.h"


Rc4_alg::Rc4_alg(const std::string & key) {
    for (int i = 0; i < 256; i++) {
        tableS[i] = (unsigned char)i;
    }

    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + tableS[i] + ((unsigned char)key[i % key.size()])) % 256;
        swap(tableS, i, j);
    }
}

void Rc4_alg::swap(unsigned char * tableS, int i, int j) {
    unsigned char tmp = tableS[i];
    tableS[i] = tableS[j];
    tableS[j] = tmp;
}

unsigned char Rc4_alg::pseudo_random_generation(int i, int& j) {
    i = (i + 1) % 256;
    j = (j + tableS[i]) % 256;
    swap(tableS, i, j);
    unsigned char K = tableS[(tableS[i] + tableS[j]) % 256];
    return K;
}

std::vector<unsigned char> Rc4_alg::alg(const std::vector<unsigned char> & data) {
    std::vector <unsigned char> res;
    int j = 0;
    for (int i = 0; i < data.size(); i++) {
        res.push_back( data[i] ^ pseudo_random_generation(i,j));
    }
    return res;
}







