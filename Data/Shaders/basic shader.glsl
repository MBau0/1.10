#Vertex
#version 450 core

layout (location = 0) in vec3 vertex;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out float height;

void main() {
	gl_Position = projection * view * model * vec4(vertex.xyz, 1.0);
	height = vertex.y;
}
#End
#Fragment
#version 450 core

layout (location = 0) out vec4 f_color;

in float height;

void main() {
	f_color = vec4(.1, .2 + (height / 100), .1, 1);
}
#End
