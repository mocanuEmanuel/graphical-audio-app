#version 330

// Electric Border Shader - Zappy lightning border effect
//
// Creates an animated electric border with noise displacement.
// The effect intensifies with audio amplitude.

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Custom uniforms
uniform float u_time;
uniform float u_amplitude;
uniform float u_temperature;
uniform vec2 u_resolution;
uniform vec3 u_color;
uniform float u_borderWidth;
uniform float u_noiseIntensity;

out vec4 finalColor;

// Hash function for noise
float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

// Value noise
float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    f = f * f * (3.0 - 2.0 * f);  // Smoothstep
    
    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));
    
    return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}

// Fractal noise
float fbm(vec2 p) {
    float value = 0.0;
    float amplitude = 0.5;
    for (int i = 0; i < 4; i++) {
        value += amplitude * noise(p);
        p *= 2.0;
        amplitude *= 0.5;
    }
    return value;
}

void main() {
    vec2 uv = fragTexCoord;
    
    // Distance from edges (0 at edge, 0.5 at center)
    vec2 fromEdge = min(uv, 1.0 - uv);
    float edgeDist = min(fromEdge.x, fromEdge.y);
    
    // Border region
    float borderWidth = u_borderWidth * (1.0 + u_amplitude * 0.5);
    
    // Noise displacement along border
    vec2 noiseCoord = uv * 20.0 + vec2(u_time * 3.0, 0.0);
    float displacement = fbm(noiseCoord) * u_noiseIntensity * (1.0 + u_amplitude * 2.0);
    
    // Distort edge distance with noise
    edgeDist += displacement;
    
    // Electric zap intensity
    float zapSpeed = 15.0 + u_amplitude * 20.0;
    float zap = noise(uv * 50.0 + vec2(u_time * zapSpeed, u_time * zapSpeed * 0.7));
    zap = pow(zap, 3.0);  // Make zaps sporadic
    
    // Border mask
    float borderMask = smoothstep(borderWidth, borderWidth * 0.5, edgeDist);
    
    // Inner glow
    float innerGlow = smoothstep(borderWidth * 2.0, 0.0, edgeDist) * 0.3;
    
    // Color with electric variations
    vec3 electricColor = u_color;
    electricColor += vec3(0.2, 0.0, 0.3) * zap * u_amplitude;  // Purple zaps
    
    // Temperature affects color (warm = more orange, cool = more blue)
    electricColor.r += (u_temperature - 0.5) * 0.4;
    electricColor.b -= (u_temperature - 0.5) * 0.3;
    
    // Final color
    vec3 color = vec3(0.0);
    
    // Add border
    color += electricColor * borderMask;
    
    // Add zap highlights
    color += vec3(1.0) * zap * borderMask * u_amplitude;
    
    // Add inner glow
    color += electricColor * innerGlow * 0.5;
    
    // Center area (dark with subtle color)
    float centerArea = 1.0 - borderMask;
    color += electricColor * 0.02 * centerArea;
    
    // Flicker effect
    float flicker = 0.9 + 0.1 * sin(u_time * 30.0) * u_amplitude;
    color *= flicker;
    
    finalColor = vec4(color, 1.0);
}
