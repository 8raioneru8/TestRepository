#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"
#include <time.h>
#include "main.h"
#include "DxLib.h"

//システム
int gameCounter;
SCENE_ID sceneID;							//シーン管理用
int GameOverImage;							//ゲームオーバーのグラフィックハンドル
int TitleImage;								//ゲームタイトルのグラフィックハンドル
int spaceKeyBack;							//スぺースキーの1フレーム前の状態
int spaceKeyTriggerDown;					//スペースキーのトリガーKey Down
int spaceKeyTriggerUp;						//スペースキーのトリガーKey Up
int hiscore;								//ハイスコアの格納処理
int score;									//スコアの格納処理
int stringWidth;							//文字列のピクセル描画幅

//プレイヤー関連
int playerImage[DIR_MAX][ANIM_MAX];			//自機画像のグラフィックハンドル
int playerPosX;								//自機のX座標
int playerPosY;								//自機のY座標
int playerSpeed;							//自機の移動量
int playerLife;								//自機の残機
bool playerAlive;							//生存フラグ

//敵
int enemyImage[ENEMY_TYPE_MAX][DIR_MAX][ANIM_MAX];	//敵画像の格納領域
int enemyPosX[ENEMY_COUNT_Y][ENEMY_COUNT_X];		//敵のX座標
int enemyPosY[ENEMY_COUNT_Y][ENEMY_COUNT_X];		//敵のY座標
int enemySpeed;										//移動量
bool enemyAlive[ENEMY_COUNT_Y][ENEMY_COUNT_X];		//生存フラグ

//敵の弾
int eShotImage;										//グラフィックハンドル
int eShotPosX[ENEMY_COUNT_Y][ENEMY_COUNT_X];		//X座標
int eShotPosY[ENEMY_COUNT_Y][ENEMY_COUNT_X];		//Y座標
int eShotSpeed;										//移動量
bool eShotAlive[ENEMY_COUNT_Y][ENEMY_COUNT_X];		//状態フラグ
	
//自機の弾
int pShotImage;										//グラフィックハンドル
int pShotPosX;										//X座標
int pShotPosY;										//Y座標
int pShotSpeed;										//移動量
bool pShotAlive;									//状態フラグ

MOVE_MODE enemyMoveMode;							//敵の左右移動か下移動
MOVE_LR enemyMoveFlagX;								//敵の左右移動移管管理
DIR_ID enemyDir;									//

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//初期化処理
	if (!SysInit())
	{
		TRACE("初期化失敗");
		return -1;
	}
	//ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		gameCounter++;
		_dbgStartDraw();

		KeyCheck();				//キートリガーのチェック

		switch (sceneID)
		{
		case SCENE_ID_INIT:
			InitScene();
			break;

		case SCENE_ID_TITLE:
			TitleScene();
			break;

		case SCENE_ID_GAME:
			GameScene();
			break;

		case SCENE_ID_GAMEOVER:
			GameOverScene();
			break;

		default:
			break;
		}

		////ゲームタイトルシーン
		//if (sceneID == SCENE_ID_TITLE)
		//{
		//	TitleScene();
		//}

		////ゲームシーン
		//else if (sceneID == SCENE_ID_GAME)		
		//{
		//	GameScene();
		//}

		////ゲームオーバーシーン
		//else if (sceneID == SCENE_ID_GAMEOVER)
		//{
		//	GameOverScene();
		//}
	}
	DxLib_End();		//DXライブラリの終了処理
	return 0;
}

//キーチェック
void KeyCheck(void)
{
	spaceKeyTriggerDown = 0;
	spaceKeyTriggerUp = 0;

	//1フレーム前の状態
	if (spaceKeyBack == 1)
	{
		TRACE("前:on");
	}
	else
	{
		TRACE("前:off");
	}

	//今の状態
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		TRACE("-今:on");
	}
	else
	{
		TRACE("-今:off");
	}
	TRACE("\n");

	if ((spaceKeyBack == 0) && ((CheckHitKey(KEY_INPUT_SPACE)) == 1))
	{
		TRACE("<--Key Down");
		spaceKeyTriggerDown = 1;
	}
	if ((spaceKeyBack == 1) && ((CheckHitKey(KEY_INPUT_SPACE)) == 0))
	{
		TRACE("<--Key Up");
		spaceKeyTriggerUp = 1;
	}
	TRACE(" : %d %d", spaceKeyTriggerDown, spaceKeyTriggerUp);
	spaceKeyBack = CheckHitKey(KEY_INPUT_SPACE);
}

