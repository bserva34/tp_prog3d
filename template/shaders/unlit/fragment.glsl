#version 330 core

in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec2 o_uv0;
in mat3 tbm;
out vec4 FragColor;

uniform vec4 color;
uniform sampler2D colorTexture;
uniform sampler2D normalMapTexture;

uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 camera;


void main() {
    //texture classique
    FragColor = texture(colorTexture,o_uv0)*color;

    /*
    //phong + normal map
    vec3 n_color = texture(normalMapTexture, o_uv0).xyz;
    n_color=n_color * 2.0 - 1.0;
    vec3 normal = normalize(tbm*n_color);


    vec3 ambiante=0.5*light_color;
    vec3 L=light_pos-o_positionWorld;
    vec3 diffuse=light_color*dot(L,normal);
    
    vec3 R=2*dot(normal,L)*normal-L;
    R=normalize(R);
    vec3 V=camera-o_positionWorld;
    V=normalize(V);
    vec3 spec=light_color*0.5*pow(max(dot(R,V),0.f),16);

    vec3 acc = (ambiante+diffuse+spec) * texture(colorTexture,o_uv0).rgb;
    FragColor=vec4(acc,1.0);*/

    // DEBUG: position
    //FragColor = vec4(o_positionWorld, 1.0);
    // DEBUG: normal
    //FragColor = vec4(0.5 * o_normalWorld + vec3(0.5) , 1.0);
    // DEBUG: uv0
    // FragColor = vec4(o_uv0, 1.0);
}
