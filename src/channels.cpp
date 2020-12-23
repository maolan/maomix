// https://behringerwiki.musictribe.com/index.php?title=Channel_(/ch)_data
#include <iomanip>
#include <sstream>
#include "imgui.h"
#include "maomix/channels.hpp"
#include "maomix/knob.hpp"
#include "maomix/state.hpp"


using namespace maomix;


static const auto state = State::get();
static std::size_t id;
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
  ImGui::Begin("Channels");
  {
    ImGui::BeginGroup();
    {
      std::stringstream s;
      s << _detail;
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      if (ImGui::Button(s.str().data(), ImVec2(30, 18))) { _detail = 0; }
      ImGui::PopStyleColor();

      auto &fader = state->channels[_detail];
      if (ImGui::VSliderFloat("##v", ImVec2(30, 160), &fader, 0.0f, 1.0f, "%.2f"))
      {
        std::stringstream s;
        s << "/ch/" << std::setw(2) << std::setfill('0') << id << "/mix/fader";
        state->address->send(s.str(), "f", fader);
      }

      auto &mute = state->mute[_detail - 1];
      muted = mute != 0;
      if (muted)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (ImGui::Button("M", ImVec2(30, 18)))
      {
        std::stringstream s;
        s << "/ch/" << std::setw(2) << std::setfill('0') << _detail << "/mix/on";
        mute = 1 - mute;
        state->address->send(s.str(), "i", mute);
      }
      if (muted) { ImGui::PopStyleColor(); }

      auto &solo = state->solo[_detail - 1];
      soloed = solo != 1;
      if (soloed)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (ImGui::Button("S", ImVec2(30, 18)))
      {
        std::stringstream s;
        s << "/-stat/solosw/" << std::setw(2) << std::setfill('0') << _detail;
        solo = 1 - solo;
        state->address->send(s.str(), "i", solo);
      }
      if (soloed) { ImGui::PopStyleColor(); }
    }
    ImGui::EndGroup();

    auto &gain = state->gain[_detail -1];
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      if (Knob(&gain, 0.0f, 1.0f))
      {
        std::stringstream s;
        s << "/headamp/" << std::setw(2) << std::setfill('0') << _detail << "/gain";
        state->address->send(s.str(), "f", gain);
      }

      auto &invert = state->invert[_detail - 1];
      static bool inverted = invert != 1;
      if (inverted)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (ImGui::Button("inv", ImVec2(30, 18)))
      {
        std::stringstream s;
        s << "/ch/" << std::setw(2) << std::setfill('0') << _detail << "/preamp/invert";
        invert = 1 - invert;
        state->address->send(s.str(), "i", invert);
      }
      if (inverted) { ImGui::PopStyleColor(); }

      auto &phantom = state->phantom[_detail - 1];
      static bool phantomed = phantom != 1;
      if (phantomed)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (ImGui::Button("48V", ImVec2(30, 18)))
      {
        // std::stringstream s;
        // s << "/ch/" << std::setw(2) << std::setfill('0') << _detail << "/preamp/invert";
        phantom = 1 - phantom;
        // state->address->send(s.str(), "i", invert);
      }
      if (phantomed) { ImGui::PopStyleColor(); }

      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      ImGui::Button("lnk", ImVec2(30, 18));
      ImGui::PopStyleColor();
    }
    ImGui::EndGroup();
  }
  ImGui::End();
}


void Channels::list()
{
  ImGui::Begin("Channels");
  {
    id = 1;
    first = true;
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
    for (auto &channel : state->channels)
    {
      ImGui::PushID(id);
      if (first) { first = false; }
      else { ImGui::SameLine(); }
      std::stringstream s;
      s << id;
      if (ImGui::Button(s.str().data(), ImVec2(30, 18))) { _detail = id; }
      ImGui::PopID();
      ++id;
    }
    ImGui::PopStyleColor();

    id = 1;
    first = true;
    for (auto &channel : state->channels)
    {
      ImGui::PushID(id);
      if (first) { first = false; }
      else { ImGui::SameLine(); }
      if (ImGui::VSliderFloat("", ImVec2(30, 160), &channel, 0.0f, 1.0f, "%.2f"))
      {
        std::stringstream s;
        s << "/ch/" << std::setw(2) << std::setfill('0') << id << "/mix/fader";
        state->address->send(s.str(), "f", channel);
      }
      ImGui::PopID();
      ++id;
    }

    id = 1;
    first = true;
    for (auto &mute : state->mute)
    {
      ImGui::PushID(id);
      muted = mute != 0;
      if (muted)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (first) { first = false; }
      else { ImGui::SameLine(); }
      if (ImGui::Button("M", ImVec2(30, 18)))
      {
        std::stringstream s;
        s << "/ch/" << std::setw(2) << std::setfill('0') << id << "/mix/on";
        mute = 1 - mute;
        state->address->send(s.str(), "i", mute);
      }
      if (muted) { ImGui::PopStyleColor(); }
      ImGui::PopID();
      ++id;
    }

    id = 1;
    first = true;
    for (auto &solo : state->solo)
    {
      ImGui::PushID(id);
      soloed = solo != 1;
      if (soloed)
      {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ImColor(0, 0, 0)));
      }
      if (first) { first = false; }
      else { ImGui::SameLine(); }
      if (ImGui::Button("S", ImVec2(30, 18)))
      {
        std::stringstream s;
        s << "/-stat/solosw/" << std::setw(2) << std::setfill('0') << id;
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
