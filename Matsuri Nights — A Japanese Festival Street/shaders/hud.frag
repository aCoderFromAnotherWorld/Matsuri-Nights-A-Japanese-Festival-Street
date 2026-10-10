#version 330 core
in vec2 TexCoords;
in vec4 Color;

out vec4 FragColor;

uniform sampler2D fontTexture;
uniform int mode; // 0 = solid quad (panel/border), 1 = font glyph

void main()
{
    if (mode == 1)
    {
        float a = texture(fontTexture, TexCoords).r;
        if (a < 0.08)
            discard;

        // Boost text alpha so glyphs are solid, fully opaque, and eye-soothing
        float textAlpha = smoothstep(0.12, 0.45, a) * Color.a;
        FragColor = vec4(Color.rgb, textAlpha);
    }
    else
    {
        FragColor = Color;
    }
}