//初期化シーン
void InitScene(void)
{
	TRACE("INIT_SCENE\n")
	//自機の弾の初期化
	pShotAlive = false;

	//自機の初期化
	playerPosX = (GAME_SCREEN_SIZE_X - PLAYER_SIZE_X) / 2;
	playerPosY = (GAME_SCREEN_SIZE_Y - PLAYER_SIZE_Y);
	playerAlive = true;

	//自機の残機
	playerLife = 3;
	score = 0;

	//敵の初期化
	for (int y = 0; y < ENEMY_COUNT_Y; y++)
	{
		for (int x = 0; x < ENEMY_COUNT_X; x++)
		{
			enemyPosX[y][x] = x * ENEMY_SIZE_X * 3 / 2;
			enemyPosY[y][x] = GET_ENEMY_POS_Y(y);
			enemyAlive[y][x] = true;
		}
	}

	//プログラム
	enemyMoveFlagX = MOVE_RIGHT;
	enemyMoveMode = MOVE_MODE_LR;

	//敵の弾の初期化
	for (int y = 0; y < ENEMY_COUNT_Y; y++)
	{
		for (int x = 0; x < ENEMY_COUNT_X; x++)
		{
			eShotPosX[y][x] = 50;
			eShotPosY[y][x] = 50;
			eShotAlive[y][x] = false;
		}
	}
	sceneID = SCENE_ID_TITLE;
}

//ゲームタイトルシーン
void TitleScene(void)
{
	if (sceneID == SCENE_ID_TITLE)
	{
		if (spaceKeyTriggerDown == 1)
		{
			sceneID = SCENE_ID_GAME;
		}
		SetDrawScreen(DX_SCREEN_BACK);
		ClsDrawScreen();
		PlaySoundFile("get.mp3", DX_PLAYTYPE_LOOP);

		DrawFormatString(0, 0, 0xFFFFFF, "gameCounter:%d", gameCounter);
		DrawGraph((SCREEN_SIZE_X - TITLE_SIZE_X) /2, 150, TitleImage, true);
		if ((gameCounter / 30 % 02) == 0)
		{
			DrawString(320, 450, "[hit space key]", 0xFFFFFF);
		}
		ScreenFlip();
	}
}

//ゲームシーン
void GameScene(void)
{
	if (sceneID == SCENE_ID_GAME)
	{
		//メイン処理
		PlayerCtl();
		PShotControl();
		EnemyCtl();
		EShotControl();
		HitCheck();
		//描画処理
		Draw();
	}
}

//ゲームオーバーシーン
void GameOverScene(void)
{
	if (sceneID == SCENE_ID_GAMEOVER)
	{
	if (spaceKeyTriggerDown == 1)
	{
		sceneID = SCENE_ID_INIT;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	DrawFormatString(0, 0, 0xFFFFFF, "gameCounter:%d", gameCounter);
	DrawGraph((SCREEN_SIZE_X - GAMEOVER_SIZE_X) / 2, 240, GameOverImage, true);

	if ((gameCounter / 30 % 2) == 0)
	{
		DrawString(320, 450, "[hit space key]", 0xFFFFFF);
	}
		ScreenFlip();
	}
}

//自機の処理
void PlayerCtl(void)
{
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		if (playerPosX - playerSpeed >= 0)
		{
			playerPosX -= playerSpeed;
		}
		else
		{
			playerPosX = 0;
		}
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		if (playerPosX + playerSpeed <= GAME_SCREEN_SIZE_X - PLAYER_SIZE_X)
		{
			playerPosX += playerSpeed;
		}
		else
		{
			playerPosX = GAME_SCREEN_SIZE_X - PLAYER_SIZE_X;
		}
	}
}

//自機の弾の処理
void PShotControl (void)
{
	//弾を打つ
	if (pShotAlive == false)
	{
		if (spaceKeyTriggerDown && playerAlive == true)
		{
			pShotAlive = true;
			pShotPosX = playerPosX + (PLAYER_SIZE_X - SHOT_SIZE_X) / 2;
			pShotPosY = playerPosY;
		}
	}
	else
	{
		//発射しているとき
		pShotPosY -= pShotSpeed;
		if (pShotPosY < 0) //画面外に行ったら
		{
			pShotAlive = false;
		}
	}
}

