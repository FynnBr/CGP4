#version 440 core
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_separate_shader_objects : enable
#define NUM_LS 5

/*
// Inputs
layout(location = 0) in vec2 texCoordinates;

// Outputs
layout(location = 0) out vec4 fragColor;

// Uniform-Variablen
uniform float uMove = 0;
uniform sampler2D tex0;

void main() {
    // Output-Farbe aus Textur setzen und Move-Variable einrechnen und auf Output schreiben
    fragColor = texture(tex0, texCoordinates + uMove);
}
*/


//..
layout(location = 0) in vec2 vTexture;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vFragPos;

layout(location = 0) out vec4 fragColor;

// Texture + Move
uniform float uMove = 0;
uniform sampler2D tex0;
uniform samplerCube cubeMap;

// Material
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// Phong Parameters
uniform vec3 uViewPosition;
uniform vec3 uLightPosition;
uniform float uKa; // Ambient
uniform float uKd; // Diffuse
uniform float uKs; // Specular
uniform float uShininess = 1;
uniform float uConstant = 1.0;
uniform float uLinear = 0.114;
uniform float uQuadratic = 0.0007;
layout(location = 100) uniform Material material; // nur ersten Teil einfügen, da
                                                  // man jetzt material.ambient aufrufen kann

struct PointLight {     // base alignment //    aligned offset //   total bytes
    vec3 position;      // 16                   0                   16
    vec3 color;         // 16                   16                  32
    float ambient;      // 4                    32                  36
    float diffuse;      // 4                    36                  40
    float specular;     // 4                    40                  44
    float constant;     // 4                    44                  48
    float linear;       // 4                    48                  52
    float quadratic;    // 4                    52                  56
};

layout (std140) uniform lightBlock { // Create a UBO with layout std140 and Title lightBlock
    PointLight lights[NUM_LS];
};

vec3 calcPhongLight(PointLight light, vec3 viewDirection, vec3 lightDirection) {
    // Vars
    vec3 normals = normalize(vNormal);
    vec3 reflectDirection = reflect(-lightDirection, normals);

    // Material (Kupfer)
    /*vec3 ambientMaterial = vec3(0.19125, 0.0735, 0.0225);
    vec3 diffuseMaterial = vec3(0.7038, 0.27048, 0.0828);
    vec3 specularMaterial = vec3(0.256777, 0.137622, 0.086014);*/

    // Ambient Licht
    vec3 ambientAnteil = vec3(uKa);
    vec3 ambient = light.ambient + material.ambient;

    // Diffuse Licht
    vec3 diffuseAnteil = vec3(uKd);
    float diff = max(dot(normals, lightDirection), 0.0);
    vec3 diffuse = (light.diffuse + material.diffuse) * diff;

    // Specular Licht
    vec3 specularAnteil = vec3(uKs);
    float spec = max(dot(viewDirection, reflectDirection), 0.0);
    vec3 specular = (light.specular + material.specular) * pow(spec, material.shininess * 128);

    // Attenuation
    float dist = distance(vFragPos, light.position);
    float fatt = 1.0/(light.constant + light.linear * dist + light.quadratic * pow(dist, 2));

    return (ambient + diffuse + specular) * fatt * light.color;
}

void main() {

    // Cubemap Reflektion
    vec4 texe1 = texture(cubeMap, refract(reflect(vNormal, normalize(uViewPosition - vFragPos)), vNormal, 1.0/2.41));
    //vec4 texel = texture(tex0, vTexture + uMove);
    //vec3 objectColor = texel.rgb;
    //vec3 objectColor = vec3(190/255.0f,154/255.0f,100/255.0f);
    vec3 result = vec3(0.0);
    for (int i = 0; i < NUM_LS; i++) {
        //if(i != 2){
            vec3 lightDir =  normalize(lights[i].position - vFragPos);
            vec3 viewDir = normalize(uViewPosition - vFragPos);
            result += calcPhongLight(lights[i], viewDir, lightDir);
        //}
    }

    //cubemap textur und result aus allen Lichtquellen
    fragColor = vec4(texe1 * vec4(result, 1.0f));
    //fragColor = vec4(result, 1.0);

}



