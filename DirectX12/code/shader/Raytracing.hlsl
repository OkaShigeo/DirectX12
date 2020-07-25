#include "Raytracing.hlsli"

/* 背景色の算出 */
float3 BackgroundColor(in Ray ray, in float3 end, in float3 begin = float3(1.0f, 1.0f, 1.0f))
{
    float time = 0.5f * (ray.direction.y + 1.0f);

    /* 線形補間 */
    return lerp(begin, end, time);
}

/* 球体のヒット判定 */
bool IsSphereHit(in Ray ray, in Sphere sp[SPHERE_MAX], out Hit hit)
{
    hit.time = -1.0f;
    hit.pos = hit.normal = hit.color = 0.0f;
    
    float min = 0.001f;
    float max = float(0xffffffff);
    for (uint i = 0; i < SPHERE_MAX; ++i)
    {
        Hit dummy = hit;
        if(sp[i].IsHit(ray, dummy, min, max) == true)
        {
            hit = dummy;
            max = hit.time;
        }
    }

    return (hit.time >= 0.0f);
}

/* ランダム値の取得 */
float Random(in float2 uv, in int seed = 0)
{
    return frac(sin(dot(sin(uv), float2(12.9898f, 78.233)) + seed) * 43758.5453);
}

/* ランバート反射 */
float3 Lambert(in out Ray ray, in Hit hit, in float albedo = 0.5f, in uint count = 50)
{
    /* ヒット位置から反射方向に単位球があると仮定し、反射位置が単位球内にあるまでループ */
    float3 random = 0.0f;
    random.x = 2.0f * Random(hit.pos.xy) - 1.0f;
    random.y = 2.0f * Random(hit.pos.yz) - 1.0f;
    random.z = 2.0f * Random(hit.pos.zx) - 1.0f;
    
    float value = float(0xffffffff);
    
    for (uint i = 0; i < count; ++i)
    {
        random.x = 2.0f * Random(random.xy, i) - 1.0f;
        random.y = 2.0f * Random(random.yz, i) - 1.0f;
        random.z = 2.0f * Random(random.zx, i) - 1.0f;
        
        //float a = acos(-1.0f) * Random(random.xy, i);
        //float z = 2.0f * Random(random.yz, i) - 1.0f;
        //float r = sqrt(1.0f - (z * z));
        
        //random.x = r * cos(a);
        //random.y = r * sin(a);
        //random.z = z;
        
        float tmp = pow(length(random), 2.0f);
        if(tmp < 1.0f)
        {
            break;
        }
        else
        {
            value = min(tmp, value);
        }
    }
    
    if (!(dot(random, hit.normal) > 0.0f))
    {
        random = -random;
    }
    
    ray.pos = hit.pos;
    ray.direction = hit.normal + random;
    
    return albedo * hit.color;
}

/* リニア色空間に変換 */
float3 Linear(in float3 color)
{
    return pow(color, 1.0f / 2.2f);
}

/* sRGB色空間に変換 */
float3 sRGB(in float3 color)
{
    return pow(color, 2.2f);
}

/* 反射 */
float3 Reflect(in float3 direction, in float3 normal)
{
    /* 入射ベクトルと法線ベクトルが単位ベクトルの場合の反射 */
    //return direction + 2.0f * normal;
    /* 法線ベクトルだけ単位ベクトルの場合の反射 */
    return direction - 2.0f * dot(direction, normal) * normal;
}

