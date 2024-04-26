#include <DxLib.h>
#include "main.h"
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"

SCN_ID scnID;			// シーン管理用
int titleImage;			// タイトル画面のグラフィックハンドル
int gameOverImage;		// ゲームオーバー画面のグラフィックハンドル
bool zKeyFlag;			// Zキーの押下フラグ
bool zKeyFlagOld;		// Zキーの押下フラグ(1フレーム前)
bool spaceKeyFlag;		// SPACEキーの押下フラグ
bool spaceKeyFlagOld;	// SPACEキーの押下フラグ(1フレーム前)
int blinkCnt;			// 明滅用カウンター
int scrCnt[SCR_MAX];

//プレイヤー関連
int playerImage[DIR_MAX][ANIM_MAX];		// グラフィックハンドル
int playerPosX;							// X座標
int playerPosY;							// Y座標
int playerSpeed;						// 移動量
int playerAnimCnt;						// アニメーションカウンター
int playerAnimData[4];					// アニメーションの再生番号
int playerDir;							// 向き
bool playerFlag;						// 状態
int playerLife;

//自機の弾関連
int pShotImage;							//グラフィックハンドル
int pShotPosX;							//X座標
int pShotPosY;							//Y座標
int pShotSpeed;							//移動量
int pShotFlag;							//状態

//敵関連
int enemyImage[ENEMY_TYPE_MAX][DIR_MAX][ANIM_MAX];
int enemyPosX[ENEMY_COUNT_Y][ENEMY_COUNT_X];
int enemyPosY[ENEMY_COUNT_Y][ENEMY_COUNT_X];
int enemySpeed;
int enemyAnimCnt;
int enemyDir;
int enemyAnimData[4];
ENEMY_TYPE enemyFlag[ENEMY_COUNT_Y][ENEMY_COUNT_X];
MOVE_LR enemyMoveFlagX;								//左右移動制御
MOVE_MODE enemyMoveMode;							//左右移動か下移動かの管理用
int enemyMoveLineX;									//移動列の制御用
int enemyMoveLineY;									//移動行の制御用
int enemyMoveCount;									//移動タイミングの制御用

//敵の弾関連
int eShotImage;										//グラフィックハンドル
int eShotPosX[ENEMY_COUNT_X];						//X座標
int eShotPosY[ENEMY_COUNT_X];						//Y座標
int eShotSpeed;										//移動量
bool eShotFlag[ENEMY_COUNT_X];						//状態

//お化け関連
int ghostImage[DIR_MAX][ANIM_MAX];
int ghostPosX;
int ghostPosY;
int ghostSpeed;
bool ghostFlag;
int ghostAnimCnt;
int ghostDir;
int ghostAnimData[4];

//エフェクト関連
int effectImage;
int effectPosX;
int effectPosY;
int effectCnt;

//サウンド関連
int bgmSound;
int enemyDeathSound;
int ghostSound;
int shotSound;
int ghostDeathSound;
int playerSound;

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	if (SysInit() == false)
	{
		return -1;
	}


	if (GameInit() == false)
	{
		return -1;
	}

	while (ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		_dbgStartDraw();
		GetKeyState();
		switch (scnID)
		{
		case SCN_ID_TITLE:
			TitleScene();
			break;
		case SCN_ID_GAME:
			GameScene();
			break;
		case SCN_ID_GAMEOVER:
			GameOverScene();
			break;
		default:
			//シーンID異常
			TRACE("シーンIDが異常\n");
			break;
		}
	}
	TitleScene();
	DxLib_End();
	return 0;
}

void TitleScene(void)
{
	//if (zKeyFlag == true && !zKeyFlagOld == false)
	if (zKeyFlag && !zKeyFlagOld)
	{
		PlaySoundMem(bgmSound, DX_PLAYTYPE_BACK, true);
		scnID = SCN_ID_GAME;
	}
	TitleDraw();
}

