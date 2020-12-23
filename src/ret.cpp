// https://behringerwiki.musictribe.com/index.php?title=FX_Return_(/fxrtn)_data
#include <iomanip>
#include <sstream>
#include "imgui.h"
#include "maomix/ret.hpp"
#include "maomix/state.hpp"


using namespace maomix;


static const auto state = State::get();
static std::size_t id;
static bool muted;
static bool soloed;
static bool first;


void Ret::draw()
{
  ImGui::Begin("Ret");
  {
    id = 1;
    first = true;
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
    for (auto &ret: state->ret)
    {
      ImGui::PushID(id);
      if (first) { first = false; }
      else { ImGui::SameLine(); }
      std::stringstream s;
      s << id;
      ImGui::Button(s.str().data(), ImVec2(30, 18));
      ImGui::PopID();
      ++id;
    }
    ImGui::PopStyleColor();

    id = 1;
    first = true;
    for (auto &ret : state->ret)
    {
      ImGui::PushID(id);
      if (first) { first = false; }
      else { ImGui::SameLine(); }
      if (ImGui::VSliderFloat("", ImVec2(30, 160), &ret, 0.0f, 1.0f, "%.2f"))
      {
        std::stringstream s;
        s << "/fxrtn/" << std::setw(2) << std::setfill('0') << id << "/mix/fader";
        state->address->send(s.str(), "f", ret);
      }
      ImGui::PopID();
      ++id;
    }

    id = 1;
    first = true;
    for (auto &mute : state->retmute)
    {
      ImGui::PushID(id);
      if (first) { first = false; }
      else { ImGui::SameLine(); }
      muted = mute != 0;
      if (muted)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (ImGui::Button("M", ImVec2(30, 18)))
      {
        std::stringstream s;
        s << "/fxrtn/" << std::setw(2) << std::setfill('0') << id << "/mix/on";
        mute = 1 - mute;
        state->address->send(s.str(), "i", mute);
      }
      if (muted) { ImGui::PopStyleColor(); }
      ImGui::PopID();
      ++id;
    }

    id = 1;
    first = true;
    for (auto &solo : state->retsolo)
    {
      ImGui::PushID(id);
      if (first) { first = false; }
      else { ImGui::SameLine(); }
      soloed = solo != 1;
      if (soloed)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (ImGui::Button("S", ImVec2(30, 18)))
      {
        std::stringstream s;
        s << "/-stat/solosw/" << std::setw(2) << std::setfill('0') << id + 17;
        solo = 1 - solo;
        state->address->send(s.str(), "i", solo);
      }
      if (soloed) { ImGui::PopStyleColor(); }
      ImGui::PopID();
      ++id;
    }
  }
  ImGui::End();
}
