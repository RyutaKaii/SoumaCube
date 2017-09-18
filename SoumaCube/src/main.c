// ---------------------------------
// ソーマキューブを解くプログラムです。
// 結果はコンソール出力します。
// 鏡像・回転は全て別物として扱います。
// PCスペックにもよりますが、処理終了まで開始から約1時間弱かかります。
// ---------------------------------
#include<stdio.h>
#include<string.h>
#include <stdlib.h>



// ---------------------------------
// 定数
// ---------------------------------
// デバッグモード
const int IS_DEBUG = 1;

// パズルの種類
// パズル1
const int NUM_PAZURU_1 = 0;
// パズル2
const int NUM_PAZURU_2 = 1;
// パズル3
const int NUM_PAZURU_3 = 2;
// パズル4
const int NUM_PAZURU_4 = 3;
// パズル5
const int NUM_PAZURU_5 = 4;
// パズル6
const int NUM_PAZURU_6 = 5;
// パズル7
const int NUM_PAZURU_7 = 6;

// 向き
// 上
const int NUM_MUKI_UE = 0;
// 右
const int NUM_MUKI_MIGI = 1;
// 下
const int NUM_MUKI_SITA = 2;
// 左
const int NUM_MUKI_HIDARI = 3;
// 前
const int NUM_MUKI_MAE = 4;
// 後
const int NUM_MUKI_USIRO = 5;

// 回転
// 1
const int NUM_KAITEN_1 = 0;
// 2
const int NUM_KAITEN_2 = 1;
// 3
const int NUM_KAITEN_3 = 2;
// 4
const int NUM_KAITEN_4 = 3;

// 空間数
const int NUM_SPACE = 27;
// 一辺の長さ
const int WIDTH = 3;
// パズル数
const int NUM_PAZURU = 7;
// 向きの数
const int NUM_MUKI = 6;
// パズル3以外の回転の数
const int NUM_KAITEN_NOT_PAZURU3 = 4;
// パズル3の回転の数
const int NUM_KAITEN_PAZURU3 = 2;

// ピース数
// パズル1のピース数
const int NUM_PIECE_PAZURU1 = 3;
// パズル1以外のピース数
const int NUM_PIECE_NOT_PAZURU1 = 4;

// 初期化数
const int INIT_NUM = -999;
// 仮完成パズル数
const int TMP_NUM = 500;



// ---------------------------------
// 構造体
// ---------------------------------
// XYZ座標(1ブロック)
typedef struct {
	// X座標
	int x;
	// Y座標
	int y;
	// Z座標
	int z;
}XYZ;

// 空間
typedef struct {
	// ポジション
	XYZ position;
}SPACE;



// ---------------------------------
// グローバル変数
// ---------------------------------
// 成功数
int successNum = 0;
// デバッグ用カウント変数
int debugNum = 0;



// ---------------------------------
// プロトタイプ宣言
// ---------------------------------
void initSpace(SPACE* space);
void initUsedPieces1(XYZ usedPieces[][3]);
void initUsedPieces2(XYZ usedPieces[][4]);
int trySetOrder(SPACE* space, int currentPazuruNum);
int isUsed1(XYZ* setPieces, XYZ usedPieces[][3]);
int isUsed2(XYZ* setPieces, XYZ usedPieces[][4]);
int isExistBlock(int x, int y, int z, SPACE* space);
int trySetSpace(XYZ* setPieces, SPACE* space, int x, int y, int z, int muki, int currentPazuruNum, int kaiten);
void createSetPieces(XYZ* setPieces, SPACE* space, int x, int y, int z, int muki, int pieceNum, int currentPazuruNum, int kaiten);
void doMukiKaiten(XYZ* xyz, int pieceNum, int currentPazuruNum, int muki, int kaiten);
void heikouidou(XYZ* xyz, int x, int y, int z, int num);
void setXyzPazuru1(XYZ* xyz);
void setXyzPazuru2(XYZ* xyz);
void setXyzPazuru3(XYZ* xyz);
void setXyzPazuru4(XYZ* xyz);
void setXyzPazuru5(XYZ* xyz);
void setXyzPazuru6(XYZ* xyz);
void setXyzPazuru7(XYZ* xyz);
int getPieceNum(int currentPazuruNum);
void doKaitenMigi(XYZ* xyz, int pieceNum);
void doKaitenSita(XYZ* xyz, int pieceNum);
void doKaitenHidari(XYZ* xyz, int pieceNum);
void doKaitenMae(XYZ* xyz, int pieceNum);
void doKaitenUsiro(XYZ* xyz, int pieceNum);
void doKaitenUe(XYZ* xyz, int pieceNum);
void doYMigikaiten(XYZ* xyz, int pieceNum);
void doXHidarikaiten(XYZ* xyz, int pieceNum);
void doZMigikaiten(XYZ* xyz, int pieceNum);
int isHamidasi(XYZ* xyz, int currentPazuruNum);
int isChoufuku(XYZ* xyz, SPACE* space, int currentPazuruNum);
void doSetPieces(XYZ* xyz, SPACE* space, int currentPazuruNum);
void removeLastSetPazuru(SPACE* space, int currentPazuruNum);
void printDebug(SPACE* space, int currentPazuruNum);



