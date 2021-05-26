#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"
#include <time.h>
#include "main.h"
#include "DxLib.h"

//�V�X�e��
int gameCounter;
SCENE_ID sceneID;							//�V�[���Ǘ��p
int GameOverImage;							//�Q�[���I�[�o�[�̃O���t�B�b�N�n���h��
int TitleImage;								//�Q�[���^�C�g���̃O���t�B�b�N�n���h��
int spaceKeyBack;							//�X�؁[�X�L�[��1�t���[���O�̏��
int spaceKeyTriggerDown;					//�X�y�[�X�L�[�̃g���K�[Key Down
int spaceKeyTriggerUp;						//�X�y�[�X�L�[�̃g���K�[Key Up
int hiscore;								//�n�C�X�R�A�̊i�[����
int score;									//�X�R�A�̊i�[����
int stringWidth;							//������̃s�N�Z���`�敝

//�v���C���[�֘A
int playerImage[DIR_MAX][ANIM_MAX];			//���@�摜�̃O���t�B�b�N�n���h��
int playerPosX;								//���@��X���W
int playerPosY;								//���@��Y���W
int playerSpeed;							//���@�̈ړ���
int playerLife;								//���@�̎c�@
bool playerAlive;							//�����t���O

//�G
int enemyImage[ENEMY_TYPE_MAX][DIR_MAX][ANIM_MAX];	//�G�摜�̊i�[�̈�
int enemyPosX[ENEMY_COUNT_Y][ENEMY_COUNT_X];		//�G��X���W
int enemyPosY[ENEMY_COUNT_Y][ENEMY_COUNT_X];		//�G��Y���W
int enemySpeed;										//�ړ���
bool enemyAlive[ENEMY_COUNT_Y][ENEMY_COUNT_X];		//�����t���O

//�G�̒e
int eShotImage;										//�O���t�B�b�N�n���h��
int eShotPosX[ENEMY_COUNT_Y][ENEMY_COUNT_X];		//X���W
int eShotPosY[ENEMY_COUNT_Y][ENEMY_COUNT_X];		//Y���W
int eShotSpeed;										//�ړ���
bool eShotAlive[ENEMY_COUNT_Y][ENEMY_COUNT_X];		//��ԃt���O
	
//���@�̒e
int pShotImage;										//�O���t�B�b�N�n���h��
int pShotPosX;										//X���W
int pShotPosY;										//Y���W
int pShotSpeed;										//�ړ���
bool pShotAlive;									//��ԃt���O

MOVE_MODE enemyMoveMode;							//�G�̍��E�ړ������ړ�
MOVE_LR enemyMoveFlagX;								//�G�̍��E�ړ��ڊǊǗ�
DIR_ID enemyDir;									//

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//����������
	if (!SysInit())
	{
		TRACE("���������s");
		return -1;
	}
	//�Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		gameCounter++;
		_dbgStartDraw();

		KeyCheck();				//�L�[�g���K�[�̃`�F�b�N

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

		////�Q�[���^�C�g���V�[��
		//if (sceneID == SCENE_ID_TITLE)
		//{
		//	TitleScene();
		//}

		////�Q�[���V�[��
		//else if (sceneID == SCENE_ID_GAME)		
		//{
		//	GameScene();
		//}

		////�Q�[���I�[�o�[�V�[��
		//else if (sceneID == SCENE_ID_GAMEOVER)
		//{
		//	GameOverScene();
		//}
	}
	DxLib_End();		//DX���C�u�����̏I������
	return 0;
}