/*
layout(location = 0) in vec2 uvCoordinates; //vTex
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vFragPos;
layout(location = 0) out vec4 fragColor;
layout(location = 0) uniform sampler2D tex0;
layout(location = 1) uniform float uMove = 0;
layout(location = 2) uniform sampler2D cubeMap;

// Phong Parameters
layout (location = 8) uniform vec3 uViewPosition;
layout (location = 9) uniform vec3 uLightPosition;
layout (location = 10) uniform float uKa; // Ambient
layout(location = 11) uniform float uKd; //Diffuse
layout(location = 12) uniform float uKs; // Specular
layout(location = 13) uniform float uShininess = 1;
layout(location = 14) uniform float uConstant = 1.0;
layout(location = 15) uniform float uLinear = 0.114;
layout(location = 16) uniform float uQuadratic = 0.0007;

//////

// Material
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

layout(location = 100) uniform Material material;

struct PointLight {     // base alignment //    aligned offset //   total bytes
    vec3 position;      // 16                   0                   16
    vec3 color;         // 16                   16                  32
    float ambient;      // 4                    32                  36
    float diffuse;      // 4                    36                  40
    float specular;     // 4                    40                  44
    float constant;     // 4                    44                  48
    float linear;       // 4                    48                  52
    float quadratic;    // 4                    52                  56
};

layout (std140) uniform lightBlock { // Create a UBO with layout std140 and Title lightBlock
    PointLight lights[NUM_LS];
};

vec3 calcPhongLight(PointLight light, vec3 viewDirection, vec3 lightDirection) {
    // Vars
    vec3 normals = normalize(vNormal);
    vec3 reflectDirection = reflect(-lightDirection, normals);

    // Material (Kupfer)
    /*vec3 ambientMaterial = vec3(0.19125, 0.0735, 0.0225);
    vec3 diffuseMaterial = vec3(0.7038, 0.27048, 0.0828);
    vec3 specularMaterial = vec3(0.256777, 0.137622, 0.086014)//;

    // Ambient Licht
    vec3 ambientAnteil = vec3(uKa);
    vec3 ambient = light.ambient + material.ambient;

    // Diffuse Licht
    vec3 diffuseAnteil = vec3(uKd);
    float diff = max(dot(normals, lightDirection), 0.0);
    vec3 diffuse = (light.diffuse + material.diffuse) * diff;

    // Specular Licht
    vec3 specularAnteil = vec3(uKs);
    float spec = max(dot(viewDirection, reflectDirection), 0.0);
    vec3 specular = (light.specular + material.specular) * pow(spec, material.shininess * 128);

    // Attenuation
    float dist = distance(vFragPos, light.position);
    float fatt = 1.0/(light.constant + light.linear * dist + light.quadratic * pow(dist, 2));

    return (ambient + diffuse + specular) * fatt * light.color;
}

void main() {

    // Cubemap Reflektion
    vec4 texel = texture(cubeMap, refract(reflect(vNormal, normalize(uViewPosition - vFragPos)), vNormal, 1.0/2.42));
    //vec4 texel = texture(tex0, vTexture + uMove);
    //vec3 objectColor = texel.rgb;
    //vec3 objectColor = vec3(190/255.0f,154/255.0f,100/255.0f);
    vec3 result = vec3(0.0);
    for (int i = 0; i < NUM_LS; i++) {
        //if(i != 2){
            vec3 lightDir =  normalize(lights[i].position - vFragPos);
            vec3 viewDir = normalize(uViewPosition - vFragPos);
            result += calcPhongLight(lights[i], viewDir, lightDir);
        //}
    }
    fragColor = vec4(texel * vec4(result, 1.0f));
    // fragColor = vec4(texel * result, 1.0f);
    //fragColor = vec4(result, 1.0);

}*/
