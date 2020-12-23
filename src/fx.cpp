// Find out which URL is apropriate
#include <iomanip>
#include <sstream>
#include "imgui.h"
#include "maomix/fx.hpp"
#include "maomix/state.hpp"


using namespace maomix;


void FX::draw()
{
  int id = 1;
  const auto state = State::get();
  ImGui::Begin("FX");
  {
    for (auto &fx : state->fx)
    {
      ImGui::PushID(id);
      ImGui::BeginGroup();
      {
        std::stringstream s;
        s << id;
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
        ImGui::Button(s.str().data(), ImVec2(30, 18));
        ImGui::PopStyleColor();

        if (ImGui::VSliderFloat("", ImVec2(30, 160), &(fx.fader), 0.0f, 1.0f, "%.2f"))
        {
          std::stringstream s;
          s << "/bus/" << std::setw(2) << std::setfill('0') << id << "/mix/mlevel";
          state->address->send(s.str(), "f", fx.fader);
        }

        static bool muted = fx.mute != 0;
        if (muted)
        {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
        }
        if (ImGui::Button("M", ImVec2(30, 18)))
        {
          std::stringstream s;
          s << "/bus/" << std::setw(2) << std::setfill('0') << id << "/mix/on";
          fx.mute = 1 - fx.mute;
          state->address->send(s.str(), "i", fx.mute);
        }
        if (muted) { ImGui::PopStyleColor(); }

        static bool soloed = fx.solo != 1;
        if (soloed)
        {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
        }
        if (ImGui::Button("S", ImVec2(30, 18)))
        {
          std::stringstream s;
          s << "/-stat/solosw/" << std::setw(2) << std::setfill('0') << id + 45;
          fx.solo = 1 - fx.solo;
          state->address->send(s.str(), "i", fx.solo);
        }
        if (soloed) { ImGui::PopStyleColor(); }
      }
      ImGui::EndGroup();
      ImGui::PopID();
      ImGui::SameLine();
      ++id;
    }
  }
  ImGui::End();
}
