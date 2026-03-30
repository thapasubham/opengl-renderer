#version 330 core
in vec3 vertexColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;
void main() { 
    FragColor =  texture(ourTexture2, TexCoord)+ vec4(vertexColor,1.0f);
}
