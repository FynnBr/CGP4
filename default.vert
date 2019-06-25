#version 440 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 aPosition;
// layout(location = 1) in vec3 aColor;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 inCoordinates;

layout(location = 0) out vec2 outCoordinates;
//layout(location = 1) out vec3 vColor;
layout(location = 1) out vec3 vNormal;
layout(location = 2) out vec3 vFragPos;

layout(location = 3) uniform mat4x4 uRotMat;
layout(location = 4) uniform mat4x4 projectMat;
layout(location = 5) uniform mat4x4 viewMat;

void main() {
    // pass color to fragment shader
    // gl_Position = vec4(aPosition, 0.0f, 1.0f);
    vec4 pos = vec4(aPosition, 1.0f);

    //projectionMat * View * Model (* Position)
    gl_Position = projectMat * viewMat * uRotMat * pos;
}

