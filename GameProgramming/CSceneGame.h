#ifndef CSCENEGAME_H
#define CSCENEGAME_H
#include "CScene.h"
#include"CVector.h"
#include"CModel.h"
#include"CCharacter.h"
#include"CPlayer.h"
#include"CColliderTriangle.h"
#include"CColliderMesh.h"

/*
ゲームのシーン
*/
class CSceneGame : public CScene {
public:
	//初期化処理のオーバーライド
	void Init();
	//更新処理のオーバーライド
	void Update();

	CVector mEye;

	//モデルクラスのインスタンス作成
	CModel mModel;

	CModel mBackGround; //背景モデル

	CPlayer mPlayer;

	//三角コライダの作成
	CColliderTriangle mColliderTriangle;
	CColliderTriangle mColliderTriangle2;

	//モデルからコライダ作成
	CColliderMesh mColliderMesh;

	CMatrix mBackGroundMatrix;
};

#endif
