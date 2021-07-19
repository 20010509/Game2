//�v���C���[�N���X�̃C���N���[�h
#include"CPlayer.h"
//�L�[���̓N���X�̃C���N���[�h
#include"CKey.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include "CUtil.h"

#define GRAVITY 0.035f			//�d��
#define JUMPPOWER 0.6f			//�W�����v��
#define MAXRUNSPEED 0.5f		//�ō����x
#define SIDEMOVESPEED 0.4f		//�����[���ֈړ�����X�s�[�h
#define INITIALIZE 0			//�l��������
#define SIDEMOVECOUNT 9			//�ׂ̃��[���ֈړ����鎞��(�t���[����)
#define SLIDINGCOUNT 30			//�X���C�f�B���O�̎�������
#define INVINCIBLETIME 60		//���G����
#define INVINCIBLETIME_ITEM 300	//���G����(�A�C�e���g�p)
#define HP 5					//�̗�
#define ACCELERATION 0.007f		//�����p
#define CRUSHMAXSPEED 0.25f		//�ׂ�Ă���Ƃ��̍ō����x
#define CRUSHTIME 120			//�ׂ�Ă��鎞��

#define RUNSPEED 1.0f

CPlayer::CPlayer()
:mLine(this, &mMatrix, CVector(0.0f, 0.0f, -1.0f), CVector(0.0f, 0.0f, 1.0f))
,mLine2(this, &mMatrix, CVector(0.0f, 1.0f, 0.0f), CVector(0.0f, -1.0f, 0.0f))
,mLine3(this, &mMatrix, CVector(1.0f, 0.0f, 0.0f), CVector(-1.0f, 0.0f, 0.0f))
,mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f),1.0f)
, mJumpPower(INITIALIZE)
, mJumpFlag(false)
, mSlidingFlag(false)
, mSlidingCount(INITIALIZE)
, mSideMoveSpeed(INITIALIZE)
, mSideMoveFlagL(false)
, mSideMoveFlagR(false)
, mSideMoveCount(INITIALIZE)
, mNowLane(INITIALIZE)
//, mSurface(INITIALIZE)
, mHp(HP)
, mInvincibleTime(INITIALIZE)
, mInvincibleFlag(false)
, mBlockUpCollision(false)
, mItem(INITIALIZE)
, mRunSpeed(INITIALIZE)
, mGoal(false)
, mCrushFlag(false)
, mCrushTime(INITIALIZE)
, mRanding(false)
, mBallCollision(false)
{
	//�e�N�X�`���t�@�C���̓ǂݍ���(1�s64��)
	mText.LoadTexture("FontWhite.tga", 1, 64);
	mTag = EPLAYER; //�^�O�ݒ�
}

