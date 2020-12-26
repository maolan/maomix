#include "imgui.h"
#include "maomix/state.hpp"


using namespace maomix;


Size::Size()
  : slider{35, 160}
  , button{35, 18}
  , knob{18}
{}


State * State::state = nullptr;


State::State()
  : connection{nullptr}
{}


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
  connection = new Connection(host, port);
}
