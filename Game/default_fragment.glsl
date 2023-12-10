#version 460

out vec4 color;

in vec2 uv0;

layout(binding=5) uniform sampler2D mytexture;

void main()
{
	color = vec4(0.5, 0.0, 0.5, 1.0);
	color = texture2D (mytexture,uv0);
}