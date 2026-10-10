#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};

struct Material {
    float shininess;
    float specularStrength;
};

#define NR_POINT_LIGHTS 14

uniform vec4 objectColor;
uniform float dayNightFactor; // 0.0 = bright day, 1.0 = festival night
uniform float totalTime;
uniform bool isSky;
uniform bool isWindow;
uniform bool isEmissive;
uniform vec3 emissiveColor;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform int numActivePointLights;
uniform SpotLight spotLight;
uniform bool spotLightActive;

uniform Material material;
uniform int shadingMode; // 0 = Blinn-Phong, 1 = Diffuse Only (Lambert), 2 = Ambient Only (Flat)

// Phase 3: Texturing uniforms
uniform bool enableTextures; // Global toggle
uniform bool useTexture;      // Per-node
uniform sampler2D diffuseTexture;
uniform float textureTiling;

// Realistic Shadows: PCF Shadow Map Uniforms
uniform sampler2D shadowMap;
uniform bool enableShadows;

float calculateShadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    if (!enableShadows)
        return 0.0;

    // Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0, 1] range
    projCoords = projCoords * 0.5 + 0.5;

    // If fragment is outside light frustum bounds, no shadow
    if (projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0 || projCoords.z > 1.0)
        return 0.0;

    // Adaptive slope-scale depth bias to eliminate shadow acne
    float bias = max(0.0035 * (1.0 - dot(normal, lightDir)), 0.0006);

    // 16-sample Percentage-Closer Filtering (PCF) with smooth disc
    float shadow = 0.0;
    const vec2 texelSize = vec2(1.0 / 2048.0);
    for(int x = -1; x <= 2; ++x)
    {
        for(int y = -1; y <= 2; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize * 1.2).r;
            shadow += (projCoords.z - bias > pcfDepth) ? 1.0 : 0.0;
        }
    }
    shadow /= 16.0;

    return shadow;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffColor, float shadow)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Outer light penetration & indoor ambient bounce through windows/doors
    vec3 indoorBounce = mix(vec3(0.48, 0.45, 0.40), vec3(0.18, 0.16, 0.14), dayNightFactor) * diffColor;
    vec3 ambient = max(light.ambient * diffColor, indoorBounce);
    vec3 diffuse = (1.0 - shadow) * light.diffuse * diff * diffColor;
    vec3 specular = vec3(0.0);

    if (shadingMode == 0 && diff > 0.0 && material.specularStrength > 0.0 && shadow < 1.0)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), max(material.shininess, 1.0));
        specular = (1.0 - shadow) * light.specular * (spec * material.specularStrength);
    }

    if (shadingMode == 1)
        return ambient + diffuse;
    if (shadingMode == 2)
        return ambient;
    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffColor)
{
    vec3 toLight = light.position - fragPos;
    float distSq = dot(toLight, toLight);
    // Early cutoff if beyond light's effective reach (dist > ~26m)
    if (distSq > 700.0)
        return vec3(0.0);

    float distance = sqrt(distSq);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distSq);
    if (attenuation < 0.005)
        return vec3(0.0);

    vec3 lightDir = toLight / distance;
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 ambient = light.ambient * diffColor * attenuation;
    vec3 diffuse = light.diffuse * diff * diffColor * attenuation;
    vec3 specular = vec3(0.0);

    if (shadingMode == 0 && diff > 0.0 && material.specularStrength > 0.0)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), max(material.shininess, 1.0));
        specular = light.specular * (spec * material.specularStrength) * attenuation;
    }

    if (shadingMode == 1)
        return ambient + diffuse;
    if (shadingMode == 2)
        return ambient;
    return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffColor)
{
    vec3 toLight = light.position - fragPos;
    float distSq = dot(toLight, toLight);
    if (distSq > 900.0)
        return vec3(0.0);

    float distance = sqrt(distSq);
    vec3 lightDir = toLight / distance;

    // Spot cone intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float spotIntensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    if (spotIntensity <= 0.0)
        return vec3(0.0);

    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distSq);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 ambient = light.ambient * diffColor * attenuation;
    vec3 diffuse = light.diffuse * diff * diffColor * attenuation * spotIntensity;
    vec3 specular = vec3(0.0);

    if (shadingMode == 0 && diff > 0.0 && material.specularStrength > 0.0)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), max(material.shininess, 1.0));
        specular = light.specular * (spec * material.specularStrength) * attenuation * spotIntensity;
    }

    if (shadingMode == 1)
        return ambient + diffuse;
    if (shadingMode == 2)
        return ambient;
    return ambient + diffuse + specular;
}

