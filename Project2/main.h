#pragma once

#define SCREEN_SIZE_X	(800)		//��ʂ̉��T�C�Y
#define SCREEN_SIZE_Y	(600)		//��ʂ̏c�T�C�Y

#define GAME_SCREEN_X		40		//�Q�[����ʂ̕`��I�t�Z�b�g
#define GAME_SCREEN_Y		60		//�Q�[����ʂ̃I�t�Z�b�g
#define GAME_SCREEN_SIZE_X	640		//�Q�[����ʂ̉��T�C�Y
#define GAME_SCREEN_SIZE_Y	480		//�Q�[����ʂ̏c�T�C�Y

#define GAMEOVER_SIZE_X		452		//�Q�[���I�[�o�[��ʂ̉��T�C�Y
#define GAMEOVER_SIZE_Y		123		//�Q�[���I�[�o�[��ʂ̏c�T�C�Y

#define TITLE_SIZE_X		423		//�Q�[���^�C�g����ʂ̉��T�C�Y
#define TITLE_SIZE_Y		198		//�Q�[���^�C�g����ʂ̏c�T�C�Y

#define PLAYER_SIZE_X		32		//���@�̉��T�C�Y
#define PLAYER_SIZE_Y		32		//���@�̏c�T�C�Y

#define ENEMY_SIZE_X		32		//�G�̏c�T�C�Y
#define ENEMY_SIZE_Y		32		//�G�ɉ��T�C�Y

#define ANIM_MAX			3		//�A�j���[�V�����̃R�}��
	
//#define ENEMY_COUNT_X		8		//�G�̉����т̐�
//#define ENEMY_COUNT_Y		7		//�G�̏c���т̐�

#define ENEMY_COUNT_X		8		//�G�̉����т̐�
#define ENEMY_COUNT_Y		2		//�G�̏c���т̐�

#define ENEMY_POINT			300		//�G��|�������̃|�C���g
#define HISCORE_DEFALUT		10000	//�ŏ��̃n�C�X�R�A
#define SCORE_MAX			99999	//�ő�X�R�A

#define SHOT_SIZE_X			6		//���@�̒e�̉��T�C�Y
#define SHOT_SIZE_Y			30		//���@�̒e�̏c�T�C�Y

#define ENEMY_SHOT_SIZE_X		16	//�G�̒e�̉��T�C�Y
#define ENEMY_SHOT_SIZE_Y		25	//�G�̒e�̏c�T�C�Y

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
	DIR_DOWN,		//������1
	DIR_LEFT,		//������2
	DIR_RIGHT,		//�E����3
	DIR_UP,			//�����4
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

//�G�̍��E�̈ړ�����
enum MOVE_LR
{
	MOVE_LEFT,		//������
	MOVE_RIGHT,		//�E����
	MOVE_MAX,
};

enum MOVE_MODE
{
	MOVE_MODE_LR,
	MOVE_MODE_DOWN,
	MOVE_MODE_MAX,
};

//-----------�v���g�^�C�v�錾----------
void KeyCheck(void);		//�L�[�`�F�b�N
void InitScene(void);		//�������V�[��
void TitleScene(void);		//�^�C�g���V�[���̏���
void GameScene(void);		//�Q�[���V�[���̏���
void GameOverScene(void);	//�Q�[���I�[�o�[�V�[���̏���

void PlayerCtl(void);		//���@�̏���
void PShotControl(void);	//���@�e�̏���	
void EnemyCtl(void);		//�G�̏���
void EShotControl(void);	//�G�e�̏���	
void HitCheck(void);		//�����蔻��
void Draw(void);			//�`�揈��

void CheckEnemyMoveLR(void);//�G�̕����]���`�F�b�N
bool SysInit(void);			//�V�X�e���n������
