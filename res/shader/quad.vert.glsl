#version 460 core
layout (location = 0) in vec3 a_Position;

out vec2 textureCoords;

uniform mat4 u_mvp;

void main() {
    gl_Position = u_mvp * vec4(a_Position, 1.0);
    textureCoords = vec2((a_Position.x + 1.0) / 2.0,
			1 - (a_Position.y + 1.0) / 2.0);
}