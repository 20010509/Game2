#include "CSceneGame.h"
//OpenGL
#include"glut.h"
#include"CTriangle.h"
#include"CKey.h"
#include"CModel.h"
#include"CMatrix.h"
#include"CTransform.h"
#include"CTaskManager.h"
#include"CEnemy.h"
#include"CEnemy2.h"
#include"CCollisionManager.h"
#include"CBillBoard.h"
#include"CCamera.h"

#include"CObstacle.h"
#include"CObstacle2.h"
#include"CObstacle3.h"
#include"CObstacle4.h"

#include"COrnament.h"
#include"CRoad.h"
#include"CWall.h"

//CS���f��
CModel mModelIC5;

CModel mObstacle1;

void CSceneGame::Init() {
	mEye = CVector(1.0f, 2.0f, 3.0f);
	//���f���t�@�C���f�[�^�̓���
	mModel.Load("sphere.obj", "sphere.mtl");

	mBackGround.Load("sky.obj", "sky.mtl");

	CMatrix matrix;
	matrix.Print();

	mBackGroundMatrix.Translate(0.0f, 0.0f, -500.0f);

	mPlayer.mpModel = &mModel;
	mPlayer.mScale = CVector(1.0f, 1.0f, 1.0f); //�g��k��
	mPlayer.mPosition = CVector(4.0f, 3.0f, 190.0f)*mBackGroundMatrix; //�ʒu���W
	mPlayer.mRotation = CVector(0.0f, 180.0f, 0.0f); //��]

	//CS���f���̓ǂݍ���
	mModelIC5.Load("c5.obj", "c5.mtl");

	//�G�@�̃C���X�^���X�쐬
	/*
	new CEnemy(&mModelIC5, CVector(0.0f, 10.0f, -100.0f)*mBackGroundMatrix, CVector(), CVector(0.1f, 0.1f, 0.1f));
	new CEnemy(&mModelIC5, CVector(30.0f, 10.0f, -130.0f)*mBackGroundMatrix, CVector(), CVector(0.1f, 0.1f, 0.1f));

	new CEnemy2(CVector(-5.0f, 1.0f, -10.0f)*mBackGroundMatrix, CVector(), CVector(0.1f, 0.1f, 0.1f));
	new CEnemy2(CVector(5.0f, 1.0f, -10.0f)*mBackGroundMatrix, CVector(), CVector(0.1f, 0.1f, 0.1f));
	*/

	//��Q��
	//������o��g�Q
	new CObstacle(CVector(0.0f, -3.75f, 85.0f)*mBackGroundMatrix, CVector(), CVector(1.5f, 1.5f, 1.5f));
	new CObstacle(CVector(4.0f, -3.75f, 85.0f)*mBackGroundMatrix, CVector(), CVector(1.5f, 1.5f, 1.5f));
	new CObstacle(CVector(8.0f, -3.75f, 85.0f)*mBackGroundMatrix, CVector(), CVector(1.5f, 1.5f, 1.5f));

	//��
	//0=�|��Ȃ����@1=�|��钌
	int PillarR[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	for (int i = 0; i < 13; i++){
		if (PillarR[i] == 0){
			new COrnament(CVector(10.0f, -1.0f, i*-30.0f + 180.0f)*mBackGroundMatrix, CVector(), CVector(1.5f, 2.0f, 1.5f));
		}
		if (PillarR[i] == 1){
			new CObstacle2(CVector(10.0f, -1.0f, i*-30.0f + 180.0f)*mBackGroundMatrix, CVector(), CVector(1.5f, 2.0f, 1.5f));
		}
	}

	int PillarL[13] = { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 };

	for (int i = 0; i < 13; i++){
		if (PillarL[i] == 0){
			new COrnament(CVector(-2.0f, -1.0f, i*-30.0f + 180.0f)*mBackGroundMatrix, CVector(0.0f,180.0f,0.0f), CVector(1.5f, 2.0f, 1.5f));
		}
		if (PillarL[i] == 1){
			new CObstacle2(CVector(-2.0f, -1.0f, i*-30.0f + 180.0f)*mBackGroundMatrix, CVector(0.0f,180.0f,0.0f), CVector(1.5f, 2.0f, 1.5f));
		}
	}

	//�]�����Ă��鋅
	new CObstacle3(CVector(6.0f, 3.0f, -180.0f)*mBackGroundMatrix, CVector(), CVector(4.0f, 4.0f, 4.0f));

	//�u���b�N
	new CObstacle4(CVector(0.0f, -1.0f, 160.0f)*mBackGroundMatrix, CVector(), CVector(1.75f, 1.75f, 1.75f));
	new CObstacle4(CVector(4.0f, -1.0f, 160.0f)*mBackGroundMatrix, CVector(), CVector(1.75f, 1.75f, 1.75f));
	new CObstacle4(CVector(8.0f, -1.0f, 160.0f)*mBackGroundMatrix, CVector(), CVector(1.75f, 1.75f, 1.75f));

	new CObstacle4(CVector(4.0f, -1.0f, 120.0f)*mBackGroundMatrix, CVector(), CVector(1.75f, 4.5f, 1.75f));
	new CObstacle4(CVector(8.0f, -1.0f, 120.0f)*mBackGroundMatrix, CVector(), CVector(1.75f, 4.5f, 1.75f));

	new CObstacle4(CVector(0.0f, -1.0f, 50.0f)*mBackGroundMatrix, CVector(), CVector(1.75f, 4.5f, 1.75f));
	new CObstacle4(CVector(4.0f, -1.0f, 50.0f)*mBackGroundMatrix, CVector(), CVector(1.75f, 4.5f, 1.75f));
	new CObstacle4(CVector(8.0f, -1.0f, 50.0f)*mBackGroundMatrix, CVector(), CVector(1.75f, 1.75f, 1.75f));

	new CObstacle4(CVector(0.0f, -1.0f, -40.0f)*mBackGroundMatrix, CVector(), CVector(1.75f, 4.5f, 1.75f));
	new CObstacle4(CVector(4.0f, -1.0f, -40.0f)*mBackGroundMatrix, CVector(), CVector(1.75f, 1.75f, 1.75f));
	new CObstacle4(CVector(8.0f, -1.0f, -40.0f)*mBackGroundMatrix, CVector(), CVector(1.75f, 4.5f, 1.75f));
	
	//new CObstacle4(CVector(4.0f, 1.0f, -30.0f)*mBackGroundMatrix, CVector(), CVector(7.0f, 3.5f, 1.0f));

	//�ǁA�V��
	new CWall(CVector(-4.0f, -1.0f, 100.0f)*mBackGroundMatrix, CVector(), CVector(1.0f, 6.0f, 300.0f));
	new CWall(CVector(12.0f, -1.0f, 100.0f)*mBackGroundMatrix, CVector(), CVector(1.0f, 6.0f, 300.0f));
	new CWall(CVector(4.0f, 11.0f, 100.0f)*mBackGroundMatrix, CVector(), CVector(9.0f, 1.0f, 300.0f));

	//��
	//new CRoad(CVector(4.0f, 0.0f, 0.0f)*mBackGroundMatrix, CVector(), CVector(1.0f, 1.0f, 50.0f));

	//�r���{�[�h�̍쐬
	new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);

	//�O�p�R���C�_�̊m�F
	mColliderTriangle.Set(NULL, NULL, CVector(-50.0f, 0.0f, -50.0f), CVector(-50.0f, 0.0f, 50.0f), CVector(50.0f, 0.0f, -50.0f));
	mColliderTriangle2.Set(NULL, NULL, CVector(50.0f, 0.0f, -50.0f), CVector(-50.0f, 0.0f, 50.0f), CVector(50.0f, 0.0f, 50.0f));

	//�w�i���f������O�p�R���C�_�𐶐�
	//�e�C���X�^���X�Ɛe�s��͂Ȃ�
	mColliderMesh.Set(NULL, &mBackGroundMatrix, &mBackGround);
}

