#include <iostream>
#include <fstream>
#include <time.h>

#include "RC4algorithm.h"
#include "ProcessingData.h"


int main() {
    std::cout << "Do you want generate random key? Y/N \n";
    std::string key;
    std::cin >> key;
    if (key == "N"){
        std::cout << "Write key \n";
        std::cin >> key;
    } else {
        key.resize(32);
        int rand;
        srand((unsigned int)time(nullptr));
        for (char &c: key) {
            rand = std::rand() % 256;
            c = (char) (rand);
        }
    }
    std::cout << key << "\n";
    //printf_s(key.c_str());
    ProcessingData p;
    p.initTable();

    std::cout << "Use @en \"InputFileName\" \"OutputFileName\" for encryption \n"
            "Use @de \"InputFileName\" \"OutputFileName\" for decryption \n"
            "Use @key \"KEY\" for new key \n"
            "Use @test for make tests \n"
            "Use @help for help \n"
            "Use @end  for end \n";
    while (true) {

        std::string str1;
        std::string str2;
        std::string str3;
        std::cin >> str1;

        if(str1 == "@end"){
            break;
        }

        if(str1 == "@test"){
            int j = 0;
            int N = 300000;
            Rc4_alg rc4(key);
            std::vector<unsigned char> s;

            for(int i = 0; i < N; i++){
                s.push_back(rc4.pseudo_random_generation(i,j));
            }

            std::vector<unsigned char> s1;
            s1.assign(N,1);

            p.FrequencyTest(s);
            p.SequentialTest(8,s);
            p.TestSeries(15,s);
            p.AutocorrelationTest(15,s);
            p.UniversalTest(s);

            /*p.FrequencyTest(s1);
            p.SequentialTest(8,s1);
            p.TestSeries(15,s1);
            p.AutocorrelationTest(15,s1);
            p.UniversalTest(s1);*/


            /*p.FrequencyTest(300000,key);
            p.SequentialTest(300000,8,key);
            p.TestSeries(5000,15,key);
            p.AutocorrelationTest(300000,8,key);
            p.AutocorrelationTest(300000,9,key);
            p.AutocorrelationTest(300000,11,key);
            p.AutocorrelationTest(300000,13,key);
            p.AutocorrelationTest(300000,15,key);
            p.UniversalTest(300000,key);*/
            continue;
        }

        std::cin >> str2;

        if(str1 == "@key"){
            key = str2;
            continue;
        }

        std::cin >> str3;

        if (str1 == "@en" || str1 == "@de"){
            p.makeEncryption(str2,str3,key);
        }

        if(str1 == "@help"){
            std::cout << "Use @en \"InputFileName\" \"OutputFileName\" for encryption \n"
                    "Use @de \"InputFileName\" \"OutputFileName\" for decryption \n"
                    "Use @co \"InputFileName\" \"OutputFileName\" for correlation \n"
                    "Use @help for help \n"
                    "Use @end  for end \n";
        }
    }

    return 0;
}
