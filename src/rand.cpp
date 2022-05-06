#include <cmath>
#include <random>
#include "rand.h"

std::mt19937 RNG;

void seed(int seed) { RNG.seed(seed); }

// get a random float in a particular range
float randFloat(const float min, const float max) {
  std::uniform_real_distribution<float> distribution(min, max);
  return distribution(RNG);
}

// get a random integer in a particular range
int randInt(const int min, const int max) {
  std::uniform_int_distribution<int> distribution(min, max);
  return distribution(RNG);
}
