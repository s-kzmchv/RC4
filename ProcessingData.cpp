//
// Created by skzmc on 02.12.2017.
//

#include <cmath>
#include "ProcessingData.h"

void ProcessingData::saveFile(const std::string & outputFile, const std::vector<unsigned char> & data) {
    std::ofstream fileOutput(outputFile,std::ios_base::binary);

    for(unsigned char c : data)
        fileOutput << c;
    fileOutput.close();
}

std::vector<unsigned char> ProcessingData::loadFile(const std::string & inputFile) {
    std::ifstream file(inputFile,std::ios_base::binary);
    std::vector<unsigned char> data;
    if (!file.is_open()) {
        std::cout << "File doesn't exist!\n";
    } else{
        int c;
        while((c = file.get()) != EOF){
            data.push_back((unsigned char)c);
        }
        file.close();
    }
    return  data;
}

void ProcessingData::makeEncryption(const std::string & inputFile, const std::string & outputFile,const std::string & key) {
    Rc4_alg rc4(key);
    std::vector<unsigned char> data = loadFile(inputFile);
    if (!data.empty()) {
        data = rc4.alg(data);
        saveFile(outputFile, data);
    }
}

void ProcessingData::FrequencyTest(int N, const std::string & key) {
    Rc4_alg rc4(key);
    unsigned char c;
    int j = 0;
    double sum = 0;
    for(int i = 0; i < N; i++){
        c = rc4.pseudo_random_generation(i,j);
        for(int k = 0; k < 8; k++) {
            sum += ((c >> k) & 1);
        }
    }
    sum -= (N * 8)/2;
    sum = (2 * sum) / sqrt(N * 8);

    std::cout << "Frequency test with N = " << N << " \n " << sum <<"\n";

    if(sum > -3 && sum < 3){
        std::cout << "Frequency test: passed \n \n";
    }
    else{
        std::cout << "Frequency test: not passed \n \n";
    }
}

void ProcessingData::SequentialTest(int N, int L, const std::string & key) {
    Rc4_alg rc4(key);
    std::vector<int> numbers;
    numbers.resize((unsigned int)pow(2,L));

    unsigned char c;
    int j = 0;
    int bit = 0;
    int tmp = 0;
    int z = 0;
    for(int i = 0; i < N; i++){
        c = rc4.pseudo_random_generation(i,j);
        for(int k = 0; k < 8; k++) {
            bit = ((c >> k) & 1);
            tmp += bit * pow(2,z);
            z++;
            if (z == L){
                numbers[tmp]++;
                z = 0;
                tmp = 0;
            }
        }
    }
    auto len = pow(2,L);
    double expectedValue = (N*8)/(L*len);
    double sum = 0;
    for (int i : numbers){
        sum += pow((i - expectedValue),2);
    }
    sum *= ((L*len)/(N*8));

    std::cout << "Sequential test with N = " << N << " and L = "<< L <<"\n " << sum <<"\n";

    if(sum <= quantiles[pow(2, L) - 1]){
        std::cout << "Sequential test: passed" << "\n \n";
    }
    else{
        std::cout << "Sequential test: not passed" << "\n \n";
    }
}

void ProcessingData::AutocorrelationTest(int N, int T, const std::string & key) {
    Rc4_alg rc4(key);
    unsigned char c;
    std::vector<int> bits;
    int j = 0;

    for(int i = 0; i < N; i++){
        c = rc4.pseudo_random_generation(i,j);
        for(int k = 0; k < 8; k++) {
            bits.push_back ((c >> k) & 1);
        }
    }

    double sum = 0;
    for(int i = 0; i < bits.size() - T; i++){
        sum += bits[i] ^ bits[i+T];
    }

    sum -= (bits.size() - T)/2;
    sum = (2 * sum) / sqrt(bits.size() - T);

    std::cout << "Test Autocorrelation test with N = " << N << " and T = "<< T <<"\n " << sum <<"\n";

    if(sum >= -3 && sum <= 3){
        std::cout << "Autocorrelation test: passed" << "\n \n";
    }
    else{
        std::cout << "Autocorrelation test: not passed" << "\n \n";
    }
}