//�L�[�`�F�b�N
void KeyCheck(void)
{
	spaceKeyTriggerDown = 0;
	spaceKeyTriggerUp = 0;

	//1�t���[���O�̏��
	if (spaceKeyBack == 1)
	{
		TRACE("�O:on");
	}
	else
	{
		TRACE("�O:off");
	}

	//���̏��
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		TRACE("-��:on");
	}
	else
	{
		TRACE("-��:off");
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

//�������V�[��
void InitScene(void)
{
	TRACE("INIT_SCENE\n")
	//���@�̒e�̏�����
	pShotAlive = false;

	//���@�̏�����
	playerPosX = (GAME_SCREEN_SIZE_X - PLAYER_SIZE_X) / 2;
	playerPosY = (GAME_SCREEN_SIZE_Y - PLAYER_SIZE_Y);
	playerAlive = true;

	//���@�̎c�@
	playerLife = 3;
	score = 0;

	//�G�̏�����
	for (int y = 0; y < ENEMY_COUNT_Y; y++)
	{
		for (int x = 0; x < ENEMY_COUNT_X; x++)
		{
			enemyPosX[y][x] = x * ENEMY_SIZE_X * 3 / 2;
			enemyPosY[y][x] = GET_ENEMY_POS_Y(y);
			enemyAlive[y][x] = true;
		}
	}

	//�v���O����
	enemyMoveFlagX = MOVE_RIGHT;
	enemyMoveMode = MOVE_MODE_LR;

	//�G�̒e�̏�����
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

//�Q�[���^�C�g���V�[��
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

//�Q�[���V�[��
void GameScene(void)
{
	if (sceneID == SCENE_ID_GAME)
	{
		//���C������
		PlayerCtl();
		PShotControl();
		EnemyCtl();
		EShotControl();
		HitCheck();
		//�`�揈��
		Draw();
	}
}

//�Q�[���I�[�o�[�V�[��
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

//���@�̏���
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

//���@�̒e�̏���
void PShotControl (void)
{
	//�e��ł�
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
		//���˂��Ă���Ƃ�
		pShotPosY -= pShotSpeed;
		if (pShotPosY < 0) //��ʊO�ɍs������
		{
			pShotAlive = false;
		}
	}
}

//�G�̏���
void EnemyCtl(void)
{
	//�G�̏W�c�ړ�����
	if (enemyMoveMode == MOVE_MODE_DOWN)
	{
		//���ړ�����
		for (int y = 0; y < ENEMY_COUNT_Y; y++)
		{
			for (int x = 0; x < ENEMY_COUNT_X; x++)
			{
				enemyPosY[y][x] += ENEMY_SIZE_Y;
				//��ʉ����ɓ����������̏���
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
		//���E�ړ�����
		for (int y = 0; y < ENEMY_COUNT_Y; y++)
		{
			//���E�ړ�
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
	//�G�̕����]���`�F�b�N
	CheckEnemyMoveLR();
}

//�G�̕����]���`�F�b�N
void CheckEnemyMoveLR(void)
{
	//�G�̕����]���`�F�b�N
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
						//�E�[�ɓ��B�����Ƃ�
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
						//���[�ɓ��B�����Ƃ�
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

//�G�e�̏���
void EShotControl(void)
{
	//�G���e��ł�	
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
				//���˂��Ă���Ƃ�
				eShotPosY[y][x] += eShotSpeed;
				if (eShotPosY[y][x] > GAME_SCREEN_SIZE_Y - ENEMY_SHOT_SIZE_Y) //��ʊO�ɍs������
				{
					eShotAlive[y][x] = false;
				}
			}
		}
	}
}

//�����蔻��
void HitCheck(void)
{
	//���@�ƓG�̒e�̓����蔻��
	for (int y = 0; y < ENEMY_COUNT_Y; y++)
	{
		for (int x = 0; x < ENEMY_COUNT_X; x++)
		{
			//�����Ă�G�������{����
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
				//�����Ă�G�������{����
				if (enemyAlive[y][x] == true)
				{
					if (pShotPosY < enemyPosY[y][x] + ENEMY_SIZE_Y &&
						enemyPosY[y][x] < pShotPosY + SHOT_SIZE_Y &&
						pShotPosX < enemyPosX[y][x] + ENEMY_SIZE_X &&
						enemyPosX[y][x] < pShotPosX + SHOT_SIZE_X)
					{
						enemyAlive[y][x] = false;
						pShotAlive = false;
						TRACE("������!\n");

						////�X�R�A�̏���l���f
						//if (score > SCORE_MAX)
						//{
						//	score = SCORE_MAX;
						//	//hiscore = SCORE_MAX;
						//}
						//else
						//{
						//	score = score;
						//}

						//�O�����Z�q�@���@�i�����j�H�@�^�F�U
						score = (score > SCORE_MAX) ? SCORE_MAX : score;
						// ���_�̒ǉ�       
						score += ENEMY_POINT;
						//�n�C�X�R�A�̎Z�o
						if (score > hiscore)	//�X�R�A���n�C�X�R�A���������Ƃ�
						{
							//�n�C�X�R�A���X�R�A�ɓ���ւ���
							hiscore = score;
						}
					}
				}
			}
		}
	}
}

//�`�揈��
void Draw(void)
{

	SetDrawScreen(DX_SCREEN_BACK);			//�o�b�N�o�b�t�@�ɐݒ�
	ClsDrawScreen();						//��ʏ���
	DrawBox(GAME_SCREEN_X + 0, GAME_SCREEN_Y + 0, GAME_SCREEN_X + GAME_SCREEN_SIZE_X, GAME_SCREEN_Y + GAME_SCREEN_SIZE_Y, GetColor(30, 30, 30), true);

	//�G
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

	//���@�̒e
	if (pShotAlive == true)
	{
		DrawGraph(GAME_SCREEN_X + pShotPosX, GAME_SCREEN_Y + pShotPosY, pShotImage, true);
	}

	//���@
	DrawGraph(GAME_SCREEN_X + playerPosX, GAME_SCREEN_Y + playerPosY, playerImage[DIR_UP][(gameCounter/10)%3], true);

	//�G�̒e
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

	//�n�C�X�R�A�̕\��
	DrawString(GAME_SCREEN_SIZE_X + GAME_SCREEN_X, 64, "H I G H",0xFFFFFF);
	DrawString(GAME_SCREEN_SIZE_X + GAME_SCREEN_X, 80, "S C O R E", 0xFFFFFF);
	DrawFormatString(GAME_SCREEN_SIZE_X + GAME_SCREEN_X + 8 * 14 - stringWidth, 96, 0xFFFFFF, "%d", hiscore);
	stringWidth = GetDrawFormatStringWidth("%d", hiscore);

	//�X�R�A�̕\��
	DrawString(GAME_SCREEN_SIZE_X + GAME_SCREEN_X,102, "score", 0xFFFFFF);
	DrawFormatString(GAME_SCREEN_SIZE_X + GAME_SCREEN_X  + 8*14 - stringWidth , 118, 0xFFFFFF, "%d",score);

	//���@�̎c�@���̕\��
	DrawFormatString(GAME_SCREEN_SIZE_X + GAME_SCREEN_X,500, 0xFFFFFF, "�c�@��%d", playerLife);
	stringWidth = GetDrawFormatStringWidth("%d",score);
	DrawFormatString(0,0, 0xFFFFFF, "�v���C���[���W;(%d ,%d)" ,playerPosX ,playerPosY );
	for (int y = 0; y < playerLife - 1; y++)	//���@�͂P�@�v���C���Ă���̂�-�P����
	{
		DrawGraph(GAME_SCREEN_SIZE_X + GAME_SCREEN_X + ( y % 3 ) * PLAYER_SIZE_X, 486 + (y % 3) + GAME_SCREEN_Y - 16, playerImage[DIR_DOWN][2], true);
	}

	_dbgAddDraw();
	ScreenFlip();
}

//�V�X�e���n������
bool SysInit(void)
{
	//�V�X�e������
	SetWindowText("�ۑ�2_2016193_�����m�l");
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);		//640�~480�h�b�g65536�F���[�h�ɐݒ�
	ChangeWindowMode(true);								//true:window false:�t���X�N���[��
	if (DxLib_Init() == -1)
	{
		return -1;				//DX���C�u����������
	}
	_dbgSetup(SCREEN_SIZE_X, SCREEN_SIZE_Y, 255);

	srand((unsigned int)time(NULL));

	//---------�摜�̓ǂݍ���----------------
	//���@�e�̓ǂݍ���
	pShotImage = LoadGraph("image/pshot.png");
	if (pShotImage == -1)
	{
		return  false;
	}
	//�G�̒e�̓ǂݍ���
	eShotImage = LoadGraph("image/eshot.png");
	if (eShotImage == -1)
	{
		return  false;
	}

	//���@�̓ǂݍ���
	if (LoadDivGraph("image/Female 05-2.png", ANIM_MAX * DIR_MAX, ANIM_MAX , DIR_MAX, PLAYER_SIZE_X, PLAYER_SIZE_Y, & playerImage[0][0])==-1)
	{
		return false;
	}

	//�G�̓ǂݍ���
	const char* enemyFileName[ENEMY_TYPE_MAX] = {
		"image/Enemy 15-3.png",
		"image/Enemy 15-2.png",
		"image/Enemy 15-6.png",
		"image/Enemy 15-4.png",
		"image/Enemy 15-5.png",
		"image/Enemy 15-1.png",
		"image/Enemy 09-1.png"
	};

	//�Q�[���I�[�o�[��ʂ̓ǂݍ���
	GameOverImage = LoadGraph("image/GameOver.png");
	if(GameOverImage == -1)
	{
		TRACE("�Q�[���I�[�o�[");
	}

	//�Q�[���^�C�g����ʂ̓ǂݍ���
	TitleImage = LoadGraph("image/TitleLogo2.png");
	if (TitleImage == -1)
	{
		TRACE("�Q�[���I�[�o�[");
	}

	//
	for (int j = 0; j < ENEMY_TYPE_MAX; j++)
	{
		if (LoadDivGraph(enemyFileName[j], ANIM_MAX * DIR_MAX, ANIM_MAX, DIR_MAX, ENEMY_SIZE_X, ENEMY_SIZE_Y, &enemyImage[j][0][0]) == -1)
		{
			return false;
		}
	}

	//-----------�ϐ��̏�����----------
	//�Q�[���V�[���̏�����
	gameCounter = 0;
	sceneID = SCENE_ID_INIT;
	spaceKeyBack = 0;
	spaceKeyTriggerDown = 0;
	spaceKeyTriggerUp = 0;
	hiscore = HISCORE_DEFALUT;
	score = 0;

	//���@�̒e�̏�����
	pShotPosX = 0;
	pShotPosY = 0;
	pShotSpeed = 12;
	pShotAlive = true;

	//���@�̏�����
	playerPosX = (GAME_SCREEN_SIZE_X - PLAYER_SIZE_X) / 2;
	playerPosY = (GAME_SCREEN_SIZE_Y - PLAYER_SIZE_Y);
	playerSpeed = 8;
	playerAlive = true;

	//���@�̎c�@
	playerLife = 3;

	//�G�̏�����
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

	//�v���O����
	enemyMoveFlagX	= MOVE_RIGHT;
	enemyMoveMode	= MOVE_MODE_LR;

	//�G�̒e�̏�����
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