// ---------------------------------
// 空間の初期化
// ---------------------------------
void initSpace(SPACE* space) {
	for (int num = 0; num < NUM_SPACE; num++) {
		for (int x = 0; x < WIDTH; x++) {
			for (int y = 0; y < WIDTH; y++) {
				for (int z = 0; z < WIDTH; z++) {
					space[num].position.x = INIT_NUM;
					space[num].position.y = INIT_NUM;
					space[num].position.z = INIT_NUM;
				}
			}
		}
	}
}

// ---------------------------------
// パズルをはめることを試す
// 空間数×向き×回転×パズルの7乗の組み合わせを試す
// = ( (3 × 3 × 3) × 6 × 4 ) ^7 = 648^7 通り
// 再帰呼び出しを利用しパズル1から7まで順番に呼び出す
// ---------------------------------
int trySetOrder(SPACE* space, int currentPazuruNum) {
	// 試行するピース
	XYZ setPieces[getPieceNum(currentPazuruNum)];

	// はめたピース
	XYZ usedPieces[TMP_NUM][getPieceNum(currentPazuruNum)];

	if (currentPazuruNum == NUM_PAZURU_1) {
		initUsedPieces1(usedPieces);
	} else {
		initUsedPieces2(usedPieces);
	}

	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < WIDTH; y++) {
			for (int z = 0; z < WIDTH; z++) {
				// 鏡像体を取り除くため、
				// パズル1のみ、(0, 0, 0)(1, 0, 0)(1, 1, 0)(1, 1, 1)の場合だけ実行する
				if (currentPazuruNum == NUM_PAZURU_1) {
					if (!((x == 0 && y == 0 && z == 0)
							|| (x == 1 && y == 0 && z == 0)
							|| (x == 1 && y == 0 && z == 1)
							|| (x == 1 && y == 1 && z == 1))) {
						break;
					}
				}

				// 処理短縮のため、ベースとなる空間にすでにブロックが存在する場合、処理をスキップする
				// XYZのベース(x = 0, y = 0, z = 0)には必ずブロックが存在することを利用する
				if(isExistBlock(x, y, z, space)) {
					break;
				}

				for (int muki = 0; muki < NUM_MUKI; muki++) {
					int kaitenNum = 0;
					if (currentPazuruNum == NUM_PAZURU_3) {
						// パズル3のみ線対称なため回転数を2とする
						kaitenNum = NUM_KAITEN_PAZURU3;
					} else {
						kaitenNum = NUM_KAITEN_NOT_PAZURU3;
					}

					for (int kaiten = 0; kaiten < kaitenNum; kaiten++) {
						if (trySetSpace(setPieces, space, x, y, z, muki, currentPazuruNum, kaiten)) {
							// 平行移動・向き・向き固定回転した結果、全く同じパズルが生成された場合は除外する
							int result = 0;
							if (currentPazuruNum == NUM_PAZURU_1) {
								result = isUsed1(setPieces, usedPieces);
							} else {
								result = isUsed2(setPieces, usedPieces);
							}

							if (!result) {
								if (currentPazuruNum == NUM_PAZURU_7) {
									// 全てのパズルをはめることに成功した場合
									successNum++;

									printf("success!! num: %d\n", successNum);
									printDebug(space, currentPazuruNum);
									fflush(stdout);

									// 最後のパズルをはめることに成功した場合ははめたパズルを空間から取り外し、前のパズルに戻る
									removeLastSetPazuru(space, currentPazuruNum);
									return 0;
								} else {
									// 再帰処理 次のパズルをはめることを試す
									int nextPazuruNum = currentPazuruNum + 1;
									trySetOrder(space, nextPazuruNum);
								}

								// はめたパズルを空間から取り外す
								removeLastSetPazuru(space, currentPazuruNum);
							}
						}
					}
				}
			}
		}
	}

	return 0;
}