void TitleDraw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();

	DrawBox(GAME_SCREEN_X,
		GAME_SCREEN_Y,
		GAME_SCREEN_SIZE_X + GAME_SCREEN_X,
		GAME_SCREEN_SIZE_Y + GAME_SCREEN_Y,
		0x323232, true);

	DrawGraph(120, 120, titleImage, true);

	blinkCnt++;
	if ((blinkCnt /30) % 2)
	{
		DrawString(220, 350, "ＰＬＥＡＳＥ　'Ｚ'　ＫＥＹ", 0xffffff);
	}

	DrawTexts();
	ScreenFlip();
}

void GameOverScene(void)
{
	StopSoundMem(bgmSound);
	if (spaceKeyFlag && !spaceKeyFlagOld)
	{
		GameInit();
		scnID = SCN_ID_TITLE;
	}
	GameoverDraw();
}

void GameoverDraw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	DrawGraph(120, 200, gameOverImage, true);
	blinkCnt++;
	if ((blinkCnt / 30) % 2)
	{
		DrawString(290, 350, "コンティニュー？", 0xffffff);
		DrawString(280, 380, "'ＳＰＡＣＥ'ＫＥＹ", 0xffffff);
	}
	DrawTexts();
	ScreenFlip();
}

void GameScene(void)
{
	PlayerCtl();
	EnemyCtl();
	GhostCtl();
	PshotCtl();
	EshotCtl();
	CheckHitObj();
	GameDraw();
}

void PlayerCtl(void)
{
	if (!playerFlag)
	{
		playerPosX = (GAME_SCREEN_SIZE_X - PLAYER_SIZE_X) / 2;
		playerPosY = (GAME_SCREEN_SIZE_Y - PLAYER_SIZE_Y);
		playerFlag = true;
	}

	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		if (playerPosX - playerSpeed >= 0)
		{
			playerPosX -= playerSpeed;
		}
		playerDir = DIR_LEFT;
	}


	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		if (playerPosX + playerSpeed <= GAME_SCREEN_SIZE_X - PLAYER_SIZE_X)
		if (playerPosX + playerSpeed <= GAME_SCREEN_SIZE_X - PLAYER_SIZE_X)
		{
			playerPosX += playerSpeed;
		}
		playerDir = DIR_RIGHT;
	}
	if (zKeyFlag && !zKeyFlagOld)
	{
		if (!pShotFlag)
		{
			pShotFlag = true;
			pShotPosX = playerPosX + (PLAYER_SIZE_X - PSHOT_SIZE_X) / 2;
			pShotPosY = playerPosY;
			PlaySoundMem(shotSound, DX_PLAYTYPE_BACK, true);
			playerDir = DIR_UP;
		}
	}
	/*if (CheckHitKey(KEY_INPUT_UP))
	{
		if (playerPosY - playerSpeed >= 0)
		{
			playerPosY -= playerSpeed;
		}
		playerDir = 3;
	}*/

	/*if (CheckHitKey(KEY_INPUT_DOWN))
	{
		if (playerPosY + playerSpeed <= GAME_SCREEN_SIZE_Y - PLAYER_SIZE_Y)
		{
			playerPosY += playerSpeed;
		}
		playerDir = 0;
	}*/
}

