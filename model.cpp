//=====================================================================
//
// Model [Model.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Model.h"
#include "baseObject.h"
#include "input.h"
#include "util.h"
#include "camera.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_TEXTURE		(8)

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
LPD3DXMESH g_pMeshModel = NULL;			// メッシュ（頂点情報）へのポインタ
LPDIRECT3DTEXTURE9 g_apTextureModel[MAX_TEXTURE] = {};
LPD3DXBUFFER g_pBuffMatModel = NULL;	// マテリアルへのポインタ
DWORD g_dwNumMatModel = 0;				// マテリアル数
BASEOBJECT g_objModel;
D3DXMATRIX g_mtxWorldModel;	// ワールドマトリックス

//=====================================================================
// 初期化処理
//=====================================================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	memset(&g_objModel, 0, sizeof(BASEOBJECT));
	g_objModel.bVisible = true;

	// Xファイルの読み込み
	D3DXLoadMeshFromX(
		"data\\MODEL\\SmileMan.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel,
		NULL,
		&g_dwNumMatModel,
		&g_pMeshModel
	);

	D3DXMATERIAL* pMat;

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	// テクスチャの読み込み
	for (int i = 0; i < (int)g_dwNumMatModel; i++)
	{
		if (pMat[i].pTextureFilename != NULL)
		{// テクスチャファイルが存在する
			D3DXCreateTextureFromFile(
				pDevice,
				pMat[i].pTextureFilename,
				&g_apTextureModel[i]
			);
		}
	}
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitModel(void)
{
	for (int i = 0; i < (int)g_dwNumMatModel; i++)
	{
		if (g_apTextureModel[i] != NULL)
		{
			g_apTextureModel[i]->Release();
			g_apTextureModel[i] = NULL;
		}
	}

	if (g_pMeshModel != NULL)
	{// メッシュの破棄
		g_pMeshModel->Release();
		g_pMeshModel = NULL;
	}

	if (g_pBuffMatModel != NULL)
	{// マテリアルの破棄
		g_pBuffMatModel->Release();
		g_pBuffMatModel = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateModel(void)
{
	CAMERA* pCamera = GetCamera();

	static D3DXVECTOR3 move = D3DXVECTOR3_ZERO;
	D3DXVECTOR3 dir = D3DXVECTOR3_ZERO;

	if (GetKeyboardPress(DIK_LEFT))
	{
		dir -= D3DXVECTOR3(cosf(pCamera->rot.y), 0.0f, -sinf(pCamera->rot.y)) * 10.0f;
		g_objModel.rot.y = D3DX_PI * 0.5f + pCamera->rot.y;
	}
	if (GetKeyboardPress(DIK_RIGHT))
	{
		dir += D3DXVECTOR3(cosf(pCamera->rot.y), 0.0f, -sinf(pCamera->rot.y)) * 10.0f;
		g_objModel.rot.y = -D3DX_PI * 0.5f + pCamera->rot.y;
	}
	if (GetKeyboardPress(DIK_UP))
	{
		dir += D3DXVECTOR3(sinf(pCamera->rot.y), 0.0f, cosf(pCamera->rot.y)) * 10.0f;
		g_objModel.rot.y = D3DX_PI + pCamera->rot.y;
	}
	if (GetKeyboardPress(DIK_DOWN))
	{
		dir -= D3DXVECTOR3(sinf(pCamera->rot.y), 0.0f, cosf(pCamera->rot.y)) * 10.0f;
		g_objModel.rot.y = 0 + pCamera->rot.y;
	}

	if (GetKeyboardTrigger(DIK_SPACE))
	{
		move.y = 10.0f;
	}

	g_objModel.pos += dir + move;
	move.y -= 0.6f;
	Clampf(&g_objModel.pos.y, 0.0f, g_objModel.pos.y);
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;				// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldModel);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(
		&mtxRot,
		g_objModel.rot.y, g_objModel.rot.x, g_objModel.rot.z
	);
	D3DXMatrixMultiply(
		&g_mtxWorldModel,
		&g_mtxWorldModel,
		&mtxRot
	);

	// 位置を反映
	D3DXMatrixTranslation(
		&mtxTrans,
		g_objModel.pos.x, g_objModel.pos.y, g_objModel.pos.z
	);
	D3DXMatrixMultiply(
		&g_mtxWorldModel,
		&g_mtxWorldModel,
		&mtxTrans
	);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModel);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatModel->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatModel; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTextureModel[nCntMat]);

		// モデル（パーツ）の描画
		g_pMeshModel->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}