// -*- mode: C++; -*-
#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H 1

class ProgressBar {
public:
  ProgressBar(int length, double min, double max);
  ProgressBar(int length, double min, double max, bool showRaw);
  ~ProgressBar() {}
  void operator++() {
    Inc();
  }
  void Inc() {
    fValue += 1;
  }
  void Inc(double amount) {
    fValue += amount;
  }
  void Display();
  void SetShowRaw(bool v) { fShowRaw = v; }
  double GetValue() {
    return fValue;
  }

private:
  double fValue;
  double fMin;
  double fMax;
  int fLength;
  bool fShowRaw;
};

#endif