void EnemyCtl(void)
{
	//移動していいタイミングかのチェック
	enemyMoveCount++;
	if ((enemyMoveCount % 4) != 0)
	{
		return;
	}

	//移動制御
	if (enemyMoveMode == MOVE_MODE_LR)
	{
		if (enemyMoveLineX < ENEMY_COUNT_X)
		{
			for (int y = 0; y < ENEMY_COUNT_Y; y++)
			{
				//for (int x = 0; x < ENEMY_COUNT_X; x++)*/
				{
					if (enemyMoveFlagX == MOVE_RIGHT)
					{
						enemyPosX[y][(ENEMY_COUNT_X - 1) - enemyMoveLineX] += enemySpeed;
						enemyDir = DIR_RIGHT;
					}
					else
					{
						enemyPosX[y][enemyMoveLineX] -= enemySpeed;
						enemyDir = DIR_LEFT;
					}
				}
				
			}
		}
		enemyMoveLineX++;
		if (enemyMoveLineX > ENEMY_COUNT_X)
		{
			enemyMoveLineX = 0;
		}
		else
		{
			return;
		}
	}
	else if (enemyMoveMode == MOVE_MODE_DOWN)
	{
		if (enemyMoveLineY < ENEMY_COUNT_Y)
		{
			for (int x = 0; x < ENEMY_COUNT_X; x++)
			{
				enemyPosY[(ENEMY_COUNT_Y - 1) - enemyMoveLineY][x] += GET_ENEMY_POS_Y(1);
			}
			enemyDir = DIR_DOWN;
		}
		enemyMoveLineY++;
		if (enemyMoveLineY >= ENEMY_COUNT_Y + 10)
		{
			enemyMoveLineY = 0;
			enemyMoveMode = MOVE_MODE_LR;
		}
	}

	//方向制御
	for (int y = 0; y < ENEMY_COUNT_Y; y++)
	{
		for (int x = 0; x < ENEMY_COUNT_X; x++)
		{
			if (enemyMoveFlagX == MOVE_RIGHT)
			{
				if (enemyPosX[y][x] >= GAME_SCREEN_SIZE_X - ENEMY_SIZE_X)
				{
					enemyMoveFlagX = MOVE_LEFT;
					enemyMoveMode = MOVE_MODE_DOWN;
					goto DIR_CHECK_END;
				}
			}
			else if (enemyMoveFlagX == MOVE_LEFT)
			{
				if (enemyPosX[y][x] <= 0)
				{
					enemyMoveFlagX = MOVE_RIGHT;
					enemyMoveMode = MOVE_MODE_DOWN;
					goto DIR_CHECK_END;
				}
			}
			else
			{
				TRACE("enemyModeFlagXが異常値\n");
			}
		}
	}
DIR_CHECK_END:
	for (int x = 0; x < ENEMY_COUNT_X; x++)
	{
		if (!eShotFlag[x])
		{
			for (int y = ENEMY_COUNT_Y - 1; y >= 0; y--)
			{
				if (enemyFlag[y][x] != ENEMY_TYPE_NON)
				{
					if (rand() % (60 / 4) == 0)
					{
						eShotPosX[x] = enemyPosX[y][x] + (ENEMY_SIZE_X - ESHOT_SIZE_X) / 2;
						eShotPosY[x] = enemyPosY[y][x] + (ENEMY_SIZE_Y - ESHOT_SIZE_Y);
						eShotFlag[x] = true;
						PlaySoundMem(shotSound, DX_PLAYTYPE_BACK, true);
						enemyDir = DIR_DOWN;
					}
					break;
				}
			}
		}
	}
}

void GhostCtl(void)
{

	//お化けの移動処理
	if (ghostFlag)
	{
		ghostPosX += ghostSpeed;
		if (ghostPosX >= GAME_SCREEN_SIZE_X - GHOST_SIZE_X)
		{
			StopSoundMem(ghostSound);
			ghostFlag = false;
		}
		ghostDir = DIR_RIGHT;
	}

	if (!ghostFlag)
	{
		if (rand() % 10000 == 0)
		{
			ghostPosX = 0;
			ghostPosY = 0;
			PlaySoundMem(ghostSound, DX_PLAYTYPE_BACK, true);
			ghostFlag = true;
		}
	}
}


void PshotCtl(void)
{
	if (!pShotFlag)
	{
		return;
	}

	if (pShotPosY <= 0)
	{
		pShotFlag = false;
		effectPosX = pShotPosX - (EFFECT_SIZE_X - PSHOT_SIZE_X) / 2;
		effectPosY = pShotPosY - (EFFECT_SIZE_Y - PSHOT_SIZE_Y) / 2;
		effectCnt = 10;
		return;
	}
	pShotPosY -= pShotSpeed;
}

