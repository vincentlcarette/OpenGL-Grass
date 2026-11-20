#version 330 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

uniform sampler2D tex0;


void main()
{
	if (texture(tex0, texCoord).a < 0.5f) {
		discard;
	}

	FragColor = texture(tex0, texCoord);
}