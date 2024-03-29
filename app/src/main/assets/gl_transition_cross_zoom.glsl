uniform float2 iResolution;
uniform float iTime;
uniform shader iChannel0;
uniform shader iChannel1;
uniform float2 iChannel0Resolution;
uniform float2 iChannel1Resolution;
uniform float strength; // = 0.4

const float PI = 3.141592653589793;

float Linear_ease(in float begin, in float change, in float duration, in float time) {
    return change * time / duration + begin;
}

float Exponential_easeInOut(in float begin, in float change, in float duration, in float time) {
    if (time == 0.0) {
        return begin;
    } else if (time == duration) {
        return begin + change;
    }
    time = time / (duration / 2.0);
    if (time < 1.0) {
        return change / 2.0 * pow(2.0, 10.0 * (time - 1.0)) + begin;
    }
    return change / 2.0 * (-pow(2.0, -10.0 * (time - 1.0)) + 2.0) + begin;
}

float Sinusoidal_easeInOut(in float begin, in float change, in float duration, in float time) {
    return -change / 2.0 * (cos(PI * time / duration) - 1.0) + begin;
}

float rand (float2 co) {
    return fract(sin(dot(co.xy ,float2(12.9898,78.233))) * 43758.5453);
}

float3 crossFade(in float2 uv, in float dissolve) {
    float4 fromColor = iChannel0.eval(uv / iResolution * iChannel0Resolution);
    float4 toColor = iChannel1.eval(uv / iResolution * iChannel1Resolution);
    return mix(fromColor.rgb, toColor.rgb, dissolve);
}

float4 main(float2 uv) {
    float2 texCoord = uv.xy / float2(1.0).xy;
    float progress = mod(iTime / 4, 1.0);
    float2 center = float2(Linear_ease(0.25, 0.5, 1.0, progress), 0.5);
    float dissolve = Exponential_easeInOut(0.0, 1.0, 1.0, progress);
    float strength = Sinusoidal_easeInOut(0.0, strength, 0.5, progress);
    float3 color = float3(0.0);
    float total = 0.0;
    float2 toCenter = center - texCoord;
    float offset = rand(uv);
    for (float t = 0.0; t <= 40.0; t++) {
        float percent = (t + offset) / 40.0;
        float weight = 4.0 * (percent - percent * percent);
        color += crossFade(texCoord + toCenter * percent * strength, dissolve) * weight;
        total += weight;
    }
    return float4(color / total, 1.0);
}