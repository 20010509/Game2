#include "CObstacle4.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CEffect.h"
#include "CBullet.h"

#define OBJ "cube.obj" //モデルのファイル
#define MTL "cube.mtl" //モデルのマテリアルファイル

CModel CObstacle4::mModel; //モデルデータの作成

//デフォルトコンストラクタ
CObstacle4::CObstacle4()
//: mColSearch(this, &mMatrix, CVector(0.0f, 0.0f, -10.0f), 30.0f)
{
	//モデルがない時は読み込む
	if (mModel.mTriangles.size() == 0)
	{
		mModel.Load(OBJ, MTL);
	}
	//モデルのポインタ設定
	mpModel = &mModel;
	//mColSearch.mTag = CCollider::ESEARCH; //タグ設定
	mTag = EBLOCK;
}

//コンストラクタ
//CEnemy(位置、回転、拡縮)
CObstacle4::CObstacle4(const CVector& position, const CVector& rotation, const CVector& scale)
:CObstacle4()
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
void CObstacle4::Update(){

	CTransform::Update();
}

//衝突処理
//Collider(コライダ1、コライダ2)
void CObstacle4::Collision(CCollider *m, CCollider *o){
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

				}
			}
		}
		return;
	}
}

void CObstacle4::TaskCollision()
{
	mColSearch.ChangePriority();
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
}