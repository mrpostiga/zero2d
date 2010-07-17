#version 120

uniform sampler2D Texture;
uniform float fade;

varying vec2 tex;

void main()
{
    vec4 color = texture2D(Texture, tex);
    color.a *= fade;
    color.a = clamp(color.a, 0.0, 1.0);
    gl_FragColor = color;
}
