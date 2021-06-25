#ifndef CPLAYER_H
#define CPLAYER_H
//キャラクタークラスのインクルード
#include"CCharacter.h"
#include"CBullet.h"
#include"CColliderLine.h"
#include"CText.h"
/*
プレイヤークラス
キャラクタクラスを継承
*/
class CPlayer :public CCharacter{
private:
	float mJumpPower;			//ジャンプ力
	bool mJumpFlag;				//ジャンプしているか判断する

	int mSlidingCount;			//スライディングの持続時間
	bool mSlidingFlag;			//スライディングしているか判断する

	float mSideMoveSpeed;		//隣のレーンへ移動するときのスピード
	bool mSideMoveFlagL;		//プレイヤーがレーンを移動しているか判断する(左方向)
	bool mSideMoveFlagR;		//プレイヤーがレーンを移動しているか判断する(右方向)
	int mSideMoveCount;			//隣のレーンへ移動する時間

	int mNowLane;				//プレイヤーが今走っているレーン

	//float mSurface;			//めり込んだ分戻す用

	int mHp;

	int mInvincibleTime;		//無敵時間

	bool mInvincibleFlag;		//無敵状態かどうかを判断

	bool mBlockUpCollision;		//ブロックの上の面に乗っているか判断

public:
	//更新処理
	void Update();

	CColliderLine mLine; //線分コライダ
	CColliderLine mLine2;
	CColliderLine mLine3;

	//デフォルトコンストラクタ
	CPlayer();

	//衝突処理
	void Collision(CCollider *m, CCollider *o);

	void TaskCollision();

	CText mText;

	void Render();

	CCollider mCollider; //本体用コライダ

};
#endif