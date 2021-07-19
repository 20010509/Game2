//プレイヤークラスのインクルード
#include"CPlayer.h"
//キー入力クラスのインクルード
#include"CKey.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include "CUtil.h"

#define GRAVITY 0.035f			//重力
#define JUMPPOWER 0.6f			//ジャンプ力
#define MAXRUNSPEED 0.5f		//最高速度
#define SIDEMOVESPEED 0.4f		//横レーンへ移動するスピード
#define INITIALIZE 0			//値を初期化
#define SIDEMOVECOUNT 9			//隣のレーンへ移動する時間(フレーム数)
#define SLIDINGCOUNT 30			//スライディングの持続時間
#define INVINCIBLETIME 60		//無敵時間
#define INVINCIBLETIME_ITEM 300	//無敵時間(アイテム使用)
#define HP 5					//体力
#define ACCELERATION 0.007f		//加速用
#define CRUSHMAXSPEED 0.25f		//潰れているときの最高速度
#define CRUSHTIME 120			//潰れている時間

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
	//テクスチャファイルの読み込み(1行64列)
	mText.LoadTexture("FontWhite.tga", 1, 64);
	mTag = EPLAYER; //タグ設定
}

//更新処理
void CPlayer::Update(){

	//アイテム使用時
	if (CKey::Once(VK_SPACE) && mItem > 0 && mInvincibleFlag == false){
		//アイテムの所持数を減らす
		mItem--;
		//無敵になる
		mInvincibleFlag = true;
		mInvincibleTime = INVINCIBLETIME_ITEM;
	}

	//無敵時間を減らす
	if (mInvincibleTime > 0){
		mInvincibleTime--;
	}
	//無敵時間が0かそれ以下になったら無敵状態を解除する
	if (mInvincibleTime <= 0){
		mInvincibleFlag = false;
	}

	//Aキー入力で左レーンへ移動
	if (CKey::Once('A') && mNowLane > -1 && mSideMoveFlagR == false && mSideMoveFlagL == false){
		//Y軸の回転値を増加
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
	//Dキー入力で右レーンへ移動
	if (CKey::Once('D') && mNowLane < 1 && mSideMoveFlagL == false && mSideMoveFlagR == false){
		//Y軸の回転値を増加
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
	
	//Sキー入力でスライディング
	if (CKey::Once('S') && mSlidingFlag == false){
		//Y軸の回転値を増加
		//mRotation.mX -= 1;
		mSlidingFlag = true;
	}
	if (mSlidingFlag == true){
		mSlidingCount++;
		mScale = CVector(1.0f, 0.5f, 1.0f); //拡大縮小
		if (mSlidingCount > SLIDINGCOUNT){
			mSlidingFlag = false;
			mSlidingCount = INITIALIZE;
			mScale = CVector(1.0f, 1.0f, 1.0f); //拡大縮小
		}
	}

	//Wキー入力でジャンプ
	if (CKey::Once('W') && mJumpFlag == false&&mBallCollision==false){
		//Y軸の回転値を増加
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

	//自動で前方に移動
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

	//加速
	if (mRunSpeed < MAXRUNSPEED&& mGoal == false){
		mRunSpeed += ACCELERATION;
	}
	if (mRunSpeed >= MAXRUNSPEED){
		mRunSpeed = MAXRUNSPEED;
	}

	//潰れているとき
	if (mCrushFlag == true){
		mScale = CVector(1.0f, 0.25f, 1.0f);

		mRunSpeed = 0.1f;

		mCrushTime--;
		if (mCrushTime <= 0){
			mCrushFlag = false;
			mScale = CVector(1.0f, 1.0f, 1.0f);
		}
	}

	//ゴールしたら減速
	if (mGoal == true){
		if (mRunSpeed > 0){
			mRunSpeed -= 0.006f;
		}
		if (mRunSpeed <= 0){
			mRunSpeed = 0;
		}
	}

	//前方へ移動
	mPosition.mZ -= mRunSpeed;
	
	//上矢印キー入力で前進
	/*if (CKey::Push(VK_UP)){
		//Z軸方向に1進んだ値を回転移動させる
		mPosition = CVector(0.0f, 0.0f, 1.0f)*mMatrix;
	}*/
	//スペースキー入力で弾発射
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

	//位置リセット用
	if (CKey::Once('R')){
		mPosition = CVector(4.0f, 0.0f, -310.0f);
		mNowLane = 0;
		mJumpPower = INITIALIZE;
	}

	mBlockUpCollision = false;
	mBallCollision = false;
	
	//CTransformの更新
	CTransform::Update();
}

void CPlayer::Collision(CCollider *m, CCollider *o){
	//自身のコライダタイプの判定
	switch (m->mType){
	case CCollider::ESPHERE: //球コライダ
		//相手のコライダが三角コライダの時
		if (o->mType == CCollider::ETRIANGLE){
			if (o->mpParent == NULL){
				return;
			}
			CVector adjust; //調整用ベクトル
			//三角形と線分の衝突判定
			if (CCollider::CollisionTriangleSphere(o, m, &adjust)){
				if (o->mpParent->mTag != ENEEDLE){
					//位置の更新(mPosition+adjust)
					mPosition = mPosition + adjust;
					//行列の更新
					CTransform::Update();
				}

				//接地したとき
				if (o->mpParent->mTag == EROAD){
					mJumpFlag = false;
					mJumpPower = INITIALIZE;
					mRanding = true;
				}

				//ブロックの上の面に接地したとき
				if (o->mpParent->mTag == EBLOCKUP){
					if (mJumpPower <= 0){
						mBlockUpCollision = true;
						mJumpFlag = false;
						mJumpPower = INITIALIZE;
					}
				}

				//ブロックと当たった時
				if (o->mpParent->mTag == EBLOCK){
					if (mRunSpeed > 0){
						//ノックバックする
						mRunSpeed = INITIALIZE;
						mRunSpeed = -0.35f;
					}
					else{
						mRunSpeed = INITIALIZE;
					}

					if (mInvincibleFlag == false && mBlockUpCollision == false){
						//ダメージ発生
						mHp--;
						//無敵状態に入る
						mInvincibleFlag = true;
						mInvincibleTime = INVINCIBLETIME;
					}
				}

				//トゲと当たった時
				if (o->mpParent->mTag == ENEEDLE){
					mJumpPower = INITIALIZE;
					mJumpPower += 0.7f;
					mJumpFlag = true;
					mRunSpeed = INITIALIZE;
					mRanding = false;
					if (mInvincibleFlag == false){
						//ダメージ発生
						mHp--;
						//無敵状態に入る
						mInvincibleFlag = true;
						mInvincibleTime = INVINCIBLETIME;
						
					}
				}

				//柱と当たった時
				if (o->mpParent->mTag == ESIRCLEPILLAR){
					if (mRunSpeed > 0){
						//ノックバックする
						mRunSpeed = INITIALIZE;
						mRunSpeed = -0.3f;
					}
					else{
						mRunSpeed = INITIALIZE;
					}

					if (mInvincibleFlag == false){
						//ダメージ発生
						mHp--;
						//無敵状態に入る
						mInvincibleFlag = true;
						mInvincibleTime = INVINCIBLETIME;
					}
				}

				//ブロック(スライディングで避ける)
				if (o->mpParent->mTag == EBLOCK2){
					//位置の更新(mPosition+adjust)
					mPosition = mPosition + adjust;
					if (mPosition.mY < 0){
						mPosition.mY += -mPosition.mY;
					}
				}
			}
		}
		//相手のコライダが球の時
		if (o->mType == CCollider::ESPHERE){
			if (o->mpParent == NULL){
				return;
			}
			//アイテムと当たった時
			if (o->mpParent->mTag == EITEM){
				//衝突しているとき
				if (CCollider::Collision(m, o))
				{
					//アイテムの所持数加算
					mItem++;
					//衝突したアイテムを削除
					o->mpParent->mEnabled = false;
				}
			}

			//転がる球と当たった時
			if (o->mpParent->mTag == EBALL){
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
						mInvincibleTime = INVINCIBLETIME;
						//スピードを0にする
						mRunSpeed = INITIALIZE;
					}
				}
			}


			//ゴールしたとき
			if (o->mpParent->mTag == EGOAL){
				//衝突しているとき
				if (CCollider::Collision(m, o))
				{
					mGoal = true;
				}
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
	glColor4f(0.0f, 1.0f, 0.0f, 0.4f);
	//文字列編集エリアの作成
	char buf[64];

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

	//体力の値を表示
	sprintf(buf, "HP:%d", mHp);
	mText.DrawString(buf, 100, -130, 8, 16);

	//無敵時間の表示
	sprintf(buf, "INVINCIBLETIME:%d", mInvincibleTime);
	mText.DrawString(buf, 100, -160, 8, 16);

	sprintf(buf, "RX:%7.2f", mJumpPower);
	//文字列の描画
	mText.DrawString(buf, 100, -190, 8, 16);

	/*if (mBlockUpCollision == true){
		sprintf(buf, "1");
		//文字列の描画
		mText.DrawString(buf, 100, -210, 8, 16);
	}
	else{
		sprintf(buf, "0");
		//文字列の描画
		mText.DrawString(buf, 100, -210, 8, 16);
	}*/

	sprintf(buf, "ITEM:%d", mItem);
	//文字列の描画
	mText.DrawString(buf, 100, -220, 8, 16);

	sprintf(buf, "SPEED:%6.3f", mRunSpeed);
	mText.DrawString(buf, 100, -250, 8, 16);
	

	//2Dの描画終了
	CUtil::End2D();
}