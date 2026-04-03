#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in vec3 LocalPos;

uniform sampler2D ourTexture;
void main()
{
    // Visualize 3D coordinates (X, Y, Z) mapped to RGB
    // Offset and scale -0.5..0.5 to 0.0..1.0 for visualization
    FragColor = vec4(LocalPos.x + 0.5, LocalPos.y + 0.5, LocalPos.z + 0.5, 1.0);
}
