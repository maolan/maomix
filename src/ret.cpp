// https://behringerwiki.musictribe.com/index.php?title=FX_Return_(/fxrtn)_data
#include <iomanip>
#include <sstream>
#include "imgui.h"
#include "maomix/ret.hpp"
#include "maomix/state.hpp"


using namespace maomix;


void Ret::draw()
{
  int id = 1;
  const auto state = State::get();
  const auto client = state->connection->client;
  ImGui::Begin("Ret");
  {
    for (auto &ret : state->ret)
    {
      ImGui::PushID(id);
      ImGui::BeginGroup();
      {
        std::stringstream s;
        s << id;
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
        ImGui::Button(s.str().data(), state->size.button);
        ImGui::PopStyleColor();

        if (ImGui::VSliderFloat("", state->size.slider, &(ret.fader), 0.0f, 1.0f, "%.2f"))
        {
          std::stringstream s;
          s << "/fxrtn/" << std::setw(2) << std::setfill('0') << id << "/mix/fader";
          client->send(s.str(), "f", ret);
        }

        bool muted = ret.mute != 0;
        if (muted)
        {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
        }
        if (ImGui::Button("M", state->size.button))
        {
          std::stringstream s;
          s << "/fxrtn/" << std::setw(2) << std::setfill('0') << id << "/mix/on";
          ret.mute = 1 - ret.mute;
          client->send(s.str(), "i", ret.mute);
        }
        if (muted) { ImGui::PopStyleColor(); }

        bool soloed = ret.solo != 1;
        if (soloed)
        {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
        }
        if (ImGui::Button("S", state->size.button))
        {
          std::stringstream s;
          s << "/-stat/solosw/" << std::setw(2) << std::setfill('0') << id + 17;
          ret.solo = 1 - ret.solo;
          client->send(s.str(), "i", ret.solo);
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
