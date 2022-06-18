#Vertex
#version 450 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in float height;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec2 size;
uniform vec2 num_tiles;
uniform vec2 highlight;

out float frag_highlight;

bool collision(vec2 a, vec4 b) {
    return (a.x >= b.x &&
            a.x <= b.x + b.z &&
            a.y >= b.y &&
            a.y <= b.y + b.w);
}

void main() {
	vec2 position = vec2(gl_InstanceID % int(num_tiles.x), gl_InstanceID / int(num_tiles.y));
    position = vec2((vertex.x + position.x) * size.x, (vertex.y + position.y) * size.y);
	gl_Position = projection * view * vec4(position.x, height + 0.4, position.y, 1.0);
    vec2 high = vec2(highlight.x * size.x, highlight.y * size.y);
    if( collision( position, vec4(high, size))) {
        frag_highlight = 1.0f;
    }
    else {
        frag_highlight = 0.0f;
    } 
}
#End
#Fragment
#version 450 core

in float frag_highlight;

layout (location = 0) out vec4 f_color;

void main() {
    if(frag_highlight == 1.0f) {
        f_color = vec4(0, 1, 1, 1);
    }
    else {
 	    f_color = vec4(.9, .9, .9, 1);
    }   
}
#End
