#include "compimg.h"
#include "consts.h"

#ifdef USE_OPENMP
#include <omp.h>
#endif

constexpr int square(int x) { return x * x; }

double img_error(const Image *im1, const Image *im2) {
  Image im1_ = *im1;
  Image im2_ = *im2;

  double err = 0;

  // limit max threads to 4
  // beyond this, overhead makes the problem worse
  #ifdef USE_OPENMP
  omp_set_num_threads(4);
#pragma omp parallel for reduction(+:err)
  #endif
  for (int y = 0; y < im1->height(); ++y) {
    for (int x = 0; x < im1->width(); ++x) {
      /// TODO: generalise to other channel counts
      int p1r = (int)im1_(x, y, 0, 0);
      int p1g = (int)im1_(x, y, 0, 1);
      int p1b = (int)im1_(x, y, 0, 2);
      int p2r = (int)im2_(x, y, 0, 0);
      int p2g = (int)im2_(x, y, 0, 1);
      int p2b = (int)im2_(x, y, 0, 2);

      err += square(p1r - p2r) + square(p1g - p2g) + square(p1b - p2b);
    }
  }
  return err;
}

double img_error_new(const Image *im1, const Image *im2) {
  using namespace cimg_library;
  CImg<int> im1_ = CImg<int>(*im1);
  CImg<int> im2_ = CImg<int>(*im2);

  CImg<int> diff = im1_ - im2_;

  return diff.dot(diff);
}
