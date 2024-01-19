#version 330 core

// Interpolated values from the vertex shaders
in vec3 Position_worldspace;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform vec3 diffuse_color;

void main(){
	color = diffuse_color;
}
