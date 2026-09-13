#version 330 core
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vFragPos;

uniform sampler2D uTexture;
uniform vec3 uLightDir;

out vec4 FragColor;

void main() {
    vec3 N = normalize(vNormal);
    vec3 L = normalize(-uLightDir);
    float diff = max(dot(N, L), 0.0);

    vec3 albedo = texture(uTexture, vTexCoord).rgb;
    vec3 ambient = 0.25 * albedo;
    vec3 diffuse = 0.85 * diff * albedo;

    FragColor = vec4(ambient + diffuse, 1.0);
}
