// https://behringerwiki.musictribe.com/index.php?title=Bus_(/bus)_data

#include <iomanip>
#include <sstream>
#include "imgui.h"
#include "maomix/busses.hpp"
#include "maomix/meter.hpp"
#include "maomix/state.hpp"


using namespace maomix;


void Busses::draw()
{
  int id = 1;
  const auto state = State::get();
  const auto client = state->connection->client;
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
        ImGui::Button(s.str().data(), state->size.button);
        ImGui::PopStyleColor();
        ImGui::BeginGroup();
        {
          if (ImGui::VSliderFloat("", state->size.slider, &(bus.fader), 0.0f, 1.0f, "%.2f"))
          {
            std::stringstream s;
            s << "/bus/" << id << "/mix/fader";
            client->send(s.str(), "f", bus);
          }
          std::stringstream s;
          s << "vu" << id;
          ImGui::SameLine(-0.01);
          Meter(s.str().data(), state->size.meter, 0.5, 0, 1);
        }
        ImGui::EndGroup();

        bool muted = bus.mute != 0;
        if (muted)
        {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
        }
        if (ImGui::Button("M", state->size.button))
        {
          std::stringstream s;
          s << "/bus/" << id << "/mix/on";
          bus.mute = 1 - bus.mute;
          client->send(s.str(), "i", bus.mute);
        }
        if (muted) { ImGui::PopStyleColor(); }

        bool soloed = bus.solo != 1;
        if (soloed)
        {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
        }
        if (ImGui::Button("S", state->size.button))
        {
          std::stringstream s;
          s << "/-stat/solosw/" << std::setw(2) << std::setfill('0') << id + 39;
          bus.solo = 1 - bus.solo;
          client->send(s.str(), "i", bus.solo);
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