/* 鏡面反射 */
float3 Metal(in out Ray ray, in Hit hit, in float albedo = 0.5f, in float fazz = 0.0f, in uint count = 10)
{
    ray.pos = hit.pos;
    ray.direction = Reflect(ray.direction, hit.normal);
    
    /* ぼかし処理 */
    if (fazz > 0.0f)
    {
        float max = float(0xffffffff);
        float3 random = 0.0f;
        random.x = 2.0f * Random(hit.pos.xy) - 1.0f;
        random.y = 2.0f * Random(hit.pos.yz) - 1.0f;
        random.z = 2.0f * Random(hit.pos.zx) - 1.0f;
        
        for (uint i = 0; i < count; ++i)
        {
            random.x = 2.0f * Random(random.xy, i) - 1.0f;
            random.y = 2.0f * Random(random.yz, i) - 1.0f;
            random.z = 2.0f * Random(random.zx, i) - 1.0f;
            
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
    
    return albedo * hit.color;
}

/* フレネルの法則 */
float Fresnel(in float refract, in float cos)
{
    /* Schlickの近似式 */
    float tmp = (1.0f - refract) / (1.0f + refract);
    tmp *= tmp;

    return tmp + (1.0f - tmp) * pow((1.0f - cos), 5.0f);
}

/* 屈折 */
float3 Refract(in out Ray ray, in Hit hit, in float refract)
{
    float rate = 1.0f / refract;
    float3 vec = normalize(ray.direction);
    float3 normal = hit.normal;
    if (dot(vec, normal) > 0.0f)
    {
        rate = refract;
        normal = -normal;
    }
    
    float cos_theta = min(dot(-vec, normal), 1.0f);
    float random = 1.0f;
    
    /* 判定式 
    * スネルの法則
    */
    float D = 1.0f - (rate * rate) * (1.0f - (cos_theta * cos_theta));
    if (D > 0.0f)
    {
        random = Fresnel(refract, cos_theta);
    }
    
    if (random < Random(ray.pos.xy))
    {
        ray.pos = hit.pos;
        ray.direction = rate * (vec + (cos_theta * normal)) - normal * sqrt(D);
    }
    else
    {
        ray.pos = hit.pos;
        ray.direction = Reflect(ray.direction, hit.normal);
    }
    
    return float3(1.0f, 1.0f, 1.0f);
}

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
    /* UV値の算出 */
    const float2 uv = (float2(semantics.group_ID.xy) + Random(semantics.group_ID.xy, semantics.group_thread_ID.x)) / float2(size);
    
    /* ヒット情報 */
    Hit hit =
    {
        -1.0f, float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), MATERIAL_NONE
    };
    /* レイ */
    Ray ray = 
    {
        eye_pos, normalize(left_pos + float3(uv * viewport, 0.0f) - eye_pos)
    };
    
    /* 球体 */
    Sphere sp[SPHERE_MAX] =
    {
        { float3(0.0f,    0.0f, -1.0f),   0.5f, float3(0.7f, 0.3f, 0.3f), MATERIAL_LAMBERT },
        { float3(0.0f, -100.5f, -2.0f), 100.0f, float3(0.8f, 0.8f, 0.0f), MATERIAL_LAMBERT },
        { float3(1.0f,    0.0f, -1.0f),   0.5f, float3(0.8f, 0.6f, 0.2f), MATERIAL_REFLECT },
        { float3(-1.0f,   0.0f, -1.0f),   0.5f, float3(0.8f, 0.8f, 0.8f), MATERIAL_REFRACT },
        { float3(-1.0f,   0.0f, -1.0f), -0.45f, float3(0.8f, 0.8f, 0.8f), MATERIAL_REFRACT },
    };
    
    float3 color = 1.0f;
    for (uint i = 0; i < 10; ++i)
    {
        if (IsSphereHit(ray, sp, hit) == true)
        {
            if (hit.material == MATERIAL_LAMBERT)
            {
                color *= Lambert(ray, hit);
            }
            else if (hit.material == MATERIAL_REFLECT)
            {
                color *= Metal(ray, hit);
            }
            else if (hit.material == MATERIAL_REFRACT)
            {
                color *= Refract(ray, hit, 1.5f);
            }
            else
            {
                color *= 0.5f * (hit.normal + 1.0f);
            }
        }
        else
        {
            color *= BackgroundColor(ray, float3(0.5f, 0.7f, 1.0f));
            break;
        }
    }
    
    /* カラー値を決定 */
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
    
    if (semantics.group_thread_ID.x == 0)
    {
        result[semantics.group_thread_ID.x] /= float(THREAD_X);
       
        tex[semantics.group_ID.xy] = float4(Linear(result[semantics.group_thread_ID.x]), 1.0f);
    }
}