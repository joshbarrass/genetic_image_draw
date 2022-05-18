#include "main_class.h"

#include <iostream>
#include <functional>
#include <signal.h>

// I think a global is somewhat unavoidable here
std::function<void(int)> callback;
void call_callback(int arg) {
  callback(arg);
}

void Main::register_signal_callback() {
  callback = std::bind(&Main::signal_callback, this, std::placeholders::_1);
  signal(SIGINT, call_callback);
}

void Main::signal_callback(int signum) {
  switch (signum) {
  case SIGINT:
    handleKBInterrupt();
    break;
  default:
    break;
  }
}

void Main::handleKBInterrupt() {
  using namespace std;
  cout << "Received keyboard interrupt..." << endl;
  FINISH_NOW = true;
}
