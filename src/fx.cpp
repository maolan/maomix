// Find out which URL is apropriate
#include <iomanip>
#include <sstream>
#include "imgui.h"
#include "maomix/fx.hpp"
#include "maomix/meter.hpp"
#include "maomix/state.hpp"


using namespace maomix;


void FX::draw()
{
  int id = 1;
  const auto state = State::get();
  const auto client = state->connection->client;
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
        ImGui::Button(s.str().data(), state->size.button);
        ImGui::PopStyleColor();

        ImGui::BeginGroup();
        {
          if (ImGui::VSliderFloat("", state->size.slider, &(fx.fader), 0.0f, 1.0f, "%.2f"))
          {
            std::stringstream s;
            s << "/fxsend/" << id << "/mix/fader";
            client->send(s.str(), "f", fx.fader);
          }
          std::stringstream s;
          s << "vu" << id;
          ImGui::SameLine(-0.01);
          Meter(s.str().data(), state->size.meter, 0.5, 0, 1);
        }
        ImGui::EndGroup();

        bool muted = fx.mute != 0;
        if (muted)
        {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
        }
        if (ImGui::Button("M", state->size.button))
        {
          std::stringstream s;
          s << "/fxsend/" << id << "/mix/on";
          fx.mute = 1 - fx.mute;
          client->send(s.str(), "i", fx.mute);
        }
        if (muted) { ImGui::PopStyleColor(); }

        bool soloed = fx.solo != 1;
        if (soloed)
        {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
        }
        if (ImGui::Button("S", state->size.button))
        {
          std::stringstream s;
          s << "/-stat/solosw/" << id + 45;
          fx.solo = 1 - fx.solo;
          client->send(s.str(), "i", fx.solo);
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
