#ifndef CBULLET_H
#define CBULLET_H
//キャラクタクラスのインクルード
#include"CCharacter.h"
//三角形クラスのインクルード
#include"CTriangle.h"
#include"CCollider.h"

/*
弾クラス
三角形を飛ばす
*/
class CBullet :public CCharacter{
public:
	//三角形
	CTriangle mT;
	//幅と奥行きの設定
	//Set(幅、奥行)
	void Set(float w, float d);
	//更新
	void Update();
	//描画
	void Render();
	//生存時間
	int mLife;
	//デフォルトコンストラクタ
	CBullet();

	CCollider mCollider;

	//衝突処理
	//Collision(コライダ、コライダ2)
	void Collision(CCollider *m, CCollider *o);

	void TaskCollision();
};
#endif