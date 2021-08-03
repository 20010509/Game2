#ifndef CPLAYER_H
#define CPLAYER_H
//�L�����N�^�[�N���X�̃C���N���[�h
#include"CCharacter.h"
#include"CBullet.h"
#include"CColliderLine.h"
#include"CText.h"
/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/
class CPlayer :public CCharacter{
private:
	float mJumpPower;			//�W�����v��
	bool mJumpFlag;				//�W�����v���Ă��邩���f����

	int mSlidingCount;			//�X���C�f�B���O�̎�������
	bool mSlidingFlag;			//�X���C�f�B���O���Ă��邩���f����

	float mSideMoveSpeed;		//�ׂ̃��[���ֈړ�����Ƃ��̃X�s�[�h
	bool mSideMoveFlagL;		//�v���C���[�����[�����ړ�����Ƃ���true��Ԃ�(������)
	bool mSideMoveFlagR;		//�v���C���[�����[�����ړ�����Ƃ���true��Ԃ�(�E����)
	int mSideMoveCount;			//�ׂ̃��[���ֈړ����鎞��
	int mNowLane;				//�v���C���[���������Ă��郌�[��

	int mInvincibleTime;		//���G����
	bool mInvincibleFlag;		//���G��Ԃ��ǂ����𔻒f

	int mHp;					//�̗�
	float mRunSpeed;			//�X�s�[�h
	int mItem;					//�A�C�e���̏�����

	bool mGoal;					//�S�[�����Ă����true��Ԃ�
	bool mGameOver;				//�Q�[���I�[�o�[�ɂȂ��true��Ԃ�

	bool mCrushFlag;			//�ׂ�Ă��邩���f
	int mCrushTime;				//�ׂ�Ă��鎞��

	bool mRanding;				//�ڒn���Ă��邩���f

	bool mBallCollision;		//�]���鋅�Ɠ������Ă�����true��Ԃ�

	bool mBlockUpCollision;		//�u���b�N�̏�̖ʂɓ������Ă����true��Ԃ�
	bool mBlockSideCollision;	//�u���b�N�̉��̖ʂɓ������Ă����true��Ԃ�

	bool mInvincibleFlagDummy;

	bool mStart;

public:
	//�X�V����
	void Update();

	CColliderLine mLine; //�����R���C�_
	CColliderLine mLine2;
	CColliderLine mLine3;

	//�f�t�H���g�R���X�g���N�^
	CPlayer();

	//�Փˏ���
	void Collision(CCollider *m, CCollider *o);

	void TaskCollision();

	CText mText;

	void Render();

	CCollider mCollider; //�{�̗p�R���C�_

};
#endif