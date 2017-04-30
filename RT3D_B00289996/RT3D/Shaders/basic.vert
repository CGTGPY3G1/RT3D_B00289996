// basic.vert
#version 330

uniform mat4 projection;

in vec3 position;
in vec3 colour;
out vec3 ex_Color;


void main(void) {
 vec4 vertexPosition = vec4(position,1.0);
 gl_Position = projection * vertexPosition;
 ex_Color = colour;
}