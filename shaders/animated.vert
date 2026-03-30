#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoord;

out vec3 vertexColor;
out vec2 TexCoord;

uniform float timeValue;

void main() { 
    vec3 center = vec3(0.0, 0.0, 0.0);
    vec3 pos = aPos - center;

    float angle = timeValue;
    float sinA = sin(angle);
    float cosA = cos(angle);

    mat3 rotation = mat3(
        cosA,  0.0, sinA,
        0.0,   1.0, 0.0,
        -sinA, 0.0, cosA
    );
    
    pos.z += sin(timeValue) * 0.5;
    pos = rotation * pos;

    // Optional circular offset
    vec2 offset = vec2(sin(timeValue) * 0.5, cos(timeValue) * 0.5);
    pos.xy += offset;

    gl_Position = vec4(pos + center, 1.0);

    vertexColor = ((pos + aPos) / 2.0) + 0.5; 
    TexCoord = aTexCoord;
}
