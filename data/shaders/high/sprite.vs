#version 130

uniform mat4 MVPM;

in vec4 MCVertex;
in vec2 TexCoord;
out vec2 tex;


void main()
{
    tex = TexCoord;
    gl_Position = MVPM * MCVertex;
}
