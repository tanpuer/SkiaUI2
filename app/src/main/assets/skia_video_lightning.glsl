uniform float2 iResolution;
uniform float iTime;
uniform shader iChannel0;

float4 main(float2 coord) {
    float4 color = iChannel0.eval(coord);
    return float4(1.0 - color.r, 1.0 - color.g, 1.0 - color.b, color.a);
}