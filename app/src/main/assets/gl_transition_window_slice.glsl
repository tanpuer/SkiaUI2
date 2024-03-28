// Author: gre
// License: MIT

uniform float2 iResolution;
uniform float iTime;
uniform float count; // = 10.0
uniform float smoothness; // = 0.5
uniform shader iChannel0;
uniform shader iChannel1;

float4 main(float2 coord) {
    float progress = mod(iTime * 80, 400.0);
    float pr = smoothstep(-smoothness, 0.0, coord.x - progress * (1.0 + smoothness));
    float s = step(pr, fract(count * coord.x));
    return mix(iChannel0.eval(coord), iChannel1.eval(coord), s);
}