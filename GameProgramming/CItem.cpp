#include "CItem.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CEffect.h"
#include "CBullet.h"
#include "CKey.h"

#define OBJ "cube.obj" //モデルのファイル
#define MTL "cube.mtl" //モデルのマテリアルファイル

CModel CItem::mModel; //モデルデータの作成

//デフォルトコンストラクタ
CItem::CItem()
: mCollider(this, &mMatrix, CVector(0.0f, 1.0f, 0.0f), 0.5f)
{
	//モデルがない時は読み込む
	if (mModel.mTriangles.size() == 0)
	{
		mModel.Load(OBJ, MTL);
	}
	//モデルのポインタ設定
	mpModel = &mModel;
	mTag = EITEM;
}

//コンストラクタ
//CEnemy(位置、回転、拡縮)
CItem::CItem(const CVector& position, const CVector& rotation, const CVector& scale)
:CItem()
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
void CItem::Update(){

	mRotation.mY += 2.0f;

	CTransform::Update();
}

//衝突処理
//Collider(コライダ1、コライダ2)
void CItem::Collision(CCollider *m, CCollider *o){

}

void CItem::TaskCollision()
{
	
	mCollider.ChangePriority();
	CCollisionManager::Get()->Collision(&mColSearch, COLLISIONRANGE);
	
}