//敵の処理
void EnemyCtl(void)
{
	//敵の集団移動処理
	if (enemyMoveMode == MOVE_MODE_DOWN)
	{
		//下移動処理
		for (int y = 0; y < ENEMY_COUNT_Y; y++)
		{
			for (int x = 0; x < ENEMY_COUNT_X; x++)
			{
				enemyPosY[y][x] += ENEMY_SIZE_Y;
				//画面下部に到着した時の処理
				if (enemyPosY[y][x] + ENEMY_SIZE_Y>= playerPosY)
				{
					playerPosY = playerPosY;
				}
			}
		}
		enemyMoveMode = MOVE_MODE_LR;
	}
	else
	{
		//左右移動処理
		for (int y = 0; y < ENEMY_COUNT_Y; y++)
		{
			//左右移動
			for (int x = 0; x < ENEMY_COUNT_X; x++)
			{
				if (enemyMoveFlagX == MOVE_RIGHT)
				{
					enemyPosX[y][x] += enemySpeed;
				}
				else
				{
					enemyPosX[y][x] -= enemySpeed;
				}
			}
		}
	}		
	//敵の方向転換チェック
	CheckEnemyMoveLR();
}

//敵の方向転換チェック
void CheckEnemyMoveLR(void)
{
	//敵の方向転換チェック
	for (int y = 0; y < ENEMY_COUNT_Y; y++)
	{
		for (int x = 0; x < ENEMY_COUNT_X; x++)
		{
			if (enemyAlive[y][x] == true)
			{
				if (enemyMoveFlagX == MOVE_RIGHT)
				{
					if (enemyPosX[y][x] >= GAME_SCREEN_SIZE_X - ENEMY_SIZE_X)
					{
						//右端に到達したとき
						enemyMoveFlagX = MOVE_LEFT;
						enemyDir = DIR_LEFT;
						enemyMoveMode = MOVE_MODE_DOWN;
						return;
					}
				}
				else
				{
					if (enemyPosX[y][x] <= 0)
					{
						//左端に到達したとき
						enemyMoveFlagX = MOVE_RIGHT;
						enemyDir = DIR_RIGHT;
						enemyMoveMode = MOVE_MODE_DOWN;
						return;
					}
				}
			}
		}
	}
}

//敵弾の処理
void EShotControl(void)
{
	//敵が弾を打つ	
	for (int y = 0; y < ENEMY_COUNT_Y; y++)
	{
		for (int x = 0; x < ENEMY_COUNT_X; x++)
		{
			if (eShotAlive[y][x] == false)
			{
				if (enemyAlive[y][x] == true)
				{
					//if (CheckHitKey(KEY_INPUT_S))
					if (rand() % 60 == 0)
					{
						eShotAlive[y][x] = true;
						eShotPosX[y][x] = enemyPosX[y][x];
						eShotPosY[y][x] = enemyPosY[y][x];
					}
				}
			}
			else
			{
				//発射しているとき
				eShotPosY[y][x] += eShotSpeed;
				if (eShotPosY[y][x] > GAME_SCREEN_SIZE_Y - ENEMY_SHOT_SIZE_Y) //画面外に行ったら
				{
					eShotAlive[y][x] = false;
				}
			}
		}
	}
}

//当たり判定
void HitCheck(void)
{
	//自機と敵の弾の当たり判定
	for (int y = 0; y < ENEMY_COUNT_Y; y++)
	{
		for (int x = 0; x < ENEMY_COUNT_X; x++)
		{
			//生きてる敵だけ実施する
			if (eShotAlive[y][x] == true)
			{
				if (playerPosY < eShotPosY[y][x] + PLAYER_SIZE_Y &&
					eShotPosY[y][x] < playerPosY + ENEMY_SHOT_SIZE_Y &&
					playerPosX < eShotPosX[y][x] + PLAYER_SIZE_X &&
					eShotPosX[y][x] < playerPosX + ENEMY_SHOT_SIZE_X)
				{
					eShotAlive[y][x] = false;
					playerLife--;
					if (playerLife == 0)
					{
						sceneID = SCENE_ID_GAMEOVER;
					}
				}
			}
		}
	}
	if (pShotAlive == true)
	{
		for (int y = 0; y < ENEMY_COUNT_Y; y++)
		{
			for (int x = 0; x < ENEMY_COUNT_X; x++)
			{
				//生きてる敵だけ実施する
				if (enemyAlive[y][x] == true)
				{
					if (pShotPosY < enemyPosY[y][x] + ENEMY_SIZE_Y &&
						enemyPosY[y][x] < pShotPosY + SHOT_SIZE_Y &&
						pShotPosX < enemyPosX[y][x] + ENEMY_SIZE_X &&
						enemyPosX[y][x] < pShotPosX + SHOT_SIZE_X)
					{
						enemyAlive[y][x] = false;
						pShotAlive = false;
						TRACE("当たり!\n");

						////スコアの上限値判断
						//if (score > SCORE_MAX)
						//{
						//	score = SCORE_MAX;
						//	//hiscore = SCORE_MAX;
						//}
						//else
						//{
						//	score = score;
						//}

						//三項演算子　＝　（条件）？　真：偽
						score = (score > SCORE_MAX) ? SCORE_MAX : score;
						// 得点の追加       
						score += ENEMY_POINT;
						//ハイスコアの算出
						if (score > hiscore)	//スコアがハイスコアを上回ったとき
						{
							//ハイスコアをスコアに入れ替える
							hiscore = score;
						}
					}
				}
			}
		}
	}
}

