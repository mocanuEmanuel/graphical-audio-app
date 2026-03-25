#version 330

// Circle Ripple Shader - Expanding concentric rings
//
// Creates pulsing circles that respond to audio amplitude.
// Uses signed distance functions for smooth anti-aliased rings.

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
uniform int u_ringCount;
uniform float u_speed;

out vec4 finalColor;

// Signed distance to a ring
float sdRing(vec2 p, float radius, float thickness) {
    return abs(length(p) - radius) - thickness;
}

void main() {
    // Center and aspect-correct coordinates
    vec2 uv = fragTexCoord * 2.0 - 1.0;
    uv.x *= u_resolution.x / u_resolution.y;
    
    float dist = length(uv);
    vec3 color = vec3(0.0);
    
    // Create multiple expanding rings
    for (int i = 0; i < u_ringCount; i++) {
        float fi = float(i);
        float ringCount = float(u_ringCount);
        
        // Each ring expands outward over time
        float phase = fract(u_time * u_speed * 0.2 + fi / ringCount);
        float radius = phase * 1.5;
        
        // Ring thickness varies with amplitude
        float thickness = 0.02 + u_amplitude * 0.03;
        
        // Signed distance to this ring
        float ringDist = sdRing(uv, radius, thickness);
        
        // Smooth ring with anti-aliasing
        float ring = smoothstep(0.01, 0.0, ringDist);
        
        // Fade out as ring expands
        float fade = 1.0 - phase;
        fade = pow(fade, 0.5);  // Ease the fade
        
        // Color variation per ring
        vec3 ringColor = u_color;
        ringColor.r += sin(fi * 0.5) * 0.2;
        ringColor.g += cos(fi * 0.7) * 0.1;
        
        // Glow around ring
        float glow = exp(-abs(ringDist) * 20.0) * 0.5;
        
        color += ringColor * (ring + glow) * fade;
    }
    
    // Add center pulsing dot
    float centerPulse = 0.05 + u_amplitude * 0.05;
    float centerDot = smoothstep(centerPulse, 0.0, dist);
    color += u_color * centerDot * 2.0;
    
    // Subtle radial gradient background
    float bgGlow = exp(-dist * 2.0) * 0.1;
    color += u_color * bgGlow;
    
    // Noise/flicker based on amplitude
    float flicker = 1.0 + (fract(sin(u_time * 100.0) * 43758.5453) - 0.5) * u_amplitude * 0.1;
    color *= flicker;
    
    finalColor = vec4(color, 1.0);
}
