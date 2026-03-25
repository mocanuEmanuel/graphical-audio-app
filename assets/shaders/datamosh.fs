#version 330

// Datamosh Shader - Glitch/displacement effect
//
// Effects:
// 1. Audio-reactive pixel displacement
// 2. Block glitching
// 3. Color channel shifting
// 4. Motion blur trails

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Custom uniforms
uniform float time;           // Time for animation
uniform vec2 resolution;      // Screen resolution
uniform float amplitude;      // Audio amplitude (0.0 - 1.0)
uniform float glitchIntensity;    // Block glitch amount (0.0 - 1.0)
uniform float displacementAmount; // Pixel displacement (0.0 - 0.1)
uniform float colorShift;         // RGB separation (0.0 - 0.05)

out vec4 finalColor;

// Pseudo-random function
float random(vec2 st) {
    return fract(sin(dot(st, vec2(12.9898, 78.233))) * 43758.5453);
}

// Block quantization for glitch effect
vec2 blockize(vec2 uv, float blockSize) {
    return floor(uv * blockSize) / blockSize;
}

void main() {
    vec2 uv = fragTexCoord;
    vec3 color;
    
    // Block glitch effect
    if (glitchIntensity > 0.0 && amplitude > 0.3) {
        // Create random blocks
        float blockSize = 16.0 + amplitude * 32.0;
        vec2 blockUV = blockize(uv, blockSize);
        float blockRand = random(blockUV + floor(time * 10.0));
        
        // Apply displacement to some blocks based on audio
        if (blockRand < glitchIntensity * amplitude) {
            float displaceX = (random(blockUV + 0.1) - 0.5) * displacementAmount * amplitude;
            float displaceY = (random(blockUV + 0.2) - 0.5) * displacementAmount * amplitude * 0.5;
            uv.x += displaceX;
            uv.y += displaceY;
        }
    }
    
    // Wave displacement based on audio
    if (displacementAmount > 0.0) {
        float wave = sin(uv.y * 20.0 + time * 5.0) * amplitude;
        uv.x += wave * displacementAmount * 0.5;
    }
    
    // Color channel shifting
    if (colorShift > 0.0) {
        float shift = colorShift * (amplitude * 2.0 + 0.5);
        color.r = texture(texture0, uv + vec2(shift, 0.0)).r;
        color.g = texture(texture0, uv).g;
        color.b = texture(texture0, uv - vec2(shift, 0.0)).b;
    } else {
        color = texture(texture0, uv).rgb;
    }
    
    // Intensity boost based on amplitude
    float boost = 1.0 + amplitude * 0.3;
    color *= boost;
    
    // Random noise overlay
    if (glitchIntensity > 0.0) {
        float noise = random(uv + time) * glitchIntensity * 0.1;
        color += noise;
    }
    
    // Clamp to valid range
    color = clamp(color, 0.0, 1.0);
    
    finalColor = vec4(color, 1.0) * colDiffuse * fragColor;
}