//描画処理
void Draw(void)
{

	SetDrawScreen(DX_SCREEN_BACK);			//バックバッファに設定
	ClsDrawScreen();						//画面消去
	DrawBox(GAME_SCREEN_X + 0, GAME_SCREEN_Y + 0, GAME_SCREEN_X + GAME_SCREEN_SIZE_X, GAME_SCREEN_Y + GAME_SCREEN_SIZE_Y, GetColor(30, 30, 30), true);

	//敵
	for (int y = 0; y < ENEMY_COUNT_Y; y++)
	{
		for (int x = 0; x < ENEMY_COUNT_X; x++)
		{
			if (enemyAlive[y][x] == true)
			{
				DrawGraph(GAME_SCREEN_X + enemyPosX[y][x], GAME_SCREEN_Y + enemyPosY[y][x], enemyImage[y % (ENEMY_TYPE_MAX - 1)][enemyDir][(gameCounter/10)%3], true);
			}
		}

	}

	//自機の弾
	if (pShotAlive == true)
	{
		DrawGraph(GAME_SCREEN_X + pShotPosX, GAME_SCREEN_Y + pShotPosY, pShotImage, true);
	}

	//自機
	DrawGraph(GAME_SCREEN_X + playerPosX, GAME_SCREEN_Y + playerPosY, playerImage[DIR_UP][(gameCounter/10)%3], true);

	//敵の弾
	for (int y = 0; y < ENEMY_COUNT_Y; y++)
	{
		for (int x = 0; x < ENEMY_COUNT_X; x++)
		{
			if (eShotAlive[y][x] == true)
			{
				DrawGraph(GAME_SCREEN_X + eShotPosX[y][x], GAME_SCREEN_Y + eShotPosY[y][x], eShotImage, true);
			}
		}
	}

	//ハイスコアの表示
	DrawString(GAME_SCREEN_SIZE_X + GAME_SCREEN_X, 64, "H I G H",0xFFFFFF);
	DrawString(GAME_SCREEN_SIZE_X + GAME_SCREEN_X, 80, "S C O R E", 0xFFFFFF);
	DrawFormatString(GAME_SCREEN_SIZE_X + GAME_SCREEN_X + 8 * 14 - stringWidth, 96, 0xFFFFFF, "%d", hiscore);
	stringWidth = GetDrawFormatStringWidth("%d", hiscore);

	//スコアの表示
	DrawString(GAME_SCREEN_SIZE_X + GAME_SCREEN_X,102, "score", 0xFFFFFF);
	DrawFormatString(GAME_SCREEN_SIZE_X + GAME_SCREEN_X  + 8*14 - stringWidth , 118, 0xFFFFFF, "%d",score);

	//自機の残機数の表示
	DrawFormatString(GAME_SCREEN_SIZE_X + GAME_SCREEN_X,500, 0xFFFFFF, "残機数%d", playerLife);
	stringWidth = GetDrawFormatStringWidth("%d",score);
	DrawFormatString(0,0, 0xFFFFFF, "プレイヤー座標;(%d ,%d)" ,playerPosX ,playerPosY );
	for (int y = 0; y < playerLife - 1; y++)	//自機は１機プレイしているので-１する
	{
		DrawGraph(GAME_SCREEN_SIZE_X + GAME_SCREEN_X + ( y % 3 ) * PLAYER_SIZE_X, 486 + (y % 3) + GAME_SCREEN_Y - 16, playerImage[DIR_DOWN][2], true);
	}

	_dbgAddDraw();
	ScreenFlip();
}

