varying vec4 diffuse,ambient;
varying vec3 lightPos,normal,halfVector;

void main()
{
    vec3 n,halfV,lightDir;
    float NdotL,NdotHV;

    lightDir = normalize(lightPos);
    float specular = 0.01;
    float shininess = 0.05;

    /* The ambient term will always be present */
    vec4 color = ambient;


    n = normalize(normal);
    
    NdotL = max(dot(n,lightDir),0.0);

    if (NdotL > 0.0) {
        color += diffuse * NdotL;
        halfV = normalize(halfVector);
        NdotHV = max(dot(n,halfV),0.0);
        color += specular *
        pow(NdotHV, shininess);
    }

    gl_FragColor = color;

}