#include "CBlockSide.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CEffect.h"
#include "CBullet.h"

#define OBJ "plane.obj" //���f���̃t�@�C��
#define MTL "plane.mtl" //���f���̃}�e���A���t�@�C��

CModel CBlockSide::mModel; //���f���f�[�^�̍쐬

//�f�t�H���g�R���X�g���N�^
CBlockSide::CBlockSide()
//: mColSearch(this, &mMatrix, CVector(0.0f, 0.0f, -10.0f), 30.0f)
{
	//���f�����Ȃ����͓ǂݍ���
	if (mModel.mTriangles.size() == 0)
	{
		mModel.Load(OBJ, MTL);
	}
	//���f���̃|�C���^�ݒ�
	mpModel = &mModel;
	//mColSearch.mTag = CCollider::ESEARCH; //�^�O�ݒ�
	mTag = EBLOCKSIDE;
}

//�R���X�g���N�^
//CEnemy(�ʒu�A��]�A�g�k)
CBlockSide::CBlockSide(const CVector& position, const CVector& rotation, const CVector& scale)
:CBlockSide()
{
	//�ʒu�A��]�A�g�k��ݒ肷��
	mPosition = position; //�ʒu�̐ݒ�
	mRotation = rotation; //��]�̐ݒ�
	mScale = scale; //�g�k�̐ݒ�
	CTransform::Update(); //�s��̐ݒ�
	//�D��x��1�ɕύX����
	mPriority = 1;
	CTaskManager::Get()->Remove(this); //�폜����
	CTaskManager::Get()->Add(this); //�ǉ�����

	mColliderMesh.Set(this, &mMatrix, mpModel);
}

//�X�V����
void CBlockSide::Update(){

	CTransform::Update();
}

//�Փˏ���
//Collider(�R���C�_1�A�R���C�_2)
void CBlockSide::Collision(CCollider *m, CCollider *o){
	//���肪�T�[�`�̎��͖߂�
	if (o->mTag == CCollider::ESEARCH)
	{
		return;
	}
	//�������T�[�`�p�̎�
	if (m->mTag == CCollider::ESEARCH)
	{
		//���肪���R���C�_�̎�
		if (o->mType == CCollider::ESPHERE)
		{
			//���肪�v���C���[�̎�
			if (o->mpParent->mTag == EPLAYER)
			{
				//�Փ˂��Ă���Ƃ�
				if (CCollider::Collision(m, o))
				{

				}
			}
		}
		return;
	}
}

void CBlockSide::TaskCollision()
{
	mColSearch.ChangePriority();
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
}