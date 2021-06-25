#include "CObstacle.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CEffect.h"
#include "CBullet.h"

#define OBJ "needle.obj" //モデルのファイル
#define MTL "needle.mtl" //モデルのマテリアルファイル

CModel CObstacle::mModel; //モデルデータの作成

//デフォルトコンストラクタ
CObstacle::CObstacle()
: mColSearch(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 30.0f)
, mRiseFlag(false)
, mStopFlag(false)
, mRiseSpeed(0)
{
	//モデルがない時は読み込む
	if (mModel.mTriangles.size() == 0)
	{
		mModel.Load(OBJ, MTL);
	}
	//モデルのポインタ設定
	mpModel = &mModel;
	mColSearch.mTag = CCollider::ESEARCH; //タグ設定
	mTag = ENEEDLE;
}

//コンストラクタ
//CEnemy(位置、回転、拡縮)
CObstacle::CObstacle(const CVector& position, const CVector& rotation, const CVector& scale)
:CObstacle()
{
	//位置、回転、拡縮を設定する
	mPosition = position; //位置の設定
	mRotation = rotation; //回転の設定
	mScale = scale; //拡縮の設定
	CTransform::Update(); //行列の設定
	//優先度を1に変更する
	mPriority = 1;
	CTaskManager::Get()->Remove(this); //削除して
	CTaskManager::Get()->Add(this); //追加する

	mColliderMesh.Set(this, &mMatrix, mpModel);
}

//更新処理
void CObstacle::Update(){
	if (mRiseFlag == true&&mStopFlag==false){
		mRiseSpeed += 0.2f;
		mPosition.mY += mRiseSpeed;
		if (mPosition.mY >= -1.0f){
			mStopFlag = true;
			mPosition.mY = -1.0f;
		}
	}

	CTransform::Update();
}

//衝突処理
//Collider(コライダ1、コライダ2)
void CObstacle::Collision(CCollider *m, CCollider *o){
	//相手がサーチの時は戻る
	if (o->mTag == CCollider::ESEARCH)
	{
		return;
	}
	//自分がサーチ用の時
	if (m->mTag == CCollider::ESEARCH)
	{
		//相手が球コライダの時
		if (o->mType == CCollider::ESPHERE)
		{
			//相手がプレイヤーの時
			if (o->mpParent->mTag == EPLAYER)
			{
				//衝突しているとき
				if (CCollider::Collision(m, o))
				{
					mRiseFlag = true;
				}
			}
		}
		return;
	}
}

void CObstacle::TaskCollision()
{
	mColSearch.ChangePriority();
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
}