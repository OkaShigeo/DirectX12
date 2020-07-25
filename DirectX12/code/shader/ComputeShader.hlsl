#include "Header.hlsli"

/* ランダム値の取得 */
float Random(in float2 uv, in int seed = 0)
{
    return frac(sin(dot(sin(uv), float2(12.9898f, 78.233)) + seed) * 43758.5453);
}

/* 反射 */
float3 Reflect(in float3 in_vec, in float3 normal)
{
    /* 入射ベクトルと法線ベクトルが単位ベクトルの場合の反射 */
    //return in_vec + 2.0f * normal;
    /* 法線ベクトルだ単位ベクトルの場合の反射 */
    return in_vec - 2.0f * dot(in_vec, normal) * normal;
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

/* ランバート反射
@ レイ情報
@ 反射強度
@ ヒット情報
@ ランダム計算回数
*/
float3 LambertReflection(out Ray ray, in Hit hit, in float albedo = 1.0f, in uint random_num = 10)
{
    float max = float(0xffffffff);
    float3 random = 0.0f;
    
    random.x = Random(hit.pos.xy);
    random.y = Random(hit.pos.yz);
    random.z = Random(hit.pos.zx);
            
    /* 単位球内に一番近い反射向きを求める */
    for (uint i = 0; i < random_num; ++i)
    {        
        /* 一様分布に近づける */
        float a = Random(random.xy, i) * 2.0f * acos(-1.0f);
        float z = 2.0f * Random(random.yz, i) - 1.0f;
        float r = sqrt(1.0f - z * z);
            
        random.x = r * cos(a);
        random.y = r * sin(a);
        random.z = z;
        
        float tmp = pow(length(random), 2.0f);
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
    ray.direction = normalize(hit.normal + random);

    return hit.color * albedo /* * max(0.0f, dot((light - hit.pos), hit.normal)) */;
}

/* 鏡面反射 
@ レイ情報
@ ヒット情報
@ 反射強度
@ ぼかし率
@ ランダム計算回数
*/
float3 MetalRefrection(in out Ray ray, in Hit hit, in float albedo = 1.0f, in float fazz = 0.0f, in uint random_num = 10)
{
    ray.pos = hit.pos;
    ray.direction = Reflect(ray.direction, hit.normal);
    
    /* ぼかし処理 */
    if(fazz > 0.0f)
    {
        float max = float(0xffffffff);
        float3 random = 0.0f;
        
        for (uint i = 0; i < random_num; ++i)
        {
            random.x = 2.0f * Random(hit.pos.xy, i) - 1.0f;
            random.y = 2.0f * Random(hit.pos.yz, i) - 1.0f;
            random.z = 2.0f * Random(hit.pos.zx, i) - 1.0f;
            
            float tmp = pow(length(random), 2.0f);
            if (tmp < 1.0f)
            {
                break;
            }
            else
            {
                max = min(max, tmp);
            }
        }
        
        ray.direction += min(fazz, 1.0f) * random;
    }
    
    return hit.color * albedo;
}

/* 誘電体 */
float3 DielectricReflection(in out Ray ray, in Hit hit, in float refract = 0.0f)
{
    float rate = 1.0f / refract;
    if(hit.inside_normal == true)
    {
        rate = refract;
    }
    
    float3 vec = normalize(ray.direction);
    float cos_theta = dot(-vec, hit.normal);
    float sin_theta = sqrt(1.0f - pow(cos_theta, 2.0f));
    
    float d = 1.0 - rate * rate * (1.0f - cos_theta * cos_theta);
    float a = 1.0f;
    if(d > 0.0f)
    {
        a = (1.0f - rate) / (1.0f + rate);
        a = a * a;
        a = a + (1.0f - a) * pow((1.0f - cos_theta), 5.0f);
        
        
    }
    
    if(Random(ray.pos.xy) > a)
    {
        float3 w = rate * (vec - hit.normal * cos_theta) - hit.normal * sqrt(d);
        
        ray.pos = hit.pos;
        ray.direction = w;
    }
    else
    {
        ray.pos = hit.pos;
        ray.direction = Reflect(ray.direction, hit.normal);
    }
    
    return float3(1.0f, 1.0f, 1.0f);
}

/* グループスレッド数 */
#define THREAD_X 64
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
    /* 球体 */
    Sphere sp[SPHERE_MAX] =
    {
        { float3( 0.0f,    0.0f, 0.0f),   0.5f, float3(0.7f, 0.3f, 0.3f), MATERIAL_LAMBERT },
        { float3( 0.0f, -100.5f, 0.0f), 100.0f, float3(0.8f, 0.8f, 0.0f), MATERIAL_LAMBERT },
        { float3( 1.0f,    0.0f, 0.0f),   0.5f, float3(0.8f, 0.6f, 0.2f), MATERIAL_METAL },
        { float3(-1.0f,    0.0f, 0.0f),   0.5f, float3(0.8f, 0.6f, 0.8f), MATERIAL_DIELECTRIC },
        { float3(-1.0f, 0.0f, 0.0f), -0.45f, float3(0.8f, 0.6f, 0.8f), MATERIAL_DIELECTRIC },
    };
    
    /* UV値の算出 */
    const float2 uv = (float2(semantics.group_ID.xy) + Random(semantics.group_ID.xy, semantics.group_thread_ID.x)) / float2(size);
	/* レイ */
    Ray ray = 
    { 
        eye_pos, normalize(left_pos + float3(viewport * uv, 0.0f) - eye_pos)
    };
    /* ヒットカラー */
    float3 color = 1.0f;
    /* 反射強度 */
    const float albedo = 0.5f;
    
    for (uint i = 0; i < 10; ++i)
    {
        /* ヒット情報 */
        Hit hit =
        {
            -1.0f, float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), MATERIAL_NONE, false
        };
        
        /* ヒット判定チェック */
        if (IsSphereHit(ray, sp, hit) == true)
        {
            if (hit.material == MATERIAL_LAMBERT)
            {
                color *= LambertReflection(ray, hit, albedo);
            }
            else if(hit.material == MATERIAL_METAL)
            {
                color *= MetalRefrection(ray, hit, albedo, 0.0f);
            }
            else if (hit.material == MATERIAL_DIELECTRIC)
            {
                color *= DielectricReflection(ray, hit, 1.5f);
            }
            else
            {
                color *= 0.0f;
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
