#version 430 core

in vec3 position;
in vec3 normal;
in vec2 texcoord;
in vec3 color;

uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;
uniform vec3 scaling = vec3(1, 1, 1);

out vec4 vso_position;
out vec3 vso_normal;
out vec2 vso_texcoord;
out vec3 vso_color;
out vec3 vso_model_scaling;

void main()
{
    gl_Position = proj * view * vec4(position, 1.0);
    vso_position = vec4(position, 1.0);
    vso_normal = (transpose(inverse(view)) * model * vec4(normal, 0)).xyz;
    vso_texcoord = texcoord;
    vso_color = color;
    vso_model_scaling = scaling;
}
