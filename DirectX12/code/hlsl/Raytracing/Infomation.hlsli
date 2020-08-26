/* エントリーポイントの引数 */
struct ComputeThreadID
{
    uint3 group_thread_ID : SV_GroupThreadID;
    uint3 group_ID        : SV_GroupID;
    uint3 dispatch_ID     : SV_DispatchThreadID;
    uint group_index      : SV_GroupIndex;
};

/* カメラ情報 */
struct Camera
{
    /* 位置 */
    float3 pos;
    /* 視野角 */
    float fov;
    /* 注視点 */
    float3 target;
    /* レンズの大きさ */
    float lens_aperture;
    /* 上方向 */
    float3 up;
    /* アライメント */
    float alignment;
};

/* レイ情報 */
struct Ray
{
    /* 始点座標 */
    float3 pos;
    /* 移動方向 */
    float3 direction;
    /* モーションブラータイミング */
    float motion_blur_timing;
    
    /* レイの位置を取得 */
    float3 GetPos(in float time)
    {
        /* 直線の方程式 */
        return pos + (time * direction);
    }
};

/* マテリアル種別 */
#define MATERIAL_NONE    0
#define MATERIAL_LAMBERT 1
#define MATERIAL_REFLECT 2
#define MATERIAL_REFRACT 3

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
    /* 移動量 */
    float3 velocity;
    /* アライメント */
    float alignment;
    
    /* 半径の取得 */
    float GetRadius(in float3 pos)
    {
        float3 tmp = pos - center;

        /* 球体方程式 */
        return sqrt(dot(tmp, tmp));
    }
    /* 中心位置の取得 */
    float3 GetCenter(in Ray ray, in float move_start = 0.0f, in float move_end = 1.0f)
    {
        /* 移動間隔 */
        float period = move_end - move_start;
        
        return center + (((period * ray.motion_blur_timing) - move_start) / period) * velocity;
    }
    /* ヒット判定 */
    bool IsHit(in Ray ray, out Hit hit, in float min_distance = 0.001f, in float max_distance = float(0xFFFFFFFF))
    {
        /* 現在の中心位置 */
        float3 now_center = GetCenter(ray);
        
        /* ax^2 + bx + c = 0 */
        float3 oc = ray.pos - now_center;
        float a = dot(ray.direction, ray.direction);
        float b = 2.0f * dot(ray.direction, oc);
        float c = dot(oc, oc) - (radius * radius);
        
        bool flag = true;
        
        /* 判定式 */
        float D = (b * b) - (4.0f * a * c);
        if (D > 0.0f)
        {
            /* ヒットタイミングを算出 */
            float time = (-b - sqrt(D)) / (2.0f * a);
            /* ヒットタイミングが範囲内か確認 */
            if (!(min_distance < time && time < max_distance))
            {
                time = (-b + sqrt(D)) / (2.0f * a);
                if (!(min_distance < time && time < max_distance))
                {
                    flag = false;
                }
            }
            
            if (flag == true)
            {
                hit.time     = time;
                hit.pos      = ray.GetPos(hit.time);
                hit.normal   = (hit.pos - now_center) / radius;
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

/* 背景色の算出 */
float3 BackgroundColor(in Ray ray, in float3 finish_color, in float3 start_color = float3(1.0f, 1.0f, 1.0f))
{
    float time = 0.5f * (ray.direction.y + 1.0f);

    /* 線形補間 */
    return lerp(start_color, finish_color, time);
}

/* ランダム値の取得 */
float Random(in float2 uv, in int seed = 0)
{
    return frac(sin(dot(sin(uv), float2(12.9898f, 78.233)) + seed) * 43758.5453);
}

/* ランバート反射 */
float3 LambertReflect(in out Ray ray, in Hit hit, in float albedo, in uint loop = 10)
{
    /* ヒット位置から反射方向に単位球があると仮定し、反射位置が単位球内にあるまでループ */
    float3 random = 0.0f;
    random.x = 2.0f * Random(hit.pos.xy) - 1.0f;
    random.y = 2.0f * Random(hit.pos.yz) - 1.0f;
    random.z = 2.0f * Random(hit.pos.zx) - 1.0f;
    
    float value = float(0xffffffff);
    
    for (uint i = 0; i < loop; ++i)
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
        if (tmp < 1.0f)
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

/* 反射 */
float3 Reflect(in float3 direction, in float3 normal)
{
    return reflect(normalize(direction), normal);
    
    /* 入射ベクトルと法線ベクトルが単位ベクトルの場合の反射 */
    return direction + 2.0f * normal;
    /* 法線ベクトルだけ単位ベクトルの場合の反射 */
    //return direction - 2.0f * dot(direction, normal) * normal;
}

/* 鏡面反射 */
float3 SpecularReflection(in out Ray ray, in Hit hit, in float albedo, in float fazz = 0.0f, in uint loop = 10)
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
        
        for (uint i = 0; i < loop; ++i)
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
float3 Refraction(in out Ray ray, in Hit hit, in float refract)
{
    float rate    = 1.0f / refract;
    float3 vec    = normalize(ray.direction);
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

/* 被写界深度のオフセットを取得 */
float3 GetDepthOfFieldOffset(in Camera cam, in float2 uv, in uint loop = 10)
{
    float max = float(0xffffffff);
    float3 random = 0.0f;
    random.x = 2.0f * Random(uv) - 1.0f;
    random.y = 2.0f * Random(uv) - 1.0f;
    random.z = 2.0f * Random(uv) - 1.0f;
        
    for (uint i = 0; i < loop; ++i)
    {
        random.x = 2.0f * Random(random.xy, i) - 1.0f;
        random.y = 2.0f * Random(random.yz, i) - 1.0f;
        random.z = 2.0f * Random(random.zx, i) - 1.0f;
            
        float tmp = pow(length(float3(random.x, random.y, 0.0f)), 2.0f);
        if (tmp < 1.0f)
        {
            break;
        }
        else
        {
            max = min(max, tmp);
        }
    }
    
    float3 dir = normalize(cam.pos - cam.target);
    float3 u = normalize(cross(cam.up, dir));
    float3 v = cross(dir, u);
    
    return (u * random.x * (cam.lens_aperture / 2.0f)) + (v * random.y * (cam.lens_aperture / 2.0f));
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

/* ラジアン角度に変換 */
float Radian(in float angle)
{
    return angle * acos(-1.0f) / 180.0f;
}
