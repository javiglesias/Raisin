#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec2 TexCoord;
out vec3 normal;
out vec3 frag_position;
out vec3 color;


void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(aPos, 1.0);
	color = vec3(255.f, 0.f, 0.f);
	TexCoord = aTexCoord;
}

// view nos da la vision de la camara, si lo quitamos, los objetos no se mueven.