void EshotCtl(void)
{
	for (int x = 0; x < ENEMY_COUNT_X; x++)
	{
		if (eShotFlag[x])
		{
			if (eShotPosY[x] + eShotSpeed + ESHOT_SIZE_Y < GAME_SCREEN_SIZE_Y)
			{
				eShotPosY[x] += eShotSpeed;
			}
			else
			{
				eShotFlag[x] = false;
			}
		}
	}
}


void GetKeyState(void)
{
	zKeyFlagOld = zKeyFlag;
	zKeyFlag = CheckHitKey(KEY_INPUT_Z);

	spaceKeyFlagOld = spaceKeyFlag;
	spaceKeyFlag = CheckHitKey(KEY_INPUT_SPACE);
}



void GameDraw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();

	DrawBox(GAME_SCREEN_X,
		GAME_SCREEN_Y,
		GAME_SCREEN_SIZE_X + GAME_SCREEN_X,
		GAME_SCREEN_SIZE_Y + GAME_SCREEN_Y,
		0x323232, true);

	if (playerFlag)
	{
		DrawGraph(
			playerPosX + GAME_SCREEN_X,
			playerPosY + GAME_SCREEN_Y,
			playerImage[playerDir][playerAnimData[(playerAnimCnt / 10) % 4]], true);
	}

	for (int y = 0; y < ENEMY_COUNT_Y; y++)
	{
		for (int x = 0; x < ENEMY_COUNT_X; x++)
		{
			if (enemyFlag[y][x] != ENEMY_TYPE_NON)
			{
				DrawGraph(
					GAME_SCREEN_X + enemyPosX[y][x],
					GAME_SCREEN_Y + enemyPosY[y][x],
					enemyImage[enemyFlag[y][x]][enemyDir][enemyAnimData[(enemyAnimCnt / 10) % 4]],
					true);
			}
		}
	}

	if (ghostFlag)
	{
		DrawGraph(
			ghostPosX + GAME_SCREEN_X,
			ghostPosY + GAME_SCREEN_Y,
			ghostImage[ghostDir][ghostAnimData[(ghostAnimCnt / 10) % 4]],true);
	}

	if (pShotFlag)
	{
		DrawGraph(GAME_SCREEN_X + pShotPosX, GAME_SCREEN_Y + pShotPosY, pShotImage, true);
	}

	for (int x = 0; x < ENEMY_COUNT_X; x++)
	{
		if (eShotFlag[x])
		{
			DrawGraph(GAME_SCREEN_X + eShotPosX[x], GAME_SCREEN_Y + eShotPosY[x], eShotImage, true);
		}
	}

	if (effectCnt > 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		DrawGraph(GAME_SCREEN_X + effectPosX, GAME_SCREEN_Y + effectPosY, effectImage, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		effectCnt--;
	}

	for (int j = 0; j < playerLife - 1; j++)
	{
		DrawGraph(GAME_SCREEN_SIZE_X + GAME_SCREEN_X + PLAYER_SIZE_X * j,
			GAME_SCREEN_SIZE_Y + GAME_SCREEN_Y - PLAYER_SIZE_Y * 2, playerImage[DIR_DOWN][1], true);
	}

	DrawString(GAME_SCREEN_SIZE_X + GAME_SCREEN_X,
		GAME_SCREEN_SIZE_Y + GAME_SCREEN_Y - PLAYER_SIZE_Y * 3,
		"ＬＩＦＥ", 0xffffff);

	DrawString(310, 5, "操作方法", 0xffffff);
	DrawString(200, 40, "←→　左右移動　　　Ｚ　弾を発射", 0xffffff);

	playerAnimCnt++;
	enemyAnimCnt++;
	ghostAnimCnt++;
	DrawTexts();
	ScreenFlip();
}

