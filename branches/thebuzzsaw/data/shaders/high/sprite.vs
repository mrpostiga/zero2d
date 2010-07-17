#version 130

uniform mat4 MVPM;
uniform float z;

in vec2 CornerVertex;
in vec2 TexCoord;
out vec2 tex;


void main()
{
    tex = TexCoord;
    gl_Position = MVPM * vec4(CornerVertex, z, 1);
}