//�X�V����
void CPlayer::Update(){

	//�A�C�e���g�p��
	if (CKey::Once(VK_SPACE) && mItem > 0 && mInvincibleFlag == false){
		//�A�C�e���̏����������炷
		mItem--;
		//���G�ɂȂ�
		mInvincibleFlag = true;
		mInvincibleTime = INVINCIBLETIME_ITEM;
	}

	//���G���Ԃ����炷
	if (mInvincibleTime > 0){
		mInvincibleTime--;
	}
	//���G���Ԃ�0������ȉ��ɂȂ����疳�G��Ԃ���������
	if (mInvincibleTime <= 0){
		mInvincibleFlag = false;
	}

	//A�L�[���͂ō����[���ֈړ�
	if (CKey::Once('A') && mNowLane > -1 && mSideMoveFlagR == false && mSideMoveFlagL == false){
		//Y���̉�]�l�𑝉�
		//mRotation.mY += 1;
		//mPosition.mX -= 5;
		mNowLane--;
		mSideMoveFlagL = true;
		mSideMoveSpeed = SIDEMOVESPEED;
	}
	if (mSideMoveFlagL == true){
		mSideMoveCount++;
		mPosition.mX -= mSideMoveSpeed;
		if (mSideMoveCount > SIDEMOVECOUNT){
			mSideMoveSpeed = INITIALIZE;
			mSideMoveCount = INITIALIZE;
			mSideMoveFlagL = false;
		}
	}
	//D�L�[���͂ŉE���[���ֈړ�
	if (CKey::Once('D') && mNowLane < 1 && mSideMoveFlagL == false && mSideMoveFlagR == false){
		//Y���̉�]�l�𑝉�
		//mRotation.mY -= 1;
		//mPosition.mX += 5;
		mNowLane++;
		mSideMoveFlagR = true;
		mSideMoveSpeed = SIDEMOVESPEED;
	}
	if (mSideMoveFlagR == true){
		mSideMoveCount++;
		mPosition.mX += mSideMoveSpeed;
		if (mSideMoveCount > SIDEMOVECOUNT){
			mSideMoveSpeed = INITIALIZE;
			mSideMoveCount = INITIALIZE;
			mSideMoveFlagR = false;
		}
	}
	
	//S�L�[���͂ŃX���C�f�B���O
	if (CKey::Once('S') && mSlidingFlag == false){
		//Y���̉�]�l�𑝉�
		//mRotation.mX -= 1;
		mSlidingFlag = true;
	}
	if (mSlidingFlag == true){
		mSlidingCount++;
		mScale = CVector(1.0f, 0.5f, 1.0f); //�g��k��
		if (mSlidingCount > SLIDINGCOUNT){
			mSlidingFlag = false;
			mSlidingCount = INITIALIZE;
			mScale = CVector(1.0f, 1.0f, 1.0f); //�g��k��
		}
	}

	//W�L�[���͂ŃW�����v
	if (CKey::Once('W') && mJumpFlag == false&&mBallCollision==false){
		//Y���̉�]�l�𑝉�
		//mRotation.mX += 1;
		mJumpFlag = true;
		mJumpPower = JUMPPOWER;
		mRanding = false;
	}
	mPosition.mY += mJumpPower;
	mJumpPower -= GRAVITY;

	/*if (mPosition.mY <= 0){
		mPosition.mY += mPosition.mY*-1;
		mJumpFlag = false;
		mJumpPower = INITIALIZE;
	}*/

	//�����őO���Ɉړ�
	//mPosition.mZ -= RUNSPEED;

	/*
	if (CKey::Push(VK_UP)){
		mPosition.mZ -= RUNSPEED;
	}
	if (CKey::Push(VK_DOWN)){
		mPosition.mZ += RUNSPEED;
	}
	*/


	if (mRunSpeed < 0.5&&CKey::Once('E')){
		mRunSpeed += 0.025;
	}

	//����
	if (mRunSpeed < MAXRUNSPEED&& mGoal == false){
		mRunSpeed += ACCELERATION;
	}
	if (mRunSpeed >= MAXRUNSPEED){
		mRunSpeed = MAXRUNSPEED;
	}

	//�ׂ�Ă���Ƃ�
	if (mCrushFlag == true){
		mScale = CVector(1.0f, 0.25f, 1.0f);

		mRunSpeed = 0.1f;

		mCrushTime--;
		if (mCrushTime <= 0){
			mCrushFlag = false;
			mScale = CVector(1.0f, 1.0f, 1.0f);
		}
	}

	//�S�[�������猸��
	if (mGoal == true){
		if (mRunSpeed > 0){
			mRunSpeed -= 0.006f;
		}
		if (mRunSpeed <= 0){
			mRunSpeed = 0;
		}
	}

	//�O���ֈړ�
	mPosition.mZ -= mRunSpeed;
	
	//����L�[���͂őO�i
	/*if (CKey::Push(VK_UP)){
		//Z��������1�i�񂾒l����]�ړ�������
		mPosition = CVector(0.0f, 0.0f, 1.0f)*mMatrix;
	}*/
	//�X�y�[�X�L�[���͂Œe����
	if (CKey::Push(VK_SPACE)){
		/*CBullet*bullet = new CBullet();
		bullet->Set(0.1f, 1.5f);
		bullet->mPosition = CVector(0.0f, 0.0f, 10.0f)*mMatrix;
		bullet->mRotation = mRotation;
		bullet->Update();*/
		//TaskManager.Add(bullet);
	}

	if (mNowLane == -1 && mSideMoveFlagL == false && mSideMoveFlagR == false){
		mPosition.mX = 0;
	}
	else if (mNowLane == 0 && mSideMoveFlagL == false && mSideMoveFlagR == false){
		mPosition.mX = 4;
	}
	else if (mNowLane == 1 && mSideMoveFlagL == false && mSideMoveFlagR == false){
		mPosition.mX = 8;
	}

	//�ʒu���Z�b�g�p
	if (CKey::Once('R')){
		mPosition = CVector(4.0f, 0.0f, -310.0f);
		mNowLane = 0;
		mJumpPower = INITIALIZE;
	}

	mBlockUpCollision = false;
	mBallCollision = false;
	
	//CTransform�̍X�V
	CTransform::Update();
}

