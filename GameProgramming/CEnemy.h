#ifndef CENEMY_H
#define CENEMY_H
//キャラクラスのインクルード
#include "CCharacter.h"
#include "CCollider.h"

/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemy : public CCharacter{
public:
	//コンストラクタ
	//CEnemy(モデル、位置、回転、拡縮)
	CEnemy(CModel*model, CVector position, CVector rotation, CVector scale);
	//更新処理
	void Update();
	//コライダ
	CCollider mCollider1;
	CCollider mCollider2;
	CCollider mCollider3;

	//衝突処理
	//Collision(コライダ、コライダ2)
	void Collision(CCollider *m, CCollider *o);

	void TaskCollision();

};
#endif