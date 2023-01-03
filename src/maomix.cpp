#include <iostream>
#include <imguiwrap.dear.h>
#include <imgui-knobs.h>
#include <imnodes.h>
#include "maomix/app.hpp"

static float value = 0;
static const int hardcoded_node_id = 1;
static const int output_attr_id = 2;

static maomix::App *app;

ImGuiWrapperReturnType
my_render_function()
{
  bool show_window { true };
  // dear::Begin("Subwindow", &show_window) && [](){
    // dear::Text("Hello, world!");
    // if (ImGuiKnobs::Knob("Volume", &value, -6.0f, 6.0f, 0.1f, "%.1fdB", ImGuiKnobVariant_WiperOnly, 0, ImGuiKnobFlags_NoTitle)) {
    //   std::cout << "Value: " << value << std::endl;
    // }

    // ImNodes::BeginNodeEditor();

    // ImNodes::BeginNode(hardcoded_node_id);
    // ImNodes::BeginNodeTitleBar();
    // ImGui::TextUnformatted("output node");
    // ImNodes::EndNodeTitleBar();
    // ImNodes::BeginOutputAttribute(output_attr_id);
    // // in between Begin|EndAttribute calls, you can call ImGui
    // // UI functions
    // ImGui::Text("output pin");
    // ImNodes::EndOutputAttribute();
    // ImGui::Dummy(ImVec2(80.0f, 45.0f));
    // ImNodes::EndNode();

    // ImNodes::BeginNode(hardcoded_node_id + 10);
    // ImNodes::BeginNodeTitleBar();
    // ImGui::TextUnformatted("output 2");
    // ImNodes::EndNodeTitleBar();
    // ImNodes::BeginInputAttribute(output_attr_id + 10);
    // // in between Begin|EndAttribute calls, you can call ImGui
    // // UI functions
    // ImGui::Text("input pin");
    // ImNodes::EndOutputAttribute();
    // ImGui::Dummy(ImVec2(80.0f, 45.0f));
    // ImNodes::EndNode();

    // ImNodes::Link(100, output_attr_id, output_attr_id + 10);

    // ImNodes::MiniMap();
    // ImNodes::EndNodeEditor();

  // };
  app->draw();
  // Return a concrete value to exit the loop.
  if (!show_window)
      return 0;
  // Return nothing to continue the loop.
  return {};
}

int main(int argc, const char** argv)
{
  ImGui::CreateContext();
  ImNodes::CreateContext();

  ImGuiWrapConfig config{};
  config.windowTitle_ = "MaoMix";
  app = new maomix::App();
  auto ret = imgui_main(config, my_render_function);

  ImNodes::DestroyContext();
  ImGui::DestroyContext();
  return ret;
}
