#ifndef COBSTACLE4_H
#define COBSTACLE4_H
//キャラクタクラスのインクルード
#include "CCharacter.h"
//コライダクラスのインクルード
#include "CCollider.h"

#include "CColliderMesh.h"
/*
エネミークラス
キャラクタクラスを継承
*/
class CObstacle4 : public CCharacter{
private:

public:
	//モデルデータ
	static CModel mModel;
	//コライダ
	CCollider mCollider;
	//コンストラクタ
	CObstacle4();
	//CEnemy(位置、回転、拡張)
	CObstacle4(const CVector& position, const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
	//衝突処理
	//Collider(コライダ1,コライダ2)
	void Collision(CCollider *m, CCollider *o);
	void TaskCollision();
	CCollider mColSearch; //サーチ用コライダ
	CCharacter *mpPlayer; //プレイヤーのポインタ
	int mHp; //ヒットポイント
	CVector mPoint; //目標地点

	CColliderMesh mColliderMesh;
};
#endif