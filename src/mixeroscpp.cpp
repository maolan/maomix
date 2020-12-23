#include "maomix/glfw/ui.hpp"
#include "maomix/state.hpp"
#include <lo/lo_cpp.h>


int main()
{
  maomix::UI *display = new maomix::GLFW();
  auto state = maomix::State::get();
  // state->init("192.168.111.2", 10024, 16, 6, 4, 4);

  display->run();

  delete state;
  delete display;
  return 0;
}