//システム系初期化
bool SysInit(void)
{
	//システム処理
	SetWindowText("課題2_2016193_庄嶋洋斗");
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);		//640×480ドット65536色モードに設定
	ChangeWindowMode(true);								//true:window false:フルスクリーン
	if (DxLib_Init() == -1)
	{
		return -1;				//DXライブラリ初期化
	}
	_dbgSetup(SCREEN_SIZE_X, SCREEN_SIZE_Y, 255);

	srand((unsigned int)time(NULL));

	//---------画像の読み込み----------------
	//自機弾の読み込み
	pShotImage = LoadGraph("image/pshot.png");
	if (pShotImage == -1)
	{
		return  false;
	}
	//敵の弾の読み込み
	eShotImage = LoadGraph("image/eshot.png");
	if (eShotImage == -1)
	{
		return  false;
	}

	//自機の読み込み
	if (LoadDivGraph("image/Female 05-2.png", ANIM_MAX * DIR_MAX, ANIM_MAX , DIR_MAX, PLAYER_SIZE_X, PLAYER_SIZE_Y, & playerImage[0][0])==-1)
	{
		return false;
	}

	//敵の読み込み
	const char* enemyFileName[ENEMY_TYPE_MAX] = {
		"image/Enemy 15-3.png",
		"image/Enemy 15-2.png",
		"image/Enemy 15-6.png",
		"image/Enemy 15-4.png",
		"image/Enemy 15-5.png",
		"image/Enemy 15-1.png",
		"image/Enemy 09-1.png"
	};

	//ゲームオーバー画面の読み込み
	GameOverImage = LoadGraph("image/GameOver.png");
	if(GameOverImage == -1)
	{
		TRACE("ゲームオーバー");
	}

	//ゲームタイトル画面の読み込み
	TitleImage = LoadGraph("image/TitleLogo2.png");
	if (TitleImage == -1)
	{
		TRACE("ゲームオーバー");
	}

	//
	for (int j = 0; j < ENEMY_TYPE_MAX; j++)
	{
		if (LoadDivGraph(enemyFileName[j], ANIM_MAX * DIR_MAX, ANIM_MAX, DIR_MAX, ENEMY_SIZE_X, ENEMY_SIZE_Y, &enemyImage[j][0][0]) == -1)
		{
			return false;
		}
	}

	//-----------変数の初期化----------
	//ゲームシーンの初期化
	gameCounter = 0;
	sceneID = SCENE_ID_INIT;
	spaceKeyBack = 0;
	spaceKeyTriggerDown = 0;
	spaceKeyTriggerUp = 0;
	hiscore = HISCORE_DEFALUT;
	score = 0;

	//自機の弾の初期化
	pShotPosX = 0;
	pShotPosY = 0;
	pShotSpeed = 12;
	pShotAlive = true;

	//自機の初期化
	playerPosX = (GAME_SCREEN_SIZE_X - PLAYER_SIZE_X) / 2;
	playerPosY = (GAME_SCREEN_SIZE_Y - PLAYER_SIZE_Y);
	playerSpeed = 8;
	playerAlive = true;

	//自機の残機
	playerLife = 3;

	//敵の初期化
	for (int y = 0; y < ENEMY_COUNT_Y; y++)
	{
		for (int x = 0; x < ENEMY_COUNT_X; x++)
		{
			enemyPosX[y][x] = x * ENEMY_SIZE_X * 3 / 2;
			enemyPosY[y][x] = GET_ENEMY_POS_Y(y);
			enemyAlive[y][x] = true;
		}
	}
	enemyDir = DIR_RIGHT;
	enemySpeed= 3;

	//プログラム
	enemyMoveFlagX	= MOVE_RIGHT;
	enemyMoveMode	= MOVE_MODE_LR;

	//敵の弾の初期化
	for (int y = 0; y < ENEMY_COUNT_Y; y++)
	{
		for (int x = 0; x < ENEMY_COUNT_X; x++)
		{
			eShotPosX[y][x] = 50;
			eShotPosY[y][x] = 50;
			eShotAlive[y][x] = false;
		}
	}
	eShotSpeed = 5;
	return true;
}
