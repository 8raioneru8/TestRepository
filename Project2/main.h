#pragma once

#define SCREEN_SIZE_X	(800)		//画面の横サイズ
#define SCREEN_SIZE_Y	(600)		//画面の縦サイズ

#define GAME_SCREEN_X		40		//ゲーム画面の描画オフセット
#define GAME_SCREEN_Y		60		//ゲーム画面のオフセット
#define GAME_SCREEN_SIZE_X	640		//ゲーム画面の横サイズ
#define GAME_SCREEN_SIZE_Y	480		//ゲーム画面の縦サイズ

#define GAMEOVER_SIZE_X		452		//ゲームオーバー画面の横サイズ
#define GAMEOVER_SIZE_Y		123		//ゲームオーバー画面の縦サイズ

#define TITLE_SIZE_X		423		//ゲームタイトル画面の横サイズ
#define TITLE_SIZE_Y		198		//ゲームタイトル画面の縦サイズ

#define PLAYER_SIZE_X		32		//自機の横サイズ
#define PLAYER_SIZE_Y		32		//自機の縦サイズ

#define ENEMY_SIZE_X		32		//敵の縦サイズ
#define ENEMY_SIZE_Y		32		//敵に横サイズ

#define ANIM_MAX			3		//アニメーションのコマ数
	
//#define ENEMY_COUNT_X		8		//敵の横並びの数
//#define ENEMY_COUNT_Y		7		//敵の縦並びの数

#define ENEMY_COUNT_X		8		//敵の横並びの数
#define ENEMY_COUNT_Y		2		//敵の縦並びの数

#define ENEMY_POINT			300		//敵を倒した時のポイント
#define HISCORE_DEFALUT		10000	//最初のハイスコア
#define SCORE_MAX			99999	//最大スコア

#define SHOT_SIZE_X			6		//自機の弾の横サイズ
#define SHOT_SIZE_Y			30		//自機の弾の縦サイズ

#define ENEMY_SHOT_SIZE_X		16	//敵の弾の横サイズ
#define ENEMY_SHOT_SIZE_Y		25	//敵の弾の縦サイズ

#define GET_ENEMY_POS_Y(Y) (Y* ENEMY_SIZE_Y *9 / 7)

enum SCENE_ID
{
	SCENE_ID_INIT,
	SCENE_ID_GAME,		//0
	SCENE_ID_GAMEOVER,	//1
	SCENE_ID_TITLE,		//2
	SCENE_ID_MAX,		//3
};

enum DIR_ID
{
	DIR_DOWN,		//下方向1
	DIR_LEFT,		//左方向2
	DIR_RIGHT,		//右方向3
	DIR_UP,			//上方向4
	DIR_MAX,		//5
};

enum ENEMY_TYPE
{
	ENEMY_TYPE_1,			//1
	ENEMY_TYPE_2,			//2
	ENEMY_TYPE_3,			//3
	ENEMY_TYPE_4,			//4
	ENEMY_TYPE_5,			//5
	ENEMY_TYPE_6,			//6
	ENEMY_TYPE_UFO,			//7
	ENEMY_TYPE_MAX,			//8
};

//敵の左右の移動方向
enum MOVE_LR
{
	MOVE_LEFT,		//左方向
	MOVE_RIGHT,		//右方向
	MOVE_MAX,
};

enum MOVE_MODE
{
	MOVE_MODE_LR,
	MOVE_MODE_DOWN,
	MOVE_MODE_MAX,
};

//-----------プロトタイプ宣言----------
void KeyCheck(void);		//キーチェック
void InitScene(void);		//初期化シーン
void TitleScene(void);		//タイトルシーンの処理
void GameScene(void);		//ゲームシーンの処理
void GameOverScene(void);	//ゲームオーバーシーンの処理

void PlayerCtl(void);		//自機の処理
void PShotControl(void);	//自機弾の処理	
void EnemyCtl(void);		//敵の処理
void EShotControl(void);	//敵弾の処理	
void HitCheck(void);		//当たり判定
void Draw(void);			//描画処理

void CheckEnemyMoveLR(void);//敵の方向転換チェック
bool SysInit(void);			//システム系初期化
