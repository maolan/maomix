#include <iomanip>
#include "maomix/app.hpp"
#include "maomix/state.hpp"


using namespace maomix;


const std::string App::title = "MaoMix";


void App::draw()
{
  auto state = State::get();
  if (state->connection)
  {
    menu.draw();
    if (state->showBusses) { busses.draw(); }
    if (state->showFx) { fx.draw(); }
    if (state->showRet) { ret.draw(); }
    if (state->showLine) { line.draw(); }
    main.draw();
    if (state->showChannels) { channels.draw(); }
  }
  else { discovery.draw(); }
}
