#include "light.h"

#include "ImGuiRenderer.h"
#include "imgui.h"
#include "imgui_internal.h"

/*###################################################
##  函数: AddGUI
##  函数描述： Add controller for lights
##  参数描述： 无
#####################################################*/
void SpotLight::AddGUI() {
  ImGui::Text("Spot Light:");
  {
    int enabled = (status == Light::ENABLED) ? 1 : 0;
    ImGui::CheckboxFlags("SL - Enabled", &enabled, true);
    status = enabled ? Light::ENABLED : Light::DISABLED;

    ImGui::SliderFloat3("SL - Position", (float *)&position, -5.0f, 5.0f,
                        "%.2f");
    ImGui::SliderFloat3("SL - Direction", (float *)&direction, -5.0f, 5.0f,
                        "%.2f");

    ImGui::ColorEdit4("SL - Ambient", (float *)&ambient);
    ImGui::ColorEdit4("SL - Diffuse", (float *)&diffuse);
    ImGui::ColorEdit4("SL - Specular", (float *)&specular);

    ImGui::SliderFloat("SL - Constant", (float *)&constant, 0.1f, 3.0f, "%.2f");
    ImGui::SliderFloat("SL - Linear", (float *)&linear, 0.01f, 2.0f, "%.2f");
    ImGui::SliderFloat("SL - Quadratic", (float *)&quadratic, 0.01f, 1.0f,
                       "%.2f");

    ImGui::SliderFloat("SL - Cut off", (float *)&cutOff, 0.01f, 90.0f, "%.2f");
    ImGui::SliderFloat("SL - Outer cut off", (float *)&outerCutOff, 0.01f,
                       90.0f, "%.2f");

    if (cutOff > outerCutOff) {
      cutOff = outerCutOff;
    }
  }
}

void PointLight::AddGUI() {
  ImGui::Text("Point Light:");

  int enabled = (status == Light::ENABLED) ? 1 : 0;
  ImGui::CheckboxFlags("PL - Enabled", &enabled, true);
  status = enabled ? Light::ENABLED : Light::DISABLED;

  ImGui::SliderFloat3("PL - Position", (float *)&position, -5.0f, 5.0f, "%.2f");

  ImGui::ColorEdit4("PL - Ambient", (float *)&ambient);
  ImGui::ColorEdit4("PL - Diffuse", (float *)&diffuse);
  ImGui::ColorEdit4("PL - Specular", (float *)&specular);

  ImGui::SliderFloat("PL - Constant", (float *)&constant, 0.1f, 3.0f, "%.2f");
  ImGui::SliderFloat("PL - Linear", (float *)&linear, 0.01f, 2.0f, "%.2f");
  ImGui::SliderFloat("PL - Quadratic", (float *)&quadratic, 0.01f, 1.0f,
                     "%.2f");
}

void DirectionalLight::AddGUI() {
  ImGui::Text("Directional Light:");

  int enabled = (status == Light::ENABLED) ? 1 : 0;
  ImGui::CheckboxFlags("DL - Enabled", &enabled, true);
  status = enabled ? Light::ENABLED : Light::DISABLED;

  ImGui::SliderFloat3("DL - Direction", (float *)&direction, -5, 5, "%.2f");
  ImGui::ColorEdit4("DL - Ambient", (float *)&ambient);
  ImGui::ColorEdit4("DL - Diffuse", (float *)&diffuse);
  ImGui::ColorEdit4("DL - Specular", (float *)&specular);
}
