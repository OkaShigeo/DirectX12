#pragma once
#include <vector>

namespace pmd
{
	/* �w�b�_�[��� */
	struct PmdHeader
	{
		union {
			struct {
				/* �}�W�b�N��` */
				char magic[3];
				/* �o�[�W���� */
				float version;
				/* ���f���� */
				char name[20];
				/* �R�����g */
				char comment[256];
			};
			/* �o�C�g�f�[�^ */
			std::uint8_t byte[283];
		};
	};
	/* ���_��� */
	struct PmdVertexInfo
	{
		union {
			struct {
				/* ���W */
				float pos[3];
				/* �@���x�N�g�� */
				float normal[3];
				/* UV���W */
				float uv[2];
				/* �{�[���ԍ� */
				std::uint16_t bone_index[2];
				/* �{�[���̉e���x(0�`100) */
				std::uint8_t bone_weight;
				/* �֊s���t���O(0:�ʏ�A1:�G�b�W����) */
				std::uint8_t edge_flag;
			};
			/* �o�C�g�f�[�^ */
			std::uint8_t byte[38];
		};
	};
	/* ���_���X�g */
	struct PmdVertexList
	{
		/* ���_�� */
		std::uint32_t num;
		/* ���_��� */
		std::vector<PmdVertexInfo>vertex;
	};
	/* �C���f�b�N�X���X�g */
	struct PmdIndexList
	{
		/* �C���f�b�N�X�� */
		std::uint32_t num;
		/* �C���f�b�N�X(3��1��) */
		std::vector<std::uint16_t>index;
	};
	/* �}�e���A����� */
	struct PmdMaterialInfo
	{
		union {
			struct {
				/* �����F */
				float diffuse[3];
				/* �����F�̕s�����x */
				float alpha;
				/* ���ʔ��˗� */
				float specularity;
				/* ����F */
				float specular[3];
				/* ���F */
				float ambient[3];
				/* �g�D�[���e�N�X�`���̎g�p�ԍ� */
				std::uint8_t toon_index;
				/* �֊s���t���O(0:�ʏ�A1:�G�b�W����) */
				std::uint8_t edge_flag;
				/* �g�p����C���f�b�N�X�� */
				std::uint32_t index_num;
				/* �e�N�X�`���t�@�C����(*.sph�ŏ�Z *.spa�ŉ��Z) */
				char texture_name[20];
			};
			/* �o�C�g�f�[�^ */
			std::uint8_t byte[70];
		};
	};
	/* �}�e���A�����X�g */
	struct PmdMaterialList
	{
		/* �}�e���A���� */
		std::uint32_t num;
		/* �}�e���A����� */
		std::vector<PmdMaterialInfo>material;
	};
	/* �{�[����� */
	struct PmdBoneInfo
	{
		/* �{�[���� */
		char name[20];
		/* �e�{�[���ԍ�(NONE=0xFFFF) */
		std::uint16_t parent_index;
		/* �q�{�[���ԍ�(NONE=0) */
		std::uint16_t child_index;
		/* ��� */
		std::uint8_t type;
		/* IK�{�[���ԍ�(NONE=0) */
		std::uint16_t ik_index;
		/* �w�b�h���W */
		float pos[3];
	};
	/* �{�[�����X�g */
	struct PmdBoneList
	{
		/* �{�[���� */
		std::uint16_t num;
		/* �{�[����� */
		std::vector<PmdBoneInfo>bone;
	};
}
