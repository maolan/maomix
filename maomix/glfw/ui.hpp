#pragma once
#include "../ui.hpp"


class GLFWwindow;


namespace maomix
{
  class App;
  class GLFW : public UI
  {
    public:
      GLFW();
      ~GLFW();

      virtual void prepare();
      virtual void render();
      virtual void run();

    protected:
      App *app;
      GLFWwindow *window;
  };
}
