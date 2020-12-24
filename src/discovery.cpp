// https://behringerwiki.musictribe.com/index.php?title=Bus_(/bus)_data
#include "imgui.h"
#include "maomix/discovery.hpp"
#include "maomix/state.hpp"


using namespace maomix;


static const auto state = State::get();
static char ip[16];
static int port = 10024;


void Discovery::draw()
{
  ImGui::Begin("Discovery");
  {
    ImGui::InputText("IP", ip, 16);
    ImGui::InputInt("Port", &port);
    if (ImGui::Button("OK")) { state->init(ip, port); }
  }
  ImGui::End();
}
