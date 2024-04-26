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
//敵の左右の移動方向
enum MOVE_LR{
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_MAX,
};

//移動モード
enum MOVE_MODE {
	MOVE_MODE_LR,		//左右移動モード
	MOVE_MODE_DOWN,		//下移動モード
	MOVE_MODE_MAX
};

enum SCR {
	SCR_PL1,		//プレイヤー
	SCR_HIGH,		//ハイスコア
	SCR_MAX
};

#define HIGH_SCR_DEF 5000	//ハイスコアのデフォルト値

#define SCREEN_SIZE_X 800	//画面横サイズ
#define SCREEN_SIZE_Y 600	//画面縦サイズ

#define GAME_SCREEN_X 40		//ゲーム画面の描画のオフセットX
#define GAME_SCREEN_Y 75		//ゲーム画面の描画のオフセットY
#define GAME_SCREEN_SIZE_X 640	//ゲーム画面の横サイズ
#define GAME_SCREEN_SIZE_Y 480	//ゲーム画面の縦サイズ

#define ANIM_MAX	3		//アニメーション総数
#define DIR_MAX		4		//キャラの向き総数

//向き
#define DIR_DOWN 0
#define DIR_LEFT 1
#define DIR_RIGHT 2
#define DIR_UP 3

#define PLAYER_SIZE_X 32	//プレイヤーの横サイズ
#define PLAYER_SIZE_Y 32	//プレイヤーの縦サイズ

//敵関連
#define ENEMY_COUNT_X 8		//敵の横並びの数
#define ENEMY_COUNT_Y 7		//敵の縦並びの数

#define ENEMY_SIZE_X 32		//横サイズ
#define ENEMY_SIZE_Y 32		//縦サイズ
#define ENEMY_SPEED 8		//速さ

//お化け関連
#define GHOST_SIZE_X 32
#define GHOST_SIZE_Y 32
#define GHOST_SPEED 2

//弾関連
#define PSHOT_SIZE_X	6
#define PSHOT_SIZE_Y	30
#define PSHOT_SPEED		10
#define ESHOT_SIZE_X	16
#define ESHOT_SIZE_Y	25

//エフェクト関連
#define EFFECT_SIZE_X 40
#define EFFECT_SIZE_Y 40

//Yで指定した行のY座標に変換する
#define GET_ENEMY_POS_Y(Y) ((ENEMY_SIZE_Y*9)/7) * (Y)



bool SysInit(void);			//システム系初期化
bool GameInit(void);		//ゲーム系の初期化
void GetKeyState(void);
void TitleScene(void);
void TitleDraw(void);
void GameScene(void);		//ゲームシーン
void GameDraw(void);		//描画処理
void GameOverScene(void);
void GameoverDraw(void);
void DrawTexts(void);
void PlayerCtl(void);		//プレイヤーの処理
void EnemyCtl(void);		//敵の処理
void GhostCtl(void);		//お化けの処理
void PshotCtl(void);		//弾の処理
void EshotCtl(void);
void CheckHitObj(void);
void AddScr(int num);
void PlayerDeathProc(bool backFlag);	//プレイヤーの死亡時処理