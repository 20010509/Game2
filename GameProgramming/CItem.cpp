#include "CItem.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CEffect.h"
#include "CBullet.h"
#include "CKey.h"

#define OBJ "cube.obj" //���f���̃t�@�C��
#define MTL "cube.mtl" //���f���̃}�e���A���t�@�C��

CModel CItem::mModel; //���f���f�[�^�̍쐬

//�f�t�H���g�R���X�g���N�^
CItem::CItem()
: mCollider(this, &mMatrix, CVector(0.0f, 1.0f, 0.0f), 0.5f)
{
	//���f�����Ȃ����͓ǂݍ���
	if (mModel.mTriangles.size() == 0)
	{
		mModel.Load(OBJ, MTL);
	}
	//���f���̃|�C���^�ݒ�
	mpModel = &mModel;
	mTag = EITEM;
}

//�R���X�g���N�^
//CEnemy(�ʒu�A��]�A�g�k)
CItem::CItem(const CVector& position, const CVector& rotation, const CVector& scale)
:CItem()
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

	//mColliderMesh.Set(this, &mMatrix, mpModel);
}

//�X�V����
void CItem::Update(){

	mRotation.mY += 2.0f;

	CTransform::Update();
}

//�Փˏ���
//Collider(�R���C�_1�A�R���C�_2)
void CItem::Collision(CCollider *m, CCollider *o){

}

void CItem::TaskCollision()
{
	
	mCollider.ChangePriority();
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
	
}