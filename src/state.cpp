#include "imgui.h"
#include "maomix/state.hpp"


using namespace maomix;


State * State::state = nullptr;


State::State()
  : address{nullptr}
  , port{10024}
{
  clear_color = new ImVec4;
}


State::~State()
{
  if (address) { delete address; }
}


State * State::get()
{
  if (state) { return state; }
  state = new State();
  return state;
}


void State::init(const std::string &host, const unsigned &port)
{
  auto a = new lo::Address(host, port);
  int ch = 16;
  int busnumber = 6;
  int fxnumber = 4;
  int retnumber = 4;

  channels.resize(ch);
  busses.resize(busnumber);
  fx.resize(fxnumber);
  ret.resize(retnumber);
  address = a;
}
