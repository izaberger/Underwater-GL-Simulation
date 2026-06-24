#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(scene, texCoord).rgb;      
    vec3 bloomColor = texture(bloomBlur, texCoord).rgb;
    
    // Dodanie rozmytego światła do sceny
    if(bloom) {
        hdrColor += bloomColor; 
    }
    
    // Mapowanie tonów (Tone mapping)
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    
    // Korekcja gamma
    result = pow(result, vec3(1.0 / gamma));
    
    FragColor = vec4(result, 1.0);
}