void initUsedPieces1(XYZ usedPieces[][3]) {
	for (int usedPiecesNum = 0; usedPiecesNum < TMP_NUM; usedPiecesNum++) {
		for (int num = 0; num < 3; num++) {
			usedPieces[usedPiecesNum][num].x = INIT_NUM;
			usedPieces[usedPiecesNum][num].y = INIT_NUM;
			usedPieces[usedPiecesNum][num].z = INIT_NUM;
		}
	}
}

void initUsedPieces2(XYZ usedPieces[][4]) {
	for (int usedPiecesNum = 0; usedPiecesNum < TMP_NUM; usedPiecesNum++) {
		for (int num = 0; num < 4; num++) {
			usedPieces[usedPiecesNum][num].x = INIT_NUM;
			usedPieces[usedPiecesNum][num].y = INIT_NUM;
			usedPieces[usedPiecesNum][num].z = INIT_NUM;
		}
	}
}

int isUsed1(XYZ* setPieces, XYZ usedPieces[][3]) {
	for (int usedPiecesNum = 0; usedPiecesNum < TMP_NUM; usedPiecesNum++) {
		int count = 0;

		for (int num = 0; num < 3; num++) {
			for (int num2 = 0; num2 < 3; num2++) {
				if (usedPieces[usedPiecesNum][num].x == setPieces[num2].x
						&& usedPieces[usedPiecesNum][num].y == setPieces[num2].y
						&& usedPieces[usedPiecesNum][num2].z
								== setPieces[num].z) {
					count++;
				}
			}

		}

		if (count == 3) {
			return 1;
		}

	}
	return 0;
}

int isUsed2(XYZ* setPieces, XYZ usedPieces[][4]) {
	for (int usedPiecesNum = 0; usedPiecesNum < TMP_NUM; usedPiecesNum++) {
		int count = 0;

		for (int num = 0; num < 4; num++) {
			for (int num2 = 0; num2 < 4; num2++) {
				if (usedPieces[usedPiecesNum][num].x == setPieces[num2].x
						&& usedPieces[usedPiecesNum][num].y == setPieces[num2].y
						&& usedPieces[usedPiecesNum][num2].z
								== setPieces[num].z) {
					count++;
				}
			}

		}

		if (count == 4) {
			return 1;
		}

	}
	return 0;
}

// ---------------------------------
// ベースとなる空間にすでにブロックが存在するか確認する
// XYZのベース(x = 0, y = 0, z = 0)には必ずブロックが存在することを利用する
// 存在する: 1, 存在しない: 0
// ---------------------------------
int isExistBlock(int x, int y, int z, SPACE* space) {
	for (int num = 0; num < NUM_SPACE; num++) {
		if (space[num].position.x == x && space[num].position.y == y
				&& space[num].position.z == z) {
			return 1;
		}
	}

	return 0;
}

// ---------------------------------
// パズルを空間にはめる
// パズルを生成して向き回転・向き固定回転・平行移動させて空間にはめてみる
// 成功: 1, 失敗: 0
// ---------------------------------
int trySetSpace(XYZ* setPieces, SPACE* space, int x, int y, int z, int muki, int currentPazuruNum, int kaiten) {
	createSetPieces(setPieces, space, x, y, z, muki, getPieceNum(currentPazuruNum), currentPazuruNum, kaiten);

	if (!isHamidasi(setPieces, currentPazuruNum) && !isChoufuku(setPieces, space, currentPazuruNum)) {
		doSetPieces(setPieces, space, currentPazuruNum);
		return 1;
	}

	return 0;
}

