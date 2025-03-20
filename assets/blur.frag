uniform sampler2D texture;
uniform float blur_radius;

void main()
{
    vec2 textureCoordinates = gl_TexCoord[0].xy;
    vec2 resolution = vec2(800.0, 600.0); 
    
    vec4 color = vec4(0.0);
    float samples = 0.0;
    
    for (float x = -blur_radius; x <= blur_radius; x += 1.0) {
        for (float y = -blur_radius; y <= blur_radius; y += 1.0) {
            vec2 offset = vec2(x, y) / resolution;
            color += texture2D(texture, textureCoordinates + offset);
            samples += 1.0;
        }
    }
    
    gl_FragColor = color / samples;
    gl_FragColor.a = 1.0;
}
