#version 330
layout (location=0) out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragWorldPos;

struct DirLight{
	vec3 direction;
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 2a
#define NR_SPOT_LIGHTS 4
uniform vec3 viewPos;

uniform SpotLight spotLight[NR_SPOT_LIGHTS];
uniform PointLight pointLight;
uniform DirLight dirLight;
uniform sampler2D baseTexture;

uniform vec4 material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(){
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragWorldPos);
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	
	result += CalcPointLight(pointLight, norm, FragWorldPos, viewDir);
	result += CalcSpotLight(spotLight[0], norm, FragWorldPos, viewDir);
	result += CalcSpotLight(spotLight[1], norm, FragWorldPos, viewDir);
	FragColor = vec4(result,1.0) * texture(baseTexture, TexCoord);
	// FragColor = vec4(norm,1);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.w);
	vec3 ambient = light.ambient * material.y;
	vec3 diffuse = light.diffuse * diff * material.y;
	vec3 specular = light.specular * spec * material.z;
     
	return(ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.w);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * material.y;
    vec3 diffuse = light.diffuse * diff * material.y;
    vec3 specular = light.specular * spec * material.z;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.w);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * material.y;
    vec3 diffuse = light.diffuse * diff * material.y;
    vec3 specular = light.specular * spec * material.z;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}