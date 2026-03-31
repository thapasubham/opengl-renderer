#version 330 core
in vec3 vertexColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;
uniform vec2 mousePos;
uniform float mixValue;
void main() { 
    vec4 mouseEffect = vec4(mousePos.x * 0.5 + 0.5, mousePos.y * 0.5 + 0.5, 0.5, 1.0);
	float stuffMix= (mousePos.x+mousePos.y+mixValue)/2+0.5;
	FragColor = mix(texture(ourTexture, TexCoord)+vec4(vertexColor,1.0), texture(ourTexture2, TexCoord), stuffMix);
}
