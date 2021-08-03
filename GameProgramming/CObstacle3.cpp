#include "CObstacle3.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CEffect.h"
#include "CBullet.h"

#define OBJ "sphere.obj" //モデルのファイル
#define MTL "sphere.mtl" //モデルのマテリアルファイル

CModel CObstacle3::mModel; //モデルデータの作成

//デフォルトコンストラクタ
CObstacle3::CObstacle3()
:mColSearch(this, &mMatrix, CVector(0.0f, 0.0f, 15.0f), 30.0f)
,mCollider(this, &mMatrix, CVector(0.0f, 0.0f, 0.0f), 4.0f)
, mMovingFlag(false)
{
	//モデルがない時は読み込む
	if (mModel.mTriangles.size() == 0)
	{
		mModel.Load(OBJ, MTL);
	}
	//モデルのポインタ設定
	mpModel = &mModel;
	mColSearch.mTag = CCollider::ESEARCH; //タグ設定
	mCollider.mTag = CCollider::EBODY; //タグ設定
	mTag = EBALL;
}

//コンストラクタ
//CEnemy(位置、回転、拡縮)
CObstacle3::CObstacle3(const CVector& position, const CVector& rotation, const CVector& scale)
:CObstacle3()
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

	//mColliderMesh.Set(this, &mMatrix, mpModel);
}

//更新処理
void CObstacle3::Update(){

	//動き出すフラグがtrueのとき
	if (mMovingFlag == true){
		//前進
		mPosition.mZ += 0.1f;
		//回転
		mRotation.mX += 1.5f;
	}

	CTransform::Update();
}

//衝突処理
//Collider(コライダ1、コライダ2)
void CObstacle3::Collision(CCollider *m, CCollider *o){
	//相手がサーチの時は戻る
	if (o->mpParent==NULL)
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
					mMovingFlag = true;
				}
			}
		}
		return;
	}
}

void CObstacle3::TaskCollision()
{
	mColSearch.ChangePriority();
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
	
	mCollider.ChangePriority();
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}