#version 400

in vec2 textureCoord; 
in vec3 norm;
in vec3 fragPos;

out vec4 fragColor;

uniform sampler2D texture1;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	//Ambient light
	float ambientStrength = 0.5f;
    vec3 ambient = ambientStrength * vec3(0.7f, 1.3f, 2.1f);;

	//Diffuse light
	vec3 normal = normalize(norm);
	vec3 lightDir = normalize(lightPos - fragPos); 
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * vec3 (1.35f, 2.06f, 2.5f);

	//Specular light
	float specularStrength = 1.9;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specularStrength * spec * lightColor; 

	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result, 0.3f);
	fragColor = fragColor * texture(texture1, textureCoord);
}