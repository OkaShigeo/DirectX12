// ルートシグネチャの定義
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
					"DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
					"DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL)"

RWTexture2D<float4>tex : register(u0);

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
#define SPHERE_MAX 2
/* マテリアル無し */
#define MATERIAL_NONE 0
/* ランバート反射マテリアル */
#define MATERIAL_LAMBERT 1

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
                    
                    return false;
                }
            }
            
            /* 法線を算出 */
            float3 normal = (ray.At(time) - pos) / radius;
            /* レイは内側から飛んできている */
            if (!(dot(ray.direction, normal) < 0.0f))
            {
                normal = float3(-normal.x, -normal.y, -normal.z);
            }
            
            hit.time     = time;
            hit.pos      = ray.At(time);
            hit.normal   = normal;
            hit.color    = color;
            hit.material = material;
            
            return true;
        }

        /* ヒット情報の初期化 */
        hit.time = -1.0f;
        hit.pos = hit.normal = hit.color = 0.0f;
        hit.material = MATERIAL_NONE;
        
        return false;
    }
};

/* エントリーポイントの引数 */
struct ComputeThreadID
{
    uint3 group_thread_ID : SV_GroupThreadID;
    uint3 group_ID        : SV_GroupID;
    uint3 dispatch_ID     : SV_DispatchThreadID;
    uint group_index      : SV_GroupIndex;
};

/* ランダム値の取得 */
float Random(in float2 uv, in int seed = 0)
{
    return frac(sin(dot(sin(uv), float2(12.9898f, 78.233)) + seed) * 43758.5453);
}

/* 背景色の算出 */
float3 BackColor(in Ray ray)
{
	/* 0～1の範囲に */
    float t = 0.5f * (ray.direction.y + 1.0f);
	/* 線形補完 */
    return (1.0f - t) * float3(1.0f, 1.0f, 1.0f) + (t * float3(0.5f, 0.7f, 1.0f));
}

/* リニア色空間に変換 */
float3 ChangeLinearColor(in float3 color)
{
    return pow(color, 2.2f);
}

/* sRGB色空間に変換 */
float3 ChangeSRGBColor(in float3 color)
{
    return pow(color, 1.0f / 2.2f);
}

/* 
* 球体とのヒット判定 
* シャドウアクネ適用
*/
bool IsSphereHit(in Ray ray, in Sphere sp[SPHERE_MAX], out Hit hit)
{
    const float min = 0.01f;
    float max = float(0xffffffff);
    
    /* ヒット情報の初期化 */
    hit.time = -1.0f;
    hit.pos = hit.normal = hit.color = 0.0f;
    hit.material = MATERIAL_NONE;
    
    for (uint i = 0; i < SPHERE_MAX; ++i)
    {
        Hit dummy_hit = hit;
        if (sp[i].IsHit(ray, dummy_hit, min, max) == true)
        {
            hit = dummy_hit;
            max = hit.time;
        }
    }
    
    return (hit.time >= 0.0f);
}

/* ランバート反射 */
float3 LambertReflection(out Ray ray, in Hit hit, in float albedo = 1.0f, uint random_num = 10)
{
    float max = float(0xffffffff);
    float3 random_vec = 0.0f;
    
    random_vec.x = Random(hit.pos.xy);
    random_vec.y = Random(hit.pos.yz);
    random_vec.z = Random(hit.pos.zx);
            
    /* 単位球内に一番近い反射向きを求める */
    for (uint i = 0; i < random_num; ++i)
    {        
        /* 一様分布に近づける */
        float a = Random(random_vec.xy, i) * 2.0f * acos(-1.0f);
        float z = 2.0f * Random(random_vec.yz, i) - 1.0f;
        float r = sqrt(1.0f - z * z);
            
        random_vec.x = r * cos(a);
        random_vec.y = r * sin(a);
        random_vec.z = z;
        
        float tmp = pow(length(random_vec), 2.0f);
        if(tmp < 1.0f)
        {
            break;
        }
        else
        {
            max = min(max, tmp);
        }
    }
            
    /* レイの反射 */
    ray.pos = hit.pos;
    ray.direction = hit.normal + random_vec;

    return hit.color * albedo;
}

/* グループスレッド数 */
#define THREAD_X 512
#define THREAD_Y 1
#define THREAD_Z 1

/* グループスレッドごとの計算結果  */
groupshared float3 result[THREAD_X];

[RootSignature(RS)]
[numthreads(THREAD_X, THREAD_Y, THREAD_Z)]
void main(ComputeThreadID semantics)
{
	/* テクスチャサイズの取得 */ 
    uint2 size = 0;
    tex.GetDimensions(size.x, size.y);
	/* アスペクト比の算出 */
    const float aspect = float(size.x) / float(size.y);
	/* ビューポート */
    const float2 viewport = float2(aspect * 2.0f, 2.0f);
     /* 左上座標 */
    float3 left_pos = eye_pos - float3(viewport / 2.0f, distance);
    /* 球体の数 */
    const uint sp_num = 2;
    /* 球体 */
    Sphere sp[sp_num] =
    {
        { float3(0.0f, 0.0f, 0.0f), 0.5f, float3(1.0f, 1.0f, 1.0f), MATERIAL_LAMBERT },
        { float3(0.0f, -100.5f, 0.0f), 100.0f, float3(1.0f, 1.0f, 1.0f), MATERIAL_LAMBERT },
    };
    
    /* UV値の算出 */
    const float2 uv = (float2(semantics.group_ID.xy) + Random(semantics.group_ID.xy, semantics.group_thread_ID.x)) / float2(size);
	/* レイ */
    Ray ray = 
    { 
        eye_pos, left_pos + float3(viewport * uv, 0.0f) - eye_pos 
    };
    /* ヒットカラー */
    float3 color = 1.0f;
    /* 反射強度 */
    const float albedo = 0.5f;
    
    for (uint ref_cnt = 0; ref_cnt < 10; ++ref_cnt)
    {
        /* ヒット情報 */
        Hit hit =
        {
            -1.0f, float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), MATERIAL_NONE
        };
        
        /* ヒット判定チェック */
        if (IsSphereHit(ray, sp, hit) == true)
        {
            if (hit.material == MATERIAL_LAMBERT)
            {
                color *= LambertReflection(ray, hit, albedo) * max(0.0f, dot((light - hit.pos), hit.normal));
            }
        }
        else
        {
            color *= BackColor(ray);
            break;
        }
    }
    
    result[semantics.group_thread_ID.x] = color;
    
    /* グループスレッドの同期 */
    GroupMemoryBarrierWithGroupSync();
    
    for (int th_index = THREAD_X / 2; th_index > 0; th_index /= 2)
    {
        if (semantics.group_thread_ID.x < th_index)
        {
            result[semantics.group_thread_ID.x] += result[semantics.group_thread_ID.x + th_index];
        }
            
        /* グループスレッドの同期 */
        GroupMemoryBarrierWithGroupSync();
    }
    
    if(semantics.group_thread_ID.x == 0)
    {
        result[semantics.group_thread_ID.x] /= THREAD_X;
       
        tex[semantics.group_ID.xy] = float4(ChangeSRGBColor(result[semantics.group_thread_ID.x]), 1.0f);
    }
}
