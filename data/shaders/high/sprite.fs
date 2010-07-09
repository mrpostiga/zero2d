#version 130

in vec2 tex;
out vec4 gl_FragColor;

uniform sampler2D Texture;

void main()
{
    //vec3 temp = vec3(texture(Texture, tex));
    //gl_FragColor = vec4(temp, 1.0);
    gl_FragColor = texture(Texture, tex);
}
