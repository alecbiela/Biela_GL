#version 430

layout (location = 0) in vec3 modelXYZ;
layout (location = 1) in vec2 modelUV;
layout (location = 2) in vec3 normalXYZ;
layout (location = 6) uniform mat4 world;
layout (location = 10) uniform mat4 camera;

out vec2 uv;


void main()
{
	uv = modelUV;
	gl_Position = camera * world * vec4(modelXYZ, 1);
}