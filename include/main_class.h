#ifndef MAIN_CLASS_H
#define MAIN_CLASS_H 1

#include <string>
#include "consts.h"

class Main {
public:
  Main();
  ~Main();
  int parseArgs(int argc, char **argv);
  void register_signal_callback();
  int run();
private:
  void signal_callback(int);
  void handleKBInterrupt();
  
  std::string IMAGE_FILE;
  bool set_image_file;
  std::string OUT_FILE;
  bool set_out_file;
  int ITERATIONS;
  bool set_iterations;
  int IMAGES_PER_GENERATION;
  std::string RESUME_FILE;
  bool set_resume_file;
  int SEED;
  bool set_seed;

  // used for outputting resume command
  std::string this_program;

  Image *target;
  Image *canvas;

  bool FINISH_NOW;
};

#endif
