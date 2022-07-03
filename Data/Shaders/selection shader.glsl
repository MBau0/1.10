#Vertex
#version 450 core

uniform mat4 ortho;
uniform mat4 view;

uniform vec2 start;
uniform vec2 end;

vec2 get_vertex() {
    switch(gl_VertexID) {
        case 0:
            return end;
        case 1:
            return vec2(start.x, end.y);
        case 2:
            return start;
        case 3:
            return vec2(end.x, start.y);
    }

    return vec2(0, 0);
}

void main() {
    vec2 position = get_vertex();
    gl_Position = ortho * vec4(position.x, 1.0 - position.y, 0, 1.0);
}

#End
#Fragment
#version 450 core

layout (location = 0) out vec4 f_color;

void main() {
    f_color = vec4(0.2, 0.2, 0.8, 0.5);
}

#End