void CPlayer::Collision(CCollider *m, CCollider *o){
	//���g�̃R���C�_�^�C�v�̔���
	switch (m->mType){
	case CCollider::ESPHERE: //���R���C�_
		//����̃R���C�_���O�p�R���C�_�̎�
		if (o->mType == CCollider::ETRIANGLE){
			if (o->mpParent == NULL){
				return;
			}
			CVector adjust; //�����p�x�N�g��
			//�O�p�`�Ɛ����̏Փ˔���
			if (CCollider::CollisionTriangleSphere(o, m, &adjust)){
				if (o->mpParent->mTag != ENEEDLE){
					//�ʒu�̍X�V(mPosition+adjust)
					mPosition = mPosition + adjust;
					//�s��̍X�V
					CTransform::Update();
				}

				//�ڒn�����Ƃ�
				if (o->mpParent->mTag == EROAD){
					mJumpFlag = false;
					mJumpPower = INITIALIZE;
					mRanding = true;
				}

				//�u���b�N�̏�̖ʂɐڒn�����Ƃ�
				if (o->mpParent->mTag == EBLOCKUP){
					if (mJumpPower <= 0){
						mBlockUpCollision = true;
						mJumpFlag = false;
						mJumpPower = INITIALIZE;
					}
				}

				//�u���b�N�Ɠ���������
				if (o->mpParent->mTag == EBLOCK){
					if (mRunSpeed > 0){
						//�m�b�N�o�b�N����
						mRunSpeed = INITIALIZE;
						mRunSpeed = -0.35f;
					}
					else{
						mRunSpeed = INITIALIZE;
					}

					if (mInvincibleFlag == false && mBlockUpCollision == false){
						//�_���[�W����
						mHp--;
						//���G��Ԃɓ���
						mInvincibleFlag = true;
						mInvincibleTime = INVINCIBLETIME;
					}
				}

				//�g�Q�Ɠ���������
				if (o->mpParent->mTag == ENEEDLE){
					mJumpPower = INITIALIZE;
					mJumpPower += 0.7f;
					mJumpFlag = true;
					mRunSpeed = INITIALIZE;
					mRanding = false;
					if (mInvincibleFlag == false){
						//�_���[�W����
						mHp--;
						//���G��Ԃɓ���
						mInvincibleFlag = true;
						mInvincibleTime = INVINCIBLETIME;
						
					}
				}

				//���Ɠ���������
				if (o->mpParent->mTag == ESIRCLEPILLAR){
					if (mRunSpeed > 0){
						//�m�b�N�o�b�N����
						mRunSpeed = INITIALIZE;
						mRunSpeed = -0.3f;
					}
					else{
						mRunSpeed = INITIALIZE;
					}

					if (mInvincibleFlag == false){
						//�_���[�W����
						mHp--;
						//���G��Ԃɓ���
						mInvincibleFlag = true;
						mInvincibleTime = INVINCIBLETIME;
					}
				}

				//�u���b�N(�X���C�f�B���O�Ŕ�����)
				if (o->mpParent->mTag == EBLOCK2){
					//�ʒu�̍X�V(mPosition+adjust)
					mPosition = mPosition + adjust;
					if (mPosition.mY < 0){
						mPosition.mY += -mPosition.mY;
					}
				}
			}
		}
		//����̃R���C�_�����̎�
		if (o->mType == CCollider::ESPHERE){
			if (o->mpParent == NULL){
				return;
			}
			//�A�C�e���Ɠ���������
			if (o->mpParent->mTag == EITEM){
				//�Փ˂��Ă���Ƃ�
				if (CCollider::Collision(m, o))
				{
					//�A�C�e���̏��������Z
					mItem++;
					//�Փ˂����A�C�e�����폜
					o->mpParent->mEnabled = false;
				}
			}

			//�]���鋅�Ɠ���������
			if (o->mpParent->mTag == EBALL){
				//�Փ˂��Ă���Ƃ�
				if (CCollider::Collision(m, o))
				{
					mBallCollision = true;
					mJumpPower /= 2;

					//�v���C���[���ڒn���Ă���Ƃ�
					if (mRanding == true){
						//�ׂ��
						mCrushFlag = true;
						mCrushTime = CRUSHTIME;
					}
					//�v���C���[���ڒn���Ă��Ȃ��Ƃ�
					else if (mRanding == false){
						//�m�b�N�o�b�N����
						mRunSpeed = INITIALIZE;
						mRunSpeed = -0.5f;
					}

					if (mInvincibleFlag == false){
						//�_���[�W����
						mHp--;
						//���G��Ԃɓ���
						mInvincibleFlag = true;
						mInvincibleTime = INVINCIBLETIME;
						//�X�s�[�h��0�ɂ���
						mRunSpeed = INITIALIZE;
					}
				}
			}


			//�S�[�������Ƃ�
			if (o->mpParent->mTag == EGOAL){
				//�Փ˂��Ă���Ƃ�
				if (CCollider::Collision(m, o))
				{
					mGoal = true;
				}
			}
		}
		break;
	}
	//�s��̍X�V
	CTransform::Update();
}

