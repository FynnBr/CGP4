#version 440 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable

// Inputs
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormals;
layout(location = 2) in vec2 inCoord;
uniform mat4x4 uProjection;
uniform mat4x4 uView;
uniform mat4x4 uModel;
uniform mat4x4 uRotMat;

// Outputs
layout(location = 0) out vec2 vTexture;
layout(location = 1) out vec3 vNormal;
layout(location = 2) out vec3 vFragPos;

void main() {
    // Farbe an Fragment-Shader übergeben
    vNormal = mat3(transpose(inverse(uModel))) * aNormals;
    //vNormal = aNormals;

    // Textur-Koordinaten an Fragment-Shader übergebe
    vTexture = inCoord.st;

    //vFragPos = normalize(vec3(uModel * vec4(aPosition, 1.0)));
    vFragPos = vec3(uModel * vec4(aPosition, 1.0));


    // Vertex-Position übernehmen
    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
}


/*
layout(location = 0) in vec3 aPosition;
// layout(location = 1) in vec3 aColor;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 inCoordinates;

layout(location = 0) out vec2 outCoordinates;
//layout(location = 1) out vec3 vColor;
layout(location = 1) out vec3 vNormal;
layout(location = 2) out vec3 vFragPos;

layout(location = 0) uniform mat4x4 uModelViewProjectionMatrix;
layout(location = 3) uniform mat4x4 uRotMat;
layout(location = 4) uniform mat4x4 projectMat;
layout(location = 5) uniform mat4x4 viewMat;

void main() {
    // Farbe an Fragment-Shader übergeben
    vNormal = mat3(transpose(inverse(uModelViewProjectionMatrix))) * aNormal;
    //vNormal = aNormals;

    // Textur-Koordinaten an Fragment-Shader übergebe
    outCoordinates = inCoordinates.st;

    vFragPos = vec3(uModelViewProjectionMatrix * vec4(aPosition, 1.0));

    // Vertex-Position übernehmen
    gl_Position = projectMat * viewMat * uModelViewProjectionMatrix * vec4(aPosition, 1.0);
}*/

