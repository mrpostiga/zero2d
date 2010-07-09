varying vec2 tex;

uniform sampler2D Texture;

void main()
{
    gl_FragColor = texture2D(Texture, tex);
}
