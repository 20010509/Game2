#include "CGoalCheck.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CEffect.h"
#include "CBullet.h"

#define OBJ "goal.obj" //���f���̃t�@�C��
#define MTL "goal.mtl" //���f���̃}�e���A���t�@�C��

CModel CGoalCheck::mModel; //���f���f�[�^�̍쐬

//�f�t�H���g�R���X�g���N�^
CGoalCheck::CGoalCheck()
//: mColSearch(this, &mMatrix, CVector(0.0f, 0.0f, -10.0f), 30.0f)
//:mCollider(this, &mMatrix, CVector(0.0f, 1.0f, 0.0f), 1.0f)
{
	//���f�����Ȃ����͓ǂݍ���
	if (mModel.mTriangles.size() == 0)
	{
		mModel.Load(OBJ, MTL);
	}
	//���f���̃|�C���^�ݒ�
	mpModel = &mModel;
	//mColSearch.mTag = CCollider::ESEARCH; //�^�O�ݒ�
}

//�R���X�g���N�^
//CEnemy(�ʒu�A��]�A�g�k)
CGoalCheck::CGoalCheck(const CVector& position, const CVector& rotation, const CVector& scale)
:CGoalCheck()
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
}

//�X�V����
void CGoalCheck::Update(){

	CTransform::Update();
}

//�Փˏ���
//Collider(�R���C�_1�A�R���C�_2)
void CGoalCheck::Collision(CCollider *m, CCollider *o){

}

void CGoalCheck::TaskCollision()
{

}