#include "CGoal.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CEffect.h"
#include "CBullet.h"

#define OBJ "plane.obj" //モデルのファイル
#define MTL "Transparent.mtl" //モデルのマテリアルファイル

CModel CGoal::mModel; //モデルデータの作成

//デフォルトコンストラクタ
CGoal::CGoal()
//: mColSearch(this, &mMatrix, CVector(0.0f, 0.0f, -10.0f), 30.0f)
//:mCollider(this, &mMatrix, CVector(0.0f, 1.0f, 0.0f), 1.0f)
{
	//モデルがない時は読み込む
	if (mModel.mTriangles.size() == 0)
	{
		mModel.Load(OBJ, MTL);
	}
	//モデルのポインタ設定
	mpModel = &mModel;
	//mColSearch.mTag = CCollider::ESEARCH; //タグ設定
	mTag = EGOAL;
}

//コンストラクタ
//CEnemy(位置、回転、拡縮)
CGoal::CGoal(const CVector& position, const CVector& rotation, const CVector& scale)
:CGoal()
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
void CGoal::Update(){

	CTransform::Update();
}

//衝突処理
//Collider(コライダ1、コライダ2)
void CGoal::Collision(CCollider *m, CCollider *o){
	//相手がプレイヤー以外の時は戻る
	if (o->mpParent->mTag != EPLAYER)
	{
		return;
	}
	//自分が球コライダの時
	if (m->mType == CCollider::ESPHERE)
	{
		//相手が球コライダの時
		if (o->mType == CCollider::ESPHERE)
		{
			//衝突しているとき
			if (CCollider::Collision(m, o))
			{
				mEnabled = false;
			}
		}
		return;
	}
}

void CGoal::TaskCollision()
{
	mCollider.ChangePriority();
	CCollisionManager::Get()->Collision(&mCollider, COLLISIONRANGE);
}