#include <iostream>
#include "progressBar.h"

ProgressBar::ProgressBar(int length, double min, double max)
  : fValue(min), fMin(min), fMax(max), fLength(length), fShowRaw(false) {}

ProgressBar::ProgressBar(int length, double min, double max, bool showRaw)
  : fValue(min), fMin(min), fMax(max), fLength(length), fShowRaw(showRaw) {}

void ProgressBar::Display() {
  using namespace std;

  const double percentage = (fValue - fMin) / (fMax - fMin);

  const int numFilled = fLength * percentage;
  const int numEmpty = fLength - numFilled;

  cerr << "\r";
  for (int i = 0; i < numFilled; ++i) {
    cerr << "#";
  }
  for (int i = 0; i < numEmpty; ++i) {
    cerr << "-";
  }

  if (fShowRaw) {
    cerr << " (" << fValue << "/" << fMax - fMin << ")";
  }
  
  cerr.flush();
}
