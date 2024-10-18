uniform float2 iResolution;
uniform float iTime;
uniform float shaderTouchX;

float4 main(float2 coord)
{
    float3 iMouse = float3(0.0, 0.0, 0.0);
    float lt = .1, // line thickness
    pt = .3, // point thickness
    pi = 3.1416,
    pi2 = pi*2.,
    pi_2 = pi/2.,
    t = iTime *pi,
    s = 1., d = 0. ;
    float2 R = iResolution.xy,
    m = (iMouse.xy-.5*R)/R.y*4.;
    float3 o = float3(0, 0, -7), // cam
    u = normalize(float3((coord.xy-.5*R)/R.y, 1)),
    c = float3(0), k = c, p;
    if (iMouse.z < 1.) m = -float2(t/20., 0);
    mat2 v = mat2(cos(m.y+radians(float4(0, -90, 90, 0)))), h = mat2(cos(m.x+radians(float4(0, -90, 90, 0)))); // pitch & yaw
    for (float i = 0.; i < 50.; i++) // raymarch
    {
        p = o+u*d;
        p.yz *= v;
        p.xz *= h;
        p.z -= 3.;
        if (p.y < -1.5) p.y = 1.8/p.y;
        k.x = min( max(p.z, length(float3(p.xy-sin(p.z+float2(pi_2, 0)+t), 0))-lt), length(p-float3(sin(t+pi_2), sin(t), 0))-pt );
        k.y = min( max(p.z, length(float3(p.xy-cos(p.z+float2(pi_2, 0)+t), 0))-lt), length(p-float3(cos(t+pi_2), cos(t), 0))-pt );
        s = min(s, min(k.x, k.y));
        if (s < .001 || d > 100.) break;
        d += s*.5;
    }
    c = max(cos(d*pi2) - s*sqrt(d) - k, 0.);
    c.gb += .1;
    return float4(c*.4 + c.brg*.6 + c*c, 1);
}