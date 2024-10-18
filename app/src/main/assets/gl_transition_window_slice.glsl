uniform float2 iResolution;
uniform float iTime;
uniform float count; // = 10.0
uniform float smoothness; // = 0.5
uniform shader iChannel0;
uniform shader iChannel1;
uniform float2 iChannel0Resolution;
uniform float2 iChannel1Resolution;
uniform float shaderTouchX;

float4 main(float2 coord) {
    float progress = shaderTouchX < 0 ? mod(iTime * 80, iResolution.x) : shaderTouchX;
    float pr = smoothstep(-smoothness, 0.0, coord.x - progress * (1.0 + smoothness));
    float s = step(pr, fract(count * coord.x));
    float4 fromColor = iChannel0.eval(coord / iResolution * iChannel0Resolution);
    float4 toColor = iChannel1.eval(coord / iResolution * iChannel1Resolution);
    return mix(fromColor, toColor, s);
}