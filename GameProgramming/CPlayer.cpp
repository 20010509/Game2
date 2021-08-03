//プレイヤークラスのインクルード
#include"CPlayer.h"
//キー入力クラスのインクルード
#include"CKey.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include "CUtil.h"

#define GRAVITY 0.035f			//重力
#define JUMPPOWER 0.5f			//ジャンプ力
#define MAXRUNSPEED 0.5f		//最高速度
#define SIDEMOVESPEED 0.4f		//横レーンへ移動するスピード
#define INITIALIZE 0			//値を初期化
#define SIDEMOVECOUNT 9			//隣のレーンへ移動する時間(フレーム数)
#define SLIDINGCOUNT 40			//スライディングの持続時間
#define INVINCIBLETIME_OBS 60	//無敵時間(障害物衝突時)
#define INVINCIBLETIME_ITEM 300	//無敵時間(アイテム使用時)
#define HP 5					//体力
#define ACCELERATION 0.007f		//加速用
#define CRUSHMAXSPEED 0.1f		//潰れているときの最高速度
#define CRUSHTIME 120			//潰れている時間

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
	//テクスチャファイルの読み込み(1行64列)
	mText.LoadTexture("FontWhite.tga", 1, 64);
	mTag = EPLAYER; //タグ設定
}

//更新処理
void CPlayer::Update(){

	if (mHp <= 0){
		mGameOver = true;
	}

	//SPACEキー入力でアイテムを使用する
	if (CKey::Once(VK_SPACE) && mItem > 0){
		//アイテムの所持数を減らす
		mItem--;
		//無敵になる
		mInvincibleFlag = true;
		mInvincibleFlagDummy = true;
		mInvincibleTime = INVINCIBLETIME_ITEM;
	}

	//無敵時間を減らす
	if (mInvincibleTime > 0){
		mInvincibleTime--;
	}
	//無敵時間が0かそれ以下になったら無敵状態を解除する
	if (mInvincibleTime <= 0){
		mInvincibleFlag = false;
		mInvincibleFlagDummy = false;
	}

	//Aキー入力で左レーンへ移動
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

	//Dキー入力で右レーンへ移動
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
	
	//Sキー入力でスライディング
	if (CKey::Once('S') && mSlidingFlag == false && mCrushFlag == false){
		//Y軸の回転値を増加
		//mRotation.mX -= 1;
		mSlidingFlag = true;
	}
	if (mSlidingFlag == true){
		mSlidingCount++;
		if (mSlidingCount <= 10){
			mScale = CVector(1.0f, 1.0f, 1.0f) - CVector(0.0f, mSlidingCount*0.05f, 0.0f); //拡大縮小
			mCollider.mRadius -= 0.05f;
		}
		else if (mSlidingCount >= 31){
			mScale = CVector(1.0f, 0.5f, 1.0f) + CVector(0.0f, mSlidingCount%30*0.05f, 0.0f); //拡大縮小
			mCollider.mRadius += 0.05f;
		}
		if (mSlidingCount >= SLIDINGCOUNT){
			mSlidingFlag = false;
			mSlidingCount = INITIALIZE;
			mScale = CVector(1.0f, 1.0f, 1.0f); //拡大縮小
			mCollider.mRadius = 1.0f;
		}
	}

	//Wキー入力でジャンプ
	if (CKey::Once('W') && mJumpFlag == false && mBallCollision == false){
		mJumpFlag = true;
		mJumpPower = JUMPPOWER;
		mRanding = false;
	}
	mPosition.mY += mJumpPower;
	mJumpPower -= GRAVITY;

	//加速
	if (mRunSpeed < MAXRUNSPEED&&mGoal == false){
		mRunSpeed += ACCELERATION;
	}
	//スピードが最高速度を超えないようにする
	if (mRunSpeed >= MAXRUNSPEED){
		mRunSpeed = MAXRUNSPEED;
	}

	//潰れているとき
	if (mCrushFlag == true){
		//モデルを潰す
		mScale = CVector(1.0f, 0.3f, 1.0f);
		mCollider.mRadius = 0.3f;

		//スピードが最高速度を超えないようにする
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

	//ゴールしたとき
	if (mGoal == true){
		//スピードを0にする
		if (mRunSpeed > 0){
			mRunSpeed -= 0.007f;
		}
		if (mRunSpeed <= 0){
			mRunSpeed = 0;
		}
	}

	//ゴールしたとき
	if (mGameOver == true){
		//スピードを0にする
		if (mRunSpeed > 0){
			mRunSpeed -= 0.007f;
		}
		if (mRunSpeed <= 0){
			mRunSpeed = 0;
		}
		mHp = 0;
	}

	//前方へ移動
	mPosition.mZ -= mRunSpeed;

	//プレイヤーがレーンを移動していないとき位置を補正する
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

	//位置リセット用
	if (CKey::Once('R')){
		mPosition = CVector(4.0f, 0.0f, -310.0f);
		mNowLane = 0;
		mJumpPower = INITIALIZE;
	}

	mBallCollision = false;
	mBlockUpCollision = false;
	mBlockSideCollision = false;
	
	//CTransformの更新
	CTransform::Update();
}

void CPlayer::Collision(CCollider *m, CCollider *o){
	//自身のコライダタイプの判定
	switch (m->mType){
	case CCollider::ESPHERE: //球コライダ
		//相手の親がNULLだとreturnする
		if (o->mpParent == NULL){
			return;
		}
		//相手のコライダが三角コライダの時
		if (o->mType == CCollider::ETRIANGLE){
			CVector adjust; //調整用ベクトル
			//三角形と線分の衝突判定
			if (CCollider::CollisionTriangleSphere(o, m, &adjust)){
				if (o->mpParent->mTag != ENEEDLE&&o->mpParent->mTag != EGOAL){
					//位置の更新(mPosition+adjust)
					mPosition = mPosition + adjust;
					//行列の更新
					CTransform::Update();
				}

				//相手の親のタグの判定
				switch (o->mpParent->mTag){
				case EROAD: //道
					mJumpFlag = false;
					mJumpPower = INITIALIZE;
					mRanding = true;
					break;

				case EBLOCKUP: //ブロックの上の面
					if (mJumpPower <= 0){
						mBlockUpCollision = true;
						mJumpFlag = false;
						mJumpPower = INITIALIZE;
					}
					break;

				case EBLOCKSIDE: //ブロックの横の面
					mBlockSideCollision = true;
					//跳ね返る
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
						//ダメージ発生
						mHp--;
						//無敵状態に入る
						mInvincibleFlag = true;
						mInvincibleTime = INVINCIBLETIME_OBS;
					}
					break;

				case EBLOCK: //ブロック
					//ブロックの上の面、横の面に当たっていないとき
					if (mBlockUpCollision == false && mBlockSideCollision == false){
						if (mRunSpeed > 0){
							//ノックバックする
							mRunSpeed = INITIALIZE;
							mRunSpeed = -0.35f;
						}
						else{
							mRunSpeed = INITIALIZE;
						}

						if (mInvincibleFlag == false){
							//ダメージ発生
							mHp--;
							//無敵状態に入る
							mInvincibleFlag = true;
							mInvincibleTime = INVINCIBLETIME_OBS;
						}
					}
					break;

				case ENEEDLE: //トゲ
					mJumpPower = INITIALIZE;
					mJumpPower += 0.5f;
					mJumpFlag = true;
					mRunSpeed = INITIALIZE;
					mRanding = false;
					if (mInvincibleFlag == false){
						//ダメージ発生
						mHp--;
						//無敵状態に入る
						mInvincibleFlag = true;
						mInvincibleTime = INVINCIBLETIME_OBS;

					}
					break;

				case ESIRCLEPILLAR: //柱
					if (mRunSpeed > 0){
						//ノックバックする
						mRunSpeed = INITIALIZE;
						mRunSpeed = -0.3f;
					}
					else{
						mRunSpeed = 0.1f;
					}

					if (mInvincibleFlag == false){
						//ダメージ発生
						mHp--;
						//無敵状態に入る
						mInvincibleFlag = true;
						mInvincibleTime = INVINCIBLETIME_OBS;
					}
					break;

				case EBLOCK2: //スライディングで避けるブロック
					if (mRunSpeed > 0){
						//ノックバックする
						mRunSpeed = INITIALIZE;
						mRunSpeed = -0.35f;
					}
					else{
						mRunSpeed = INITIALIZE;
					}
					if (mInvincibleFlag == false){
						//ダメージ発生
						mHp--;
						//無敵状態に入る
						mInvincibleFlag = true;
						mInvincibleTime = INVINCIBLETIME_OBS;
					}
					break;

				case EGOAL: //ゴール
					mGoal = true;
					break;
				}
			}
		}

		//相手のコライダが球の時
		if (o->mType == CCollider::ESPHERE){
			//相手のコライダの親のタグを判定
			switch (o->mpParent->mTag){
			case EITEM: //アイテム
				//衝突しているとき
				if (CCollider::Collision(m, o))
				{
					//アイテムの所持数加算
					mItem++;
					//衝突したアイテムを削除
					o->mpParent->mEnabled = false;
				}
				break;

			case EBALL: //転がる球
				//相手のコライダのタグが本体用のとき
				if (o->mTag == CCollider::EBODY){
					//衝突しているとき
					if (CCollider::Collision(m, o))
					{
						mBallCollision = true;
						mJumpPower /= 2;

						//プレイヤーが接地しているとき
						if (mRanding == true){
							//潰れる
							mCrushFlag = true;
							mCrushTime = CRUSHTIME;
						}
						//プレイヤーが接地していないとき
						else if (mRanding == false){
							//ノックバックする
							mRunSpeed = INITIALIZE;
							mRunSpeed = -0.5f;
						}

						if (mInvincibleFlag == false){
							//ダメージ発生
							mHp--;
							//無敵状態に入る
							mInvincibleFlag = true;
							mInvincibleTime = INVINCIBLETIME_OBS;
							//スピードを0にする
							mRunSpeed = INITIALIZE;
						}
					}
				}
				break;
			}
		}
		break;
	}
	//行列の更新
	CTransform::Update();
}

