#version 430

in vec2 uv;
uniform sampler2D myTexture;

void main()
{
	gl_FragColor = texture(myTexture, uv);
}