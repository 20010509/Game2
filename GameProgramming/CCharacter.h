#ifndef CCHARACTER_H
#define CCHARACTER_H
//変換行列クラスのインクルード
#include"CTransform.h"
//モデルクラスのインクルード
#include"CModel.h"
#include"CTask.h"

//コライダクラスの宣言
class CCollider;

/*
キャラクタークラス
ゲームキャラクタの基本的な機能を定義する
*/
class CCharacter :public CTransform,public CTask{
public:
	CModel*mpModel; //モデルのポインタ
	//描画処理
	void Render();
	//デストラクタ
	~CCharacter();
	//デフォルトコンストラクタ
	CCharacter();
	//衝突処理
	virtual void Collision(CCollider *m, CCollider *o){}

	enum ETag
	{
		EZERO,			//初期値
		EPLAYER,		//プレイヤー
		EENEMY,			//敵
		EBULLETPLAYER,	//プレイヤー弾
		EBULLETENEMY,	//敵弾
		EBLOCK,			//ブロック
		EBLOCKUP,		//ブロックの上の面
		ENEEDLE,		//トゲ
		ESIRCLEPILLAR,	//柱
		EBALL,			//転がる球
		EROAD,			//道
		EOBSTACLE,		//障害物
		EITEM,			//アイテム
	};
	ETag mTag;
};
#endif