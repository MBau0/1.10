#Vertex
#version 450 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 out_uv;

void main() {
    gl_Position = projection * view * model * vec4(vertex.xyz, 1.0);
    out_uv = uv;
}

#End
#Fragment
#version 450 core

in vec2 out_uv;

layout (location = 0) out vec4 f_color;

uniform sampler2D texture_sampler;

void main() {
    f_color = texture(texture_sampler, out_uv);
}

#End
