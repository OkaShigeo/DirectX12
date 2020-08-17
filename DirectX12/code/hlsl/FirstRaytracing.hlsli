/* ルートシグネチャ */
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
					"DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL)"

/* グループスレッド数 */
#define THREAD_X 64
#define THREAD_Y 1
#define THREAD_Z 1

/* 球体の最大数 */
#define SPHERE_MAX 128

/* マテリアル種別 */
#define MATERIAL_NONE    0
#define MATERIAL_LAMBERT 1
#define MATERIAL_REFLECT 2
#define MATERIAL_REFRACT 3

/* エントリーポイントの引数 */
struct ComputeThreadID
{
    uint3 group_thread_ID : SV_GroupThreadID;
    uint3 group_ID : SV_GroupID;
    uint3 dispatch_ID : SV_DispatchThreadID;
    uint group_index : SV_GroupIndex;
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

/* カメラ */
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
};

/* レイ */
struct Ray
{
    /* 始点座標 */
    float3 pos;
    /* 移動方向 */
    float3 direction;
    /* モーションブラータイミング */
    float time;
    
    /* レイの位置を取得 */
    float3 GetPos(in float time)
    {
        /* 直線の方程式 */
        return pos + (time * direction);
    }
};

/* Axis Aligned Bounding Box */
struct AABB
{
    /* 最小位置 */
    float3 min_pos;
    /* 最大位置 */
    float3 max_pos;
    
    /* ヒット判定 */
    bool IsHit(in Ray ray, in out float min_time = 0.001f, in out float max_time = float(0xffffffff))
    {
        for (uint i = 0; i < 3; ++i)
        {
            float tmin = (min_pos[i] - ray.pos[i]) / ray.direction[i];
            float tmax = (max_pos[i] - ray.pos[i]) / ray.direction[i];

            /* 逆方向の場合は入れ替え */
            if (ray.direction[i] < 0.0f)
            {
                float tmp = tmin;
                tmin = tmax;
                tmax = tmp;
            }
            
            tmin = max(tmin, min_time);
            tmax = min(tmax, max_time);
            
            if (!(tmin < tmax))
            {
                return false;
            }
        }

        return true;
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
    float3 GetCenter(in Camera cam, in Ray ray, in float motion_start = 0.0f, in float motion_end = 1.0f)
    {
        float period = motion_end - motion_start;
        return center + (((period * ray.time) - motion_start) / (motion_end - motion_start)) * velocity;
    }
    
    /* ヒット判定 */
    bool IsHit(in Camera cam, in Ray ray, out Hit hit, in float min_distance = 0.001f, in float max_distance = float(0xffffffff))
    {
        float3 now_center = GetCenter(cam, ray);
        
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

/* 必要パラメータ */
cbuffer RaytracingParam : register(b0)
{
    Camera cam;
    uint sphere_max;
    Sphere sp[SPHERE_MAX];
}
/* テクスチャ */
RWTexture2D<float4> tex : register(u0);