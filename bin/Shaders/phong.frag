#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

uniform vec3 cameraPosition;
uniform vec3 Ka; // Material ambient color
uniform vec3 Kd; // Material diffuse color
uniform vec3 Ks; // Material specular color
uniform float specularPower; // Material specular power

uniform vec3 AmbientColour; // Ambient light colour
uniform vec3 LightColour;   // Light colour
uniform vec3 LightDirection; // Light direction

uniform sampler2D diffuseTex; // Diffuse texture map

out vec4 FragColour;

void main() {
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);
    float lambertTerm = max(0.0, dot(N, -L));

    vec3 V = normalize(cameraPosition - vPosition.xyz);
    vec3 R = reflect(L, N);
    float specularTerm = pow(max(0.0, dot(R, V)), specularPower);

    // Sample the texture
    vec3 textureColour = texture(diffuseTex, vTexCoords).rgb;

    // Combine material properties with texture
    vec3 ambient = AmbientColour * Ka * textureColour;
    vec3 diffuse = LightColour * Kd * lambertTerm * textureColour;
    vec3 specular = LightColour * Ks * specularTerm;

    FragColour = vec4(ambient + diffuse + specular, 1.0);
}
