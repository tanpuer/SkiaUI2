uniform shader y_tex;
uniform shader u_tex;
uniform shader v_tex;
uniform float widthRatio;
uniform float heightRatio;
half4 main(float2 coord) {
    float2 texCoord = float2(coord.x / widthRatio, coord.y / heightRatio);
    float2 uv_coord = texCoord / 2.0;
    half y = y_tex.eval(texCoord).r;
    half u = u_tex.eval(uv_coord).r - 0.5;
    half v = v_tex.eval(uv_coord).r - 0.5;
    half r = y + 1.402 * v;
    half g = y - 0.344 * u - 0.714 * v;
    half b = y + 1.772 * u;
    return half4(r, g, b, 1.0);
}