#version 130

uniform float Time;
uniform mat4 MVPM;

in vec4 MCVertex;
in vec4 MColor;
in vec3 Velocity;
in float StartTime;
out vec4 Color;

void main()
{
    vec4 vert;
    float t = Time - StartTime;
    
    if (t >= 0.0)
    {
        t = mod(t, 1.0);
        vert = MCVertex + vec4(Velocity * t, 0.0);
        vert.y -= 4.9 * t * t;
        Color = MColor;
    }
    else
    {
        vert = MCVertex;
        Color = vec4(0.0, 0.0, 0.0, 0.0);
    }
    
    //Color.a = 0.5;
    
    gl_Position = MVPM * vert;
}
