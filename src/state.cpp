#include "imgui.h"
#include "maomix/state.hpp"


using namespace maomix;


Size::Size()
  : slider{40, 160}
  , button{40, 18}
  , knob{18}
  , meter{3, 160}
{}


State * State::state = nullptr;


State::State()
  : connection{nullptr}
  , showBusses{true}
  , showFx{true}
  , showRet{true}
  , showLine{true}
  , showChannels{true}
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
