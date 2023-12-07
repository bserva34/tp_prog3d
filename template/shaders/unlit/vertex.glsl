#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 uv0;
layout(location = 4) in vec3 bitangeant;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//uniform mat4 mvp;
//uniform mat4 modelView;
//uniform mat4 normalMatrix;


out vec3 o_positionWorld;
out vec3 o_normalWorld;
out vec2 o_uv0;
out mat3 tbm;

void main() {
  mat3 normalMatrix = mat3(transpose(inverse(model)));

  vec3 t= normalize(normalMatrix*tangent);
  vec3 b=normalize(normalMatrix*bitangeant);
  vec3 n=normalize(normalMatrix*normal);
  tbm=transpose(mat3(t,b,n));

  o_uv0 = uv0;
  //o_uv0.y=1-uv0.y;
  vec4 positionWorld = model * vec4(position, 1.0);
  o_positionWorld = positionWorld.xyz;
  o_normalWorld = normalMatrix * normal;
  gl_Position = projection * view * model * vec4(position,1.0);

}
