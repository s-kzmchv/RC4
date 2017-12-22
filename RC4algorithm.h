//
// Created by skzmc on 02.12.2017.
//

#ifndef RC4_RC4ALGORITHM_H
#define RC4_RC4ALGORITHM_H


#include <vector>
#include <string>

class Rc4_alg
{
private:
    unsigned char tableS [256];

    void swap(unsigned char* tableS, int i, int j);


public:
    std::vector <unsigned char> alg(const std::vector<unsigned char> & data);
    Rc4_alg(const std::string & key);
    unsigned char pseudo_random_generation(int i, int& j);
};

#endif //RC4_RC4ALGORITHM_H
