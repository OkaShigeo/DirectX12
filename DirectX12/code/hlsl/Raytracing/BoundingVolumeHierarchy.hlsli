#include "Ray.hlsli"

/* Axis Aligned Bounding Box�̏�� */
struct AxisAlignedBoundingBox
{
    /* �ŏ��ʒu */
    float3 min_pos;
    /* �ő�ʒu */
    float3 max_pos;
    
    /* ���S�ʒu�̎擾 */
    float3 GetCenter()
    {
        return (min_pos + max_pos) / 2.0f;
    }
    /* �����̖̂ʐς̎擾 */
    float GetArea()
	{
        return 2.0f * pow((max_pos.x - min_pos.x), 2.0f) + pow((max_pos.y - min_pos.y), 2.0f) + pow((max_pos.z - min_pos.z), 2.0f);
    }
    /* AABB�̃}�[�W */
    void Merge(in AxisAlignedBoundingBox aabb)
    {
        for (uint i = 0; i < 3; ++i)
        {
            min_pos[i] = min(min_pos[i], aabb.min_pos[i]);
            max_pos[i] = max(max_pos[i], aabb.max_pos[i]);
        }
    }
    /* �q�b�g���� */
    bool IsHit(in Ray ray, in out float min_time = 0.001f, in out float max_time = float(0xffffffff))
    {
        for (uint i = 0; i < 3; ++i)
        {
            float tmin = (min_pos[i] - ray.pos[i]) / ray.direction[i];
            float tmax = (max_pos[i] - ray.pos[i]) / ray.direction[i];

            /* �t�����̏ꍇ�͓���ւ� */
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

/* Bounding Volume�̏�� */
struct BoundingVolume
{
    /* AABB */
    AxisAlignedBoundingBox aabb;
	/* ���m�[�h�ԍ� */
    int left;
	/* �E�m�[�h�ԍ� */
    int right;
};

/* Bounding Volume Hierarchy��� */
struct BoundingVolumeHierarchy
{
    /* Bounding Volume�m�[�h */
    BoundingVolume node[512];
};