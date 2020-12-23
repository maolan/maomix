#pragma once
#include <string>
#include "./busses.hpp"
#include "./channels.hpp"
#include "./discovery.hpp"
#include "./fx.hpp"
#include "./line.hpp"
#include "./main.hpp"
#include "./ret.hpp"


namespace maomix
{
  class App
  {
    public:
      void draw();
      static const std::string title;

    protected:
      Busses busses;
      Discovery discovery;
      Channels channels;
      FX fx;
      Line line;
      Main main;
      Ret ret;
  };
}
