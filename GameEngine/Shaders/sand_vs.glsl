#version 400

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 texCoord;

out vec2 textureCoord;
out vec3 norm;
out vec3 fragPos;

uniform mat4 MVP;
uniform mat4 model;

void main()
{
	textureCoord = texCoord;
	vec3 pos2=pos;
	pos2.y=2 * 3 * pow((sin( dot( vec2(3.0f, 5.0f), vec2(pos2.x, pos2.z)) * 6) + 1)/2, 2);
	fragPos = vec3(model * vec4(pos, 1.0f));
	norm = mat3(transpose(inverse(model)))*normals;
	gl_Position = MVP * vec4(pos2, 1.0f);
}