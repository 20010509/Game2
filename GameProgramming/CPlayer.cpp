//�v���C���[�N���X�̃C���N���[�h
#include"CPlayer.h"
//�L�[���̓N���X�̃C���N���[�h
#include"CKey.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include "CUtil.h"

#define GRAVITY 0.035f			//�d��
#define JUMPPOWER 0.5f			//�W�����v��
#define MAXRUNSPEED 0.5f		//�ō����x
#define SIDEMOVESPEED 0.4f		//�����[���ֈړ�����X�s�[�h
#define INITIALIZE 0			//�l��������
#define SIDEMOVECOUNT 9			//�ׂ̃��[���ֈړ����鎞��(�t���[����)
#define SLIDINGCOUNT 40			//�X���C�f�B���O�̎�������
#define INVINCIBLETIME_OBS 60	//���G����(��Q���Փˎ�)
#define INVINCIBLETIME_ITEM 300	//���G����(�A�C�e���g�p��)
#define HP 5					//�̗�
#define ACCELERATION 0.007f		//�����p
#define CRUSHMAXSPEED 0.1f		//�ׂ�Ă���Ƃ��̍ō����x
#define CRUSHTIME 120			//�ׂ�Ă��鎞��

CPlayer::CPlayer()
:mLine(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f))
,mLine2(this, &mMatrix, CVector(0.0f, 1.0f, 0.0f), CVector(0.0f, -1.0f, 0.0f))
,mLine3(this, &mMatrix, CVector(1.0f, 0.0f, 1.0f), CVector(-1.0f, 0.0f, 1.0f))
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
, mHp(HP)
, mInvincibleTime(INITIALIZE)
, mInvincibleFlag(false)
, mItem(INITIALIZE)
, mRunSpeed(INITIALIZE)
, mGoal(false)
, mCrushFlag(false)
, mCrushTime(INITIALIZE)
, mRanding(false)
, mBallCollision(false)
, mBlockUpCollision(false)
, mBlockSideCollision(false)
, mGameOver(false)
, mInvincibleFlagDummy(false)
{
	//�e�N�X�`���t�@�C���̓ǂݍ���(1�s64��)
	mText.LoadTexture("FontWhite.tga", 1, 64);
	mTag = EPLAYER; //�^�O�ݒ�
}