// ---------------------------------
// はめこむピースを生成する
// ---------------------------------
void createSetPieces(XYZ* setPieces, SPACE* space, int x, int y, int z,
		int muki, int pieceNum, int currentPazuruNum, int kaiten) {
	// 基本座標設定のための関数ポインタ
	void (*setXyzPazuru[])(XYZ*) = {setXyzPazuru1, setXyzPazuru2, setXyzPazuru3, setXyzPazuru4, setXyzPazuru5, setXyzPazuru6, setXyzPazuru7
	};
	// 向き変更のための関数ポインタ
	void (*doKaiten[])(XYZ*,
			int) = {doKaitenMigi, doKaitenSita, doKaitenHidari, doKaitenMae, doKaitenUsiro, doKaitenUe
	};

	// パズル種別に合わせて空間座標を設定
	(*setXyzPazuru[currentPazuruNum])(setPieces);

	// 向き種別に合わせて向き回転
	(*doKaiten[muki])(setPieces, pieceNum);

	// 回転種別に合わせて向き固定回転
	doMukiKaiten(setPieces, pieceNum, currentPazuruNum, muki, kaiten);

	// xyzに合わせて平行移動
	heikouidou(setPieces, x, y, z, pieceNum);
}

// ---------------------------------
// 向きを固定したまま回転する
// 向きによって回転軸を決定する
// ---------------------------------
void doMukiKaiten(XYZ* xyz, int pieceNum, int currentPazuruNum, int muki, int kaiten) {
	switch (muki) {
	case 0:// NUM_MUKI_UE
	case 2:// NUM_MUKI_SITA
		for (int num = 0;num < kaiten;num++) {
			doZMigikaiten(xyz, pieceNum);
		}
		break;
	case 1:// NUM_MUKI_UE
	case 3:// NUM_MUKI_SITA
		for (int num = 0;num < kaiten;num++) {
			doXHidarikaiten(xyz, pieceNum);
		}
		break;
	case 4:// NUM_MUKI_UE
	case 5:// NUM_MUKI_SITA
		for (int num = 0;num < kaiten;num++) {
			doYMigikaiten(xyz, pieceNum);
		}
		break;
	default:
		printf("error method: doMukiKaiten----------\n");
	}
}

// ---------------------------------
// xyzを元に平行移動する
// ---------------------------------
void heikouidou(XYZ* xyz, int x, int y, int z, int num) {
	for (int i = 0;i < num; i++) {
		xyz[i].x += x;
		xyz[i].y += y;
		xyz[i].z += z;
	}
}

// ---------------------------------
// パズル1の基本座標を設定
// ---------------------------------
void setXyzPazuru1(XYZ* xyz) {
	xyz[0].x = 0;
	xyz[0].y = 0;
	xyz[0].z = 0;

	xyz[1].x = 1;
	xyz[1].y = 0;
	xyz[1].z = 0;

	xyz[2].x = 0;
	xyz[2].y = 0;
	xyz[2].z = 1;
}

// ---------------------------------
// パズル2の基本座標を設定
// ---------------------------------
void setXyzPazuru2(XYZ* xyz) {
	xyz[0].x = 0;
	xyz[0].y = 0;
	xyz[0].z = 0;

	xyz[1].x = 1;
	xyz[1].y = 0;
	xyz[1].z = 0;

	xyz[2].x = 2;
	xyz[2].y = 0;
	xyz[2].z = 0;

	xyz[3].x = 0;
	xyz[3].y = 0;
	xyz[3].z = 1;
}

// ---------------------------------
// パズル3の基本座標を設定
// ---------------------------------
void setXyzPazuru3(XYZ* xyz) {
	xyz[0].x = 0;
	xyz[0].y = 0;
	xyz[0].z = 0;

	xyz[1].x = 1;
	xyz[1].y = 0;
	xyz[1].z = 0;

	xyz[2].x = 2;
	xyz[2].y = 0;
	xyz[2].z = 0;

	xyz[3].x = 1;
	xyz[3].y = 0;
	xyz[3].z = 1;
}

// ---------------------------------
// パズル4の基本座標を設定(Wikiの表示とは上下逆とする)
// ---------------------------------
void setXyzPazuru4(XYZ* xyz) {
	xyz[0].x = 0;
	xyz[0].y = 0;
	xyz[0].z = 0;

	xyz[1].x = 1;
	xyz[1].y = 0;
	xyz[1].z = 0;

	xyz[2].x = 1;
	xyz[2].y = 1;
	xyz[2].z = 0;

	xyz[3].x = 2;
	xyz[3].y = 1;
	xyz[3].z = 0;
}

