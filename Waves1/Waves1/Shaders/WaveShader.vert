#version 330 core
#define M_PI 3.1415926535897932384626433832795
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;


//x and y = direction, z is steepness, w is wavelength
vec4 waveA = vec4(1,1,0.24,60);
vec4 waveB = vec4(1,0.6,0.25,31);
vec4 waveC = vec4(1,1.3,0.25,18);

float waveLengthMult = 0.005;
float gravity = 0.98;
//float steepness = 0.5;
//float waveLength = 1;
//vec2 direction = vec2(1,1);


vec3 GerstnerWave(vec4 waveVals, vec4 pos)
{
    float steepness = waveVals.z;
    float waveLength = waveVals.w * waveLengthMult;

    float k = 2 * M_PI / waveLength;
    float c = sqrt(gravity / k);
    vec2 d = normalize(waveVals.xy);

    float f = k * (dot(d,pos.xz) - c * time);
    float a = steepness / k;



    return vec3(d.x * (a * cos(f)),a * sin(f),d.y * (a * cos(f)));
}


void main()
{   
    vec4 ogPos = vec4(aPos, 1.0);
    vec4 newPos = ogPos;


    newPos += vec4(GerstnerWave(waveA,ogPos),1.0);
    newPos += vec4(GerstnerWave(waveB,ogPos),1.0);
    newPos += vec4(GerstnerWave(waveC,ogPos),1.0);

    
    gl_Position = projection * view * model * newPos;
}