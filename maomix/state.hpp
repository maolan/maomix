#pragma once
#include <memory>
#include <vector>
#include "imgui.h"
#include <lo/lo_cpp.h>
#include "./connection.hpp"


class ImVec4;


namespace maomix
{
  class DynState
  {
    public:
      float fader;
      int on;
  };


  class GateState
  {
    public:
      float fader;
      int on;
  };


  class ChannelState
  {
    public:
      float fader;
      float gain;
      int mute;
      int solo;
      int invert;
      int phantom;
      GateState gate;
      DynState dyn;
  };


  class BusState
  {
    public:
      float fader;
      int mute;
      int solo;
  };


  class FXState
  {
    public:
      float fader;
      int mute;
      int solo;
  };


  class ReturnState
  {
    public:
      float fader;
      int mute;
      int solo;
  };


  class LineState
  {
    public:
      float fader;
      int mute;
      int solo;
  };


  class MainState
  {
    public:
      float fader;
      int mute;
      int solo;
  };


  class Size
  {
    public:
      Size();

      ImVec2 slider;
      ImVec2 button;
  };


  class State
  {
    public:
      ~State();
      static State * get();

      void init(const std::string &host, const int &port);

      ImVec4 *clear_color;
      Connection *connection;
      char ip[16];
      int port;

      std::vector<BusState> busses;
      std::vector<ChannelState> channels;
      std::vector<FXState> fx;
      std::vector<ReturnState> ret;
      MainState output;
      LineState line;
      Size size;

    protected:
      State();
      static State * state;
  };
}