void DrawTexts(void)
{
	int textColor = GetColor(255, 255, 255);
	//ハイスコア
	DrawString(GAME_SCREEN_X + GAME_SCREEN_SIZE_X, 70, "ＨＩＧＨ", textColor);
	int drawScr;

	/*if(scrCnt[SCR_HIGH] < 100000)
	{
		drawScr = scrCnt[SCR_HIGH];
	}
	else
	{
		drawScr = 99999;
	}*/

	//三項演算子
	drawScr = (scrCnt[SCR_HIGH] < 100000 ? scrCnt[SCR_HIGH] : 99999);
	int drawLength = GetDrawFormatStringWidth("%d", drawScr);
	DrawFormatString(GAME_SCREEN_X + GAME_SCREEN_SIZE_X + 100 - drawLength, 120, textColor, "%d", scrCnt[SCR_HIGH]);
	DrawString(GAME_SCREEN_X + GAME_SCREEN_SIZE_X + 17, 90, "ＳＣＯＲＥ", textColor);

	//PL1
	DrawString(GAME_SCREEN_X + GAME_SCREEN_SIZE_X, 170, "１ＵＰ", textColor);
	drawLength = GetDrawFormatStringWidth("%d", scrCnt[SCR_PL1]);
	DrawFormatString(GAME_SCREEN_X + GAME_SCREEN_SIZE_X + 100 - drawLength, 200, textColor, "%d", scrCnt[SCR_PL1]);
}

void CheckHitObj(void)
{
	//自機の弾と敵
	if (!pShotFlag)
	{
		goto CHECK_PL_ES;
	}

	for (int y = 0; y < ENEMY_COUNT_Y; y++)
	{
		for (int x = 0; x < ENEMY_COUNT_X; x++)
		{
			if (enemyFlag[y][x] != ENEMY_TYPE_NON)
			{
				if (pShotPosX < enemyPosX[y][x] + ENEMY_SIZE_X &&
					enemyPosX[y][x] < pShotPosX + PSHOT_SIZE_X &&
					pShotPosY < enemyPosY[y][x] + ENEMY_SIZE_Y &&
					enemyPosY[y][x] < pShotPosY + PSHOT_SIZE_Y)
				{
					pShotFlag = false;
					enemyFlag[y][x] = ENEMY_TYPE_NON;
					effectCnt = 30;
					effectPosX = enemyPosX[y][x] - (EFFECT_SIZE_X - ENEMY_SIZE_X) / 2;
					effectPosY = enemyPosY[y][x] - (EFFECT_SIZE_Y - ENEMY_SIZE_Y) / 2;
					AddScr(100);
					PlaySoundMem(enemyDeathSound, DX_PLAYTYPE_BACK, true);
					goto CHECK_PL_ES;
				}
			}
		}
	}

CHECK_PL_ES:

	//敵の弾と自機
	for (int x = 0; x < ENEMY_COUNT_X; x++)
	{
		if (eShotFlag[x])
		{
			if (playerPosX < eShotPosX[x] + ESHOT_SIZE_X &&
				eShotPosX[x] < playerPosX + PLAYER_SIZE_X &&
				playerPosY < eShotPosY[x] + ESHOT_SIZE_Y &&
				eShotPosY[x] < playerPosY + PLAYER_SIZE_Y)
			{
				PlaySoundMem(enemyDeathSound, DX_PLAYTYPE_BACK, true);
				PlayerDeathProc(false);
			}
		}
	}

	if (enemyMoveMode == MOVE_MODE_LR)
	{
		//敵が最終ラインに到達しているか
		for (int y = 0; y < ENEMY_COUNT_Y; y++)
		{
			for (int x = 0; x < ENEMY_COUNT_X; x++)
			{
				if (enemyFlag[y][x] != ENEMY_TYPE_NON)
				{
					if (enemyPosY[y][x] >= GET_ENEMY_POS_Y(ENEMY_COUNT_Y + 3))
					{
						PlayerDeathProc(false);
					}
				}
			}
		}
	}

	//自機の弾とお化け
	if (ghostFlag)
	{
		if (pShotPosX < ghostPosX + GHOST_SIZE_X &&
			ghostPosX < pShotPosX + PSHOT_SIZE_X &&
			pShotPosY < ghostPosY + GHOST_SIZE_Y &&
			ghostPosY < pShotPosY + PSHOT_SIZE_Y)
		{
			PlaySoundMem(ghostDeathSound, DX_PLAYTYPE_BACK, true);
			pShotFlag = false;
			ghostFlag = false;
			effectCnt = 30;
			effectPosX = ghostPosX - (EFFECT_SIZE_X - GHOST_SIZE_X) / 2;
			effectPosY = ghostPosY - (EFFECT_SIZE_Y - GHOST_SIZE_Y) / 2;
			AddScr(1000);
		}
	}
}

