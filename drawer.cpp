#include <iostream>
#include <stdio.h>
#include <ctime>
#include "rand.h"
#include "rand_ops.h"
#include "compimg.h"
#include "progressBar.h"
#include "generation.h"

#define IMAGE_FILE "jamescrews.bmp"
constexpr int ITERATIONS = 1000;
constexpr int IMAGES_PER_GENERATION = 1;

int main() {
  // seed rng
  seed(time(0));
  
  Image *target = new Image(IMAGE_FILE);

  // std::cout << "Width: " << target->width() << std::endl;
  unsigned char testColour[3];
  int x = 360;
  int y = 460;
  // retrieve colour from this location
  int channels = target->spectrum();
  for (int i = 0; i < channels; ++i) {
    testColour[i] = (*target)._atXY(x, y, 0, i);
  }
  {
    using namespace std;
    cout << "C: " << (int)testColour[0] << " " << (int)testColour[1] << " " << (int)testColour[2] << endl;
  }

  // create a blank canvas
  Image *canvas = new Image(target->width(), target->height(), target->depth(), target->spectrum());
  canvas->fill(255);

  // find error between target and canvas
  double err = img_error(target, canvas);

  // create a genetic generation
  Generation gen(IMAGES_PER_GENERATION, 1.0, 0.5);

  ProgressBar pbar(50, 0, ITERATIONS);
  pbar.Display();
  do {
    // generate a new generation
    gen.NextGeneration(canvas, target);

    // find best error
    Image *best_image;
    double new_err = gen.GetBestError(target, best_image);

    if (new_err < err) {
      // free old canvas, move pointer
      delete canvas;
      canvas = new Image(*best_image); // must duplicate, otherwise it will be destroyed on next generation
      err = new_err;
      ++pbar;
      pbar.Display();
      std::cout << " (" << pbar.GetValue() << "/" <<  ITERATIONS << ")";
      printf(" (error: %.3E)", err);
    } // otherwise, do nothing. Memory will be freed next generation.
  } while (pbar.GetValue() < ITERATIONS);

  std::cout << std::endl;
  canvas -> save("test.jpg");
}
