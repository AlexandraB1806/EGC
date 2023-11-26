#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;

// TODO(student): Declare various other uniforms
uniform int is_mixed;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    //out_color = vec4(1);

    vec4 first_color = texture2D(texture_1, texcoord);
	vec4 second_color = texture2D(texture_2, texcoord);

    if (first_color.a < 0.5f) {
		discard;
	}

	if (second_color.a < 0.5f) {
		discard;
	}

	if (is_mixed == 1) {
		out_color = mix(first_color, second_color, 0.5f);
	} else {
		out_color = first_color;
	}
}
