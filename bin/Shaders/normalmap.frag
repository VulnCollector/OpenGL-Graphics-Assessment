#version 410

// Directional light uniforms
uniform vec3 AmbientColour;
uniform vec3 LightColour;
uniform vec3 LightDirection;

// Multiple point lights
uniform int numLights;
uniform vec3 PointLightPosition[4];
uniform vec3 PointLightColour[4];

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;

uniform vec3 cameraPosition;

// Material properties
uniform vec3 Ka; // Ambient reflectivity
uniform vec3 Kd; // Diffuse reflectivity
uniform vec3 Ks; // Specular reflectivity
uniform float specularPower;

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;

out vec4 FragColour;

void main() {
    // Sample textures.
    vec3 texDiffuse = texture(diffuseTex, vTexCoord).rgb;
    vec3 texSpecular = texture(specularTex, vTexCoord).rgb;
    vec3 texNormal = texture(normalTex, vTexCoord).rgb;
    
    // Transform texture normal from [0,1] to [-1,1].
    texNormal = texNormal * 2.0 - 1.0;
    
    // Build TBN matrix and transform texture-space normal to world-space.
    mat3 TBN = mat3(normalize(vTangent), normalize(vBiTangent), normalize(vNormal));
    vec3 N = normalize(TBN * texNormal);
    
    // Compute view vector.
    vec3 V = normalize(cameraPosition - vPosition.xyz);
    
    // -----------------------------
    // Directional light contribution
    // -----------------------------
    vec3 L_dir = normalize(LightDirection);
    float lambertDir = max(dot(N, -L_dir), 0.0);
    vec3 R_dir = reflect(L_dir, N);
    float specularDir = pow(max(dot(R_dir, V), 0.0), specularPower);
    
    vec3 ambient = AmbientColour * Ka * texDiffuse;
    vec3 diffuseDir = LightColour * Kd * lambertDir * texDiffuse;
    vec3 specularDirColor = LightColour * Ks * specularDir * texSpecular;
    
    // --------------------------------------
    // Point lights contribution (up to 4)
    // --------------------------------------
    vec3 diffusePoint = vec3(0.0);
    vec3 specularPoint = vec3(0.0);
    
    for(int i = 0; i < numLights; i++) {
        // Compute direction and distance from fragment to point light.
        vec3 lightDir = normalize(vPosition.xyz - PointLightPosition[i]);
        float distance = length(vPosition.xyz - PointLightPosition[i]);
        
        // Inverse square attenuation.
        float attenuation = 1.0 / (distance * distance);
        vec3 attenuatedColor = PointLightColour[i] * attenuation;
        
        // Diffuse term.
        float lambertPoint = max(dot(N, -lightDir), 0.0);
        vec3 diff = attenuatedColor * Kd * lambertPoint * texDiffuse;
        
        // Specular term.
        vec3 R_point = reflect(lightDir, N);
        float spec = pow(max(dot(R_point, V), 0.0), specularPower);
        vec3 specCol = attenuatedColor * Ks * spec * texSpecular;
        
        diffusePoint += diff;
        specularPoint += specCol;
    }
    
    // Combine all contributions.
    vec3 color = ambient + diffuseDir + specularDirColor + diffusePoint + specularPoint;
    FragColour = vec4(color, 1.0);
}
