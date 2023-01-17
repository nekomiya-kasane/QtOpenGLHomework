#version 430 core

in VS_OUT {
    vec4 position;
    vec3 normal;
    vec2 texcoord;
} gs_in;

vec3 position;

out vec4 FragColor;

void main() {
    if (position.x > 0 && position.y > 0 && position.z > 0) {
        FragColor = vec4(1, 1, 1, 1);
    }
    else if (position.x < 0 && position.y < 0 && position.z < 0) {
        FragColor = vec4(0, 0, 0, 1);
    }
    else if (position.x > 0 && position.y < 0 && position.z < 0) {
        FragColor = vec4(1, 0, 0, 1);
    }
    else if (position.x > 0 && position.y > 0 && position.z < 0) {
        FragColor = vec4(1, 1, 0, 1);
    }
    else if (position.x > 0 && position.y < 0 && position.z > 0) {
        FragColor = vec4(1, 0, 1, 1);
    }
    else if (position.x < 0 && position.y > 0 && position.z > 0) {
        FragColor = vec4(0, 1, 1, 1);
    }
    else if (position.x < 0 && position.y > 0 && position.z < 0) {
        FragColor = vec4(0, 1, 0, 1);
    }
    else {
        FragColor = vec4(0, 0, 1, 1);
    }
}
