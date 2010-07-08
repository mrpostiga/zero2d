uniform mat4 MVPM;

attribute vec4 MCVertex;
attribute vec2 TexCoord;
varying vec2 tex;


void main()
{
    tex = TexCoord;
    gl_Position = MVPM * MCVertex;
}
