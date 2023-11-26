#version 330

// Input
// TODO(student): Get values from vertex shader
// in = de input
in vec3 v_normal;
in vec3 v_color;
in vec3 v_pos;
in vec2 v_coord;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Write pixel out color
    out_color = vec4(v_color, 1);
}
