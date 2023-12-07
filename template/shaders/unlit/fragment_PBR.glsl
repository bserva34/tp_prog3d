#version 330 core

#define M_M_PI 3.14159265359

in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec2 o_uv0;
in mat3 tbm;

out vec4 FragColor;


//uniform Material material;
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform vec3 light_pos;
uniform vec3 camera;

vec3 FresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = M_M_PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

void main() {
    float M_PI=3.14159265359;
    vec3 lightColors = vec3(1,1,1);
    // Calcul des vecteurs de vue et de lumière...
    //vec3 albedo = pow(texture(albedoMap,o_uv0).rgb,2.2);
    vec3 albedo = vec3(pow(texture(albedoMap,o_uv0).r,2.2),
        pow(texture(albedoMap,o_uv0).g,2.2),
        pow(texture(albedoMap,o_uv0).b,2.2));

    vec3 n_color = texture(normalMap, o_uv0).xyz;
    n_color=n_color * 2.0 - 1.0;
    vec3 normal = normalize(tbm*n_color);

    //float metallic = texture(metallicMap,o_uv0).r;
    /*
    float metallic =1.f;
    float roughness = texture(roughnessMap,o_uv0).r;*/
    
    float metallic =0.f;
    float roughness = texture(metallicMap,o_uv0).r;

    vec3 N = normalize(normal);
    vec3 V = normalize(camera - o_positionWorld);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
               
    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 1; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(light_pos - o_positionWorld);
        vec3 H = normalize(V + L);
        float distance    = length(light_pos - o_positionWorld);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     =vec3(lightColors[0] * attenuation,
                            lightColors[1] * attenuation,
                            lightColors[2] * attenuation);        
        
        // cook-torrance brdf
        float NDF = DistributionGGX(N, H, roughness);        
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = FresnelSchlick(max(dot(H, V), 0.0), F0);       
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;     
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular     = numerator / denominator;  
            
        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);                
        Lo += (kD * albedo / M_PI + specular) * radiance * NdotL; 
    }   
  
    //vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 ambient = vec3(0.03) * albedo;
    vec3 color = ambient + Lo;
    
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  
   
    FragColor = vec4(color, 1.0);
}
