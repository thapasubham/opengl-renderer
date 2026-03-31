#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec2 aTexCoord;

out vec3 vertexColor;
out vec2 TexCoord;

uniform float timeValue;
uniform vec2 mousePos;

void main() { 
    vec3 center = vec3(0.0, -0.166, 0.2); 
     float offset = float(gl_InstanceID) * 0.3;
    vec3 instanceOffset = vec3(offset, 0.0, 0.0);
    vec3 pos = aPos - center+ instanceOffset;

    float angle = timeValue;
    float sinA = sin(angle);
    float cosA = cos(angle);

    mat3 rotation = mat3(
        cosA,  sinA,0.0, 
       sinA, cosA, 0.0 , 
       0.0,   0.0, 1.0
       
    );
    mat3 rotationZ = mat3(
        cosA,  0.0, sinA,       
       0.0,   1.0, 0.0,
       sinA,  0.0 ,cosA
    );
    //pos.z += sin(timeValue) * 0.5;
   pos =  pos*rotation;

    
    gl_Position = vec4(pos + center, 1.0);

    vertexColor = ((pos + aPos) / 2.0) + 0.5; 
    TexCoord = aTexCoord;
}
