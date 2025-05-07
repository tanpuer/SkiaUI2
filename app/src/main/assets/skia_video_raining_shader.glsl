uniform float2 iResolution;
uniform float iTime;
uniform shader iChannel0;

float S(float a, float b, float t) {
    return smoothstep(a, b, t);
}


float3 N13(float p) {
   float3 p3 = fract(float3(p) * float3(.1031,.11369,.13787));
   p3 += dot(p3, p3.yzx + 19.19);
   return fract(float3((p3.x + p3.y)*p3.z, (p3.x+p3.z)*p3.y, (p3.y+p3.z)*p3.x));
}

float4 N14(float t) {
	return fract(sin(t*float4(123., 1024., 1456., 264.))*float4(6547., 345., 8799., 1564.));
}

float N(float t) {
    return fract(sin(t*12345.564)*7658.76);
}


float Saw(float b, float t) {
	return S(0., b, t)*S(1., b, t);
}


float2 DropLayer2(float2 uv, float t) {
    float2 UV = uv;
    
    uv.y += t*0.75;
    float2 a = float2(6., 1.);
    float2 grid = a*2.;
    float2 id = floor(uv*grid);
    
    float colShift = N(id.x); 
    uv.y += colShift;
    
    id = floor(uv*grid);
    float3 n = N13(id.x*35.2+id.y*2376.1);
    float2 st = fract(uv*grid)-float2(.5, 0);
    
    float x = n.x-.5;
    
    float y = UV.y*20.;
    float wiggle = sin(y+sin(y));
    x += wiggle*(.5-abs(x))*(n.z-.5);
    x *= .7;
    float ti = fract(t+n.z);
    y = (Saw(.85, ti)-.5)*.9+.5;
    float2 p = float2(x, y);
    
    float d = length((st-p)*a.yx);
    
    float mainDrop = S(.4, .0, d);
    
    float r = sqrt(S(1., y, st.y));
    float cd = abs(st.x-x);
    float trail = S(.23*r, .15*r*r, cd);
    float trailFront = S(-.02, .02, st.y-y);
    trail *= trailFront*r*r;
    
    y = UV.y;
    float trail2 = S(.2*r, .0, cd);
    float droplets = max(0., (sin(y*(1.-y)*120.)-st.y))*trail2*trailFront*n.z;
    y = fract(y*10.)+(st.y-.5);
    float dd = length(st-float2(x, y));
    droplets = S(.3, 0., dd);
    float m = mainDrop+droplets*r*trailFront;

    return float2(m, trail);
}

float StaticDrops(float2 uv, float t) {
	uv *= 40.;
    
    float2 id = floor(uv);
    uv = fract(uv)-.5;
    float3 n = N13(id.x*107.45+id.y*3543.654);
    float2 p = (n.xy-.5)*.7;
    float d = length(uv-p);
    
    float fade = Saw(.025, fract(t+n.z));
    float c = S(.3, 0., d)*fract(n.z*10.)*fade;
    return c;
}

float2 Drops(float2 uv, float t, float l0, float l1, float l2) {
    float s = StaticDrops(uv, t)*l0; 
    float2 m1 = DropLayer2(uv, t)*l1;
    float2 m2 = DropLayer2(uv*1.85, t)*l2;
    
    float c = s+m1.x+m2.x;
    c = S(.3, 1., c);
    
    return float2(c, max(m1.y*l0, m2.y*l1));
}

float4 main(float2 coord)
{
	float4 fragColor;
	float2 fragCoord = (coord.xy- .5*iResolution.xy) /iResolution.y;
    float2 uv = fragCoord;
    uv.y = - uv.y;

    float2 UV = coord;

    float T = iTime;

    
    float t = T*.2;
    
    float rainAmount = sin(T*.05)*.3+.7;
    
    float maxBlur = mix(3., 6., rainAmount);
    float minBlur = 2.;
    
    float story = 0.;
    float heart = 0.;
    
 
    float zoom = -cos(T*.2);
    
    
    float staticDrops = S(-.5, 1., rainAmount)*2.;
    float layer1 = S(.25, .75, rainAmount);
    float layer2 = S(.0, .5, rainAmount);
    
    
    float2 c = Drops(uv, t, staticDrops, layer1, layer2);

    float2 e = float2(.001, 0.);
    float cx = Drops(uv+e, t, staticDrops, layer1, layer2).x;
    float cy = Drops(uv+e.yx, t, staticDrops, layer1, layer2).x;
    float2 n = float2(cx-c.x, cy-c.x);		// expensive normals

    
    float focus = mix(maxBlur-c.y, minBlur, S(.1, .2, c.x));
    float3 col = iChannel0.eval((UV/iResolution.xy + n) * iResolution.xy).rgb;
    fragColor = float4(col, 1.);
	return fragColor;
}