#include "light.h"

#include "ImGuiRenderer.h"
#include "imgui.h"
#include "imgui_internal.h"

void SpotLight::AddGUI() {
  ImGui::Text("Spot Light:");
  {
    int enabled = (status == Light::ENABLED) ? 1 : 0;
    ImGui::CheckboxFlags("Enabled", &enabled, true);
    status = enabled ? Light::ENABLED : Light::DISABLED;

    ImGui::SliderFloat3("Position", (float *)&position, -5.0f, 5.0f, "%.2f");
    ImGui::SliderFloat3("Direction", (float *)&direction, -5.0f, 5.0f, "%.2f");

    ImGui::ColorEdit4("Ambient", (float *)&ambient);
    ImGui::ColorEdit4("Diffuse", (float *)&diffuse);
    ImGui::ColorEdit4("Specular", (float *)&specular);

    ImGui::SliderFloat("Constant", (float *)&constant, 0.1f, 3.0f, "%.2f");
    ImGui::SliderFloat("Linear", (float *)&linear, 0.01f, 2.0f, "%.2f");
    ImGui::SliderFloat("Quadratic", (float *)&quadratic, 0.01f, 1.0f, "%.2f");

    ImGui::SliderFloat("Cut off", (float *)&cutOff, 0.01f, 90.0f, "%.2f");
    ImGui::SliderFloat("Outer cut off", (float *)&outerCutOff, 0.01f, 90.0f,
                       "%.2f");

    if (cutOff > outerCutOff) {
      cutOff = outerCutOff;
    }
  }
}

void PointLight::AddGUI() {
  ImGui::Text("Directional Light:");

  int enabled = (status == Light::ENABLED) ? 1 : 0;
  ImGui::CheckboxFlags("Enabled", &enabled, true);
  status = enabled ? Light::ENABLED : Light::DISABLED;

  ImGui::SliderFloat3("Position", (float *)&position, -5.0f, 5.0f, "%.2f");

  ImGui::ColorEdit4("Ambient", (float *)&ambient);
  ImGui::ColorEdit4("Diffuse", (float *)&diffuse);
  ImGui::ColorEdit4("Specular", (float *)&specular);

  ImGui::SliderFloat("Constant", (float *)&constant, 0.1f, 3.0f, "%.2f");
  ImGui::SliderFloat("Linear", (float *)&linear, 0.01f, 2.0f, "%.2f");
  ImGui::SliderFloat("Quadratic", (float *)&quadratic, 0.01f, 1.0f, "%.2f");
}

void DirectionalLight::AddGUI() {
  ImGui::Text("Directional Light:");

  int enabled = (status == Light::ENABLED) ? 1 : 0;
  ImGui::CheckboxFlags("Enabled", &enabled, true);
  status = enabled ? Light::ENABLED : Light::DISABLED;

  ImGui::SliderFloat3("Direction", (float *)&direction, -5, 5, "%.2f");
  ImGui::ColorEdit4("Ambient", (float *)&ambient);
  ImGui::ColorEdit4("Diffuse", (float *)&diffuse);
  ImGui::ColorEdit4("Specular", (float *)&specular);
}
