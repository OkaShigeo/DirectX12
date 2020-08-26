#include "Ray.hlsli"

/* Axis Aligned Bounding Boxの情報 */
struct AxisAlignedBoundingBox
{
    /* 最小位置 */
    float3 min_pos;
    /* 最大位置 */
    float3 max_pos;
    
    /* 中心位置の取得 */
    float3 GetCenter()
    {
        return (min_pos + max_pos) / 2.0f;
    }
    /* 直方体の面積の取得 */
    float GetArea()
	{
        return 2.0f * pow((max_pos.x - min_pos.x), 2.0f) + pow((max_pos.y - min_pos.y), 2.0f) + pow((max_pos.z - min_pos.z), 2.0f);
    }
    /* AABBのマージ */
    void Merge(in AxisAlignedBoundingBox aabb)
    {
        for (uint i = 0; i < 3; ++i)
        {
            min_pos[i] = min(min_pos[i], aabb.min_pos[i]);
            max_pos[i] = max(max_pos[i], aabb.max_pos[i]);
        }
    }
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

/* Bounding Volumeの情報 */
struct BoundingVolume
{
    /* AABB */
    AxisAlignedBoundingBox aabb;
	/* 左ノード番号 */
    int left;
	/* 右ノード番号 */
    int right;
};

/* Bounding Volume Hierarchy情報 */
struct BoundingVolumeHierarchy
{
    /* Bounding Volumeノード */
    BoundingVolume node[512];
};