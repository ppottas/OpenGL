#version 430


layout (location = 0) in vec3 position;

out vec2 pos;


void main() {

pos = vec2(position.x*0.5+0.5,position.y*0.5+0.5);


gl_Position = vec4(position,1.0);


}