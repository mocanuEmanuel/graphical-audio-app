#version 330

// Waveform Shader - Oscilloscope-style visualization
// 
// Renders a sine wave that responds to audio parameters.
// The wave animates with time and amplitude.

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Custom uniforms
uniform float u_time;
uniform float u_amplitude;
uniform float u_frequency;
uniform vec2 u_resolution;
uniform vec3 u_color;
uniform float u_glow;
uniform float u_thickness;

out vec4 finalColor;

// Smooth line function using signed distance
float sdLine(vec2 p, float y, float thickness) {
    return abs(p.y - y) - thickness;
}

void main() {
    // Normalize coordinates to -1 to 1 range, centered
    vec2 uv = fragTexCoord * 2.0 - 1.0;
    uv.x *= u_resolution.x / u_resolution.y;  // Aspect ratio correction
    
    // Generate waveform
    float waveCount = 3.0 + u_frequency * 5.0;  // More waves with higher freq
    float waveAmp = 0.3 + u_amplitude * 0.4;    // Wave height based on amplitude
    
    // Multiple overlapping waves for richness
    float wave1 = sin(uv.x * waveCount * 3.14159 + u_time * 4.0) * waveAmp;
    float wave2 = sin(uv.x * waveCount * 2.0 * 3.14159 + u_time * 3.0) * waveAmp * 0.3;
    float wave3 = sin(uv.x * waveCount * 0.5 * 3.14159 - u_time * 2.0) * waveAmp * 0.5;
    
    float wave = wave1 + wave2 + wave3;
    
    // Distance to wave line
    float dist = abs(uv.y - wave);
    
    // Line with glow
    float thickness = u_thickness * (1.0 + u_amplitude);
    float line = smoothstep(thickness, 0.0, dist);
    
    // Glow effect
    float glow = exp(-dist * (10.0 - u_glow * 8.0)) * u_glow;
    
    // Combine line and glow
    vec3 color = u_color * (line + glow);
    
    // Add subtle grid in background
    float gridX = abs(fract(uv.x * 10.0) - 0.5);
    float gridY = abs(fract(uv.y * 10.0) - 0.5);
    float grid = smoothstep(0.48, 0.5, gridX) + smoothstep(0.48, 0.5, gridY);
    color += vec3(0.0, 0.1, 0.05) * grid * 0.3;
    
    // Center line
    float centerLine = smoothstep(0.005, 0.0, abs(uv.y)) * 0.3;
    color += u_color * centerLine * 0.5;
    
    // Vignette
    float vignette = 1.0 - length(uv) * 0.3;
    color *= vignette;
    
    finalColor = vec4(color, 1.0);
}
