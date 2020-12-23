// https://behringerwiki.musictribe.com/index.php?title=Bus_(/bus)_data

#include <iomanip>
#include <sstream>
#include "imgui.h"
#include "maomix/busses.hpp"
#include "maomix/state.hpp"


using namespace maomix;




void Busses::draw()
{
  int id = 1;
  const auto state = State::get();
  ImGui::Begin("Busses");
  {
    for (auto &bus : state->busses)
    {
      ImGui::PushID(id);
      ImGui::BeginGroup();
      {
        std::stringstream s;
        s << id;
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
        ImGui::Button(s.str().data(), ImVec2(30, 18));
        ImGui::PopStyleColor();
        if (ImGui::VSliderFloat("", ImVec2(30, 160), &(bus.fader), 0.0f, 1.0f, "%.2f"))
        {
          std::stringstream s;
          s << "/bus/" << id << "/mix/fader";
          state->address->send(s.str(), "f", bus);
        }

        static bool muted = bus.mute != 0;
        if (muted)
        {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
        }
        if (ImGui::Button("M", ImVec2(30, 18)))
        {
          std::stringstream s;
          s << "/bus/" << id << "/mix/on";
          bus.mute = 1 - bus.mute;
          state->address->send(s.str(), "i", bus.mute);
        }
        if (muted) { ImGui::PopStyleColor(); }

        static bool soloed = bus.solo != 1;
        if (soloed)
        {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
        }
        if (ImGui::Button("S", ImVec2(30, 18)))
        {
          std::stringstream s;
          s << "/-stat/solosw/" << std::setw(2) << std::setfill('0') << id + 39;
          bus.solo = 1 - bus.solo;
          state->address->send(s.str(), "i", bus.solo);
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
