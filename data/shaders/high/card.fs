#version 130

uniform sampler2D CardTexture;
uniform bool UseTexture;
uniform vec4 CardColor;

in vec2 TextureCoordinate;
out vec4 gl_FragColor;

void main()
{
    if (UseTexture)
        gl_FragColor = texture(CardTexture, TextureCoordinate);
    else
        gl_FragColor = CardColor;
}
