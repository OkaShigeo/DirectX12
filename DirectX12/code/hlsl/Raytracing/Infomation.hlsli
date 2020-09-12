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
    /* ヒットUV座標 */
    float2 uv;
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
    
    /*＊ 半径の取得 
     * @param pos 位置
     * @return 円の半径
     */
    float GetRadius(in float3 pos)
    {
        float3 tmp = pos - center;

        /* 球体方程式 */
        return sqrt(dot(tmp, tmp));
    }
    /*＊ 中心位置の取得
     * @param ray レイの情報
     * @param 移動開始時間
     * @param 移動終了時間
     * @return 円の中心位置
     */
    float3 GetCenter(in Ray ray, in float move_start = 0.0f, in float move_end = 1.0f)
    {
        /* 移動間隔 */
        float period = move_end - move_start;
        
        return center + (((period * ray.motion_blur_timing) - move_start) / period) * velocity;
    }
    /*＊ ヒット判定 
     * @param ray レイの情報
     * @param hit ヒット情報の格納先
     * @param min_distance レイの最短距離
     * @param max_distance レイの最大距離
     * @return true:ヒットあり　false:ヒットなし
     */
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
                
                float pi    = acos(-1.0f);
                float phi   = atan2(hit.normal.z, hit.normal.x);
                float theta = asin(hit.normal.y);
                
                hit.uv = float2(1.0f - (phi + pi) / (2.0f * pi), ((theta + pi) / 2.0f) / pi);
                
                return true;
            }
        }
        
        hit.time = -1.0f;
        hit.pos = hit.normal = hit.color = 0.0f;
        hit.material = MATERIAL_NONE;
        
        return false;
    }
};

/*＊ ランダム値の取得 
 * @param uv 適当な数値
 * @param seed シード値
 * @return ランダム値(0.0f～1.0f)
 */
float Random(in float2 uv, in int seed = 0)
{
    return frac(sin(dot(sin(uv), float2(12.9898f, 78.233)) + seed) * 43758.5453);
}

/*＊ 二次元なランダム値の取得 
 * @param uv 適当な数値
 * @param seed シード値
 * @return ランダム値(0.0f～1.0f)
 */
float2 Random2(in float2 uv, in int seed = 0)
{
    float2 tmp = float2(dot(uv, float2(127.1f, 311.7f)), dot(uv, float2(269.5f, 183.3f)));
    
    return 2.0f * frac(sin(tmp + seed) * 43758.5453123f) - 1.0f;
}

/** ランバート反射 
 * @param ray レイの情報
 * @param hit ヒット情報の格納先
 * @param specularity 鏡面率(0.0f～1.0f)
 * @param loop 模索回数
 * @return カラー値(0.0f～1.0f)
 */
float3 LambertReflect(in out Ray ray, in Hit hit, in float specularity, in uint loop = 10)
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
    
    return specularity * hit.color;
}

/*＊ 反射
 * @param direction レイの方向
 * @param normal 法線
 * @return 反射方向
 */
float3 Reflect(in float3 direction, in float3 normal)
{
    return reflect(normalize(direction), normal);
    
    /* 入射ベクトルと法線ベクトルが単位ベクトルの場合の反射 */
    return direction + 2.0f * normal;
    /* 法線ベクトルだけ単位ベクトルの場合の反射 */
    //return direction - 2.0f * dot(direction, normal) * normal;
}

/** 鏡面反射 
 * @param ray レイの情報
 * @param hit ヒット情報の格納先
 * @param specularity 鏡面率(0.0f～1.0f)
 * @param fazz ぼやかし(0.0f～1.0f)
 * @param loop 模索回数
 */
float3 SpecularReflection(in out Ray ray, in Hit hit, in float specularity, in float fazz = 0.0f, in uint loop = 10)
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
    
    return specularity * hit.color;
}

/** フレネルの法則 
 * @param refract 屈折率
 * @param cos コサイン値
 * @return フレネル値
 */
float Fresnel(in float refract, in float cos)
{
    /* Schlickの近似式 */
    float tmp = (1.0f - refract) / (1.0f + refract);
    tmp *= tmp;

    return tmp + (1.0f - tmp) * pow((1.0f - cos), 5.0f);
}

/** 屈折 
 * @param ray レイの情報
 * @param hit ヒット情報の格納先
 * @param refract 屈折率
 * @return カラー値(0.0f～1.0f)
 */
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

/*＊ 被写界深度のオフセットを取得 
 * @param cam カメラ情報
 * @param uv UV座標(0.0f～1.0f)
 * @param loop 模索回数
 */
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

/*＊ リニア色空間に変換 
 * @param color カラー値(0.0f～1.0f)
 * @return ガンマ補正適用カラー値(0.0f～1.0f)
 */
float3 Linear(in float3 color)
{
    return pow(color, 1.0f / 2.2f);
}

/*＊ sRGB色空間に変換 
 * @param color カラー値(0.0f～1.0f)
 * @return SRGB補正を適用したカラー値(0.0f～1.0f)
 */
float3 sRGB(in float3 color)
{
    return pow(color, 2.2f);
}

/*＊ ラジアン角度に変換 
 * @param angle 弧度法(0.0f～360.0f)
 * @return ラジアン角度
 */
float Radian(in float angle)
{
    return angle * acos(-1.0f) / 180.0f;
}

