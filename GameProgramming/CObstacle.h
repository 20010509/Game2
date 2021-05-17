#ifndef COBSTACLE_H
#define COBSTACLE_H
//キャラクタクラスのインクルード
#include "CCharacter.h"
//コライダクラスのインクルード
#include "CCollider.h"

#include "CColliderMesh.h"
/*
エネミークラス
キャラクタクラスを継承
*/
class CObstacle : public CCharacter{
private:
	bool mRiseFlag;
	bool mStopFlag;
	float mRiseSpeed;

public:
	//モデルデータ
	static CModel mModel;
	//コライダ
	CCollider mCollider;
	//コンストラクタ
	CObstacle();
	//CEnemy(位置、回転、拡張)
	CObstacle(const CVector& position, const CVector& rotation, const CVector& scale);
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