//衝突処理
void CPlayer::TaskCollision()
{
	//コライダの優先度変更
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	mCollider.ChangePriority();

	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine3, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}

void CPlayer::Render()
{
	//親の描画処理
	CCharacter::Render();

	//2Dの描画開始
	CUtil::Start2D(-400, 400, -300, 300);
	//描画色の設定(緑色の半透明)
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	//文字列編集エリアの作成
	char buf[64];

	//体力の値を表示
	sprintf(buf, "HP:%d", mHp);
	mText.DrawString(buf, -350, 250, 10, 20);
	//アイテムの値を表示
	sprintf(buf, "ITEM:%d", mItem);
	mText.DrawString(buf, -230, 250, 10, 20);
	
	if (mInvincibleFlagDummy == true){
		//無敵時間の表示
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
	//Y座標の表示
	//文字列の設定
	sprintf(buf, "PY:%7.2f", mPosition.mY);
	//文字列の描画
	mText.DrawString(buf, 100, 30, 8, 16);

	//X軸回転値の表示
	//文字列の設定
	sprintf(buf, "RX:%7.2f", mRotation.mX);
	//文字列の描画
	mText.DrawString(buf, 100, 0, 8, 16);

	sprintf(buf, "RY:%7.2f", mRotation.mY);
	mText.DrawString(buf, 100, -100, 8, 16);

	sprintf(buf, "RX:%7.2f", mPosition.mX);
	//文字列の描画
	mText.DrawString(buf, 100, -70, 8, 16);

	//無敵時間の表示
	sprintf(buf, "INVINCIBLETIME:%d", mInvincibleTime);
	mText.DrawString(buf, 100, -160, 8, 16);

	sprintf(buf, "RX:%7.2f", mJumpPower);
	//文字列の描画
	mText.DrawString(buf, 100, -190, 8, 16);

	sprintf(buf, "SPEED:%6.3f", mRunSpeed);
	mText.DrawString(buf, 100, -250, 8, 16);
	*/

	//2Dの描画終了
	CUtil::End2D();
}