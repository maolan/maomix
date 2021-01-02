#include <iostream>
#include "imgui.h"
#include "maomix/meter.hpp"


void Meter(const char *label, const ImVec2 &size, const float &value, float v_min, float v_max)
{
  float min_val;
  float max_val;
  float vrange = v_max - v_min;
  const float limitgreen = 7.0 / 10.0 * vrange + v_min;
  const float limityellow = 9.0 / 10.0 * vrange + v_min;
  ImColor color;
  const ImVec2 thetop = ImGui::GetCursorScreenPos();
  ImVec2 p_min = thetop;
  ImVec2 p_max = thetop;
  p_max.x += size.x;
  p_max.y += size.y;
  const ImVec2 thebottom = p_max;
  ImDrawList* draw_list = ImGui::GetWindowDrawList();


  max_val = v_max;
  min_val = limityellow;
  p_min = thetop;
  p_max.y = p_min.y + (size.y / 10.0);
  if (value >= max_val)
  {
    color = ImColor(255, 0, 0);
    draw_list->AddRectFilled(p_min, p_max, color);
  }
  else if (value < min_val)
  {
    color = ImColor(255, 0, 0, 100);
    draw_list->AddRectFilled(p_min, p_max, color);
  }
  else
  {
    float myrange = (max_val - min_val);
    float percent = (value - min_val) / myrange;
    color = ImColor(255, 0, 0, 100);
    ImVec2 intermediate = p_min;
    intermediate.x = p_max.x;
    intermediate.y += (1 - percent) * (p_max.y - p_min.y);
    draw_list->AddRectFilled(p_min, intermediate, color);
    intermediate.x = p_min.x;
    color = ImColor(255, 0, 0);
    draw_list->AddRectFilled(intermediate, p_max, color);
  }

  max_val = limityellow;
  min_val = limitgreen;
  p_min.y = p_max.y;
  p_max.y = p_min.y + (2.0 / 10 * size.y);
  if (value >= max_val)
  {
    color = ImColor(255, 255, 0);
    draw_list->AddRectFilled(p_min, p_max, color);
  }
  else if (value < min_val)
  {
    color = ImColor(255, 255, 0, 100);
    draw_list->AddRectFilled(p_min, p_max, color);
  }
  else
  {
    float myrange = (max_val - min_val);
    float percent = (value - min_val) / myrange;
    color = ImColor(255, 255, 0, 100);
    ImVec2 intermediate = p_min;
    intermediate.x = p_max.x;
    intermediate.y += (1 - percent) * (p_max.y - p_min.y);
    draw_list->AddRectFilled(p_min, intermediate, color);
    intermediate.x = p_min.x;
    color = ImColor(255, 255, 0);
    draw_list->AddRectFilled(intermediate, p_max, color);
  }

  max_val = limitgreen;
  min_val = v_min;
  p_min.y = p_max.y;
  p_max.y = thetop.y + size.y;
  if (value >= max_val)
  {
    color = ImColor(0, 255, 0);
    draw_list->AddRectFilled(p_min, p_max, color);
  }
  else if (value < min_val)
  {
    color = ImColor(0, 255, 0, 100);
    draw_list->AddRectFilled(p_min, p_max, color);
  }
  else
  {
    float myrange = (max_val - min_val);
    float percent = (value - min_val) / myrange;
    color = ImColor(0, 255, 0, 100);
    ImVec2 intermediate = p_min;
    intermediate.x = p_max.x;
    intermediate.y += (1 - percent) * (p_max.y - p_min.y);
    draw_list->AddRectFilled(p_min, intermediate, color);
    intermediate.x = p_min.x;
    color = ImColor(0, 255, 0);
    draw_list->AddRectFilled(intermediate, p_max, color);
  }
}
