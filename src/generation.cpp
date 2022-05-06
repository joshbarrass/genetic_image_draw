#include "generation.h"
#include "rand_ops.h"
#include "compimg.h"

Generation::Generation(const int num_images, const float max_feature_size,
                       const float min_feature_size)
    : fNumImages(num_images), fMaxFeatureSize(max_feature_size),
      fMinFeatureSize(min_feature_size), fInitialised(false) {
  fImageArray = new Image *[num_images];
}

Generation::~Generation() {
  if (fInitialised) {
    for (int i = 0; i < fNumImages; ++i) {
      delete fImageArray[i];
    }
  }
  delete fImageArray;
}

void Generation::NextGeneration(const Image *canvas, const Image *target) {
  for (int i = 0; i < fNumImages; ++i) {
    if (fInitialised) {
      delete fImageArray[i];
    }
    fImageArray[i] = rand_triangle_centre_colour(canvas, target, 1.0);
  }

  fInitialised = true;
}

// returns the error and sets best_image to the location of the best image
double Generation::GetBestError(const Image *target, Image *&best_image) {
  double lowest_error = img_error(target, fImageArray[0]);
  int best_index = 0;
  for (int i = 1; i < fNumImages; ++i) {
    double err = img_error(target, fImageArray[i]);
    if (err < lowest_error) {
      lowest_error = err;
      best_index = i;
    }
  }

  best_image = fImageArray[best_index];
  return lowest_error;
}
