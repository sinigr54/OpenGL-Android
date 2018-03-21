//
// Created by sinigr on 3/21/18.
//

#include "Random.h"

Random::Random() : generator(rd()) {

}

int Random::nextInt(int start, int end) {
    std::uniform_int_distribution<> distribution(start, end);
    return distribution(generator);
}

double Random::nextDouble(double start, double end) {
    std::uniform_real_distribution<> distribution(start, end);
    return distribution(generator);
}
