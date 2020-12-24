#include <iomanip>
#include "maomix/app.hpp"
#include "maomix/state.hpp"


using namespace maomix;


const std::string App::title = "MaoMix";


void App::draw()
{
  static auto state = State::get();
  if (state->connection)
  {
    busses.draw();
    fx.draw();
    ret.draw();
    line.draw();
    main.draw();
    channels.draw();
  }
  else { discovery.draw(); }
}