//�X�V����
void CPlayer::Update(){

	if (mHp <= 0){
		mGameOver = true;
	}

	//SPACE�L�[���͂ŃA�C�e�����g�p����
	if (CKey::Once(VK_SPACE) && mItem > 0){
		//�A�C�e���̏����������炷
		mItem--;
		//���G�ɂȂ�
		mInvincibleFlag = true;
		mInvincibleFlagDummy = true;
		mInvincibleTime = INVINCIBLETIME_ITEM;
	}

	//���G���Ԃ����炷
	if (mInvincibleTime > 0){
		mInvincibleTime--;
	}
	//���G���Ԃ�0������ȉ��ɂȂ����疳�G��Ԃ���������
	if (mInvincibleTime <= 0){
		mInvincibleFlag = false;
		mInvincibleFlagDummy = false;
	}

	//A�L�[���͂ō����[���ֈړ�
	if (CKey::Once('A') && mNowLane > -1 && mSideMoveFlagR == false && mSideMoveFlagL == false){
		mNowLane--;
		mSideMoveFlagL = true;
		mSideMoveSpeed = SIDEMOVESPEED;
	}
	if (mSideMoveFlagL == true){
		mPosition.mX -= mSideMoveSpeed;
		if (mNowLane == 0){
			if (mPosition.mX <= 4){
				mSideMoveFlagL = false;
				mSideMoveSpeed = INITIALIZE;
			}
		}
		if (mNowLane == -1){
			if (mPosition.mX <= 0){
				mSideMoveFlagL = false;
				mSideMoveSpeed = INITIALIZE;
			}
		}
	}

	//D�L�[���͂ŉE���[���ֈړ�
	if (CKey::Once('D') && mNowLane < 1 && mSideMoveFlagL == false && mSideMoveFlagR == false){
		mNowLane++;
		mSideMoveFlagR = true;
		mSideMoveSpeed = SIDEMOVESPEED;
	}
	
	if (mSideMoveFlagR == true){
		mPosition.mX += mSideMoveSpeed;
		if (mNowLane == 0){
			if (mPosition.mX >= 4){
				mSideMoveFlagR = false;
				mSideMoveSpeed = INITIALIZE;
			}
		}
		if (mNowLane == 1){
			if (mPosition.mX >= 8){
				mSideMoveFlagR = false;
				mSideMoveSpeed = INITIALIZE;
			}
		}
	}
	
	//S�L�[���͂ŃX���C�f�B���O
	if (CKey::Once('S') && mSlidingFlag == false && mCrushFlag == false){
		//Y���̉�]�l�𑝉�
		//mRotation.mX -= 1;
		mSlidingFlag = true;
	}
	if (mSlidingFlag == true){
		mSlidingCount++;
		if (mSlidingCount <= 10){
			mScale = CVector(1.0f, 1.0f, 1.0f) - CVector(0.0f, mSlidingCount*0.05f, 0.0f); //�g��k��
			mCollider.mRadius -= 0.05f;
		}
		else if (mSlidingCount >= 31){
			mScale = CVector(1.0f, 0.5f, 1.0f) + CVector(0.0f, mSlidingCount%30*0.05f, 0.0f); //�g��k��
			mCollider.mRadius += 0.05f;
		}
		if (mSlidingCount >= SLIDINGCOUNT){
			mSlidingFlag = false;
			mSlidingCount = INITIALIZE;
			mScale = CVector(1.0f, 1.0f, 1.0f); //�g��k��
			mCollider.mRadius = 1.0f;
		}
	}

	//W�L�[���͂ŃW�����v
	if (CKey::Once('W') && mJumpFlag == false && mBallCollision == false){
		mJumpFlag = true;
		mJumpPower = JUMPPOWER;
		mRanding = false;
	}
	mPosition.mY += mJumpPower;
	mJumpPower -= GRAVITY;

	//����
	if (mRunSpeed < MAXRUNSPEED&&mGoal == false){
		mRunSpeed += ACCELERATION;
	}
	//�X�s�[�h���ō����x�𒴂��Ȃ��悤�ɂ���
	if (mRunSpeed >= MAXRUNSPEED){
		mRunSpeed = MAXRUNSPEED;
	}

	//�ׂ�Ă���Ƃ�
	if (mCrushFlag == true){
		//���f����ׂ�
		mScale = CVector(1.0f, 0.3f, 1.0f);
		mCollider.mRadius = 0.3f;

		//�X�s�[�h���ō����x�𒴂��Ȃ��悤�ɂ���
		if (mRunSpeed >= CRUSHMAXSPEED){
			mRunSpeed = CRUSHMAXSPEED;
		}

		mCrushTime--;
		if (mCrushTime <= 0){
			mCrushFlag = false;
			mScale = CVector(1.0f, 1.0f, 1.0f);
			mCollider.mRadius = 1.0f;
		}
	}

	//�S�[�������Ƃ�
	if (mGoal == true){
		//�X�s�[�h��0�ɂ���
		if (mRunSpeed > 0){
			mRunSpeed -= 0.007f;
		}
		if (mRunSpeed <= 0){
			mRunSpeed = 0;
		}
	}

	//�S�[�������Ƃ�
	if (mGameOver == true){
		//�X�s�[�h��0�ɂ���
		if (mRunSpeed > 0){
			mRunSpeed -= 0.007f;
		}
		if (mRunSpeed <= 0){
			mRunSpeed = 0;
		}
		mHp = 0;
	}

	//�O���ֈړ�
	mPosition.mZ -= mRunSpeed;

	//�v���C���[�����[�����ړ����Ă��Ȃ��Ƃ��ʒu��␳����
	if (mSideMoveFlagL == false && mSideMoveFlagR == false){
		switch (mNowLane){
		case -1:
			mPosition.mX = 0;
			break;
		case 0:
			mPosition.mX = 4;
			break;
		case 1:
			mPosition.mX = 8;
			break;
		}
	}

	//�ʒu���Z�b�g�p
	if (CKey::Once('R')){
		mPosition = CVector(4.0f, 0.0f, -310.0f);
		mNowLane = 0;
		mJumpPower = INITIALIZE;
	}

	mBallCollision = false;
	mBlockUpCollision = false;
	mBlockSideCollision = false;
	
	//CTransform�̍X�V
	CTransform::Update();
}

