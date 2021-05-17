//プレイヤークラスのインクルード
#include"CPlayer.h"
//キー入力クラスのインクルード
#include"CKey.h"
#include"CTaskManager.h"
#include"CCollisionManager.h"
#include "CUtil.h"

#define GRAVITY 0.03f			//重力
#define JUMPPOWER 0.6f			//ジャンプ力
#define RUNSPEED 0.8f			//前方へ移動するスピード
#define SIDEMOVESPEED 0.375f	//横レーンへ移動するスピード
#define INITIALIZE 0			//値を初期化
#define SIDEMOVECOUNT 12		//隣のレーンへ移動する時間(フレーム)
#define SLIDINGCOUNT 30			//スライディングの持続時間

CPlayer::CPlayer()
:mLine(this, &mMatrix, CVector(0.0f, 0.0f, -0.75f), CVector(0.0f, 0.0f, 0.75f))
,mLine2(this, &mMatrix, CVector(0.0f, 0.75f, 0.0f), CVector(0.0f, -0.75f, 0.0f))
,mLine3(this, &mMatrix, CVector(0.75f, 0.0f, 0.0f), CVector(-0.75f, 0.0f, 0.0f))
,mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f),1.5f)
, mJumpPower(INITIALIZE)
, mJumpFlag(false)
, mSlidingFlag(false)
, mSlidingCount(INITIALIZE)
, mSideMoveSpeed(INITIALIZE)
, mSideMoveFlagL(false)
, mSideMoveFlagR(false)
, mSideMoveCount(INITIALIZE)
, mNowLane(INITIALIZE)
, mSurface(INITIALIZE)
{
	//テクスチャファイルの読み込み(1行64列)
	mText.LoadTexture("FontWhite.tga", 1, 64);
	mTag = EPLAYER; //タグ設定
}

//更新処理
void CPlayer::Update(){
	//Aキー入力で左レーンへ移動
	if (CKey::Once('A') && mNowLane > -1&&mSideMoveFlagR==false){
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
	if (CKey::Once('D') && mNowLane < 1&&mSideMoveFlagL==false){
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
		mScale = CVector(1.5f, 0.75f, 1.5f); //拡大縮小
		if (mSlidingCount > SLIDINGCOUNT){
			mSlidingFlag = false;
			mSlidingCount = INITIALIZE;
			mScale = CVector(1.5f, 1.5f, 1.5f); //拡大縮小
		}
	}

	//Wキー入力でジャンプ
	if (CKey::Once('W') && mJumpFlag == false){
		//Y軸の回転値を増加
		//mRotation.mX += 1;
		mJumpFlag = true;
		mJumpPower = JUMPPOWER;
	}
	mPosition.mY += mJumpPower;
	mJumpPower -= GRAVITY;

	if (mPosition.mY <= 0){
		mSurface = mPosition.mY*-1;
		mPosition.mY += mSurface;
		mJumpFlag = false;
		mJumpPower = INITIALIZE;
	}

	//自動で前方に移動
	mPosition.mZ -= RUNSPEED;

	/*if (CKey::Push(VK_UP)){
		mPosition.mZ -= 0.5;
	}
	if (CKey::Push(VK_DOWN)){
		mPosition.mZ += 0.5;
	}*/

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
	
	//CTransformの更新
	CTransform::Update();
}

void CPlayer::Collision(CCollider *m, CCollider *o){
	//自身のコライダタイプの判定
	switch (m->mType){
	case CCollider::ELINE: //線分コライダ
		//相手のコライダが三角コライダの時
		if (o->mType == CCollider::ETRIANGLE){
			CVector adjust; //調整用ベクトル
			//三角形と線分の衝突判定
			CCollider::CollisionTriangleLine(o, m, &adjust);
			//位置の更新(mPosition+adjust)
			mPosition = mPosition - adjust*-1;
			//行列の更新
			CTransform::Update();
		}
		break;
	}
}

//衝突処理
void CPlayer::TaskCollision()
{
	//コライダの優先度変更
	mLine.ChangePriority();
	mLine2.ChangePriority();
	mLine3.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Get()->Collision(&mLine, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine2, COLLISIONRANGE);
	CCollisionManager::Get()->Collision(&mLine3, COLLISIONRANGE);

	mCollider.ChangePriority();
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

	//2Dの描画終了
	CUtil::End2D();
}