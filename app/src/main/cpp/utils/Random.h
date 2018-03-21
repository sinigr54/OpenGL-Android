//
// Created by sinigr on 3/21/18.
//

#ifndef OPENGLCOURSE_RANDOM_H
#define OPENGLCOURSE_RANDOM_H

#include <random>

class Random {
    std::random_device rd;
    std::mt19937 generator;

public:
    Random();

    int nextInt(int start, int end);

    double nextDouble(double start, double end);
};


#endif //OPENGLCOURSE_RANDOM_H
