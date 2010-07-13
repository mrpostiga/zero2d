#version 130

uniform sampler2D Texture;
uniform float fade;

in vec2 tex;
out vec4 gl_FragColor;

void main()
{
    //vec3 temp = vec3(texture(Texture, tex));
    //gl_FragColor = vec4(temp, 1.0);
    vec4 color = texture(Texture, tex);
    //color.a *= fade;
    color.a = clamp(color.a, 0.0, 1.0);
    gl_FragColor = color;
}
