#include "CGoalCheck.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CEffect.h"
#include "CBullet.h"

#define OBJ "goal.obj" //モデルのファイル
#define MTL "goal.mtl" //モデルのマテリアルファイル

CModel CGoalCheck::mModel; //モデルデータの作成

//デフォルトコンストラクタ
CGoalCheck::CGoalCheck()
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
}

//コンストラクタ
//CEnemy(位置、回転、拡縮)
CGoalCheck::CGoalCheck(const CVector& position, const CVector& rotation, const CVector& scale)
:CGoalCheck()
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
}

//更新処理
void CGoalCheck::Update(){

	CTransform::Update();
}

//衝突処理
//Collider(コライダ1、コライダ2)
void CGoalCheck::Collision(CCollider *m, CCollider *o){

}

void CGoalCheck::TaskCollision()
{

}