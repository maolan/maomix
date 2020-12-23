// https://behringerwiki.musictribe.com/index.php?title=Bus_(/bus)_data
#include "imgui.h"
#include "maomix/discovery.hpp"
#include "maomix/state.hpp"


using namespace maomix;


static const auto state = State::get();


void Discovery::draw()
{
  ImGui::Begin("Discovery");
  {
    ImGui::InputText("IP", state->ip, 16);
    ImGui::InputInt("Port", &(state->port));
    if (ImGui::Button("OK"))
    {
      state->init(state->ip, state->port);
    }
  }
  ImGui::End();
}