void ProcessingData::UniversalTest(int N, const std::string & key) {
    int L = 8;
    double V = pow(2,L);
    double Q = 10*V;
    double K = 100 * Q;

    if ( N*8 < L * (10 * V + 1000 * V))
        N = (int) (L * (10 * V + 1000 * V)) / 8;
    std::vector<int> table;
    table.assign((int)V,0);

    Rc4_alg rc4(key);
    std::vector<int> parts;
    unsigned char c;
    int j = 0;
    int bit = 0;
    int tmp = 0;
    int z = 0;
    for(int i = 0; i < N; i++){
        c = rc4.pseudo_random_generation(i,j);
        for(int k = 0; k < 8; k++) {
            bit = ((c >> k) & 1);

            tmp += bit * pow(2,z);
            z++;
            if (z == L){
                parts.push_back(tmp);
                z = 0;
                tmp = 0;
            }
        }
    }

    int i = 0;
    for (i; i < Q; i++){
        int b = parts[i];
        table[b] = i;
    }

    double sum = 0;

    for (i; i < Q + K; i++){
        int b = parts[i];
        sum += log2(i - table[b]);
        table[b] = i;
    }

    sum = sum/K ;

    double e = 7.1836656;
    double d = 3.238;
    double C = 0.7 - 0.8 / L + ((4 + 32 / L) * pow(K, -(double)3 / L)) / 15;

    double Z = (sum - e) / (C * sqrt(d));

    std::cout << "Universal test with N = " << N << " and L = "<< L <<"\n " << Z <<"\n";

    if(Z >= -1.96 && Z <= 1.96){
        std::cout << "Universal test : passed" << "\n";
    }
    else{
        std::cout << "Universal test : not passed" << "\n";
    }
}

void ProcessingData::TestSeries(int N, int L, const std::string &key) {
    if (L > 15)
        L = 15;
    Rc4_alg rc4(key);
    std::vector<unsigned long long> sequencesZero;
    std::vector<unsigned long long> sequenceOne;
    sequenceOne.assign(L+1,0);
    sequencesZero.assign(L+1,0);
    unsigned char c;
    int j = 0;
    int sOne = 0;
    int sZero = 0;


    for(int i = 0; i < N; i++){
        c = rc4.pseudo_random_generation(i,j);
        for(int k = 0; k < 8; k++) {
            int bit = ((c >> k) & 1);
            if (bit == 1){
                sOne++;
                sequencesZero[sZero]++;
                sZero = 0;
            } else{
                sZero++;
                sequenceOne[sOne]++;
                sOne = 0;
            }
        }
    }

    double sumOne = 0;
    double sumZero = 0;

    for(int i = 1; i < L+1; i++){
        double koef = ((N * 8) / pow(2, i + 2));
        sumOne += pow(sequenceOne[i] - koef, 2) / koef;
        sumZero += pow(sequencesZero[i] - koef, 2) / koef;
    }
    double sum = sumOne + sumZero;

    std::cout << "Test series with N = " << N << " and L = "<< L <<"\n " << sum <<"\n";

    if(sum <= quantiles[2 * L]){
        std::cout << "Test series: passed" << "\n \n";
    }else {
        std::cout << "Test series: not passed" << "\n \n";
    }
}

void ProcessingData::FrequencyTest(const std::vector<unsigned char> &s) {
    double sum = 0;
    for(auto c : s){
        for(int k = 0; k < 8; k++) {
            sum += ((c >> k) & 1);
        }
    }
    sum -= (s.size() * 8)/2;
    sum = (2 * sum) / sqrt(s.size() * 8);

    std::cout << "Frequency test with N = " << s.size() << " \n " << sum <<"\n";

    if(sum > -3 && sum < 3){
        std::cout << "Frequency test: passed \n \n";
    }
    else{
        std::cout << "Frequency test: not passed \n \n";
    }
}

void ProcessingData::SequentialTest(int L, const std::vector<unsigned char> &s) {
    std::vector<int> numbers;
    numbers.resize((unsigned int)pow(2,L));

    int bit = 0;
    int tmp = 0;
    int z = 0;

    for(auto c : s){
        for(int k = 0; k < 8; k++) {
            bit = ((c >> k) & 1);
            tmp += bit * pow(2,z);
            z++;
            if (z == L){
                numbers[tmp]++;
                z = 0;
                tmp = 0;
            }
        }
    }
    auto len = pow(2,L);
    double expectedValue = (s.size()*8)/(L*len);
    double sum = 0;
    for (int i : numbers){
        sum += pow((i - expectedValue),2);
    }
    sum *= ((L*len)/(s.size()*8));

    std::cout << "Sequential test with N = " << s.size() << " and L = "<< L <<"\n " << sum <<"\n";

    if(sum <= quantiles[pow(2, L) - 1]){
        std::cout << "Sequential test: passed" << "\n \n";
    }
    else{
        std::cout << "Sequential test: not passed" << "\n \n";
    }

}

