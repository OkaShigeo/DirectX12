#include "Infomation.hlsli"

/* ���[�g�V�O�l�`�� */
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL)"

/* ���̂̏�� */
RWStructuredBuffer<Sphere> sphere : register(u0);