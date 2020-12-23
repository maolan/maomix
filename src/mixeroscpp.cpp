#include "maomix/glfw/ui.hpp"
#include "maomix/state.hpp"
#include <lo/lo_cpp.h>


int main()
{
  maomix::UI *display = new maomix::GLFW();
  auto state = maomix::State::get();

  display->run();

  delete state;
  delete display;
  return 0;
}
