#version 430 core
in vec4 vso_position;
in vec3 vso_normal;
in vec2 vso_texcoord;
in vec3 vso_color;
in vec3 vso_model_scaling;

out vec4 FragColor;

struct DirectionalLight {
  vec4 direction;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;

  float status;

  float padding[3];
};

struct PointLight {
  vec4 position;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;

  float constant;
  float linear;
  float quadratic;

  float status;
};

struct SpotLight {
  vec4 position;
  vec4 direction;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;

  float constant;
  float linear;
  float quadratic;

  float cutOff;
  float outerCutOff;

  float status;

  float padding[2];
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

const uint LAMBERT = 0x0100u;
const uint HALF_LAMBERT = 0x0200u;
const uint PHONG = 0x0400u;
const uint PHONG_BLINN = 0x1000u;
const uint SHOW_NORM = 0x2000u;
const uint SHOW_COLOR = 0x4000u;

uint lightingModel = PHONG;
uniform float lightingModel_float = 0.5;

uniform Material material;

uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;
uniform PointLight pointLight;

uniform vec3 camera_position;

vec3 CalculateDirectionLight(DirectionalLight light, vec3 norm) {
    if (light.status < 0.5)
        return vec3(0, 0, 0);
    // Ambient
    vec3 ambient = light.ambient.xyz * vec3(material.diffuse);

    // Diffuse
    vec3 lightDirection = normalize(-light.direction.xyz);
    float factor = max(dot(lightDirection, norm), 0.0);
    vec3 diffuse = light.diffuse.xyz * vec3(material.diffuse) * factor;
    if (bool(lightingModel & LAMBERT))
        return diffuse;
    else if (bool(lightingModel & HALF_LAMBERT))
        return light.diffuse.xyz * material.diffuse * (factor * 0.5 + 0.5);

    // Specular
    vec3 viewDirection = normalize(camera_position - vso_position.xyz);
    if (bool(lightingModel & PHONG)) {
        vec3 reflectDirection = normalize(reflect(lightDirection, norm));
        vec3 specular = light.specular.xyz * vec3(material.specular)
                        * pow(
                            max(dot(viewDirection, reflectDirection), 0.0),
                            material.shininess
                       );
        return vec3(ambient + diffuse + specular);
    } else { // phong blinn
        vec3 bisector = normalize(normalize(lightDirection) + viewDirection);
        vec3 specular = light.specular.xyz * vec3(material.specular)
                        * pow(
                            max(dot(norm, bisector), 0.0),
                            material.shininess
                       );
        return vec3(ambient + diffuse + specular);
    }
}

vec3 CalculateSpotLight(SpotLight light, vec3 norm) {
    if (light.status < 0.5)
        return vec3(0, 0, 0);
    vec3 lightDirection = normalize(light.position - vso_position).xyz;

    // Parameters
    float theta = dot(lightDirection, normalize(-light.direction.xyz));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Attenuation Factor
    float dist = length(light.position - vso_position);
    float attenuation = 1.0 / (light.constant + light.linear * dist +
        light.quadratic * (dist * dist));

    // Ambient
    vec3 ambient = light.ambient.xyz * material.ambient;


    // Diffuse
    float factor = max(dot(lightDirection, norm), 0.0);
    vec3 diffuse = light.diffuse.xyz * vec3(material.diffuse) * max(dot(lightDirection, norm), 0.0);
    if (bool(lightingModel & LAMBERT))
        return diffuse;
    else if (bool(lightingModel & HALF_LAMBERT))
        return light.diffuse.xyz * material.diffuse * (factor * 0.5 + 0.5);

    // Specular
    vec3 viewDirection = normalize(camera_position - vso_position.xyz);
    if (bool(lightingModel & PHONG)) {
        vec3 reflectDirection = normalize(reflect(lightDirection, norm));
        vec3 specular = light.specular.xyz * vec3(material.specular)
                        * pow(
                            max(dot(viewDirection, reflectDirection), 0.0),
                            material.shininess
                       );
        return vec3(ambient + diffuse + specular) * intensity * attenuation;
    } else { // phong blinn
        vec3 bisector = normalize(normalize(lightDirection) + viewDirection);
        vec3 specular = light.specular.xyz * vec3(material.specular)
                        * pow(
                            max(dot(norm, bisector), 0.0),
                            material.shininess
                       );
        return vec3(ambient + diffuse + specular) * intensity * attenuation;
    }
}

vec3 CalculatePointLight(PointLight light, vec3 norm) {
    if (light.status < 0.5)
        return vec3(0, 0, 0);
    vec3 ambient = light.ambient.xyz * material.ambient;

    // Direction to vertex
    vec3 lightDirection = normalize((light.position - vso_position).xyz);

    // Attenuation Factor
    float dist = length((light.position - vso_position).xyz);
    float attenuationFactor = 1.0 / (light.constant + light.linear * dist +
                                     light.quadratic * (dist * dist));

    // Diffuse
    float factor = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = light.diffuse.xyz * factor * material.diffuse;

    if (bool(lightingModel & LAMBERT))
        return diffuse;
    else if (bool(lightingModel & HALF_LAMBERT))
        return light.diffuse.xyz * material.diffuse * (factor * 0.5 + 0.5);

    // Specular
    vec3 viewDirection = normalize(camera_position - vso_position.xyz);
    if (bool(lightingModel & PHONG)) {
        vec3 reflectDirection = normalize(reflect(lightDirection, norm));
        vec3 specular = light.specular.xyz * vec3(material.specular)
                        * pow(
                            max(dot(viewDirection, reflectDirection), 0.0),
                            material.shininess
                       );

        return vec3(ambient + diffuse + specular) * attenuationFactor;
    } else { // phong blinn
        vec3 bisector = normalize(normalize(lightDirection) + viewDirection);
        vec3 specular = light.specular.xyz * vec3(material.specular)
                        * pow(
                            max(dot(norm, bisector), 0.0),
                            material.shininess
                       );
        return vec3(ambient + diffuse + specular) * attenuationFactor;
    }
}

void main() {
    vec4 position = vso_position;
    vec3 norm = normalize(vec4(vso_model_scaling, 0) * vec4(vso_normal.xyz, 0)).xyz;

    if (lightingModel_float < -0.2) {
        lightingModel = SHOW_COLOR;
    }
    else if (lightingModel_float < 0) {
        lightingModel = SHOW_NORM;
    }
    else if (lightingModel_float < 0.2) {
        lightingModel = LAMBERT;
    }
    else if (lightingModel_float < 0.4) {
        lightingModel = HALF_LAMBERT;
    }
    else if (lightingModel_float > 0.6) {
        lightingModel = PHONG_BLINN;
    }
    else {
        lightingModel = PHONG;
    }

    if (lightingModel == SHOW_NORM) {
        FragColor = vec4(norm, 1);
        return;
    }
    if (lightingModel == SHOW_COLOR) {
        FragColor = vec4(vso_color, 1);
        return;
    }


//    if (position.x > 0 && position.y > 0 && position.z > 0) {
//        FragColor = vec4(1, 1, 1, 1);
//    }
//    else if (position.x < 0 && position.y < 0 && position.z < 0) {
//        FragColor = vec4(0, 0, 0, 1);
//    }
//    else if (position.x > 0 && position.y < 0 && position.z < 0) {
//        FragColor = vec4(1, 0, 0, 1);
//    }
//    else if (position.x > 0 && position.y > 0 && position.z < 0) {
//        FragColor = vec4(1, 1, 0, 1);
//    }
//    else if (position.x > 0 && position.y < 0 && position.z > 0) {
//        FragColor = vec4(1, 0, 1, 1);
//    }
//    else if (position.x < 0 && position.y > 0 && position.z > 0) {
//        FragColor = vec4(0, 1, 1, 1);
//    }
//    else if (position.x < 0 && position.y > 0 && position.z < 0) {
//        FragColor = vec4(0, 1, 0, 1);
//    }
//    else {
//        FragColor = vec4(0, 0, 1, 1);
//    }
      FragColor = vec4(0, 0, 0, 1);

      FragColor += vec4(CalculateDirectionLight(directionalLight, norm), 0);
      FragColor += vec4(CalculatePointLight(pointLight, norm), 0);
      FragColor += vec4(CalculateSpotLight(spotLight, norm), 0);

      //FragColor += vec4(material.ambient, 0);
      //FragColor = vec4(norm, 1);
}
