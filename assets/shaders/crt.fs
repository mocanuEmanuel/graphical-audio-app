#version 330

// CRT Shader - Terminal aesthetic with scanlines and distortion
// 
// Effects:
// 1. Scanlines - horizontal lines
// 2. Barrel distortion - screen curvature
// 3. Chromatic aberration - RGB color separation
// 4. Vignette - dark edges

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Custom uniforms
uniform float time;           // Time for animation
uniform vec2 resolution;      // Screen resolution
uniform float curvature;      // Barrel distortion (0.0 - 0.5)
uniform float scanlineIntensity;  // Scanline darkness (0.0 - 1.0)
uniform float chromaticAberration; // Color separation (0.0 - 0.01)
uniform float vignetteStrength;    // Edge darkness (0.0 - 1.0)

out vec4 finalColor;

// Barrel distortion function
vec2 distort(vec2 uv, float k) {
    vec2 center = uv - 0.5;
    float r2 = dot(center, center);
    float f = 1.0 + k * r2;
    return center * f + 0.5;
}

void main() {
    vec2 uv = fragTexCoord;
    
    // Apply barrel distortion
    if (curvature > 0.0) {
        uv = distort(uv, curvature);
        
        // Discard pixels outside screen
        if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
            finalColor = vec4(0.0, 0.0, 0.0, 1.0);
            return;
        }
    }
    
    // Chromatic aberration
    vec3 color;
    if (chromaticAberration > 0.0) {
        vec2 offset = (uv - 0.5) * chromaticAberration;
        color.r = texture(texture0, uv + offset).r;
        color.g = texture(texture0, uv).g;
        color.b = texture(texture0, uv - offset).b;
    } else {
        color = texture(texture0, uv).rgb;
    }
    
    // Scanlines
    if (scanlineIntensity > 0.0) {
        float scanline = sin(uv.y * resolution.y * 3.14159) * 0.5 + 0.5;
        scanline = pow(scanline, 1.5);
        color *= 1.0 - scanlineIntensity * (1.0 - scanline);
    }
    
    // Vignette
    if (vignetteStrength > 0.0) {
        vec2 vignetteUV = uv * (1.0 - uv);
        float vignette = vignetteUV.x * vignetteUV.y * 15.0;
        vignette = pow(vignette, vignetteStrength);
        color *= vignette;
    }
    
    // Slight color boost for terminal feel
    color = pow(color, vec3(0.95));
    
    finalColor = vec4(color, 1.0) * colDiffuse * fragColor;
}
