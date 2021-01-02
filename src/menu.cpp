#include "imgui.h"
#include "maomix/menu.hpp"
#include "maomix/state.hpp"


using namespace maomix;


void Menu::draw()
{
  auto state = State::get();
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("View"))
    {
      if (ImGui::MenuItem("Channels"))
      {
        state->showChannels = !state->showChannels;
      }
      if (ImGui::MenuItem("Busses"))
      {
        state->showBusses = !state->showBusses;
      }
      if (ImGui::MenuItem("FX"))
      {
        state->showFx = !state->showFx;
      }
      if (ImGui::MenuItem("Return"))
      {
        state->showRet = !state->showRet;
      }
      if (ImGui::MenuItem("Line"))
      {
        state->showLine = !state->showLine;
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}
