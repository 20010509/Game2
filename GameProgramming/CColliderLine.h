#ifndef CCOLLIDERLINE_H
#define CCOLLIDERLINE_H
#include "CCollider.h"
/*
線分コライダの定義
*/
class CColliderLine : public CCollider
{
public:
	CColliderLine(){}
	//コンストラクタ(線分コライダ)
	//CColliderLine(親、親行列、頂点1、頂点2)
	CColliderLine(CCharacter *parent, CMatrix *matrix, const CVector &v0, const CVector &v1);
	//線分コライダの設定
	//Set(親、親行列、頂点1、頂点2)
	void Set(CCharacter *parent, CMatrix *matrix, const CVector &v0, const CVector &v1);
	//描画
	void Render();
	void ChangePriority();
};

#endif