#include <iostream>
#include <stdio.h>
#include <ctime>
#include "rand.h"
#include "rand_ops.h"
#include "compimg.h"
#include "progressBar.h"

#define IMAGE_FILE "jamescrews.bmp"
constexpr int ITERATIONS = 1000;

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

  ProgressBar pbar(50, 0, ITERATIONS);
  pbar.Display();
  do {
    // draw a random triangle on the canvas
    Image *canvas_ = rand_triangle_centre_colour(canvas, target, 1.0);

    // recalculate error
    double new_err = img_error(target, canvas_);

    if (new_err < err) {
      // free old canvas, move pointer
      delete canvas;
      canvas = canvas_;
      err = new_err;
      ++pbar;
      pbar.Display();
      std::cout << " (" << pbar.GetValue() << "/" <<  ITERATIONS << ")";
      printf(" (error: %.3E)", err);
    } else {
      // otherwise, free the new image
      delete canvas_;
    }
    // seed(time(0));
  } while (pbar.GetValue() < ITERATIONS);

  std::cout << std::endl;
  canvas -> save("test.jpg");
}
