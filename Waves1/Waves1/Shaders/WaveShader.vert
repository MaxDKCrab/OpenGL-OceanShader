#version 330 core
#define M_PI 3.1415926535897932384626433832795
layout(location = 0) in vec3 aPos;

out vec3 Normal;
out vec3 FragPos;
out vec3 BasePos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform float time;
uniform vec4[500] waves;



float waveLengthMult = 0.005;
float gravity = 0.98;

float fbmAmpVal = 0.82;
float fbmWaveVal = 1.18;

vec3 GerstnerWave(vec4 waveVals, vec4 pos, inout vec3 tangent, inout vec3 binormal,float ampMult,float waveMult )
{
    float steepness = waveVals.z;
    float waveLength = waveVals.w * waveLengthMult * waveMult;

    float k = 2 * M_PI / waveLength;
    float c = sqrt(gravity / k);
    vec2 d = normalize(waveVals.xy);

    float f = k * (dot(d,pos.xz) - c * time);
    float a = steepness / k ;

    tangent += ampMult * (vec3(
    -d.x * d.x * (steepness * sin(f)),
    d.x * (steepness* cos(f)),
    -d.x * d.y * (steepness * sin(f))));

    binormal += ampMult * (vec3(
     -d.x * d.y * (steepness * sin(f)),
     d.y * (steepness * cos(f)),
     -d.y * d.y * (steepness * sin(f))));

    return vec3(d.x * (a * cos(f)),
                a * sin(f),
                d.y * (a * cos(f)));
}


void main()
{   
    vec4 ogPos = vec4(aPos,1.0);
    vec4 newPos = ogPos;
    vec3 tangent = vec3(1,0,0);
    vec3 binormal = vec3(0,0,1);

    float ampMult = 1;
    float waveMult = 1;


    for (int i = 0; i < waves.length; i++)
    {
        newPos += vec4(GerstnerWave(waves[i],ogPos,tangent,binormal,ampMult,waveMult),1.0) * ampMult;
        ampMult *= fbmAmpVal;
        waveMult *= fbmWaveVal;
    }



    vec3 normal = normalize(cross(binormal,tangent));


    FragPos = vec3 (model * newPos);
    BasePos = vec3 (model * ogPos);
    Normal =  normal;
    //TO FIGURE OUT
    //i try to use my generated normal matrix to multiply my normals, safegaurding it from non uniform scaling
    //however when i use use this commented line of code as my normals my diffuse lighting suddenly breaks
    //Normal =  normalMatrix * normal;
    gl_Position = projection * view * model * newPos;
}