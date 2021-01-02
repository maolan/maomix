// https://behringerwiki.musictribe.com/index.php?title=Channel_(/ch)_data
#include <iostream>
#include <iomanip>
#include <sstream>
#include "imgui.h"
#include "imgui_internal.h"
#include "maomix/channels.hpp"
#include "maomix/knob.hpp"
#include "maomix/meter.hpp"
#include "maomix/state.hpp"


using namespace maomix;


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
  const auto state = State::get();
  auto &channel = state->channels[_detail];
  const auto client = state->connection->client;
  ImGui::Begin("Channels");
  {
    ImGui::BeginGroup();
    {
      std::stringstream s;
      s << _detail;

      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      if (ImGui::Button(s.str().data(), state->size.button)) { _detail = 0; }
      ImGui::PopStyleColor();

      if (ImGui::VSliderFloat("##v", state->size.slider, &(channel.fader), 0.0f, 1.0f, "%.2f"))
      {
        std::stringstream s;
        s << "/ch/" << std::setw(2) << std::setfill('0') << _detail << "/mix/fader";
        client->send(s.str(), "f", channel.fader);
      }

      muted = channel.mute != 0;
      if (muted)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (ImGui::Button("M", state->size.button))
      {
        std::stringstream s;
        s << "/ch/" << std::setw(2) << std::setfill('0') << _detail << "/mix/on";
        channel.mute = 1 - channel.mute;
        client->send(s.str(), "i", channel.mute);
      }
      if (muted) { ImGui::PopStyleColor(); }

      soloed = channel.solo != 1;
      if (soloed)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (ImGui::Button("S", state->size.button))
      {
        std::stringstream s;
        s << "/-stat/solosw/" << std::setw(2) << std::setfill('0') << _detail;
        channel.solo = 1 - channel.solo;
        client->send(s.str(), "i", channel.solo);
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
      float value = 0.5;
      std::stringstream s;
      s << value;
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      ImGui::Button(s.str().data(), state->size.button);
      ImGui::PopStyleColor();
      Meter("vu", state->size.slider, value, 0, 1);
    }
    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();

    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      ImGui::Text("Gain");
      if (Knob("gain", state->size.knob, &(channel.gain), 0.0f, 1.0f))
      {
        std::stringstream s;
        s << "/headamp/" << std::setw(2) << std::setfill('0') << _detail << "/gain";
        client->send(s.str(), "f", channel.gain);
      }

      bool inverted = channel.invert != 1;
      if (inverted)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (ImGui::Button("inv", state->size.button))
      {
        std::stringstream s;
        s << "/ch/" << std::setw(2) << std::setfill('0') << _detail << "/preamp/invert";
        channel.invert = 1 - channel.invert;
        client->send(s.str(), "i", channel.invert);
      }
      if (inverted) { ImGui::PopStyleColor(); }

      bool phantomed = channel.phantom != 1;
      if (phantomed)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (ImGui::Button("48V", state->size.button))
      {
        // std::stringstream s;
        // s << "/ch/" << std::setw(2) << std::setfill('0') << _detail << "/preamp/invert";
        channel.phantom = 1 - channel.phantom;
        // client->send(s.str(), "i", invert);
      }
      if (phantomed) { ImGui::PopStyleColor(); }

      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      ImGui::Button("lnk", state->size.button);
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
      if (ImGui::Button("gate", state->size.button))
      {
        std::stringstream s;
        s << "/ch/" << std::setw(2) << std::setfill('0') << _detail << "/gate/on";
        channel.gate.on = 1 - channel.gate.on;
        client->send(s.str(), "i", channel.gate.on);
      }
      if (gated) { ImGui::PopStyleColor(); }
      if (Knob("Gate", state->size.knob, &(channel.gate.fader), 0.0f, 1.0f))
      {
        std::stringstream s;
        s << "/ch/" << std::setw(2) << std::setfill('0') << _detail << "/gate/thr";
        client->send(s.str(), "f", channel.gate.fader);
      }

      bool dyned = channel.dyn.on != 1;
      if (dyned)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (ImGui::Button("dyn", state->size.button))
      {
        std::stringstream s;
        s << "/ch/" << std::setw(2) << std::setfill('0') << _detail << "/dyn/on";
        channel.dyn.on = 1 - channel.dyn.on;
        client->send(s.str(), "i", channel.dyn.on);
      }
      if (dyned) { ImGui::PopStyleColor(); }
      if (Knob("dyn", state->size.knob, &(channel.dyn.fader), 0.0f, 1.0f))
      {
        std::stringstream s;
        s << "/ch/" << std::setw(2) << std::setfill('0') << _detail << "/dyn/thr";
        client->send(s.str(), "f", channel.dyn.fader);
      }
    }
    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();

    ImGui::BeginGroup();
    {
      ImGui::Text("Send");
      auto &channel = state->channels[_detail - 1];
      int busnumber = 1;
      for (auto &bus : channel.send)
      {
        std::stringstream s;
        s << "Bus " << busnumber;
        if (ImGui::SliderFloat(s.str().data(), &bus, 0.0f, 1.0f, "%.2f"))
        {
          std::stringstream s;
          s << "/ch/" << std::setw(2) << std::setfill('0') << _detail;
          s << "/mix/" << std::setw(2) << std::setfill('0') << busnumber << "/level";
          client->send(s.str(), "f", bus);
        }
        ++busnumber;
      }
      int fxnumber = 1;
      for (auto &fx : channel.fx)
      {
        std::stringstream s;
        s << "FX " << fxnumber;
        if (ImGui::SliderFloat(s.str().data(), &fx, 0.0f, 1.0f, "%.2f"))
        {
          std::stringstream s;
          s << "/ch/" << std::setw(2) << std::setfill('0') << _detail;
          s << "/mix/" << std::setw(2) << std::setfill('0') << fxnumber + busnumber - 1 << "/level";
          client->send(s.str(), "f", fx);
        }
        ++fxnumber;
      }
    }
    ImGui::EndGroup();

    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
  }
  ImGui::End();
}


void Channels::list()
{
  int id = 1;
  const auto state = State::get();
  const auto client = state->connection->client;
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
        if (ImGui::Button(s.str().data(), state->size.button)) { _detail = id; }
        ImGui::PopStyleColor();

        ImGui::BeginGroup();
        {
          if (ImGui::VSliderFloat("", state->size.slider, &(channel.fader), 0.0f, 1.0f, "%.2f"))
          {
            std::stringstream s;
            s << "/ch/" << std::setw(2) << std::setfill('0') << id << "/mix/fader";
            client->send(s.str(), "f", channel.fader);
          }
          std::stringstream s;
          s << "vu" << id;
          ImGui::SameLine(-0.01);
          Meter(s.str().data(), state->size.meter, 0.5, 0, 1);
        }
        ImGui::EndGroup();

        muted = channel.mute != 0;
        if (muted)
        {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
        }
        if (ImGui::Button("M", state->size.button))
        {
          std::stringstream s;
          s << "/ch/" << std::setw(2) << std::setfill('0') << id << "/mix/on";
          channel.mute = 1 - channel.mute;
          client->send(s.str(), "i", channel.mute);
        }
        if (muted) { ImGui::PopStyleColor(); }

        soloed = channel.solo != 1;
        if (soloed)
        {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
        }
        if (ImGui::Button("S", state->size.button))
        {
          std::stringstream s;
          s << "/-stat/solosw/" << std::setw(2) << std::setfill('0') << id;
          channel.solo = 1 - channel.solo;
          client->send(s.str(), "i", channel.solo);
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
