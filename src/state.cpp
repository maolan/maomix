#include "imgui.h"
#include "maomix/state.hpp"


using namespace maomix;


State * State::state = nullptr;


State::State()
  : connection{nullptr}
{
  clear_color = new ImVec4;
}


State::~State()
{
  if (connection) { delete connection; }
}


State * State::get()
{
  if (state) { return state; }
  state = new State();
  return state;
}


void State::init(const std::string &host, const int &port)
{
  auto c = new Connection(host, port);
  int ch = 16;
  int busnumber = 6;
  int fxnumber = 4;
  int retnumber = 4;

  channels.resize(ch);
  busses.resize(busnumber);
  fx.resize(fxnumber);
  ret.resize(retnumber);
  connection = c;
}