void CSceneGame::Update() {
	//�^�X�N�}�l�[�W���̍X�V
	CTaskManager::Get()->Update();
	//�R���W�����}�l�[�W���̏Փˏ���
	//CCollisionManager::Get()->Collision();
	CTaskManager::Get()->TaskCollision();

	//���_1,���_2,���_3,�@���f�[�^�̍쐬
	CVector v0, v1, v2, n;

	//�@����������Őݒ肷��
	n.mX = 0.0f; n.mY = 1.0f; n.mZ = 0.0f;
	//���_1�̍��W��ݒ肷��
	v0.mX = 0.0f; v0.mY = 0.0f; v0.mZ = 0.5f;
	//���_2�̍��W��ݒ肷��
	v1.mX = 1.0f; v1.mY = 0.0f; v1.mZ = 0.0f;
	//���_3�̍��W��ݒ肷��
	v2.mX = 0.0f; v2.mY = 0.0f; v2.mZ = -0.5f;

	//���_�̈ړ�
	if (CKey::Push('J'))
	{
		mEye.mX -= 0.1f;
	}
	if (CKey::Push('L'))
	{
		mEye.mX += 0.1;
	}
	if (CKey::Push('I'))
	{
		mEye.mZ -= 0.1f;
	}
	if (CKey::Push('K'))
	{
		mEye.mZ += 0.1;
	}
	if (CKey::Push('O'))
	{
		mEye.mY += 0.1;
	}
	if (CKey::Push('M'))
	{
		mEye.mY -= 0.1;
	}
	
	//�J�����̃p�����[�^���쐬����
	CVector e, c, u; //���_�A�����_�A�����
	//���_�����߂�
	e = CVector(0.0f, 2.5f, -5.0f)*mPlayer.mMatrix;
	//�����_�����߂�
	c = mPlayer.mPosition;
	//����������߂�
	u = CVector(0, 1, 0)*mPlayer.mMatrixRotate;
	//�J�����̐ݒ�
	//glLookAt(e.mX,e.mY,e.mZ,c.mX,c.mY,c.mZ,u.mX,u.mY,u.mZ)
	//�J�����N���X�̐ݒ�
	Camera.Set(e, c, u);
	Camera.Render();
	
	mBackGround.Render(mBackGroundMatrix);
	
	//�^�X�N���X�g�̍폜
	CTaskManager::Get()->Delete();
	//�^�X�N�}�l�[�W���̕`��
	CTaskManager::Get()->Render();

	CCollisionManager::Get()->Render();
}
