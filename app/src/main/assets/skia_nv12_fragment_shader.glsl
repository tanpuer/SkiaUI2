uniform shader y_tex;
uniform shader uv_tex;
uniform float widthRatio;
uniform float heightRatio;
uniform float rotation;
half4 main(float2 coord) {
    float2 texCoord = float2(coord.x / widthRatio, coord.y / heightRatio);
    float2 uv_coord = float2(texCoord.x / 2.0, texCoord.y / 2.0);
    half y = y_tex.eval(texCoord).r;
    half2 uv = uv_tex.eval(uv_coord).rg - 0.5;
    half u = uv.x;
    half v = uv.y;
    half r = y + 1.402 * v;
    half g = y - 0.344 * u - 0.714 * v;
    half b = y + 1.772 * u;
    return half4(r, g, b, 1.0);
}