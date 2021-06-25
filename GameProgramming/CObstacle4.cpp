#include "CObstacle4.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CEffect.h"
#include "CBullet.h"

#define OBJ "cube.obj" //���f���̃t�@�C��
#define MTL "cube.mtl" //���f���̃}�e���A���t�@�C��

CModel CObstacle4::mModel; //���f���f�[�^�̍쐬

//�f�t�H���g�R���X�g���N�^
CObstacle4::CObstacle4()
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
	mTag = EBLOCK;
}

//�R���X�g���N�^
//CEnemy(�ʒu�A��]�A�g�k)
CObstacle4::CObstacle4(const CVector& position, const CVector& rotation, const CVector& scale)
:CObstacle4()
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
void CObstacle4::Update(){

	CTransform::Update();
}

//�Փˏ���
//Collider(�R���C�_1�A�R���C�_2)
void CObstacle4::Collision(CCollider *m, CCollider *o){
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

void CObstacle4::TaskCollision()
{
	mColSearch.ChangePriority();
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
}