// ---------------------------------
// パズル5の基本座標を設定
// ---------------------------------
void setXyzPazuru5(XYZ* xyz) {
	xyz[0].x = 0;
	xyz[0].y = 0;
	xyz[0].z = 0;

	xyz[1].x = 0;
	xyz[1].y = 0;
	xyz[1].z = 1;

	xyz[2].x = 0;
	xyz[2].y = 1;
	xyz[2].z = 0;

	xyz[3].x = 1;
	xyz[3].y = 1;
	xyz[3].z = 0;
}

// ---------------------------------
// パズル6の基本座標を設定
// ---------------------------------
void setXyzPazuru6(XYZ* xyz) {
	xyz[0].x = 0;
	xyz[0].y = 0;
	xyz[0].z = 0;

	xyz[1].x = 1;
	xyz[1].y = 0;
	xyz[1].z = 0;

	xyz[2].x = 0;
	xyz[2].y = 1;
	xyz[2].z = 0;

	xyz[3].x = 0;
	xyz[3].y = 1;
	xyz[3].z = 1;
}

// ---------------------------------
// パズル7の基本座標を設定
// ---------------------------------
void setXyzPazuru7(XYZ* xyz) {
	xyz[0].x = 0;
	xyz[0].y = 0;
	xyz[0].z = 0;

	xyz[1].x = 0;
	xyz[1].y = 1;
	xyz[1].z = 0;

	xyz[2].x = 1;
	xyz[2].y = 1;
	xyz[2].z = 0;

	xyz[3].x = 0;
	xyz[3].y = 1;
	xyz[3].z = 1;
}

// ---------------------------------
// ピース数を取得する
// ---------------------------------
int getPieceNum(int currentPazuruNum) {
	if (currentPazuruNum == NUM_PAZURU_1) {
		return NUM_PIECE_PAZURU1;
	}

	return NUM_PIECE_NOT_PAZURU1;
}

// ---------------------------------
// 上向きから右向きになるよう回転させる
// ---------------------------------
void doKaitenMigi(XYZ* xyz, int pieceNum) {
	doYMigikaiten(xyz, pieceNum);
}

// ---------------------------------
// 上向きから下向きになるよう回転させる
// ---------------------------------
void doKaitenSita(XYZ* xyz, int pieceNum) {
	doYMigikaiten(xyz, pieceNum);
	doYMigikaiten(xyz, pieceNum);
}

// ---------------------------------
// 上向きから左向きになるよう回転させる
// ---------------------------------
void doKaitenHidari(XYZ* xyz, int pieceNum) {
	doYMigikaiten(xyz, pieceNum);
	doYMigikaiten(xyz, pieceNum);
	doYMigikaiten(xyz, pieceNum);
}

// ---------------------------------
// 上向きから前向きになるよう回転させる
// ---------------------------------
void doKaitenMae(XYZ* xyz, int pieceNum) {
	doXHidarikaiten(xyz, pieceNum);
}

// ---------------------------------
// 上向きから後向きになるよう回転させる
// ---------------------------------
void doKaitenUsiro(XYZ* xyz, int pieceNum) {
	doXHidarikaiten(xyz, pieceNum);
	doXHidarikaiten(xyz, pieceNum);
	doXHidarikaiten(xyz, pieceNum);
}

// ---------------------------------
// 上向きから上向きになるよう回転させる
// ---------------------------------
void doKaitenUe(XYZ* xyz, int pieceNum) {
	// 何もしない
}

// ---------------------------------
// 向き: 上→右など
// y軸を軸に90度右回転
// x → -z, z → x
// ---------------------------------
void doYMigikaiten(XYZ* xyz, int pieceNum) {
	for (int num = 0; num < pieceNum; num++) {
		int tmpX = xyz[num].x;
		int tmpZ = xyz[num].z;

		xyz[num].x = tmpZ * -1;
		xyz[num].z = tmpX;
	}
}

// ---------------------------------
// 向き: 上→前など
// x軸を軸に90度左回転
// y → -z, z → y
// ---------------------------------
void doXHidarikaiten(XYZ* xyz, int pieceNum) {
	for (int num = 0; num < pieceNum; num++) {
		int tmpY = xyz[num].y;
		int tmpZ = xyz[num].z;

		xyz[num].y = tmpZ * -1;
		xyz[num].z = tmpY;
	}
}

