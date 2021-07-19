#include "CGoal.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CEffect.h"
#include "CBullet.h"

#define OBJ "cube.obj" //���f���̃t�@�C��
#define MTL "cube.mtl" //���f���̃}�e���A���t�@�C��

CModel CGoal::mModel; //���f���f�[�^�̍쐬

//�f�t�H���g�R���X�g���N�^
CGoal::CGoal()
//: mColSearch(this, &mMatrix, CVector(0.0f, 0.0f, -10.0f), 30.0f)
:mCollider(this, &mMatrix, CVector(0.0f, 1.0f, 0.0f), 1.0f)
{
	//���f�����Ȃ����͓ǂݍ���
	if (mModel.mTriangles.size() == 0)
	{
		mModel.Load(OBJ, MTL);
	}
	//���f���̃|�C���^�ݒ�
	mpModel = &mModel;
	//mColSearch.mTag = CCollider::ESEARCH; //�^�O�ݒ�
	mTag = EGOAL;
}

//�R���X�g���N�^
//CEnemy(�ʒu�A��]�A�g�k)
CGoal::CGoal(const CVector& position, const CVector& rotation, const CVector& scale)
:CGoal()
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
void CGoal::Update(){

	CTransform::Update();
}

//�Փˏ���
//Collider(�R���C�_1�A�R���C�_2)
void CGoal::Collision(CCollider *m, CCollider *o){
	//���肪�v���C���[�ȊO�̎��͖߂�
	if (o->mpParent->mTag != EPLAYER)
	{
		return;
	}
	//���������R���C�_�̎�
	if (m->mType == CCollider::ESPHERE)
	{
		//���肪���R���C�_�̎�
		if (o->mType == CCollider::ESPHERE)
		{
			//�Փ˂��Ă���Ƃ�
			if (CCollider::Collision(m, o))
			{
				mEnabled = false;
			}
		}
		return;
	}
}

void CGoal::TaskCollision()
{
	mCollider.ChangePriority();
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}