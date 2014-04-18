#version 150

// Input vertex data, different for all executions of this shader.
in vec3 vertex_pos;

void main(){
	gl_Position = vec4(vertex_pos, 1.0);
}
