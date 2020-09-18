#include "DirectXRaytracing.hlsli"

/* ���C�����V�F�[�_ */
[shader("raygeneration")]
void RayGen()
{
    /* ���C���˃O���b�h�ԍ��̎擾 */
    uint2 index = DispatchRaysIndex();

    /* ���C�̏����` */
    RayDesc ray;
    /* ���_ */
    ray.Origin    = float3(0.0f, 0.0f, 0.0f);
    /* ���C�̐i�ޕ��@ */
    ray.Direction = float3(0.0f, 0.0f, 1.0f);
    /* ���C�̃q�b�g����J�n���� */
    ray.TMin      = 0.0f;
    /* ���C�̃q�b�g����I������ */
    ray.TMax      = 10000.0f;

    Payload payload;
    
    /* ���C�̐���t���O */
    uint flag           = RAY_FLAG_NONE;
    /* �q�b�g����}�X�N */
    uint hit_mask       = 0x00FF;
    /* �q�b�g�O���[�v�ԍ��̃I�t�Z�b�g */
    uint group_offset   = 0x00;
    /* �q�b�g�O���[�v�̉��� */
    uint group_multiple = 0x01;
    /* �~�X�V�F�[�_�̌Ăяo���ԍ� */
    uint miss_index     = 0x00000000;

    /* ���C�̐��� */
    TraceRay(Scene, flag, hit_mask, group_offset, group_multiple, miss_index, ray, payload);

    /* ���ʂ��i�[ */
    Texture[index.xy] = payload.color;
}