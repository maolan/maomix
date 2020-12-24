#include <iostream>
#include "maomix/glfw/ui.hpp"
#include <lo/lo_cpp.h>


int main()
{
  lo::ServerThread st(10024);
  if (!st.is_valid()) {
      std::cerr << "Failed creating server thread" << std::endl;
      return 1;
  }
  st.set_callbacks([&st](){printf("Thread init: %p.\n",&st);},
                   [](){printf("Thread cleanup.\n");});
  std::cout << "URL: " << st.url() << std::endl;
  st.add_method(nullptr, nullptr, []{std::cout << "example" << std::endl;});
  st.start();

  maomix::UI *display = new maomix::GLFW();
  display->run();
  delete display;
  return 0;
}
