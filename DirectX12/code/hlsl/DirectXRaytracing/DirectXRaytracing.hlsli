/* 加速構造のデータ */
RaytracingAccelerationStructure Scene : register(t0, space0);
/* 出力先テクスチャデータ */
RWTexture2D<float4> Texture : register(u0, space0);

/* ヒット情報 */
struct Payload
{
    /* ヒットした色 */
    float4 color;
};

/* ヒット属性 */
struct Attributes
{
    /* 三角形の中心位置 */
    float2 center;
};