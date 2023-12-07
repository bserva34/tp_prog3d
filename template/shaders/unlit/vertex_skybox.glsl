#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 uv0;
layout(location = 4) in vec3 bitangeant;

uniform mat4 projection;
uniform mat4 view;


out vec3 tc;


void main() {
  tc=position;
  gl_Position = projection * view * vec4(position, 1.0);
  

}
