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
    bool IsHit(in Ray ray, out float time, in float min = 0.001f, in float max = float(0xffffffff))
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
                //normal = float3(-normal.x, -normal.y, -normal.z);
            }
            
            return true;
        }

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

/* 角度からラジアン変換 */
float ChangeRadian(in float degree)
{
    return degree * acos(-1.0f) / 180.0f;
}

/* ランダム値の取得 */
float Randam(in float2 uv, in int seed = 0)
{
    return frac(sin(dot(sin(uv), float2(12.9898f, 78.233)) + seed) * 43758.5453);
}

/* 範囲内に収める */
float Clamp(in float value, in float min = 0.0f, in float max = 1.0f)
{
    if(value < min)
    {
        return min;
    }
    else if(value > max)
    {
        return max;
    }
    
    return value;
}

/* 背景色の算出 */
float3 BackColor(in Ray ray)
{
	/* 0～1の範囲に */
    float t = 0.5f * (ray.direction.y + 1.0f);
	/* 線形補完 */
    return (1.0f - t) * float3(1.0f, 1.0f, 1.0f) + (t * float3(0.5f, 0.7f, 1.0f));
}

/* ランバート反射 */
float3 LambertReflection(in float3 direction, in float3 normal, in float3 brightness = 1.0f, in float reflect = 1.0f)
{
    return brightness * max(0.0f, dot(direction, normal)) * reflect;
}

/* オーレン・ネイヤー反射 */
float3 OrenNayerReflection(in Ray ray, in float3 normal, in float3 reflect_vector, in float3 brightness = 1.0f, in float albedo = 1.0f, in float roughness = 0.0f)
{
    float s = dot(reflect_vector, ray.direction) - (dot(normal, reflect_vector) * dot(normal, ray.direction));
    float t = 1.0f;
    if(s > 0.0f)
    {
        t = max(dot(normal, reflect_vector), dot(normal, ray.direction));
    }
    
    float A = 1.0f / acos(-1.0f) * (1.0f - (0.5f * (pow(roughness, 2.0f) / (pow(roughness, 2.0f) + 0.33f))) + (0.17 * albedo * (pow(roughness, 2.0f) / (pow(roughness, 2.0f) + 0.13f))));
    float B = 1.0f / acos(-1.0f) * (0.45f * (pow(roughness, 2.0f) / (pow(roughness, 2.0f) + 0.09f)));
    return albedo * dot(normal, reflect_vector) * (A + (B * (s / t))) * brightness;

}

/* グループスレッド数 */
#define THREAD_X 256
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
        { float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), 0.5f },
        { float3(0.0f, -100.5f, 0.0f), float3(0.0f, 0.0f, 0.0f), 100.0f },
    };
    
    /* UV値の算出 */
    const float2 uv = (float2(semantics.group_ID.xy) + Randam(semantics.group_ID.xy, semantics.group_thread_ID.x)) / float2(size);
	/* レイ */
    Ray ray = { eye_pos, left_pos + float3(viewport * uv, 0.0f) - eye_pos };
        
    /* ヒットタイム */
    float hit_time = float(0xffffffff);
    /* ヒットカラー */
    result[semantics.group_thread_ID.x] = BackColor(ray);
    
    ///* 
    //* ヒット最小距離 
    //* シャドウアクネの除去
    //*/
    //const float min = 0.001f;
    ///* ヒット最大距離 */
    //float max = float(0xffffffff);
    ///* ヒット判定フラグ */
    //bool hit_flag = false;
    ///* ヒット位置の法線 */
    //float3 hit_normal = 0.0f;
    //for (uint sp_index = 0; sp_index < sp_num; ++sp_index)
    //{
    //    if (sp[sp_index].IsHit(ray, hit_time, min, max) == true)
    //    {
    //        hit_flag = true;
    //        hit_normal = sp[sp_index].normal;
    //        max = hit_time;
    //        result[semantics.group_thread_ID.x] = 0.5f * (sp[sp_index].normal + 1.0f);
    //    }
    //}
    
    float3 tmp = 1.0f;
    uint ref_cnt = 0;
    while (true)
    {
        if(ref_cnt >= 50)
        {
            tmp *= 0.0f;
            break;
        }
        
        /* 
        * ヒット最小距離 
        * シャドウアクネの除去
        */
        const float min = 0.001f;
        /* ヒット最大距離 */
        float max = float(0xffffffff);
        /* ヒット判定フラグ */
        bool hit_flag = false;
        /* ヒット位置の法線 */
        float3 hit_normal = 0.0f;
        for (uint sp_index = 0; sp_index < sp_num; ++sp_index)
        {
            if (sp[sp_index].IsHit(ray, hit_time, min, max) == true)
            {
                hit_flag = true;
                hit_normal = sp[sp_index].normal;
                max = hit_time;
                //result[semantics.group_thread_ID.x] = 0.5f * (sp[sp_index].normal + 1.0f);
            }
        }
        
        if (hit_flag == true)
        {
            
            float a = Randam(uv, ref_cnt) * 2.0f * acos(-1.0f);
            float z = 2.0f * Randam(uv, ref_cnt) - 1.0f;
            float r = sqrt(1.0f - z * z);
            
            float3 rand;
            rand.x = r * cos(a);
            rand.y = r * sin(a);
            rand.z = z;
            
            
            /* 反射すると色が半減していく */
            tmp *= 0.5f;
            /* 反射レイ */
            ray.pos = ray.At(hit_time);
            float3 target = ray.pos + hit_normal + rand;
            ray.direction = target - ray.pos;
            
            
            //ray.pos = ray.At(hit_time);
            //uint index = 0;
            //while (true)
            //{
            //    ray.direction.x = hit_normal.x + cos(Randam(uv, ref_index + index++) * acos(-1.0f));
            //    ray.direction.y = hit_normal.y + cos(Randam(uv, ref_index + index++) * acos(-1.0f));
            //    ray.direction.z = hit_normal.z + cos(Randam(uv, ref_index + index++) * acos(-1.0f));
            //    if (pow(length(ray.direction), 2.0f) < 1.0f)
            //    {
            //        break;
            //    }
            //}
            
            //tmp *= 0.5f;
            
            //Ray r = ray;
            //ray.pos = ray.At(hit_time);
            //ray.direction = Randam(uv, ref_index);
            
            //tmp *= OrenNayerReflection(r, hit_normal, ray.direction, float3(1.0f, 1.0f, 1.0f), 1.0f, acos(-1.0f) / 2.0f);

        }
        else
        {
            tmp *= BackColor(ray);
            break;
        }
        
        ++ref_cnt;
    }
    
    result[semantics.group_thread_ID.x] = tmp;
    
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
        /* ガンマ補正 */
        result[semantics.group_thread_ID.x] = sqrt((1.0f / THREAD_X) * result[semantics.group_thread_ID.x]);
        result[semantics.group_thread_ID.x].x = clamp(result[semantics.group_thread_ID.x].x, 0.001f, 1.0f);
        result[semantics.group_thread_ID.x].y = clamp(result[semantics.group_thread_ID.x].y, 0.001f, 1.0f);
        result[semantics.group_thread_ID.x].z = clamp(result[semantics.group_thread_ID.x].z, 0.001f, 1.0f);
        
        tex[semantics.group_ID.xy] = float4(result[semantics.group_thread_ID.x], 1.0f);
    }
}
