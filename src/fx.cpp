// Find out which URL is apropriate
#include <iomanip>
#include <sstream>
#include "imgui.h"
#include "maomix/fx.hpp"
#include "maomix/state.hpp"


using namespace maomix;


static const auto state = State::get();
static std::size_t id;
static bool muted;
static bool soloed;
static bool first;


void FX::draw()
{
  ImGui::Begin("FX");
  {
    id = 1;
    first = true;
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
    for (auto &fx: state->fx)
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
    for (auto &fx : state->fx)
    {
      ImGui::PushID(id);
      if (first) { first = false; }
      else { ImGui::SameLine(); }
      if (ImGui::VSliderFloat("", ImVec2(30, 160), &fx, 0.0f, 1.0f, "%.2f"))
      {
        std::stringstream s;
        s << "/bus/" << std::setw(2) << std::setfill('0') << id << "/mix/mlevel";
        state->address->send(s.str(), "f", fx);
      }
      ImGui::PopID();
      ++id;
    }

    id = 1;
    first = true;
    for (auto &mute : state->fxmute)
    {
      ImGui::PushID(id);
      muted = mute != 0;
      if (first) { first = false; }
      else { ImGui::SameLine(); }
      if (muted)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (ImGui::Button("M", ImVec2(30, 18)))
      {
        std::stringstream s;
        s << "/bus/" << std::setw(2) << std::setfill('0') << id << "/mix/on";
        mute = 1 - mute;
        state->address->send(s.str(), "i", mute);
      }
      if (muted) { ImGui::PopStyleColor(); }
      ImGui::PopID();
      ++id;
    }

    id = 1;
    first = true;
    for (auto &solo : state->fxsolo)
    {
      ImGui::PushID(id);
      soloed = solo != 1;
      if (first) { first = false; }
      else { ImGui::SameLine(); }
      if (soloed)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (ImGui::Button("S", ImVec2(30, 18)))
      {
        std::stringstream s;
        s << "/-stat/solosw/" << std::setw(2) << std::setfill('0') << id + 45;
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