// ---------------------------------
// 向き: 前→右など
// z軸を軸に90度右回転
// y → x, x → -y
// ---------------------------------
void doZMigikaiten(XYZ* xyz, int pieceNum) {
	for (int num = 0; num < pieceNum; num++) {
		int tmpY = xyz[num].y;
		int tmpX = xyz[num].x;

		xyz[num].y = tmpX;
		xyz[num].x = tmpY * -1;
	}
}


// ---------------------------------
// はみ出しチェック
// 各座標0～2を範囲内とする
// はみ出している: 1, はみ出していない: 0
// ---------------------------------
int isHamidasi(XYZ* xyz, int currentPazuruNum) {
	for (int i = 0; i < getPieceNum(currentPazuruNum); i++) {
		if ((xyz[i].x < 0 || xyz[i].x > (WIDTH - 1))
				|| (xyz[i].y < 0 || xyz[i].y > (WIDTH - 1))
				|| (xyz[i].z < 0 || xyz[i].z > (WIDTH - 1))) {
			return 1;
		}
	}

	return 0;
}

// ---------------------------------
// 重複チェック
// 重複している: 1, 重複していない: 0
// ---------------------------------
int isChoufuku(XYZ* xyz, SPACE* space, int currentPazuruNum) {
	for (int spaceNum = 0;spaceNum < NUM_SPACE; spaceNum++) {
		for (int i = 0;i < getPieceNum(currentPazuruNum); i++) {
			if (space[spaceNum].position.x == xyz[i].x &&
					space[spaceNum].position.y == xyz[i].y &&
					space[spaceNum].position.z == xyz[i].z) {
				return 1;
			}
		}
	}

	return 0;
}

// ---------------------------------
// 空間にパズルをはめる
// ---------------------------------
void doSetPieces(XYZ* xyz, SPACE* space, int currentPazuruNum) {
	// パズルをはめる空間の位置を決定
	// パズル1は0,その他のパズルは算出する
	int setPosition = 0;
	if (currentPazuruNum != NUM_PAZURU_1) {
		setPosition += NUM_PIECE_PAZURU1 + (currentPazuruNum - 1) * NUM_PIECE_NOT_PAZURU1;
	}

	for (int i = 0;i < getPieceNum(currentPazuruNum); i++) {
		// パズルをはめる位置からピースの数分、生成したピースを設定する
		space[setPosition + i].position.x = xyz[i].x;
		space[setPosition + i].position.y = xyz[i].y;
		space[setPosition + i].position.z = xyz[i].z;
	}
}

// ---------------------------------
// 空間から最後にはめたパズルを取り除く
// ---------------------------------
void removeLastSetPazuru(SPACE* space, int currentPazuruNum) {
	// パズルを取り除く空間の位置を決定
	// パズル1は0,その他のパズルは算出する
	int setPosition = 0;
	if (currentPazuruNum != NUM_PAZURU_1) {
		setPosition = NUM_PIECE_PAZURU1
				+ (currentPazuruNum - 1) * NUM_PIECE_NOT_PAZURU1;
	}

	for (int i = 0; i < getPieceNum(currentPazuruNum); i++) {
		// パズルを取り除く位置からピースの数分、初期化する
		space[setPosition + i].position.x = INIT_NUM;
		space[setPosition + i].position.y = INIT_NUM;
		space[setPosition + i].position.z = INIT_NUM;
	}
}

// ---------------------------------
// デバッグ出力する
// ---------------------------------
void printDebug(SPACE* space, int currentPazuruNum) {
	printf("START--------------------\n");

	printf("successNum: %d\n", successNum);

	printf("currentPazuruNum: %d\n", currentPazuruNum);

	printf("XYZ----------\n");
	for (int num = 0;num < NUM_SPACE;num++) {
		printf("X: %d, Y: %d, Z: %d\n", space[num].position.x, space[num].position.y, space[num].position.z);
	}

	printf("END--------------------\n");

	fflush(stdout);
}



// ---------------------------------
// メイン
// ---------------------------------
int main() {
	SPACE space[NUM_SPACE];
	initSpace(space);

	// 再帰処理を用いてパズル1からパズル7まで順番に試していく
	trySetOrder(space, NUM_PAZURU_1);

	printf("finished. num: %d\n", successNum);
	fflush(stdout);

	return EXIT_SUCCESS;
}
