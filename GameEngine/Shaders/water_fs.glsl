#version 400

in vec2 textureCoord; 
in vec3 norm;
in vec3 fragPos;

out vec4 fragColor;

uniform sampler2D texture1;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float opacity;

void main()
{
	//Ambient light
	float ambientStrength = 0.9f;
    vec3 ambient = ambientStrength * lightColor;
	//vec3 objectColor = vec3(1.0f, 0.5f, 0.31f);

	//vec3 result1 = ambient * objectColor;
	//fragColor = vec4(result, 1.0f);

	//Diffuse light
	vec3 normal = normalize(norm);
	vec3 lightDir = normalize(lightPos - fragPos); 

	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;
	//vec3 result2 = diffuse * objectColor;

	//Specular light
	float specularStrength = 0.7;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specularStrength * spec * lightColor; 
	//vec3 result3 = specular * objectColor;

	vec3 result = ambient + diffuse + specular;
	//vec3 result = result1 + result2 + result3;;
	fragColor = vec4(result, opacity);
	fragColor = fragColor * texture(texture1, textureCoord);
}