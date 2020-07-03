// ルートシグネチャの定義
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
					"DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
					"DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL)"

RWTexture2D<float4>tex : register(u0);

cbuffer RaytracingParam : register(b0)
{
	/* カメラ位置 */
    float3 eye_pos;
	/* レイの最大距離 */
    float distance;
}

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
        return pos + (time * direction);
    }
};

/* 球体の情報 */
struct Sphere
{
	/* 中心座標 */
    float3 pos;
    /* 法線 */
    float3 normal;
	/* 半径 */
    float radius;
    
    /* 球体の方程式 */
    float At(in float3 vec)
    {
        return dot((vec - pos), (vec - pos)) - pow(radius, 2.0f);
    }
    /* ヒット判定 */
    bool IsHit(in Ray ray, out float time, in float min = 0.0f, in float max = 10.0f)
    {
        /* 同じベクトル同士の内積はベクトルの長さの二乗に等しい */
        float a = pow(length(ray.direction), 2.0f);
        float b = dot((ray.pos - pos), ray.direction);
        /* 二次方程式 */
        float discriminant = b * b - a * At(ray.pos);
        
        /* ヒットあり */
        if (discriminant > 0.0f)
        {
            /* ヒットしたタイミングを算出 */
            time = (-b - sqrt(discriminant)) / a;
            if (!(min < time && time < max))
            {
                time = (-b + sqrt(discriminant)) / a;
                if (!(min < time && time < max))
                {
                    return false;
                }
            }
            
            /* 法線を算出 */
            normal = (ray.At(time) - pos) / radius;
            /* レイは内側から飛んできている */
            if (dot(ray.direction, normal) > 0.0f)
            {
                normal = float3(-normal.x, -normal.y, -normal.z);
            }
            
            return true;
        }

        return false;
    }
};

struct ComputeThreadID
{
	uint3 group_thread_ID : SV_GroupThreadID;
	uint3 group_ID        : SV_GroupID;
	uint3 dispatch_ID     : SV_DispatchThreadID;
	uint group_index      : SV_GroupIndex;
};

/* 角度からラジアン変換 */
float ChangeRadian(in float degree)
{
    return degree * acos(-1.0f) / 180.0f;
}

/* 背景色の算出 */
float3 BackColor(in Ray ray)
{
	/* 0～1の範囲に */
    float t = 0.5f * ray.direction.y + 1.0f;
	/* 線形補完 */
    return (1.0f - t) * float3(1.0f, 1.0f, 1.0f) + (t * float3(0.5f, 0.7f, 1.0f));
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void main(ComputeThreadID semantics)
{
	/* テクスチャサイズの取得 */ 
    uint2 size = 0;
    tex.GetDimensions(size.x, size.y);
	/* アスペクト比の算出 */
    const float aspect = float(size.x) / float(size.y);
	/* UV値の算出 */
    const float2 uv = float2(semantics.dispatch_ID.xy) / float2(size);
	/* ビューポート */
    const float2 viewport = float2(aspect * 2.0f, 2.0f);
	/* 左上座標 */
    float3 left_pos = eye_pos - float3(viewport / 2.0f, distance);
	
    Ray ray = { eye_pos, left_pos + float3(viewport * uv, 0.0f) - eye_pos };
    const uint obj_cnt = 2;
    Sphere sp[obj_cnt] =
    {
        { float3(0.0f, 0.0f, 0.0f),     float3(0.0f, 0.0f, 0.0f), 0.5f },
        { float3(0.0f, -100.5f, 0.0f), float3(0.0f, 0.0f, 0.0f), 100.0f },
    };
    
    float hit_distance = 0.0f;
    for (uint i = 0; i < obj_cnt; ++i)
    {
        if (sp[i].IsHit(ray, hit_distance) == true)
        {
            float3 normal = 0.5f * (sp[i].normal + 1.0f);
            tex[semantics.dispatch_ID.xy] = float4(normal, 1.0f);
            
            break;
        }
        
        tex[semantics.dispatch_ID.xy] = float4(BackColor(ray), 1.0f);
    }
}
