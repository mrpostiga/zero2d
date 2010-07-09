#version 130

in vec2 tex;
out vec4 gl_FragColor;

uniform sampler2D Texture;

void main()
{
    gl_FragColor = texture(Texture, tex);
}
