/* ルートシグネチャ */
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
					"DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
					"DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL)"
/* テクスチャ */
RWTexture2D<float4> tex : register(u0);
/* 必要パラメータ */
cbuffer RaytracingParam : register(b0)
{
	/* 視点位置 */
    float3 eye_pos;
	/* レイの最大距離 */
    float distance;
    /* ライト位置 */
    float3 light;
}

/* グループスレッド数 */
#define THREAD_X 64
#define THREAD_Y 1
#define THREAD_Z 1

/* 球体の最大数 */
#define SPHERE_MAX 5

/* マテリアル種別 */
#define MATERIAL_NONE    0
#define MATERIAL_LAMBERT 1
#define MATERIAL_REFLECT 2
#define MATERIAL_REFRACT 3

/* エントリーポイントの引数 */
struct ComputeThreadID
{
    uint3 group_thread_ID : SV_GroupThreadID;
    uint3 group_ID        : SV_GroupID;
    uint3 dispatch_ID     : SV_DispatchThreadID;
    uint group_index      : SV_GroupIndex;
};

/* ヒット情報 */
struct Hit
{
    /* ヒットタイミング */
    float time;
    /* ヒット座標 */
    float3 pos;
    /* 法線ベクトル */
    float3 normal;
    /* ヒットカラー */
    float3 color;
    /* マテリアル種別 */
    uint material;
};

/* レイ */
struct Ray
{
    /* 始点座標 */
    float3 pos;
    /* 移動方向 */
    float3 direction;
    
    /* レイの位置を取得 */
    float3 GetPos(in float time)
    {
        /* 直線の方程式 */
        return pos + (time * direction);
    }
};

/* 球体 */
struct Sphere
{
    /* 中心座標 */
    float3 center;
    /* 半径 */
    float radius;
    /* カラー */
    float3 color;
    /* マテリアル種別 */
    uint material;
    
    /* 半径の取得 */
    float GetRadius(in float3 pos)
    {
        float3 tmp = pos - center;

        /* 球体方程式 */
        return sqrt(dot(tmp, tmp));
    }
    
    /* ヒット判定 */
    bool IsHit(in Ray ray, out Hit hit, in float min = 0.001f, in float max = float(0xffffffff))
    {
        /* ax^2 + bx + c = 0 */
        float3 oc = ray.pos - center;
        float a = dot(ray.direction, ray.direction);
        float b = 2.0f * dot(ray.direction, oc);
        float c = dot(oc, oc) - (radius * radius);
        
        bool flag = true;
        
        /* 判定式 */
        float D = (b * b) - (4.0f * a * c);
        if(D > 0.0f)
        {
            /* ヒットタイミングを算出 */
            float time = (-b - sqrt(D)) / (2.0f * a);
            /* ヒットタイミングが範囲内か確認 */
            if (!(min < time && time < max))
            {
                time = (-b + sqrt(D)) / (2.0f * a);
                if (!(min < time && time < max))
                {
                    flag = false;
                }
            }
            
            if(flag == true)
            {
                hit.time     = time;
                hit.pos      = ray.GetPos(hit.time);
                hit.normal   = (hit.pos - center) / radius;
                hit.color    = color;
                hit.material = material;
                
                return true;
            }
        }
        
        hit.time = -1.0f;
        hit.pos = hit.normal = hit.color = 0.0f;
        hit.material = MATERIAL_NONE;
        
        return false;
    }
};