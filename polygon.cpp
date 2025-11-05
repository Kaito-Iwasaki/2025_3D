//=====================================================================
//
// Polygon [polygon.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "polygon.h"
#include "input.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	"data\\TEXTURE\\sakai000.png"
#define INIT_POS			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define INIT_SIZE			D3DXVECTOR3(1000.0f, 0.0f, 1000.0f)
#define INIT_COLOR			D3DXCOLOR_WHITE

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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffPolygon = NULL;
POLYGON g_Polygon;
D3DXMATRIX g_mtxWorldPolygon;	// ワールドマトリックス

//=====================================================================
// 初期化処理
//=====================================================================
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 構造体の初期化
	memset(&g_Polygon, 0, sizeof(POLYGON));
	g_Polygon.obj.pos = INIT_POS;
	g_Polygon.obj.size = INIT_SIZE;
	g_Polygon.obj.color = INIT_COLOR;
	g_Polygon.obj.bVisible = true;

	if (TEXTURE_FILENAME)
	{// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffPolygon
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL
	);

	VERTEX_3D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(g_Polygon.obj.pos.x - g_Polygon.obj.size.x, 0.0f, g_Polygon.obj.pos.z + g_Polygon.obj.size.z);
	pVtx[1].pos = D3DXVECTOR3(g_Polygon.obj.pos.x + g_Polygon.obj.size.x, 0.0f, g_Polygon.obj.pos.z + g_Polygon.obj.size.z);
	pVtx[2].pos = D3DXVECTOR3(g_Polygon.obj.pos.x - g_Polygon.obj.size.x, 0.0f, g_Polygon.obj.pos.z - g_Polygon.obj.size.z);
	pVtx[3].pos = D3DXVECTOR3(g_Polygon.obj.pos.x + g_Polygon.obj.size.x, 0.0f, g_Polygon.obj.pos.z - g_Polygon.obj.size.z);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = D3DXCOLOR_WHITE;
	pVtx[1].col = D3DXCOLOR_WHITE;
	pVtx[2].col = D3DXCOLOR_WHITE;
	pVtx[3].col = D3DXCOLOR_WHITE;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((g_Polygon.obj.size.x / 100.0f), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, (g_Polygon.obj.size.z / 100.0f));
	pVtx[3].tex = D3DXVECTOR2((g_Polygon.obj.size.x / 100.0f), (g_Polygon.obj.size.z / 100.0f));

	// 頂点バッファをアンロック
	g_pVtxBuffPolygon->Unlock();
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitPolygon(void)
{
	if (g_pTexBuffPolygon != NULL)
	{// テクスチャの破棄
		g_pTexBuffPolygon->Release();
		g_pTexBuffPolygon = NULL;
	}

	if (g_pVtxBuffPolygon != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdatePolygon(void)
{

}

//=====================================================================
// 描画処理
//=====================================================================
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldPolygon);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(
		&mtxRot,
		g_Polygon.obj.rot.y, g_Polygon.obj.rot.x, g_Polygon.obj.rot.z
	);
	D3DXMatrixMultiply(
		&g_mtxWorldPolygon,
		&g_mtxWorldPolygon,
		&mtxRot
	);

	// 位置を反映
	D3DXMatrixTranslation(
		&mtxTrans,
		g_Polygon.obj.pos.x, g_Polygon.obj.pos.y, g_Polygon.obj.pos.z
	);
	D3DXMatrixMultiply(
		&g_mtxWorldPolygon,
		&g_mtxWorldPolygon,
		&mtxTrans
	);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPolygon);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	if (g_Polygon.obj.bVisible)
	{// 表示状態
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexBuffPolygon);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}