void AddScr(int num)
{
	scrCnt[SCR_PL1] += num;
	if (scrCnt[SCR_PL1] > scrCnt[SCR_HIGH])
	{
		scrCnt[SCR_HIGH] = scrCnt[SCR_PL1];
	}
}

void PlayerDeathProc(bool backFlag)
{
	PlaySoundMem(playerSound, DX_PLAYTYPE_BACK, true);
	playerFlag = false;
	playerLife--;
	if (playerLife <= 0)
	{
		scnID = SCN_ID_GAMEOVER;
	}

	if (backFlag)
	{
		for (int y = 0; y < ENEMY_COUNT_Y; y++)
		{
			for (int x = 0; x < ENEMY_COUNT_X; x++)
			{
				enemyPosY[y][x] -= GET_ENEMY_POS_Y(2);
			}
		}
	}
	for (int x = 0; x < ENEMY_COUNT_X; x++)
	{
		eShotFlag[x] = false;
	}
}

bool SysInit(void)
{
	srand(time(NULL));
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);
	ChangeWindowMode(true);
	SetWindowText("2216008 寺師 遼");
	if (DxLib_Init() == -1)
	{
		TRACE("DxLibの初期化失敗\n");
		return false;
	}

	_dbgSetup(SCREEN_SIZE_X, SCREEN_SIZE_Y, 255);

	titleImage = LoadGraph("image/TitleLogo2.png");
	gameOverImage = LoadGraph("image/GameOver.png");

	//playerImage = LoadGraph("image/Female 05-2.png");
	LoadDivGraph("image/Female 05-2.png",
		ANIM_MAX * DIR_MAX,
		ANIM_MAX, DIR_MAX,
		PLAYER_SIZE_X, PLAYER_SIZE_Y, &playerImage[0][0]);

	const char* enemyFileName[ENEMY_TYPE_MAX] = {
		"image/Enemy 15-3.png",
		"image/Enemy 15-2.png",
		"image/Enemy 15-4.png",
		"image/Enemy 15-5.png",
		"image/Enemy 15-6.png",
		"image/Enemy 15-1.png",
		"image/Enemy 09-1.png"
	};

	for (int j = 0; j < ENEMY_TYPE_MAX; j++)
	{
		if (LoadDivGraph(
			enemyFileName[j],
			ANIM_MAX * DIR_MAX,
			ANIM_MAX,
			DIR_MAX,
			ENEMY_SIZE_X, ENEMY_SIZE_Y,
			&enemyImage[j][0][0]) == -1)
		{
			TRACE("ファイルの読み込みエラー:%s\n", enemyFileName[j]);
			break;
		}
	}

	LoadDivGraph("image/Enemy 09-1.png",
		ANIM_MAX * DIR_MAX,
		ANIM_MAX, DIR_MAX,
		GHOST_SIZE_X, GHOST_SIZE_Y, &ghostImage[0][0]);

	pShotImage = LoadGraph("image/pshot.png");
	if (pShotImage == -1)
	{
		return false;
	}

	eShotImage = LoadGraph("image/eshot.png");
	if (eShotImage == -1)
	{
		return false;
	}

	effectImage = LoadGraph("image/baku.png");
	if (effectImage == -1)
	{
		return false;
	}

	bgmSound = LoadSoundMem("sound/bgm.ogg");
	if (bgmSound == -1)
	{
		return false;
	}

	enemyDeathSound = LoadSoundMem("sound/enemyDeath.ogg");
	if (enemyDeathSound == -1)
	{
		return false;
	}
		
	ghostSound = LoadSoundMem("sound/ghost.ogg");
	if (ghostSound == -1)
	{
		return false;
	}

	shotSound = LoadSoundMem("sound/shot.ogg");
	if (shotSound == -1)
	{
		return false;
	}

	ghostDeathSound = LoadSoundMem("sound/ghostDeath.ogg");
	if (ghostDeathSound == -1)
	{
		return false;
	}

	playerSound = LoadSoundMem("sound/player.ogg");
	if (playerSound == -1)
	{
		return false;
	}

	zKeyFlagOld = true;
	zKeyFlag = false;

	scrCnt[SCR_HIGH] = HIGH_SCR_DEF;

	return true;
}

