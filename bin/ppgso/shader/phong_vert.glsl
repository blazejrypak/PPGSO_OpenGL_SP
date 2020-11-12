#version 330 core
layout (location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;

out vec3 FragPos;
out vec3 normal;

// This will be passed to the fragment shader
out vec2 texCoord;

// Matrices as program attributes
uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

void main() {
    // Copy the input to the fragment shader
    texCoord = TexCoord;

    FragPos = vec3(ModelMatrix * vec4(Position, 1.0));
    normal = mat3(transpose(inverse(ModelMatrix))) * Normal;

    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(Position, 1.0);
}