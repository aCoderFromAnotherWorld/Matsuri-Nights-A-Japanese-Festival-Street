#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec4 objectColor;
uniform float dayNightFactor; // 0.0 = bright day, 1.0 = festival night
uniform bool isSky;
uniform bool isWindow;
uniform bool isEmissive;
uniform vec3 emissiveColor;

void main()
{
    if (isSky)
    {
        // Smooth gradient sky: vibrant daytime blue to deep festive starry night indigo
        float heightRatio = clamp((FragPos.y + 10.0) / 100.0, 0.0, 1.0);
        vec3 daySky = mix(vec3(0.55, 0.75, 0.98), vec3(0.82, 0.88, 0.98), heightRatio);
        vec3 nightSky = mix(vec3(0.02, 0.03, 0.08), vec3(0.06, 0.08, 0.18), heightRatio);
        vec3 skyColor = mix(daySky, nightSky, dayNightFactor);
        FragColor = vec4(skyColor, 1.0);
        return;
    }

    if (isEmissive)
    {
        // Glowing elements like lanterns or fireworks
        vec3 col = emissiveColor;
        // Make lanterns glow extra bright at night
        col = mix(col * 0.85, col * 1.35, dayNightFactor);
        FragColor = vec4(col, objectColor.a);
        return;
    }

    if (isWindow)
    {
        // Shoji paper screens: bright neutral paper in day, warm glowing yellow-amber at night
        vec3 dayPaper = vec3(0.90, 0.88, 0.82);
        vec3 nightGlow = vec3(1.0, 0.82, 0.45);
        vec3 winColor = mix(dayPaper, nightGlow, dayNightFactor);
        FragColor = vec4(winColor, 1.0);
        return;
    }

    // Default object shading:
    // Directional sunlight / moonlight provides natural 3D depth to all primitives
    vec3 sunDir = normalize(vec3(0.4, 0.8, 0.5));
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, sunDir), 0.0);

    // Day/night ambient and diffuse weighting
    float ambientStrength = mix(0.45, 0.18, dayNightFactor);
    float lightIntensity = mix(0.65, 0.35, dayNightFactor);
    float lighting = ambientStrength + diff * lightIntensity;

    // Apply lighting to object base color
    vec3 result = objectColor.rgb * lighting;
    FragColor = vec4(result, objectColor.a);
}