bool GameInit(void)
{
	scnID = SCN_ID_TITLE;

	//プレイヤー関連
	playerPosX = (GAME_SCREEN_SIZE_X - PLAYER_SIZE_X) / 2;	//X座標
	playerPosY = GAME_SCREEN_SIZE_Y - PLAYER_SIZE_Y;		//Y座標
	playerSpeed = 2;										//移動量
	playerAnimCnt = 0;										//アニメーションカウンター
	playerDir = 0;										//向き
	playerAnimData[0] = 0;
	playerAnimData[1] = 1;
	playerAnimData[2] = 2;
	playerAnimData[3] = 1;
	playerFlag = true;
	playerLife = 3;

	//敵関連
	ENEMY_TYPE tmpID = ENEMY_TYPE_RED;
	for (int y = 0; y < ENEMY_COUNT_Y; y++)
	{
		for (int x = 0; x < ENEMY_COUNT_X; x++)
		{
			enemyPosX[y][x] = ((ENEMY_SIZE_X * 12) / 8) * x;
			enemyPosY[y][x] = GET_ENEMY_POS_Y(y + 1);
			enemyFlag[y][x] = tmpID;
		}
		tmpID = (ENEMY_TYPE)(tmpID + 1);
		if (tmpID >= ENEMY_TYPE_UFO)
		{
			tmpID = ENEMY_TYPE_RED;
		}
	}

	enemySpeed = ENEMY_SPEED;
	enemyMoveFlagX = MOVE_RIGHT;
	enemyMoveMode = MOVE_MODE_LR;
	enemyMoveLineX = 0;
	enemyMoveLineY = 0;
	enemyMoveCount = 0;
	enemyAnimCnt = 0;
	enemyDir = 0;
	enemyAnimData[0] = 0;
	enemyAnimData[1] = 1;
	enemyAnimData[2] = 2;
	enemyAnimData[3] = 1;

	//弾関連
	pShotPosX = playerPosX;		//X座標
	pShotPosY = playerPosY;		//Y座標
	pShotFlag = false;
	pShotSpeed = PSHOT_SPEED;		//移動量

	for (int x = 0; x < ENEMY_COUNT_X; x++)
	{
		eShotPosX[x] = 0;							//X座標
		eShotPosY[x] = 0;							//Y座標
		eShotFlag[x] = false;
	}
	eShotSpeed = 5;								//移動量
	
	//お化け関連
	 ghostPosX = 0;
	 ghostPosY = 0;
	 ghostSpeed = GHOST_SPEED;
	 ghostFlag = false;
	 ghostAnimCnt = 0;
	 ghostDir = 0;
	 ghostAnimData[0] = 0;
	 ghostAnimData[1] = 1;
	 ghostAnimData[2] = 2;
	 ghostAnimData[3] = 1;

	//エフェクト
	effectPosX = 0;
	effectPosY = 0;
	effectCnt = 0;

	scrCnt[SCR_PL1] = 0;

	return true;
}


