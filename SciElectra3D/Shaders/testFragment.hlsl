#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;


uniform sampler2D texture_diffuse1;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;


float ambientStrength =0.8;
float specularStrength = 0.6;

void main()
{
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient+ diffuse + specular) ;


	
	FragColor =  vec4(result,1.0)*texture(texture_diffuse1, TexCoord);
}
