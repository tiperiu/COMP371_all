#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;

out vec3 normal;
out vec3 fragPosition;

uniform mat4 m = mat4(1.0);
uniform mat4 mvp = mat4(1.0);

void main()
{
    gl_Position = mvp * vec4(pos, 1);

    normal = normalize(mat3(transpose(inverse(m))) * norm);

    fragPosition = vec3(m * vec4(pos, 1));
}
