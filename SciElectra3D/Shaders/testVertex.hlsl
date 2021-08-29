#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoords;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Bitangent;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 fNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(Position, 1.0);
	TexCoord = vec2(TexCoords.x, TexCoords.y);
	FragPos = vec3(model * vec4(Position, 1.0));
	fNormal = mat3(transpose(inverse(model))) * Normal;
}