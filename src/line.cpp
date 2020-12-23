// Find the apropriate URL
#include "imgui.h"
#include "maomix/line.hpp"
#include "maomix/state.hpp"


using namespace maomix;


static const auto state = State::get();
static bool muted;
static bool soloed;


void Line::draw()
{
  ImGui::Begin("Line");
  {
    if (ImGui::VSliderFloat("", ImVec2(30, 160), &(state->line), 0.0f, 1.0f, "%.2f"))
    {
      state->address->send("/rtn/aux/mix/fader", "f", state->line);
    }

    muted = state->linemuted != 0;
    if (muted)
    {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
    }
    if (ImGui::Button("M", ImVec2(30, 18)))
    {
      state->linemuted = 1 - state->linemuted;
      state->address->send("/rtn/aux/mix/on", "i", state->linemuted);
    }
    if (muted) { ImGui::PopStyleColor(); }

    soloed = state->linesolo != 1;
    if (soloed)
    {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
    }
    if (ImGui::Button("S", ImVec2(30, 18)))
    {
      state->linesolo = 1 - state->linesolo;
      state->address->send("/-stat/solosw/17", "i", state->linesolo);
    }
    if (soloed) { ImGui::PopStyleColor(); }
  }
  ImGui::End();
}
