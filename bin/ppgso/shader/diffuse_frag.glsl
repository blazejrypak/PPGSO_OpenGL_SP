#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

// Direction of light
uniform vec3 LightDirection;
uniform vec3 LightColor;

uniform vec3 ViewPosition;

// (optional) Transparency
uniform float Transparency;

// (optional) Texture offset
uniform vec2 TextureOffset;

// The vertex shader will feed this input
in vec2 texCoord;
in vec3 FragPosition;

// Wordspace normal passed from vertex shader
in vec4 normal;

// The final color
out vec4 FragmentColor;

void main()
{
    vec4 ambient = vec4(0.1 * LightColor, 1.0);
    vec4 diffuse = vec4(max(dot(normal, vec4(normalize(LightDirection), 1.0f)), 0.0) * LightColor, 1.0);

    //    float specularStrength = 0.5;
    //    vec3 ViewDirection = normalize(ViewPosition - FragPosition);
    //    vec3 ReflectDirection = reflect(vec4(-LightDirection, 1.0), normal);
    //    vec4 specular = vec4(specularStrength * pow(max(dot(ViewDirection, ReflectDirection), 0.0), 32) * LightColor, 1.0);

    FragmentColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset) * (ambient + diffuse);// +specular
    FragmentColor.a = Transparency;
} 