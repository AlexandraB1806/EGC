#version 330

// Input
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_texture_coord;
layout (location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 CarPos;

// Output
out vec3 fragment_color;

void main()
{
    fragment_color = v_color;

	gl_Position	= Projection * View * Model * vec4(v_position, 1.0);
	float d = length((CarPos, 1) - gl_Position);
	gl_Position.y = gl_Position.y - d * d * 0.02;
}
