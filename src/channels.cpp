// https://behringerwiki.musictribe.com/index.php?title=Channel_(/ch)_data
#include <iomanip>
#include <sstream>
#include "imgui.h"
#include "imgui_internal.h"
#include "maomix/channels.hpp"
#include "maomix/knob.hpp"
#include "maomix/state.hpp"


using namespace maomix;


static const auto state = State::get();
static bool muted;
static bool soloed;
static bool first;


void Channels::draw()
{
  if (_detail) { detail(); }
  else { list(); }
}


void Channels::detail()
{
  auto &channel = state->channels[_detail];
  ImGui::Begin("Channels");
  {
    ImGui::BeginGroup();
    {
      std::stringstream s;
      s << _detail;

      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      if (ImGui::Button(s.str().data(), ImVec2(35, 18))) { _detail = 0; }
      ImGui::PopStyleColor();

      if (ImGui::VSliderFloat("##v", ImVec2(35, 160), &(channel.fader), 0.0f, 1.0f, "%.2f"))
      {
        std::stringstream s;
        s << "/ch/" << std::setw(2) << std::setfill('0') << _detail << "/mix/fader";
        state->address->send(s.str(), "f", channel.fader);
      }

      muted = channel.mute != 0;
      if (muted)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (ImGui::Button("M", ImVec2(35, 18)))
      {
        std::stringstream s;
        s << "/ch/" << std::setw(2) << std::setfill('0') << _detail << "/mix/on";
        channel.mute = 1 - channel.mute;
        state->address->send(s.str(), "i", channel.mute);
      }
      if (muted) { ImGui::PopStyleColor(); }

      soloed = channel.solo != 1;
      if (soloed)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (ImGui::Button("S", ImVec2(35, 18)))
      {
        std::stringstream s;
        s << "/-stat/solosw/" << std::setw(2) << std::setfill('0') << _detail;
        channel.solo = 1 - channel.solo;
        state->address->send(s.str(), "i", channel.solo);
      }
      if (soloed) { ImGui::PopStyleColor(); }
    }
    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();

    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::Button("Gain", ImVec2(35, 18));
      if (Knob("gain", &(channel.gain), 0.0f, 1.0f))
      {
        std::stringstream s;
        s << "/headamp/" << std::setw(2) << std::setfill('0') << _detail << "/gain";
        state->address->send(s.str(), "f", channel.gain);
      }

      bool inverted = channel.invert != 1;
      if (inverted)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (ImGui::Button("inv", ImVec2(35, 18)))
      {
        std::stringstream s;
        s << "/ch/" << std::setw(2) << std::setfill('0') << _detail << "/preamp/invert";
        channel.invert = 1 - channel.invert;
        state->address->send(s.str(), "i", channel.invert);
      }
      if (inverted) { ImGui::PopStyleColor(); }

      bool phantomed = channel.phantom != 1;
      if (phantomed)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (ImGui::Button("48V", ImVec2(35, 18)))
      {
        // std::stringstream s;
        // s << "/ch/" << std::setw(2) << std::setfill('0') << _detail << "/preamp/invert";
        channel.phantom = 1 - channel.phantom;
        // state->address->send(s.str(), "i", invert);
      }
      if (phantomed) { ImGui::PopStyleColor(); }

      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      ImGui::Button("lnk", ImVec2(35, 18));
      ImGui::PopStyleColor();
    }
    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();

    ImGui::BeginGroup();
    {
      bool gated = channel.gate.on != 1;
      if (gated)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (ImGui::Button("Gate", ImVec2(35, 18)))
      {
        std::stringstream s;
        s << "/ch/" << std::setw(2) << std::setfill('0') << _detail << "/gate/on";
        channel.gate.on = 1 - channel.gate.on;
        state->address->send(s.str(), "i", channel.gate.on);
      }
      if (gated) { ImGui::PopStyleColor(); }
      if (Knob("Gate", &(channel.gate.fader), 0.0f, 1.0f))
      {
        std::stringstream s;
        s << "/ch/" << std::setw(2) << std::setfill('0') << _detail << "/gate/thr";
        state->address->send(s.str(), "f", channel.gate.fader);
      }

      bool dyned = channel.dyn.on != 1;
      if (dyned)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (ImGui::Button("dyn", ImVec2(35, 18)))
      {
        std::stringstream s;
        s << "/ch/" << std::setw(2) << std::setfill('0') << _detail << "/dyn/on";
        channel.dyn.on = 1 - channel.dyn.on;
        state->address->send(s.str(), "i", channel.dyn.on);
      }
      if (dyned) { ImGui::PopStyleColor(); }
      if (Knob("dyn", &(channel.dyn.fader), 0.0f, 1.0f))
      {
        std::stringstream s;
        s << "/ch/" << std::setw(2) << std::setfill('0') << _detail << "/dyn/thr";
        state->address->send(s.str(), "f", channel.dyn.fader);
      }
    }
    ImGui::EndGroup();
  }
  ImGui::End();
}


void Channels::list()
{
  int id = 1;
  ImGui::Begin("Channels");
  {
    for (auto &channel : state->channels)
    {
      ImGui::PushID(id);
      ImGui::BeginGroup();
      {
        std::stringstream s;
        s << id;
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
        if (ImGui::Button(s.str().data(), ImVec2(35, 18))) { _detail = id; }
        ImGui::PopStyleColor();

        if (ImGui::VSliderFloat("", ImVec2(35, 160), &(channel.fader), 0.0f, 1.0f, "%.2f"))
        {
          std::stringstream s;
          s << "/ch/" << std::setw(2) << std::setfill('0') << id << "/mix/fader";
          state->address->send(s.str(), "f", channel.fader);
        }

        muted = channel.mute != 0;
        if (muted)
        {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
        }
        if (ImGui::Button("M", ImVec2(35, 18)))
        {
          std::stringstream s;
          s << "/ch/" << std::setw(2) << std::setfill('0') << id << "/mix/on";
          channel.mute = 1 - channel.mute;
          state->address->send(s.str(), "i", channel.mute);
        }
        if (muted) { ImGui::PopStyleColor(); }

        soloed = channel.solo != 1;
        if (soloed)
        {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
        }
        if (ImGui::Button("S", ImVec2(35, 18)))
        {
          std::stringstream s;
          s << "/-stat/solosw/" << std::setw(2) << std::setfill('0') << id;
          channel.solo = 1 - channel.solo;
          state->address->send(s.str(), "i", channel.solo);
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