void main()
{
    if (isSky)
    {
        // 1. Normalized celestial gaze vector (pure direction, infinite position)
        vec3 V = normalize(FragPos - viewPos);
        float h = clamp((V.y + 0.15) / 1.15, 0.0, 1.0);

        // Dynamic Sky Gradient
        vec3 daySky = mix(vec3(0.52, 0.72, 0.96), vec3(0.78, 0.86, 0.98), h);
        vec3 nightSky = mix(vec3(0.015, 0.02, 0.06), vec3(0.04, 0.06, 0.15), h);
        vec3 skyColor = mix(daySky, nightSky, dayNightFactor);

        // 2. CELESTIAL SUN (Radiant golden solar disk + atmospheric corona bloom at infinity)
        vec3 sunDir = normalize(vec3(-0.40, 0.85, 0.50));
        float sunDot = dot(V, sunDir);
        if (sunDot > 0.0)
        {
            float sunCorona = pow(sunDot, 64.0) * 1.10 + pow(sunDot, 6.0) * 0.40;
            vec3 coronaCol = vec3(1.25, 0.98, 0.65) * sunCorona;
            float sunDisk = smoothstep(0.9972, 0.9985, sunDot);
            vec3 sunCore = vec3(2.6, 2.3, 1.8) * sunDisk;
            skyColor += (coronaCol + sunCore) * (1.0 - dayNightFactor);
        }

        // 3. CELESTIAL MOON (Silvery-white lunar disk with craters & ethereal halo)
        vec3 moonDir = normalize(vec3(0.35, 0.75, -0.40));
        float moonDot = dot(V, moonDir);
        if (moonDot > 0.0)
        {
            float moonHalo = pow(moonDot, 80.0) * 0.95 + pow(moonDot, 10.0) * 0.28;
            vec3 haloCol = vec3(0.40, 0.60, 0.95) * moonHalo;

            if (moonDot > 0.9975)
            {
                float diskU = (V.x - moonDir.x) * 45.0 + 0.5;
                float diskV = (V.y - moonDir.y) * 45.0 + 0.5;
                float crater = sin(diskU * 16.0) * cos(diskV * 18.0) * 0.12 +
                               sin(diskU * 32.0 + diskV * 28.0) * 0.08;
                float moonEdge = smoothstep(0.9975, 0.9984, moonDot);
                vec3 moonDiskCol = vec3(0.94, 0.96, 1.05) * (0.88 + crater) * 1.9;
                skyColor += (haloCol + moonDiskCol * moonEdge) * dayNightFactor;
            }
            else
            {
                skyColor += haloCol * dayNightFactor;
            }
        }

        // 4. TWINKLING STARS IN FESTIVAL NIGHT SKY
        if (dayNightFactor > 0.04 && V.y > -0.04)
        {
            // Primary Sparkling Constellations
            vec3 sCoord = V * 160.0;
            vec3 cell = floor(sCoord);
            vec3 f = fract(sCoord) - vec3(0.5);

            float starSeed = fract(sin(dot(cell, vec3(127.1, 311.7, 74.7))) * 43758.5453);
            if (starSeed > 0.82)
            {
                float starDist = length(f);
                float starRadius = 0.07 + 0.08 * fract(starSeed * 13.3);
                if (starDist < starRadius)
                {
                    float starBrightness = smoothstep(starRadius, 0.0, starDist);
                    float twinkle = 0.65 + 0.35 * sin(totalTime * (2.5 + starSeed * 4.0) + starSeed * 62.8);
                    
                    vec3 starTint = mix(vec3(0.95, 0.98, 1.0), 
                                        mix(vec3(1.0, 0.85, 0.65), vec3(0.70, 0.85, 1.0), fract(starSeed * 7.1)),
                                        0.45);

                    float horizonFade = smoothstep(-0.02, 0.22, V.y);
                    skyColor += starTint * starBrightness * twinkle * horizonFade * pow(dayNightFactor, 1.25) * 2.0;
                }
            }

            // Secondary Faint Milky Way Star Dust
            vec3 sCoord2 = V * 320.0;
            vec3 cell2 = floor(sCoord2);
            float starSeed2 = fract(sin(dot(cell2, vec3(269.5, 183.3, 419.2))) * 18453.213);
            if (starSeed2 > 0.90)
            {
                vec3 f2 = fract(sCoord2) - vec3(0.5);
                float starDist2 = length(f2);
                if (starDist2 < 0.09)
                {
                    float b2 = smoothstep(0.09, 0.0, starDist2) * 0.60;
                    float twinkle2 = 0.70 + 0.30 * sin(totalTime * 3.2 + starSeed2 * 31.4);
                    float horizonFade2 = smoothstep(0.0, 0.28, V.y);
                    skyColor += vec3(0.88, 0.92, 1.0) * b2 * twinkle2 * horizonFade2 * pow(dayNightFactor, 1.45);
                }
            }
        }

        FragColor = vec4(skyColor, 1.0);
        return;
    }

    if (isEmissive)
    {
        // Glowing elements like lanterns or fireworks
        vec3 col = emissiveColor;
        col = mix(col * 0.9, col * 1.45, dayNightFactor);
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

    // Determine base diffuse color (pure color or texture modulated with color tint)
    vec4 baseColor = objectColor;
    if (enableTextures && useTexture)
    {
        vec4 texColor = texture(diffuseTexture, TexCoords * textureTiling);
        baseColor = texColor * objectColor;
    }

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Realistic directional shadow calculation
    float shadow = calculateShadow(FragPosLightSpace, norm, normalize(-dirLight.direction));

    // 1. Directional Sun/Moonlight with Soft PCF Shadow
    vec3 result = CalcDirLight(dirLight, norm, viewDir, baseColor.rgb, shadow);

    // 2. Dynamic Point Lights (orbiting orb, stall lights, swinging lanterns, fireworks)
    for (int i = 0; i < numActivePointLights; ++i)
    {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, baseColor.rgb);
    }

    // 3. Stage Spotlight
    if (spotLightActive)
    {
        result += CalcSpotLight(spotLight, norm, FragPos, viewDir, baseColor.rgb);
    }

    FragColor = vec4(result, baseColor.a);
}
