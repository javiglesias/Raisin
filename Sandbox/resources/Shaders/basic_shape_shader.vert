#version 460 core
layout (location = 0) in vec3 aPos;

out vec3 color;


void main()
{
	gl_Position = vec4(aPos, 1.f);
	color = vec3(255.f, 0.f, 0.f);
}

// view nos da la vision de la camara, si lo quitamos, los objetos no se mueven.