void ProcessingData::TestSeries(int L, const std::vector<unsigned char> &s) {
    if (L > 15)
        L = 15;

    std::vector<unsigned long long> sequencesZero;
    std::vector<unsigned long long> sequenceOne;

    sequenceOne.assign(L+1,0);
    sequencesZero.assign(L+1,0);

    long int sOne = 0;
    long int sZero = 0;


    for(auto c : s){
        for(int k = 0; k < 8; k++) {
            int bit = ((c >> k) & 1);
            if (bit == 1){
                sOne++;
                if (sZero <= L)
                    sequencesZero[sZero]++;
                sZero = 0;
            } else{
                sZero++;
                if (sOne <= L)
                    sequenceOne[sOne]++;
                sOne = 0;
            }
        }
    }

    long double sumOne = 0;
    long double sumZero = 0;

    for(int i = 1; i < L+1; i++){
        double koef = ((s.size() * 8) / pow(2, i + 2));
        sumOne += pow(sequenceOne[i] - koef, 2) / koef;
        sumZero += pow(sequencesZero[i] - koef, 2) / koef;
    }
    long double sum = sumOne + sumZero;

    std::cout << "Test series with N = " << s.size() << " and L = "<< L <<"\n " << sum <<"\n";

    if(sum <= quantiles[2 * L]){
        std::cout << "Test series: passed" << "\n \n";
    }else {
        std::cout << "Test series: not passed" << "\n \n";
    }
}

void ProcessingData::AutocorrelationTest(int T, const std::vector<unsigned char> &s) {
    std::vector<int> bits;
    int j = 0;

    for(auto c : s){
        for(int k = 0; k < 8; k++) {
            bits.push_back ((c >> k) & 1);
        }
    }

    double sum = 0;
    for(int i = 0; i < bits.size() - T; i++){
        sum += bits[i] ^ bits[i+T];
    }

    sum -= (bits.size() - T)/2;
    sum = (2 * sum) / sqrt(bits.size() - T);

    std::cout << "Test Autocorrelation test with N = " << s.size() << " and T = "<< T <<"\n " << sum <<"\n";

    if(sum >= -3 && sum <= 3){
        std::cout << "Autocorrelation test: passed" << "\n \n";
    }
    else{
        std::cout << "Autocorrelation test: not passed" << "\n \n";
    }

}

void ProcessingData::UniversalTest(const std::vector<unsigned char> &s) {
    int L = 8;
    double V = pow(2,L);
    double Q = 10*V;
    double K = 100 * Q;
    int N = s.size();

    if ( N*8 < L * (10 * V + 1000 * V))
        N = (int) (L * (10 * V + 1000 * V)) / 8;
    std::vector<int> table;
    table.assign((int)V,0);

    std::vector<int> parts;
    int j = 0;
    int bit = 0;
    int tmp = 0;
    int z = 0;
    for(auto c : s){
        for(int k = 0; k < 8; k++) {
            bit = ((c >> k) & 1);

            tmp += bit * pow(2,z);
            z++;
            if (z == L){
                parts.push_back(tmp);
                z = 0;
                tmp = 0;
            }
        }
    }

    int i = 0;
    for (i; i < Q; i++){
        int b = parts[i];
        table[b] = i;
    }

    double sum = 0;

    for (i; i < Q + K; i++){
        int b = parts[i];
        sum += log2(i - table[b]);
        table[b] = i;
    }

    sum = sum/K ;

    double e = 7.1836656;
    double d = 3.238;
    double C = 0.7 - 0.8 / L + ((4 + 32 / L) * pow(K, -(double)3 / L)) / 15;

    double Z = (sum - e) / (C * sqrt(d));

    std::cout << "Universal test with N = " << N << " and L = "<< L <<"\n " << Z <<"\n";

    if(Z >= -1.96 && Z <= 1.96){
        std::cout << "Universal test : passed" << "\n";
    }
    else{
        std::cout << "Universal test : not passed" << "\n";
    }
}









