#version 440 core

// Inputs
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormals;
layout(location = 2) in vec2 tC;

layout(location = 18) uniform mat4x4 uProjection;
layout(location = 19) uniform mat4x4 uView;
layout(location = 20) uniform mat4x4 uModel;
layout(location = 21) uniform mat4x4 uRotMat;

// Outputs
layout(location = 0) out vec2 texCoordinates;

void main() {


    // Vertex-Position übernehmen
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}
