uniform float2 iResolution;
uniform float iTime;
uniform shader iChannel0;

float4 main(float2 coord) {
    float4 color = iChannel0.eval(coord);
    float luminance = dot(color.rgb, float3(0.299, 0.587, 0.114));
    float3 grayscale = mix(color.rgb, float3(luminance), 1.0);
    return float4(grayscale, color.a);
}