void CPlayer::Collision(CCollider *m, CCollider *o){
	//���g�̃R���C�_�^�C�v�̔���
	switch (m->mType){
	case CCollider::ESPHERE: //���R���C�_
		//����̐e��NULL����return����
		if (o->mpParent == NULL){
			return;
		}
		//����̃R���C�_���O�p�R���C�_�̎�
		if (o->mType == CCollider::ETRIANGLE){
			CVector adjust; //�����p�x�N�g��
			//�O�p�`�Ɛ����̏Փ˔���
			if (CCollider::CollisionTriangleSphere(o, m, &adjust)){
				if (o->mpParent->mTag != ENEEDLE&&o->mpParent->mTag != EGOAL){
					//�ʒu�̍X�V(mPosition+adjust)
					mPosition = mPosition + adjust;
					//�s��̍X�V
					CTransform::Update();
				}

				//����̐e�̃^�O�̔���
				switch (o->mpParent->mTag){
				case EROAD: //��
					mJumpFlag = false;
					mJumpPower = INITIALIZE;
					mRanding = true;
					break;

				case EBLOCKUP: //�u���b�N�̏�̖�
					if (mJumpPower <= 0){
						mBlockUpCollision = true;
						mJumpFlag = false;
						mJumpPower = INITIALIZE;
					}
					break;

				case EBLOCKSIDE: //�u���b�N�̉��̖�
					mBlockSideCollision = true;
					//���˕Ԃ�
					if (mSideMoveFlagL == true){
						mSideMoveFlagL = false;
						mSideMoveFlagR = true;
						mSideMoveSpeed = 0.2f;
						mNowLane++;
						mRunSpeed /= 4;
					}
					else if (mSideMoveFlagR == true){
						mSideMoveFlagR = false;
						mSideMoveFlagL = true;
						mSideMoveSpeed = 0.2f;
						mNowLane--;
						mRunSpeed /= 4;
					}

					if (mInvincibleFlag == false){
						//�_���[�W����
						mHp--;
						//���G��Ԃɓ���
						mInvincibleFlag = true;
						mInvincibleTime = INVINCIBLETIME_OBS;
					}
					break;

				case EBLOCK: //�u���b�N
					//�u���b�N�̏�̖ʁA���̖ʂɓ������Ă��Ȃ��Ƃ�
					if (mBlockUpCollision == false && mBlockSideCollision == false){
						if (mRunSpeed > 0){
							//�m�b�N�o�b�N����
							mRunSpeed = INITIALIZE;
							mRunSpeed = -0.35f;
						}
						else{
							mRunSpeed = INITIALIZE;
						}

						if (mInvincibleFlag == false){
							//�_���[�W����
							mHp--;
							//���G��Ԃɓ���
							mInvincibleFlag = true;
							mInvincibleTime = INVINCIBLETIME_OBS;
						}
					}
					break;

				case ENEEDLE: //�g�Q
					mJumpPower = INITIALIZE;
					mJumpPower += 0.5f;
					mJumpFlag = true;
					mRunSpeed = INITIALIZE;
					mRanding = false;
					if (mInvincibleFlag == false){
						//�_���[�W����
						mHp--;
						//���G��Ԃɓ���
						mInvincibleFlag = true;
						mInvincibleTime = INVINCIBLETIME_OBS;

					}
					break;

				case ESIRCLEPILLAR: //��
					if (mRunSpeed > 0){
						//�m�b�N�o�b�N����
						mRunSpeed = INITIALIZE;
						mRunSpeed = -0.3f;
					}
					else{
						mRunSpeed = 0.1f;
					}

					if (mInvincibleFlag == false){
						//�_���[�W����
						mHp--;
						//���G��Ԃɓ���
						mInvincibleFlag = true;
						mInvincibleTime = INVINCIBLETIME_OBS;
					}
					break;

				case EBLOCK2: //�X���C�f�B���O�Ŕ�����u���b�N
					if (mRunSpeed > 0){
						//�m�b�N�o�b�N����
						mRunSpeed = INITIALIZE;
						mRunSpeed = -0.35f;
					}
					else{
						mRunSpeed = INITIALIZE;
					}
					if (mInvincibleFlag == false){
						//�_���[�W����
						mHp--;
						//���G��Ԃɓ���
						mInvincibleFlag = true;
						mInvincibleTime = INVINCIBLETIME_OBS;
					}
					break;

				case EGOAL: //�S�[��
					mGoal = true;
					break;
				}
			}
		}

		//����̃R���C�_�����̎�
		if (o->mType == CCollider::ESPHERE){
			//����̃R���C�_�̐e�̃^�O�𔻒�
			switch (o->mpParent->mTag){
			case EITEM: //�A�C�e��
				//�Փ˂��Ă���Ƃ�
				if (CCollider::Collision(m, o))
				{
					//�A�C�e���̏��������Z
					mItem++;
					//�Փ˂����A�C�e�����폜
					o->mpParent->mEnabled = false;
				}
				break;

			case EBALL: //�]���鋅
				//����̃R���C�_�̃^�O���{�̗p�̂Ƃ�
				if (o->mTag == CCollider::EBODY){
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
							mInvincibleTime = INVINCIBLETIME_OBS;
							//�X�s�[�h��0�ɂ���
							mRunSpeed = INITIALIZE;
						}
					}
				}
				break;
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
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	//������ҏW�G���A�̍쐬
	char buf[64];

	//�̗͂̒l��\��
	sprintf(buf, "HP:%d", mHp);
	mText.DrawString(buf, -350, 250, 10, 20);
	//�A�C�e���̒l��\��
	sprintf(buf, "ITEM:%d", mItem);
	mText.DrawString(buf, -230, 250, 10, 20);
	
	if (mInvincibleFlagDummy == true){
		//���G���Ԃ̕\��
		sprintf(buf, "INVINCIBLE");
		mText.DrawString(buf, -80, 250, 10, 20);
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	if (mGoal == true){
		sprintf(buf, "CLEAR");
		mText.DrawString(buf, -80, 0, 20, 30);
	}

	if (mGameOver == true){
		sprintf(buf, "GAME OVER");
		mText.DrawString(buf, -160, 0, 20, 30);
	}



	/*
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

	//���G���Ԃ̕\��
	sprintf(buf, "INVINCIBLETIME:%d", mInvincibleTime);
	mText.DrawString(buf, 100, -160, 8, 16);

	sprintf(buf, "RX:%7.2f", mJumpPower);
	//������̕`��
	mText.DrawString(buf, 100, -190, 8, 16);

	sprintf(buf, "SPEED:%6.3f", mRunSpeed);
	mText.DrawString(buf, 100, -250, 8, 16);
	*/

	//2D�̕`��I��
	CUtil::End2D();
}