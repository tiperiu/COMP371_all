#version 330

in vec3 normal;
in vec3 fragPosition;

out vec3 color;

uniform vec3 viewPos = vec3(0, 0, 0);

uniform vec3 objectColor = vec3(1, 0, 0);

uniform vec3 lightPos = vec3(0, 1, 0);
uniform vec3 lightColor = vec3(1, 1, 1);

void main()
{
    vec3 lightDir = normalize(lightPos - fragPosition);
    float NdotL = max(dot(normal, lightDir), 0); 

    vec3 diffuse;

    // NOTE: This if-statement can be replaced with a color ramp image
    if(NdotL > 0.9)
    {
        diffuse = vec3(1, 1, 1);
    } else if (NdotL > 0.75) {
        diffuse = vec3(0.75, 0.75, 0.75);
    } else if (NdotL > 0.5) {
        diffuse = vec3(0.5, 0.5, 0.5);
    } else {
        diffuse = vec3(0.1, 0.1, 0.1);
    }

    color = diffuse * objectColor;
}
