// https://behringerwiki.musictribe.com/index.php?title=Main_Stereo_(/main/st)_data
#include "imgui.h"
#include "maomix/main.hpp"
#include "maomix/state.hpp"


using namespace maomix;


static const auto state = State::get();
static bool muted;


void Main::draw()
{
  ImGui::Begin("Main");
  {
    if (ImGui::VSliderFloat("", ImVec2(30, 160), &(state->output), 0.0f, 1.0f, "%.2f"))
    {
      state->address->send("/lr/mix/fader", "f", state->output);
    }
    muted = state->muted != 0;
    if (muted)
    {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
    }
    if (ImGui::Button("M", ImVec2(30, 18)))
    {
      state->muted = 1 - state->muted;
      state->address->send("/lr/mix/on", "i", state->muted);
    }
    if (muted) { ImGui::PopStyleColor(); }
  }
  ImGui::End();
}
