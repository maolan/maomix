// https://behringerwiki.musictribe.com/index.php?title=Main_Stereo_(/main/st)_data
#include "imgui.h"
#include "maomix/main.hpp"
#include "maomix/state.hpp"


using namespace maomix;


void Main::draw()
{
  const auto state = State::get();
  bool muted;
  ImGui::Begin("Main");
  {
    if (ImGui::VSliderFloat("", ImVec2(35, 160), &(state->output.fader), 0.0f, 1.0f, "%.2f"))
    {
      state->address->send("/lr/mix/fader", "f", state->output);
    }
    muted = state->output.mute != 0;
    if (muted)
    {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
    }
    if (ImGui::Button("M", ImVec2(35, 18)))
    {
      state->output.mute = 1 - state->output.mute;
      state->address->send("/lr/mix/on", "i", state->output.mute);
    }
    if (muted) { ImGui::PopStyleColor(); }
  }
  ImGui::End();
}
