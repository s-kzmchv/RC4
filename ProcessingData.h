//
// Created by skzmc on 02.12.2017.
//

#ifndef RC4_PROCESSINGDATA_H
#define RC4_PROCESSINGDATA_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include "RC4algorithm.h"

class ProcessingData
{
private:
    void saveFile(const std::string & outputFile, const std::vector<unsigned char> & data);
    std::vector<unsigned char> loadFile(const std::string & inputFile);
    std::map<int, double> quantiles;

public:
    void makeEncryption(const std::string & inputFile, const std::string & outputFile,const std::string & key);

    void FrequencyTest(int N, const std::string & key);
    void SequentialTest(int N, int L, const std::string & key);
    void TestSeries(int N, int L, const std::string & key);
    void AutocorrelationTest(int N, int T, const std::string & key);
    void UniversalTest(int N, const std::string & key);

    void FrequencyTest( const std::vector<unsigned char> & s);
    void SequentialTest(int L, const std::vector<unsigned char> & s);
    void TestSeries(int L, const std::vector<unsigned char> & s);
    void AutocorrelationTest( int T, const std::vector<unsigned char> & s);
    void UniversalTest(const std::vector<unsigned char> & s );

    void initTable(){
        quantiles[1] = 2.7055;
        quantiles[2] = 4.6052;
        quantiles[3] = 6.2514;
        quantiles[4] = 7.7794;
        quantiles[5] = 9.2364;
        quantiles[6] = 10.6446;
        quantiles[7] = 12.0170;
        quantiles[8] = 13.3616;
        quantiles[9] = 14.6837;
        quantiles[10] = 15.9872;
        quantiles[11] = 17.2750;
        quantiles[12] = 18.5493;
        quantiles[13] = 19.8119;
        quantiles[14] = 21.0641;
        quantiles[15] = 22.3071;
        quantiles[16] = 23.5418;
        quantiles[17] = 24.7690;
        quantiles[30] = 40.2560;
        quantiles[127] = 147.8048;
        quantiles[255] = 284.3359;
        quantiles[511] = 552.3739;
        quantiles[1023] = 1081.3794;
    }
};

#endif //RC4_PROCESSINGDATA_H
