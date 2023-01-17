#version 430 core

in vec3 position;
in vec3 normal;
in vec2 texcoord;

uniform camera {
    mat4 view;
    mat4 proj;
};

uniform mat4 model;

out VS_OUT {
    vec4 vso_position;
    vec3 vso_normal;
    vec2 vso_texcoord;
} vs_out;

void main()
{
    gl_Position = proj * view * model * vec4(position, 1.0);
    vs_out.position = vec4(position + 0.00001 * (vec3(texcoord, 1) + normal), 1.0);
    vs_out.normal = (transpose(inverse(view)) * model * vec4(normal, 0)).xyz;
    vs_out.texcoord = texcoord;
}