//�Փˏ���
void CPlayer::TaskCollision()
{
	//�R���C�_�̗D��x�ύX
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	mCollider.ChangePriority();

	//�Փˏ��������s
	CCollisionManager::Get()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine3, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}

void CPlayer::Render()
{
	//�e�̕`�揈��
	CCharacter::Render();

	//2D�̕`��J�n
	CUtil::Start2D(-400, 400, -300, 300);
	//�`��F�̐ݒ�(�ΐF�̔�����)
	glColor4f(0.0f, 1.0f, 0.0f, 0.4f);
	//������ҏW�G���A�̍쐬
	char buf[64];

	//Y���W�̕\��
	//������̐ݒ�
	sprintf(buf, "PY:%7.2f", mPosition.mY);
	//������̕`��
	mText.DrawString(buf, 100, 30, 8, 16);

	//X����]�l�̕\��
	//������̐ݒ�
	sprintf(buf, "RX:%7.2f", mRotation.mX);
	//������̕`��
	mText.DrawString(buf, 100, 0, 8, 16);

	sprintf(buf, "RY:%7.2f", mRotation.mY);
	mText.DrawString(buf, 100, -100, 8, 16);



	sprintf(buf, "RX:%7.2f", mPosition.mX);
	//������̕`��
	mText.DrawString(buf, 100, -70, 8, 16);

	//�̗͂̒l��\��
	sprintf(buf, "HP:%d", mHp);
	mText.DrawString(buf, 100, -130, 8, 16);

	//���G���Ԃ̕\��
	sprintf(buf, "INVINCIBLETIME:%d", mInvincibleTime);
	mText.DrawString(buf, 100, -160, 8, 16);

	sprintf(buf, "RX:%7.2f", mJumpPower);
	//������̕`��
	mText.DrawString(buf, 100, -190, 8, 16);

	/*if (mBlockUpCollision == true){
		sprintf(buf, "1");
		//������̕`��
		mText.DrawString(buf, 100, -210, 8, 16);
	}
	else{
		sprintf(buf, "0");
		//������̕`��
		mText.DrawString(buf, 100, -210, 8, 16);
	}*/

	sprintf(buf, "ITEM:%d", mItem);
	//������̕`��
	mText.DrawString(buf, 100, -220, 8, 16);

	sprintf(buf, "SPEED:%6.3f", mRunSpeed);
	mText.DrawString(buf, 100, -250, 8, 16);
	

	//2D�̕`��I��
	CUtil::End2D();
}