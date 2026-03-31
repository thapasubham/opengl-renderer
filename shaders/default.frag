#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec2 mousePos;

void main()
{
    vec4 texColor = texture(ourTexture, TexCoord);
    vec4 mouseEffect = vec4(mousePos.x * 0.5 + 0.5, mousePos.y * 0.5 + 0.5, 0.5, 1.0);
    FragColor = texColor * vec4(ourColor, 1.0) ;
}
