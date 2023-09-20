#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;
layout (location = 4) in mat4 instanceMatrix;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec2 texCoord;
out vec3 normal;
out vec3 frag_position;

void main()
{	
	vec4 current_position = vec4(aPos.x + gl_InstanceID, aPos.y, gl_InstanceID + aPos.z, 1.0); 
	// position where draw the current instance
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * current_position;
	texCoord = aTexCoord;
	frag_position = aPos;
	normal = aNormal;
}