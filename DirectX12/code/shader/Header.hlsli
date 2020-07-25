// ルートシグネチャの定義
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
					"DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
					"DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL)"

RWTexture2D<float4> tex : register(u0);

cbuffer RaytracingParam : register(b0)
{
	/* 視点 */
    float3 eye_pos;
	/* レイの最大距離 */
    float distance;
    /* ライト位置 */
    float3 light;
}

/* 球体の最大数 */
#define SPHERE_MAX 5
/* マテリアル無し */
#define MATERIAL_NONE 0
/* ランバート反射マテリアル */
#define MATERIAL_LAMBERT 1
/* 鏡面反射マテリアル */
#define MATERIAL_METAL 2
/* 誘電体マテリアル */
#define MATERIAL_DIELECTRIC 3

/* レイの情報 */
struct Ray
{
	/* 位置 */
    float3 pos;
	/* 向き */
    float3 direction;
    
    /* 直線の方程式 */
    float3 At(in float time)
    {
        /* 時間経過による位置を求める */
        return pos + (time * direction);
    }
};

/* ヒット情報 */
struct Hit
{
    /* ヒットタイミング */
    float time;
    /* ヒットした位置 */
    float3 pos;
    /* ヒットした法線 */
    float3 normal;
    /* ヒットカラー */
    float3 color;
    /* マテリアル */
    uint material;
    /* 法線が内側向き確認フラグ */
    bool inside_normal;
};

/* 球体の情報 */
struct Sphere
{
	/* 中心座標 */
    float3 pos;
	/* 半径 */
    float radius;
    /* カラー */
    float3 color;
    /* マテリアル */
    uint material;
    
    /* 球体の方程式 */
    float At(in float3 vec)
    {
        return dot((vec - pos), (vec - pos)) - pow(radius, 2.0f);
    }
    /* ヒット判定 */
    bool IsHit(in Ray ray, out Hit hit, in float min = 0.001f, in float max = float(0xffffffff))
    {
        float3 oc = ray.pos - pos;
        
        /* 同じベクトル同士の内積はベクトルの長さの二乗に等しい */
        float a = pow(length(ray.direction), 2.0f);
        float b = dot(oc, ray.direction);
        float c = pow(length(oc), 2.0f) - pow(radius, 2.0f);
        /* 二次方程式 */
        float discriminant = b * b - a * c;
        
        /* ヒットあり 
        * D > 0 ヒットあり(２箇所)
        * D = 0 ヒットあり(１箇所)
        * D < 0 ヒットなし
        */
        if (discriminant > 0.0f)
        {
            /* ヒットしたタイミングを算出 */
            float time = (-b - sqrt(discriminant)) / a;
            if (!(min < time && time < max))
            {
                time = (-b + sqrt(discriminant)) / a;
                if (!(min < time && time < max))
                {
                    /* ヒット情報の初期化 */
                    hit.time = -1.0f;
                    hit.pos = hit.normal = hit.color = 0.0f;
                    hit.material = MATERIAL_NONE;
                    hit.inside_normal = false;
                    
                    return false;
                }
            }
            
            /* 法線を算出 */
            float3 normal = (ray.At(time) - pos) / radius;
            /* レイは内側から飛んできている */
            if (dot(ray.direction, hit.normal) > 0.0f)
            {
                normal = -normal;
                hit.inside_normal = true;
            }
            
            hit.time = time;
            hit.pos = ray.At(time);
            hit.normal = (normal);
            hit.color = color;
            hit.material = material;
            
            return true;
        }

        /* ヒット情報の初期化 */
        hit.time = -1.0f;
        hit.pos = hit.normal = hit.color = 0.0f;
        hit.material = MATERIAL_NONE;
        hit.inside_normal = false;
        
        return false;
    }
};

/* エントリーポイントの引数 */
struct ComputeThreadID
{
    uint3 group_thread_ID : SV_GroupThreadID;
    uint3 group_ID : SV_GroupID;
    uint3 dispatch_ID : SV_DispatchThreadID;
    uint group_index : SV_GroupIndex;
};