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

#define NR_POINT_LIGHTS 6

uniform vec4 objectColor;
uniform float dayNightFactor; // 0.0 = bright day, 1.0 = festival night
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

    // If fragment is outside light frustum far plane, no shadow
    if (projCoords.z > 1.0)
        return 0.0;

    // Adaptive slope-scale depth bias to eliminate shadow acne
    float bias = max(0.0035 * (1.0 - dot(normal, lightDir)), 0.0006);

    // 16-sample Percentage-Closer Filtering (PCF) with smooth disc
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
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
    // Blinn-Phong Specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), max(material.shininess, 1.0));

    vec3 ambient = light.ambient * diffColor;
    vec3 diffuse = (1.0 - shadow) * light.diffuse * diff * diffColor;
    vec3 specular = (1.0 - shadow) * light.specular * (spec * material.specularStrength);

    if (shadingMode == 1)
        return ambient + diffuse;
    if (shadingMode == 2)
        return ambient;
    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffColor)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Blinn-Phong Specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), max(material.shininess, 1.0));
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * diffColor * attenuation;
    vec3 diffuse = light.diffuse * diff * diffColor * attenuation;
    vec3 specular = light.specular * (spec * material.specularStrength) * attenuation;

    if (shadingMode == 1)
        return ambient + diffuse;
    if (shadingMode == 2)
        return ambient;
    return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffColor)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // Spot cone intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float spotIntensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    if (spotIntensity <= 0.0)
        return vec3(0.0);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Blinn-Phong Specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), max(material.shininess, 1.0));
    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * diffColor * attenuation;
    vec3 diffuse = light.diffuse * diff * diffColor * attenuation * spotIntensity;
    vec3 specular = light.specular * (spec * material.specularStrength) * attenuation * spotIntensity;

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
