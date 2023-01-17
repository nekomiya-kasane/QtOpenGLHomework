#version 430 core

in vec3 position;
in vec3 normal;
in vec2 texcoord;
in vec3 color;

uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;

out vec4 vso_position;
out vec3 vso_normal;
out vec2 vso_texcoord;
out vec3 vso_color;

void main()
{
    gl_Position = vec4(position, 1.0);
    vso_position = gl_Position;
    vso_normal = (transpose(inverse(view)) * model * vec4(normal, 0)).xyz;
    vso_texcoord = texcoord;
    vso_color = color;
}
