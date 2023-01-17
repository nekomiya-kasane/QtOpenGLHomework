#version 430 core
in vec4 vso_position;
in vec3 vso_normal;
in vec2 vso_texcoord;
in vec3 vso_color;

out vec4 FragColor;

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

uniform uint lightingModel = PHONG_BLINN;

uniform Material material;

void main() {
    vec4 position = vso_position;

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
    FragColor = vec4(material.diffuse, 1.0);
}
