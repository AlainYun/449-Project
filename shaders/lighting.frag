#version 330
// A fragment shader for rendering fragments in the Phong reflection model.
layout (location=0) out vec4 FragColor;

// Inputs: the texture coordinates, world-space normal, and world-space position
// of this fragment, interpolated between its vertices.
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragWorldPos;

struct Light {
    vec3 position;  
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};
// Uniforms: MUST BE PROVIDED BY THE APPLICATION.

// The mesh's base (diffuse) texture.
uniform sampler2D baseTexture;

// Material parameters for the whole mesh: k_a, k_d, k_s, shininess.
uniform vec4 material; // k_a, k_d, k_s, and shininess
uniform sampler2D diffuseMap; // This is the “texture map”, giving the diffuse color of the fragments.
uniform sampler2D specularMap;
uniform sampler2D normalMap;


// Location of the camera.
uniform vec3 viewPos;
uniform Light light;


void main() {
// ambient
    vec3 ambient = material.x * light.ambient;
// diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragWorldPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.y;
// specular
    vec3 eyeDir = normalize(viewPos - FragWorldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(eyeDir, reflectDir), 0.0), material.w);
    vec3 specular = light.specular * spec * pow(spec, material.z);
// attenuation
    float distance = length(light.position - FragWorldPos); 
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

    //ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation; 

    vec3 lightIntensity = ambient + diffuse + specular;
    FragColor = vec4(lightIntensity, 1.0) * texture(baseTexture, TexCoord);
}