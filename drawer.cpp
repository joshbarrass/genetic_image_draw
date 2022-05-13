#include <iostream>
#include <stdio.h>
#include <ctime>
#include <getopt.h>
#include <unistd.h>
#include "rand.h"
#include "rand_ops.h"
#include "compimg.h"
#include "progressBar.h"
#include "generation.h"


int main(int argc, char **argv) {
  // seed rng
  seed(time(0));

  std::string IMAGE_FILE;
  bool set_image_file = false;
  std::string OUT_FILE;
  bool set_out_file = false;
  int ITERATIONS;
  bool set_iterations = false;
  int IMAGES_PER_GENERATION = 1;

  option longopts[] = {{"triangles", required_argument, NULL, 'n'},
                       {"output", required_argument, NULL, 'o'},
                       {"target", required_argument, NULL, 'f'},
                       {"per-generation", required_argument, NULL, 'g'},
                       {0}};

  int opt;
  while ((opt = getopt_long(argc, argv, "n:o:f:g:", longopts, 0)) != -1) {
    switch (opt) {
    case 'n':
      ITERATIONS = atoi(optarg);
      set_iterations = true;
      break;
    case 'o':
      OUT_FILE = std::string(optarg);
      set_out_file = true;
      break;
    case 'f':
      IMAGE_FILE = std::string(optarg);
      set_image_file = true;
      break;
    case 'g':
      IMAGES_PER_GENERATION = atoi(optarg);
      break;
    case ':':
      using namespace std;
      cerr << "Option '" << optopt << "' missing arg" << endl;
    case '?':
      // using namespace std;
      // cerr << "Unrecognised argument: '" << optopt << "'" << endl;
      // return 1;
      break;
    default:
      abort();
    }
  }

  {
    using namespace std;
    if (!set_image_file) {
      cerr << "Required argument --target not set" << endl;
      return 2;
    }
    if (!set_out_file) {
      cerr << "Required argument --output not set" << endl;
      return 2;
    }
    if (!set_iterations) {
      cerr << "Required argument --triangles not set" << endl;
      return 2;
    }
  }

  Image *target = new Image(IMAGE_FILE.c_str());

  #ifdef BUILD_DEBUG
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
  #endif

  // create a blank canvas
  Image *canvas = new Image(target->width(), target->height(), target->depth(), target->spectrum());
  canvas->fill(0);

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
  canvas -> save(OUT_FILE.c_str());
}
