/* �����\���̃f�[�^ */
RaytracingAccelerationStructure Scene : register(t0, space0);
/* �o�͐�e�N�X�`���f�[�^ */
RWTexture2D<float4> Texture : register(u0, space0);

/* �q�b�g��� */
struct Payload
{
    /* �q�b�g�����F */
    float4 color;
};

/* �q�b�g���� */
struct Attributes
{
    /* �O�p�`�̒��S�ʒu */
    float2 center;
};