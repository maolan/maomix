#pragma once
#include <string>
#include "../ui.hpp"


class GLFWwindow;


namespace maomix
{
  class App;
  class GLFW : public UI
  {
    public:
      GLFW(const std::string &title = "ImGui");
      ~GLFW();

      virtual void prepare();
      virtual void render();
      virtual void run(App *app);

    protected:
      GLFWwindow *window;
  };
}
