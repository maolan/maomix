// https://behringerwiki.musictribe.com/index.php?title=Main_Stereo_(/main/st)_data
#include "imgui.h"
#include "maomix/main.hpp"
#include "maomix/meter.hpp"
#include "maomix/state.hpp"


using namespace maomix;


void Main::draw()
{
  const auto state = State::get();
  const auto client = state->connection->client;
  bool muted;
  ImGui::Begin("Main");
  {
    ImGui::BeginGroup();
    {
      if (ImGui::VSliderFloat("", state->size.slider, &(state->output.fader), 0.0f, 1.0f, "%.2f"))
      {
        client->send("/lr/mix/fader", "f", state->output);
      }
      ImGui::SameLine(-0.01);
      Meter("vu", state->size.meter, 0.5, 0, 1);
    }
    ImGui::EndGroup();

    muted = state->output.mute != 0;
    if (muted)
    {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
    }
    if (ImGui::Button("M", state->size.button))
    {
      state->output.mute = 1 - state->output.mute;
      client->send("/lr/mix/on", "i", state->output.mute);
    }
    if (muted) { ImGui::PopStyleColor(); }
  }
  ImGui::End();
}