/** ランダムノイズ
 * @param uv 適当な数値(0.0f～1.0f)
 * @param seed シード値
 * @return ランダム値(0.0f～1.0f)
 */
float RandomNoise(in float2 uv, in int seed = 0)
{
    return 2.0f * Random(uv, seed) - 1.0f;
}

/** ブロックノイズ
 * @param uv 適当な値(0.0f～1.0f)
 * @param block_num ブロックの数
 * @param seed シード値
 * @return ランダム値(0.0f～1.0f)
 */
float BlockNoise(in float2 uv, in float block_num, in int seed = 0)
{
    /* 整数部の抽出 */
    float2 integer = float2(floor(uv.x * block_num), floor(uv.y * block_num));
    
    return RandomNoise(integer, seed);
}

/** バリューノイズ 
 * @param uv 適当な数値(0.0f～1.0f)
 * @param block_num ブロックの数
 * @param seed シード値
 * @return ランダム値(0.0f～1.0f)
 */
float ValueNoise(in float2 uv, in float block_num, in int seed = 0)
{
    /* 拡大後のサイズ */
    float2 size = uv * block_num;
    
    /* 整数部の抽出 */
    float2 integer = float2(floor(size.x), floor(size.y));
    /* 小数部の抽出 */
    float2 decimal = float2(frac(size.x), frac(size.y));
    
   /* 補間基準値(三次エルミート曲線) */
    float2 interpolation = -(2.0f * pow(decimal, 3.0f)) + (3.0f * pow(decimal, 2.0f));
    
    /* 最小値 */
    float2 min_value = float2(Random(integer + float2(0.0f, 0.0f)), Random(integer + float2(0.0f, 1.0f)));
    /* 最大値 */
    float2 max_value = float2(Random(integer + float2(1.0f, 0.0f)), Random(integer + float2(1.0f, 1.0f)));
    
    /* X軸に対する補間値 */
    float2 lerp_value = float2(lerp(min_value.x, max_value.x, interpolation.x), lerp(min_value.y, max_value.y, interpolation.x));
    
    /* Y軸に対する補間値 */
    return lerp(lerp_value.x, lerp_value.y, interpolation.y);
}

/** パーリンノイズ
 * @param uv 適当な数値(0.0f～1.0f)
 * @param block_num ブロック数
 * @param seed シード値
 * @return ランダム値
 */
float PerlinNoise(in float2 uv, in float block_num, in int seed = 0)
{
    /* 拡大後のサイズ */
    float2 size = uv * block_num;
    
    /* 整数部の抽出 */
    float2 integer = float2(floor(size.x), floor(size.y));
    /* 小数部の抽出 */
    float2 decimal = float2(frac(size.x), frac(size.y));
    
    /* 補間基準値(三次エルミート曲線) */
    float2 interpolation = -(2.0f * pow(decimal, 3.0f)) + (3.0f * pow(decimal, 2.0f));

    /* 勾配ベクトル */
    float2 gradient_1 = Random2(integer + float2(0.0f, 0.0f), seed);
    float2 gradient_2 = Random2(integer + float2(1.0f, 0.0f), seed);
    float2 gradient_3 = Random2(integer + float2(0.0f, 1.0f), seed);
    float2 gradient_4 = Random2(integer + float2(1.0f, 1.0f), seed);
    
    /* 距離ベクトル */
    float2 distance_1 = decimal - float2(0.0f, 0.0f);
    float2 distance_2 = decimal - float2(1.0f, 0.0f);
    float2 distance_3 = decimal - float2(0.0f, 1.0f);
    float2 distance_4 = decimal - float2(1.0f, 1.0f);
    
    /* 各X軸の線形補間 & 各Y軸の線形補間 */
    return lerp(lerp(dot(gradient_1, distance_1), dot(gradient_2, distance_2), interpolation.x),
                lerp(dot(gradient_3, distance_3), dot(gradient_4, distance_4), interpolation.x), interpolation.y) * 0.5f + 0.5f;
}

/** フェード関数
 * @param time タイミング(0.0f～1.0f)
 * @return フェード値
 */
float Fade(in float time)
{
    /* イーズ曲線による補間 */
    return (6.0f * pow(time, 5.0f)) - (15.0f * pow(time, 4.0f)) + (10.0f * pow(time, 3.0f));
}

/** シンプレックスノイズ 
 */
float3 SimplexNoise(in float2 uv, in float block_num)
{
     /* 拡大後のサイズ */
    float2 size = uv * block_num;
    
    size.x *= 1.1547f;
    size.y += size.x * 0.5f;
    
    /* 整数部の抽出 */
    float2 integer = float2(floor(size.x), floor(size.y));
    /* 小数部の抽出 */
    float2 decimal = float2(frac(size.x), frac(size.y));
    
    /* 補間基準値(イーズ曲線) */
    float2 interpolation = (6.0f * pow(decimal, 5.0f)) - (15.0f * pow(decimal, 4.0f)) + (10.0f * pow(decimal, 3.0f));
    
    if (decimal.x > decimal.y)
    {
        return float3(float2(1.0f, 1.0f) - float2(decimal.x, decimal.y - decimal.x), decimal.y);
    }
    
    return float3(float2(1.0f, 1.0f) - float2(decimal.x - decimal.y, decimal.y), decimal.x);
}
