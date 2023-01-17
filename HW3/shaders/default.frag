#version 430 core
in vec4 vso_position;
in vec3 vso_normal;
in vec2 vso_texcoord;
in vec3 vso_color;

out vec4 FragColor;

void main() {
    vec4 position = vso_position;

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
