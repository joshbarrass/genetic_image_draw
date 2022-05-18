#include <iostream>
#include <stdio.h>
#include <ctime>
#include <getopt.h>
#include <unistd.h>
#include <filesystem>
#include "rand.h"
#include "rand_ops.h"
#include "compimg.h"
#include "progressBar.h"
#include "generation.h"

#include "main_class.h"

const std::string HELP_TEXT_FORMAT = R"(%1$s -- Draw Images Genetically
Usage:
  %1$s -f <target> -n <num_triangles> -o <output> [-g <per_generation>] [-r <resume>]
  %1$s --help

-f --target          Path to image file to aim for
-o --output          Path to output final image to
-n --triangles       Number of triangles to draw
-g --per-generation  Number of triangles to attempt to draw each generation
-r --resume          Path to image to load instead of blank canvas
-h --help            Print this message
)";

Main::Main() : set_image_file(false), set_out_file(false), set_iterations(false), IMAGES_PER_GENERATION(1), set_resume_file(false), set_seed(false), target(0), canvas(0), FINISH_NOW(false) {}
Main::~Main() {
  if (target) {
    delete target;
  }
  if (canvas) {
    delete canvas;
  }
}

int Main::parseArgs(int argc, char **argv) {
  option longopts[] = {
      {"triangles", required_argument, NULL, 'n'},
      {"output", required_argument, NULL, 'o'},
      {"target", required_argument, NULL, 'f'},
      {"per-generation", required_argument, NULL, 'g'},
      {"resume", required_argument, NULL, 'r'},
      {"seed", required_argument, NULL, 's'}, {"help", no_argument, NULL, 'h'},
      {0}};

  int opt;
  // drop into new scope because this_file may never be initialised
  // and is not needed beyond this
  {std::filesystem::path this_file;
  while ((opt = getopt_long(argc, argv, "n:o:f:g:r:s:h", longopts, 0)) != -1) {
    switch (opt) {
    case 'h':
      this_file = std::filesystem::path(argv[0]);
      std::fprintf(stderr, HELP_TEXT_FORMAT.c_str(),
                   this_file.filename().u8string().c_str());
      return 0;
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
    case 'r':
      RESUME_FILE = std::string(optarg);
      set_resume_file = true;
      break;
    case 's':
      SEED = atoi(optarg);
      set_seed = true;
      break;
    case ':':
      using namespace std;
      cerr << "Option '" << optopt << "' missing arg" << endl;
      break;
    case '?':
      // using namespace std;
      // cerr << "Unrecognised argument: '" << optopt << "'" << endl;
      // return 1;
      break;
    default:
      abort();
    }
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
  return 0;
}

int Main::run() {
  // seed rng
  if (!set_seed) {
    SEED = time(0);
  }
  std::cerr << "Seed: " << SEED << std::endl;
  seed(SEED);

  // load target image
  target = new Image(IMAGE_FILE.c_str());

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

  // create canvas
  if (set_resume_file) {
    canvas = new Image(RESUME_FILE.c_str());
    if ((canvas->width() != target->width()) ||
        (canvas->height() != target->height())) {
      using namespace std;
      cerr << "Resume image does not have the same dimensions as target image" << endl;
      return 4;
    }
  } else {
    // create a blank canvas
    canvas = new Image(target->width(), target->height(),
                              target->depth(), target->spectrum());
    canvas->fill(0);
  }

  // find error between target and canvas
  double err = img_error(target, canvas);
  
  #ifdef BUILD_DEBUG
  double err1 = img_error_new(target, canvas);
  double err2 = img_error_new(canvas, target);

  {
    using namespace std;
    cout << "True Error: " << err << endl;
    cout << "target - canvas: " << err1 << endl;
    cout << "canvas - target: " << err2 << endl;
  }
  #endif

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
  } while (pbar.GetValue() < ITERATIONS && (!FINISH_NOW));

  std::cout << std::endl;
  canvas -> save(OUT_FILE.c_str());

  return 0;
}

int main(int argc, char **argv) {
  Main program = Main();
  if (int err = program.parseArgs(argc, argv); err != 0) {
    return err;
  }
  program.register_signal_callback();
  return program.run();
}
