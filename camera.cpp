//=====================================================================
//
// カメラ処理 [camera.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "camera.h"
#include "input.h"
#include "util.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define CAMERA_SPEED	(5.0f)

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
CAMERA g_camera;

//=====================================================================
// 初期化処理
//=====================================================================
void InitCamera(void)
{
	g_camera.posV = D3DXVECTOR3(0.0f, 100.0f, -200.0f);
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.fDistanceFromVToR = 200.0f;

	g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistanceFromVToR;
	g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistanceFromVToR;
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitCamera(void)
{

}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateCamera(void)
{
	// カメラ移動
	if (GetKeyboardPress(DIK_A))
	{// 左
		g_camera.posV += D3DXVECTOR3(
			-cosf(g_camera.rot.y),
			0.0f,
			sinf(g_camera.rot.y)
		) * CAMERA_SPEED;

		g_camera.posR += D3DXVECTOR3(
			-cosf(g_camera.rot.y),
			0.0f,
			sinf(g_camera.rot.y)
		) * CAMERA_SPEED;
	}
	if (GetKeyboardPress(DIK_D))
	{// 右
		g_camera.posV -= D3DXVECTOR3(
			-cosf(g_camera.rot.y),
			0.0f,
			sinf(g_camera.rot.y)
		) * CAMERA_SPEED;

		g_camera.posR -= D3DXVECTOR3(
			-cosf(g_camera.rot.y),
			0.0f,
			sinf(g_camera.rot.y)
		) * CAMERA_SPEED;
	}
	if (GetKeyboardPress(DIK_W))
	{// 前
		g_camera.posV -= D3DXVECTOR3(
			sinf(g_camera.rot.y + D3DX_PI),
			0.0f,
			cosf(g_camera.rot.y + D3DX_PI)
		) * CAMERA_SPEED;

		g_camera.posR -= D3DXVECTOR3(
			sinf(g_camera.rot.y + D3DX_PI),
			0.0f,
			cosf(g_camera.rot.y + D3DX_PI)
		) * CAMERA_SPEED;
	}
	if (GetKeyboardPress(DIK_S))
	{// 後方
		g_camera.posV -= D3DXVECTOR3(
			sinf(g_camera.rot.y),
			0.0f,
			cosf(g_camera.rot.y)
		) * CAMERA_SPEED;

		g_camera.posR -= D3DXVECTOR3(
			sinf(g_camera.rot.y),
			0.0f,
			cosf(g_camera.rot.y)
		) * CAMERA_SPEED;
	}

	// [カメラ視点]上下移動
	if (GetKeyboardPress(DIK_T))
	{
		g_camera.posV.y += 5.0f;
	}
	if (GetKeyboardPress(DIK_B))
	{
		g_camera.posV.y -= 5.0f;
	}

	// [カメラ注視点]上下移動
	if (GetKeyboardPress(DIK_Y))
	{
		g_camera.posR.y += 5.0f;
	}
	if (GetKeyboardPress(DIK_N))
	{
		g_camera.posR.y -= 5.0f;
	}

	// [カメラ注視点]左右回転
	if (GetKeyboardPress(DIK_Q))
	{
		g_camera.rot.y -= 0.05f;
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistanceFromVToR;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistanceFromVToR;
	}
	if (GetKeyboardPress(DIK_E))
	{
		g_camera.rot.y += 0.05f;
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fDistanceFromVToR;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fDistanceFromVToR;
	}

	// [カメラ視点]左右回転
	if (GetKeyboardPress(DIK_Z))
	{
		g_camera.rot.y -= 0.05f;
		g_camera.posV.x = g_camera.posR.x + sinf(g_camera.rot.y + D3DX_PI) * g_camera.fDistanceFromVToR;
		g_camera.posV.z = g_camera.posR.z + cosf(g_camera.rot.y + D3DX_PI) * g_camera.fDistanceFromVToR;
	}
	if (GetKeyboardPress(DIK_C))
	{
		g_camera.rot.y += 0.05f;
		g_camera.posV.x = g_camera.posR.x + sinf(g_camera.rot.y + D3DX_PI) * g_camera.fDistanceFromVToR;
		g_camera.posV.z = g_camera.posR.z + cosf(g_camera.rot.y + D3DX_PI) * g_camera.fDistanceFromVToR;
	}

	if (GetKeyboardPress(DIK_U))
	{
		g_camera.fDistanceFromVToR -= 2.0f;
		g_camera.posV.x = g_camera.posR.x + sinf(g_camera.rot.y + D3DX_PI) * g_camera.fDistanceFromVToR;
		g_camera.posV.z = g_camera.posR.z + cosf(g_camera.rot.y + D3DX_PI) * g_camera.fDistanceFromVToR;
	}
	if (GetKeyboardPress(DIK_J))
	{
		g_camera.fDistanceFromVToR += 2.0f;
		g_camera.posV.x = g_camera.posR.x + sinf(g_camera.rot.y + D3DX_PI) * g_camera.fDistanceFromVToR;
		g_camera.posV.z = g_camera.posR.z + cosf(g_camera.rot.y + D3DX_PI) * g_camera.fDistanceFromVToR;
	}
}

//=====================================================================
// カメラ設定処理
//=====================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(
		&g_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		10000.0f
	);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	// ビューマトリックスの設定
	D3DXMatrixLookAtLH(
		&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU
	);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//=====================================================================
// カメラ取得処理
//=====================================================================
CAMERA* GetCamera(void)
{
	return &g_camera;
}
