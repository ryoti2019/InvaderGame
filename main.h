#pragma once

enum SCN_ID {
	SCN_ID_TITLE,
	SCN_ID_GAME,
	SCN_ID_GAMEOVER,
	SCN_ID_MAX
};

enum ENEMY_TYPE {
	ENEMY_TYPE_RED,
	ENEMY_TYPE_BLUE,
	ENEMY_TYPE_GREEN,
	ENMY_TYPE_YERROW,
	ENEMY_TYPE_PURPLE,
	ENEMY_TYPE_GRAY,
	ENEMY_TYPE_UFO,
	ENEMY_TYPE_MAX,
	ENEMY_TYPE_NON
};
//�G�̍��E�̈ړ�����
enum MOVE_LR{
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_MAX,
};

//�ړ����[�h
enum MOVE_MODE {
	MOVE_MODE_LR,		//���E�ړ����[�h
	MOVE_MODE_DOWN,		//���ړ����[�h
	MOVE_MODE_MAX
};

enum SCR {
	SCR_PL1,		//�v���C���[
	SCR_HIGH,		//�n�C�X�R�A
	SCR_MAX
};

#define HIGH_SCR_DEF 5000	//�n�C�X�R�A�̃f�t�H���g�l

#define SCREEN_SIZE_X 800	//��ʉ��T�C�Y
#define SCREEN_SIZE_Y 600	//��ʏc�T�C�Y

#define GAME_SCREEN_X 40		//�Q�[����ʂ̕`��̃I�t�Z�b�gX
#define GAME_SCREEN_Y 75		//�Q�[����ʂ̕`��̃I�t�Z�b�gY
#define GAME_SCREEN_SIZE_X 640	//�Q�[����ʂ̉��T�C�Y
#define GAME_SCREEN_SIZE_Y 480	//�Q�[����ʂ̏c�T�C�Y

#define ANIM_MAX	3		//�A�j���[�V��������
#define DIR_MAX		4		//�L�����̌�������

//����
#define DIR_DOWN 0
#define DIR_LEFT 1
#define DIR_RIGHT 2
#define DIR_UP 3

#define PLAYER_SIZE_X 32	//�v���C���[�̉��T�C�Y
#define PLAYER_SIZE_Y 32	//�v���C���[�̏c�T�C�Y

//�G�֘A
#define ENEMY_COUNT_X 8		//�G�̉����т̐�
#define ENEMY_COUNT_Y 7		//�G�̏c���т̐�

#define ENEMY_SIZE_X 32		//���T�C�Y
#define ENEMY_SIZE_Y 32		//�c�T�C�Y
#define ENEMY_SPEED 8		//����

//�������֘A
#define GHOST_SIZE_X 32
#define GHOST_SIZE_Y 32
#define GHOST_SPEED 2

//�e�֘A
#define PSHOT_SIZE_X	6
#define PSHOT_SIZE_Y	30
#define PSHOT_SPEED		10
#define ESHOT_SIZE_X	16
#define ESHOT_SIZE_Y	25

//�G�t�F�N�g�֘A
#define EFFECT_SIZE_X 40
#define EFFECT_SIZE_Y 40

//Y�Ŏw�肵���s��Y���W�ɕϊ�����
#define GET_ENEMY_POS_Y(Y) ((ENEMY_SIZE_Y*9)/7) * (Y)



bool SysInit(void);			//�V�X�e���n������
bool GameInit(void);		//�Q�[���n�̏�����
void GetKeyState(void);
void TitleScene(void);
void TitleDraw(void);
void GameScene(void);		//�Q�[���V�[��
void GameDraw(void);		//�`�揈��
void GameOverScene(void);
void GameoverDraw(void);
void DrawTexts(void);
void PlayerCtl(void);		//�v���C���[�̏���
void EnemyCtl(void);		//�G�̏���
void GhostCtl(void);		//�������̏���
void PshotCtl(void);		//�e�̏���
void EshotCtl(void);
void CheckHitObj(void);
void AddScr(int num);
void PlayerDeathProc(bool backFlag);	//�v���C���[�̎��S������