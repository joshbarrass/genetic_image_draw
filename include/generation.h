#ifndef GENERATION_H
#define GENERATION_H 1

#include "consts.h"

class Generation {
 public:
  Generation(const int, const float, const float);
  ~Generation();
  void NextGeneration(const Image*, const Image*);
  Image **GetImageArray() { return fImageArray; }
  double GetBestError(const Image*, Image*&);

private:
  float fMaxFeatureSize;
  float fMinFeatureSize;
  int fNumImages;
  Image **fImageArray;
  bool fInitialised;
};

#endif
