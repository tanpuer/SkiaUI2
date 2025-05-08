uniform float2 iResolution;
uniform float iTime;
uniform shader iChannel0;

float4 main(float2 coord) {
    float uBrightness = 0.5;
    float4 textureColor = iChannel0.eval(coord);
    return float4((textureColor.rgb + float3(uBrightness)), textureColor.w);
}