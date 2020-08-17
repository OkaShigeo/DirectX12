#include "FirstRaytracing.hlsli"

/* �w�i�F�̎Z�o */
float3 BackgroundColor(in Ray ray, in float3 end, in float3 begin = float3(1.0f, 1.0f, 1.0f))
{
    float time = 0.5f * (ray.direction.y + 1.0f);

    /* ���`��� */
    return lerp(begin, end, time);
}

/* ���̂̃q�b�g���� */
bool IsSphereHit(in Camera cam, in Ray ray, in Sphere sp[SPHERE_MAX], out Hit hit)
{
    hit.time = -1.0f;
    hit.pos = hit.normal = hit.color = 0.0f;
    
    float min = 0.001f;
    float max = float(0xffffffff);
    for (uint i = 0; i < sphere_max; ++i)
    {
        if (i < SPHERE_MAX)
        {
            Hit dummy = hit;
              
            if (sp[i].IsHit(cam, ray, dummy, min, max) == true)
            {
                hit = dummy;
                max = hit.time;
            }
        }
        else
        {
            break;
        }
    }

    return (hit.time >= 0.0f);
}

/* �����_���l�̎擾 */
float Random(in float2 uv, in int seed = 0)
{
    return frac(sin(dot(sin(uv), float2(12.9898f, 78.233)) + seed) * 43758.5453);
}

/* �����o�[�g���� */
float3 Lambert(in out Ray ray, in Hit hit, in float albedo = 0.5f, in uint count = 10)
{
    /* �q�b�g�ʒu���甽�˕����ɒP�ʋ�������Ɖ��肵�A���ˈʒu���P�ʋ����ɂ���܂Ń��[�v */
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

/* ���j�A�F��Ԃɕϊ� */
float3 Linear(in float3 color)
{
    return pow(color, 1.0f / 2.2f);
}

/* sRGB�F��Ԃɕϊ� */
float3 sRGB(in float3 color)
{
    return pow(color, 2.2f);
}

/* ���� */
float3 Reflect(in float3 direction, in float3 normal)
{
    return reflect(normalize(direction), normal);
    
    /* ���˃x�N�g���Ɩ@���x�N�g�����P�ʃx�N�g���̏ꍇ�̔��� */
    return direction + 2.0f * normal;
    /* �@���x�N�g�������P�ʃx�N�g���̏ꍇ�̔��� */
    //return direction - 2.0f * dot(direction, normal) * normal;
}

/* ���ʔ��� */
float3 Metal(in out Ray ray, in Hit hit, in float albedo = 0.5f, in float fazz = 0.0f, in uint count = 10)
{
    ray.pos = hit.pos;
    ray.direction = Reflect(ray.direction, hit.normal);
    
    /* �ڂ������� */
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

/* �t���l���̖@�� */
float Fresnel(in float refract, in float cos)
{
    /* Schlick�̋ߎ��� */
    float tmp = (1.0f - refract) / (1.0f + refract);
    tmp *= tmp;

    return tmp + (1.0f - tmp) * pow((1.0f - cos), 5.0f);
}

/* ���� */
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
    
    /* ���莮 
    * �X�l���̖@��
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

/* ���W�A���p�x�ɕϊ� */
float Radian(in float angle)
{
    return angle * acos(-1.0f) / 180.0f;
}

/* ��ʊE�[�x */
float3 DepthOfField(in Camera cam, in float2 uv, in uint count = 10)
{
    float max = float(0xffffffff);
    float3 random = 0.0f;
    random.x = 2.0f * Random(uv) - 1.0f;
    random.y = 2.0f * Random(uv) - 1.0f;
    random.z = 2.0f * Random(uv) - 1.0f;
        
    for (uint i = 0; i < count; ++i)
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
    float3 u   = normalize(cross(cam.up, dir));
    float3 v   = cross(dir, u);
    
    return (u * random.x * (cam.lens_aperture / 2.0f)) + (v * random.y * (cam.lens_aperture / 2.0f));
}

/* �O���[�v�X���b�h���Ƃ̌v�Z����  */
groupshared float3 result[THREAD_X];

[RootSignature(RS)]
[numthreads(THREAD_X, THREAD_Y, THREAD_Z)]
void main(ComputeThreadID semantics)
{
    /* �q�b�g��� */
    Hit hit =
    {
        -1.0f, float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), float3(0.0f, 0.0f, 0.0f), MATERIAL_NONE
    };
    
	/* �e�N�X�`���T�C�Y�̎擾 */ 
    uint2 size = 0;
    tex.GetDimensions(size.x, size.y);
    
    /* �A�X�y�N�g�䗦 */
    const float aspect = float(size.x) / float(size.y);
    /* �r���[�|�[�g */
    const float2 viewport = float2(aspect * 2.0f * tan(Radian(cam.fov) / 2.0f), 2.0f * tan(Radian(cam.fov) / 2.0f));
    /* �W������ */
    const float focus_distance = length(cam.pos - cam.target);
    
    /* �ʒu�C�� */
    float3 dir        = normalize(cam.pos - cam.target);
    float3 horizontal = viewport.x * normalize(cross(cam.up, dir)) * focus_distance;
    float3 vertical   = viewport.y * cross(dir, normalize(cross(cam.up, dir))) * focus_distance;
    
    float3 left = cam.pos - ((horizontal + vertical) / 2.0f) - (dir * focus_distance);
    /* UV�l�̎Z�o */
    const float2 uv = (float2(semantics.group_ID.xy) + Random(semantics.group_ID.xy, semantics.group_thread_ID.x)) / float2(size);
    
    float3 offset = DepthOfField(cam, uv);
    
    /* ���C */
    Ray ray =
    {
        cam.pos + offset, left + (horizontal * uv.x) + (vertical * uv.y) - ray.pos, Random(uv, semantics.group_index)
    };
    
    float3 color = 1.0f;
    for (uint i = 0; i < 10; ++i)
    {
        if (IsSphereHit(cam, ray, sp, hit) == true)
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
            color *= BackgroundColor(ray, float3(0.6f, 0.8f, 1.0f));
            break;
        }
    }
    
    /* �J���[�l������ */
    result[semantics.group_thread_ID.x] = color;
    
    /* �O���[�v�X���b�h�̓��� */
    GroupMemoryBarrierWithGroupSync();
    
    for (int th_index = THREAD_X / 2; th_index > 0; th_index /= 2)
    {
        if (semantics.group_thread_ID.x < th_index)
        {
            result[semantics.group_thread_ID.x] += result[semantics.group_thread_ID.x + th_index];
        }
            
        /* �O���[�v�X���b�h�̓��� */
        GroupMemoryBarrierWithGroupSync();
    }
    
    if (semantics.group_thread_ID.x == 0)
    {
        result[semantics.group_thread_ID.x] /= float(THREAD_X);
       
        tex[semantics.group_ID.xy] = float4(Linear(result[semantics.group_thread_ID.x]), 1.0f);
    }
}