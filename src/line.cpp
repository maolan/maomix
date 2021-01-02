// Find the apropriate URL
#include "imgui.h"
#include "maomix/line.hpp"
#include "maomix/meter.hpp"
#include "maomix/state.hpp"


using namespace maomix;


void Line::draw()
{
  const auto state = State::get();
  const auto client = state->connection->client;
  bool muted;
  bool soloed;
  ImGui::Begin("Line");
  {
    ImGui::BeginGroup();
    {
      if (ImGui::VSliderFloat("", state->size.slider, &(state->line.fader), 0.0f, 1.0f, "%.2f"))
      {
        client->send("/rtn/aux/mix/fader", "f", state->line);
      }
      ImGui::SameLine(-0.01);
      Meter("vu", state->size.meter, 0.5, 0, 1);
    }
    ImGui::EndGroup();

    muted = state->line.mute != 0;
    if (muted)
    {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
    }
    if (ImGui::Button("M", state->size.button))
    {
      state->line.mute = 1 - state->line.mute;
      client->send("/rtn/aux/mix/on", "i", state->line.mute);
    }
    if (muted) { ImGui::PopStyleColor(); }

    soloed = state->line.solo != 1;
    if (soloed)
    {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
    }
    if (ImGui::Button("S", state->size.button))
    {
      state->line.solo = 1 - state->line.solo;
      client->send("/-stat/solosw/17", "i", state->line.solo);
    }
    if (soloed) { ImGui::PopStyleColor(); }
  }
  ImGui::End();
}
