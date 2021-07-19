#ifndef COBSTACLE5_H
#define COBSTACLE5_H
//�L�����N�^�N���X�̃C���N���[�h
#include "CCharacter.h"
//�R���C�_�N���X�̃C���N���[�h
#include "CCollider.h"

#include "CColliderMesh.h"
/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/
class CObstacle5 : public CCharacter{
private:

public:
	//���f���f�[�^
	static CModel mModel;
	//�R���C�_
	CCollider mCollider;
	//�R���X�g���N�^
	CObstacle5();
	//CEnemy(�ʒu�A��]�A�g��)
	CObstacle5(const CVector& position, const CVector& rotation, const CVector& scale);
	//�X�V����
	void Update();
	//�Փˏ���
	//Collider(�R���C�_1,�R���C�_2)
	void Collision(CCollider *m, CCollider *o);
	void TaskCollision();
	CCollider mColSearch; //�T�[�`�p�R���C�_
	CCharacter *mpPlayer; //�v���C���[�̃|�C���^
	int mHp; //�q�b�g�|�C���g
	CVector mPoint; //�ڕW�n�_

	CColliderMesh mColliderMesh;
};
#endif