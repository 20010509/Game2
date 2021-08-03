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
	bool mSideMoveFlagL;		//プレイヤーがレーンを移動するときにtrueを返す(左方向)
	bool mSideMoveFlagR;		//プレイヤーがレーンを移動するときにtrueを返す(右方向)
	int mSideMoveCount;			//隣のレーンへ移動する時間
	int mNowLane;				//プレイヤーが今走っているレーン

	int mInvincibleTime;		//無敵時間
	bool mInvincibleFlag;		//無敵状態かどうかを判断

	int mHp;					//体力
	float mRunSpeed;			//スピード
	int mItem;					//アイテムの所持数

	bool mGoal;					//ゴールしているとtrueを返す
	bool mGameOver;				//ゲームオーバーになるとtrueを返す

	bool mCrushFlag;			//潰れているか判断
	int mCrushTime;				//潰れている時間

	bool mRanding;				//接地しているか判断

	bool mBallCollision;		//転がる球と当たっていたらtrueを返す

	bool mBlockUpCollision;		//ブロックの上の面に当たっているとtrueを返す
	bool mBlockSideCollision;	//ブロックの横の面に当たっているとtrueを返す

	bool mInvincibleFlagDummy;

	bool mStart;

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