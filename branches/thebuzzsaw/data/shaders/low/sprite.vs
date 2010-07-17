#version 120

uniform mat4 MVPM;
uniform float z;

attribute vec2 CornerVertex;
attribute vec2 TexCoord;
varying vec2 tex;


void main()
{
    tex = TexCoord;
    gl_Position = MVPM * vec4(CornerVertex, z, 1);
}
