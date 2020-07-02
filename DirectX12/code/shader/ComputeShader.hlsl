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
    bool IsHit(in Ray ray, out float time)
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
            /* 法線を算出 */
            normal = (ray.At(time) - pos) / radius;
        
            return true;
        }

        time = (-b + sqrt(discriminant)) / a;
        
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

/* 背景色の算出 */
float3 BackColor(in Ray ray)
{
	/* 0～1の範囲に */
    float t = 0.5f * ray.direction.y + 1.0f;
	/* 線形補完 */
    return (1.0f - t) * float3(1.0f, 1.0f, 1.0f) + (t * float3(0.5f, 0.7f, 1.0f));
}

/* 球体との当たり判定 */
bool CheckHitSphere(in Ray ray, in Sphere sphere, out float time)
{
    float a = dot(ray.direction, ray.direction);
    float b = 2.0f * dot(ray.direction, (ray.pos - sphere.pos));
	/* 球体の方程式 */
    float c = dot((ray.pos - sphere.pos), (ray.pos - sphere.pos)) - pow(sphere.radius, 2.0f);
	
    /* 二次方程式 */  
    float discriminant = b * b - (4.0f * a * c);
    /* ヒットあり */
    if (discriminant > 0.0f)
    {
        /* ヒットした時間を算出 */
        time = (-b - sqrt(discriminant)) / (2.0f * a);
        
        return true;
    }
    
    return false;
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
    Sphere sp = { float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), 0.5f };
    
    float hit_distance = 0.0f;
    if (sp.IsHit(ray, hit_distance) == true)
    {
        float3 normal = 0.5f * ((ray.pos + (hit_distance * ray.direction)) - float3(0.0f, 0.0f, -1.0f) + 1.0f);
        tex[semantics.dispatch_ID.xy] = float4(sp.normal, 1.0f);
    }
    else
    {
        tex[semantics.dispatch_ID.xy] = float4(BackColor(ray), 1.0f);
    }
}
