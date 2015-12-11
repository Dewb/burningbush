#version 120

varying vec4 diffuse,ambient;
varying vec3 lightPos, normal,halfVector;
uniform mat4 normalMatrix;

void main()
{
    /* first transform the normal into eye space and
     normalize the result */
    normal = normalize(mat3(normalMatrix) * gl_Normal);

    lightPos = vec3(-0.3, 1.2, 0.5);

    /* Compute the diffuse, ambient and globalAmbient terms */
    diffuse = gl_Color;
    ambient = vec4(0.1, 0.1, 0.1, 1.0);
    gl_Position = ftransform();

    halfVector = normalize(lightPos + gl_Position.xyz);

}