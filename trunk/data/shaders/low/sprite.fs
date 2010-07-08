varying vec2 tex;

uniform sampler2D Texture;

void main()
{
    vec3 Color = vec3(texture2D(Texture, tex));
    gl_FragColor = vec4(Color, 1.0);
}
