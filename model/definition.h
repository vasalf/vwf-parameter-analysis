#ifndef DEFINITION_H
#define DEFINITION_H
#include <chrono>
#include <fstream>

//#define TEST_MODE

#define TIME_MEASURE(some) \
    std::ofstream ofstr;\
    ofstr.open("time.dat");\
    auto start = std::chrono::high_resolution_clock::now(); \
    some \
    auto diff = std::chrono::high_resolution_clock::now() - start;\
    auto t1 = std::chrono::duration_cast<std::chrono::milliseconds>(diff);\
    ofstr << "Time: " << t1.count() << std::endl;\
    ofstr.close();

#endif // DEFINITION_H

