#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;
layout (location = 4) in mat4 instanceMatrix;

out vec3 color;
out vec2 texCoord;

uniform mat4 camMatrix;
uniform float time;

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main()
{
	float wind = 0.0f;
	if (aPos.y > 0) {
		wind = sin(time + random(vec2(gl_InstanceID))) / 2.0f;
	}
	gl_Position = camMatrix * instanceMatrix * vec4(aPos + vec3(wind, 0.0f, 0.0f), 1.0f);
	color = aColor;
	texCoord = aTex;
}