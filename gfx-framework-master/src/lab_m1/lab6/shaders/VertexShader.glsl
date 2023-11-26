#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textcoord;
layout (location = 3) in vec3 color;

// Uniform properties -> input global (si pt fragment)
// info globale
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output
// TODO(student): Output values to fragment shader
// out = de output
out vec3 v_color;
out vec2 v_coord;
out vec3 v_normal;
out vec3 v_pos;

void main()
{
     // TODO(student): Send output to fragment shader
     v_color = color + vec3(sin(Time), cos(Time), sin(Time) + cos(Time));

     v_pos = position;

     v_normal = normal + vec3(sin(Time), cos(Time), sin(Time) + cos(Time));
     //v_normal = color;

     v_coord = textcoord;

    // TODO(student): Compute gl_Position
    // sp ->(toate transf) NDC
    //gl_Position = Projection * View * Model * vec4(position, 1);

   

	// compute gl_Position
	vec4 vec_final_position = Model * vec4(v_pos, 1.f) + vec4(0.f, cos(Time), 0.f, 0.f) * 2.f;
	gl_Position	= Projection * View * vec_final_position;
}
