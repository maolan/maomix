// Find the apropriate URL
#include "imgui.h"
#include "maomix/line.hpp"
#include "maomix/state.hpp"


using namespace maomix;


void Line::draw()
{
  const auto state = State::get();
  bool muted;
  bool soloed;
  ImGui::Begin("Line");
  {
    if (ImGui::VSliderFloat("", ImVec2(30, 160), &(state->line.fader), 0.0f, 1.0f, "%.2f"))
    {
      state->address->send("/rtn/aux/mix/fader", "f", state->line);
    }

    muted = state->line.mute != 0;
    if (muted)
    {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
    }
    if (ImGui::Button("M", ImVec2(30, 18)))
    {
      state->line.mute = 1 - state->line.mute;
      state->address->send("/rtn/aux/mix/on", "i", state->line.mute);
    }
    if (muted) { ImGui::PopStyleColor(); }

    soloed = state->line.solo != 1;
    if (soloed)
    {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
    }
    if (ImGui::Button("S", ImVec2(30, 18)))
    {
      state->line.solo = 1 - state->line.solo;
      state->address->send("/-stat/solosw/17", "i", state->line.solo);
    }
    if (soloed) { ImGui::PopStyleColor(); }
  }
  ImGui::End();
}
