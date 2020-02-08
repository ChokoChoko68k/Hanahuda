#include"utility.h"
#include"GameScene.h"
#include"Choko.h"

//そもそも各札に、一月の四枚は0から3,二月の四枚は4から7,.....十二月の四枚は44から47と、0から47のインデックスが割り振ってある

GameScene::GameScene(int _gamenum, int _teban, int _score_player0, int _score_player1) {

	CardDataLoad();

	graph_back = LoadGraph("graphic/a0007_001048_m.png");

	//各札に0~47の番号を割り振ってあり、"「その番号」.png"の画像ファイルが存在することが前提。
	for (int i = 0; i < 48; i++) {
		//Finally all in one picture
		/*if (card[i].kind == 0) card[i].graph = LoadGraph("graphic/28.png");
		if (card[i].kind == 5) card[i].graph = LoadGraph("graphic/22.png");
		if (card[i].kind == 10) card[i].graph = LoadGraph("graphic/30.png");
		if (card[i].kind == 20) card[i].graph = LoadGraph("graphic/31.png");*/
		if (i == 8)card[i].graph = LoadGraph("graphic/8.png");
		else if(i==12)card[i].graph = LoadGraph("graphic/12.png");
		else if (i == 13)card[i].graph = LoadGraph("graphic/13.png");
		else if (i == 14)card[i].graph = LoadGraph("graphic/14.png");
		else if (i == 15)card[i].graph = LoadGraph("graphic/15.png");
		else if (i == 20)card[i].graph = LoadGraph("graphic/20.png");
		else if (i == 21)card[i].graph = LoadGraph("graphic/21.png");
		else if (i == 22)card[i].graph = LoadGraph("graphic/22.png");
		else if (i == 23)card[i].graph = LoadGraph("graphic/23.png");
		else if (i == 24)card[i].graph = LoadGraph("graphic/24.png");
		else if (i == 25)card[i].graph = LoadGraph("graphic/25.png");
		else if (i == 26)card[i].graph = LoadGraph("graphic/26.png");
		else if (i == 27)card[i].graph = LoadGraph("graphic/27.png");
		else if (i == 28)card[i].graph = LoadGraph("graphic/28.png");
		else if (i == 29)card[i].graph = LoadGraph("graphic/29.png");
		else if (i == 30)card[i].graph = LoadGraph("graphic/30.png");
		else if (i == 31)card[i].graph = LoadGraph("graphic/31.png");
		else card[i].graph = LoadGraph("graphic/ura.png");

	}

	gamenum = _gamenum + 1;
	teban = _teban;
	player[0].score = _score_player0;
	player[1].score = _score_player1;
	player[0].Clear();
	player[1].Clear();
	deck.clear();
	field.clear();
	samemonthcard.clear();

	while (true) {
		Shuffle();
		if (IsDealOK())break;
	}
	Deal();
	isdrawing = false;
	cellkind = Cellkind::NONE;
}

GameScene::~GameScene() {
}

void GameScene::Update() {
	//セル表示処理
	//手札選択Select()
	////捨て札と場札を合わせる処理Trash()
	////山札から引いて場札を合わせる処理DecktoField()
	//終了処理

	//描画演出中は更新しない
	if (isdrawing)return;

	//こいこいしますか？もしくはどの札を選択しますか？という表示が出ている
	int num_choosed = -1;//switch~case文内だとLocal変数の初期化がされないのでここに；ONLY"case MULTICHOICE"
	switch (cellkind)
	{
	case Cellkind::KOIKOI:
		if (click_left == 1) {
			//こいこい
			if (mousex >= 0 && mousex < 320 && mousey >= 0 && mousey < 480) {
				cellkind = Cellkind::NONE;
				teban ^= 1;
			}
			//勝負
			else if (mousex >= 320 && mousex < 640 && mousey >= 0 && mousey < 480) {
				player[teban].score += player[teban].nowscore;
				player[(teban + 1) % 2].score -= player[teban].nowscore;
				SceneManager::GetInstance()->CreateScene(SceneID::RESULT, SceneLayer::UPPER, gamenum, teban, player[0].score, player[1].score);
				return;
			}
		}
		return;//表示が出ている間は手札選択にいけない
	case Cellkind::MULTICHOICE:
		num_choosed = Choose();

		if (num_choosed != -1) {
			//TODO:どれを取りますか？と言われているにも関わらず月が合致しないものを選ぶとエラー

			//Trash関数からコピペ
			//Get the card that player selected
			player[teban].index_get.push_back(player[teban].index_hold[num_choosed]);
			std::vector<byte>::iterator itr_temp1 = player[teban].index_hold.begin();
			itr_temp1 += num_choosed;
			player[teban].index_hold.erase(itr_temp1);

			//deck to player
			player[teban].index_get.push_back(field[num_choosed]);
			std::vector<byte>::iterator itr_temp2 = field.begin();
			itr_temp2 += num_choosed;
			field.erase(itr_temp2);

			DeckToField();

			if (YakuHantei() != 0) {
				cellkind = Cellkind::KOIKOI;
				return;
			}

			teban ^= 1;
			cellkind = Cellkind::NONE;
			return;//表示が出ている間は手札選択にいけない
		}
		break;
	case Cellkind::NONE:
	default:
		break;
	}

	//手札選択
	index_selected = Select();//normally returns -1, not 0

	if (index_selected != -1) {

		if (Trash(index_selected) != 0)return;

		DeckToField();

		if (YakuHantei() != 0) {
			cellkind = Cellkind::KOIKOI;
			return;
		}

		teban ^= 1;
	}

	//役なしで終了
	if ((player[0].index_hold).size() == 0 && (player[1].index_hold).size() == 0) {
		SceneManager::GetInstance()->CreateScene(SceneID::RESULT, SceneLayer::UPPER, gamenum, teban, player[0].score, player[1].score);
		return;
	}
}

void GameScene::Draw() {

	DrawGraph(0, 0, graph_back, TRUE);

	for (unsigned int i = 0; i < (player[0].index_hold).size(); i++) {
		DrawGraph(xblank + (cardwidth + xspace) * (i % 2), yblank + (cardheight + yspace) * (i / 2), card[player[0].index_hold[i]].graph, TRUE);
	}

	for (unsigned int i = 0; i < field.size(); i++) {
		DrawGraph(160 + xblank + (cardwidth + 23) * (i % 4), 40 + yblank + (cardheight + 12) * (i / 4), card[field[i]].graph, TRUE);
	}

	for (unsigned int i = 0; i < (player[1].index_hold).size(); i++) {
		DrawGraph(500 + xblank + (cardwidth + xspace) * (i % 2), yblank + (cardheight + yspace) * (i / 2), card[player[1].index_hold[i]].graph, TRUE);
	}

	//todo:種類別にソート;後回し
	for (unsigned int i = 0; i < (player[0].index_get).size(); i++) {
		DrawGraph(30 + (i % 12) * 21, 320 + 70 * (i / 12), card[player[0].index_get[i]].graph, TRUE);//30は適当
	}

	for (unsigned int i = 0; i < (player[1].index_get).size(); i++) {
		DrawGraph(640 - cardwidth - 30 - (i % 12) * 21, 320 + 70 * (i / 12), card[player[1].index_get[i]].graph, TRUE);
	}

	if (cellkind == Cellkind::KOIKOI) {
		DrawBox(180, 50, 460, 460, BLACK, TRUE);
		unsigned int yaku = player[teban].yaku;

		DrawExtendFormatStringToHandle(100, 30, 1.0, 1.0, WHITE, fonthandle, "手番=%d  役=%d", teban, yaku);

		int nowyakunum = 0;
		//&演算子は順位が==より低い。括弧を付ける。
		if ((yaku & 0x0001) == 0x0001) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "カス　　%d文", 1 + player[teban].num_kasu - 10); nowyakunum++; }
		if ((yaku & 0x0002) == 0x0002) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "タン　　%d文", 1 + player[teban].num_tan - 5); nowyakunum++; }
		if ((yaku & 0x0004) == 0x0004) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "タネ　　%d文", 1 + player[teban].num_tane - 5); nowyakunum++; }
		if ((yaku & 0x0008) == 0x0008) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "三光　　%d文", 5); nowyakunum++; }
		if ((yaku & 0x0010) == 0x0010) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "雨四光　　%d文", 8); nowyakunum++; }
		if ((yaku & 0x0020) == 0x0020) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "四光　　%d文", 10); nowyakunum++; }
		if ((yaku & 0x0040) == 0x0040) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "五光　　%d文", 15); nowyakunum++; }
		if ((yaku & 0x0080) == 0x0080) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "赤タン　　%d文", 5 + player[teban].num_tan - 3); nowyakunum++; }
		if ((yaku & 0x0100) == 0x0100) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "青タン　　%d文", 5 + player[teban].num_tan - 3); nowyakunum++; }
		if ((yaku & 0x0200) == 0x0200) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "猪鹿蝶　　%d文", 5); nowyakunum++; }

		DrawExtendFormatStringToHandle(200, 400, 1.0, 1.0, WHITE, fonthandle, "こいこいしますか？");
		DrawExtendFormatStringToHandle(200, 430, 1.0, 1.0, WHITE, fonthandle, "はい　　　いいえ");
	}
	else if (cellkind == Cellkind::MULTICHOICE) {
		DrawBox(180, 400, 460, 460, BLACK, TRUE);
		DrawExtendFormatStringToHandle(200, 420, 1.0, 1.0, WHITE, fonthandle, "どれを取りますか？");
	}
	if (keystate_2 == 1) {
		int i = 0;
	}
}

int GameScene::CardDataLoad() {

	static const int LIMIT_OF_DIGITS = 4;//入力されるxやpatternの桁数の限界＋１;null文字分
	char chardata[LIMIT_OF_DIGITS];

	int fp = FileRead_open("hanahuda.csv");
	if (fp == NULL)return -1;

	int datanum = 0, ordernum = 0;
	while (true) {
		//一単位に分割
		for (int i = 0; i<LIMIT_OF_DIGITS; i++) {
			chardata[i] = FileRead_getc(fp);//FileRead系の関数は内部にポインタを持っていて、呼び出す度に見えないところでポインタがずれていく注意
			if (chardata[i] == ';' || chardata[i] == '\n') {
				chardata[i] = '\0';
				break;
			}
			if (chardata[i] == EOF)goto EXFILE;
		}
		//それを整数型or浮動小数点型に変換しenemydataに入れる
		switch (ordernum) {
		case 0:	card[datanum].month = atoiDx(chardata); break;
		case 1:	card[datanum].kind = atoiDx(chardata); break;
		}
		ordernum++;
		if (ordernum == 2) {
			ordernum = 0;
			datanum++;
		}

	}
EXFILE:
	FileRead_close(fp);

	return 0;
}

void GameScene::Shuffle() {
	deck.clear();

	bool isused[48] = {false};//{false,}48個は面倒

	for (int i = 0; i < 48; i++) {
		isused[i] = false;
	}

	for (int i = 0; i < 48; i++) {
		//使用されてないカードの番号をランダムで探し当てる；使用されていたら隣のカードを調べる
		int index = GetRand(47);
		while (isused[index] == true) {
			index = (index + 1) % 48;
		}
		isused[index] = true;

		deck.push_back(index);
	}
}

//配りかたが正当か；花札のルール上、場札に同じ月が3枚あるとゲームがうまく進まない。これを避けるための判定
bool GameScene::IsDealOK() {
	//fieldに渡す８枚のうち同じ月が3枚になっていたらリセット
	byte month_exist[12] = {};//これで「初期化」になっているのだろうか->なっとる

	for (int i = 0; i < 8; i++) {
		//fieldに入る各札のインデックスは既定(松のカス札を０として４７の桐に鳳凰まで割り振られている)ので、月を割り出せる
		month_exist[deck.at(i) / 4]++;
	}

	for (int i = 0; i < 12; i++) {
		if (month_exist[i] >= 3)return false;
	}
	return true;
}

void GameScene::Deal() {

	for (int i = 0; i < 8; i++) {
		field.push_back(deck[0]);
		deck.erase(deck.begin());
	}

	for (int i = 0; i < 8; i++) {
		player[0].index_hold.push_back(deck[0]);
		deck.erase(deck.begin());
	}
	
	for (int i = 0; i < 8; i++) {
		player[1].index_hold.push_back(deck[0]);
		deck.erase(deck.begin());
	}
}

int GameScene::Select() {
	if (click_left == 1) {
		if (teban == 0) {//player 0's turn
			if (mousex < xblank + (cardwidth + xspace) * 2) {//if mousecursor is in player1 area
				for (int i = 0; i < (signed int)player[0].index_hold.size(); i++) {
					if (mousex >= xblank + (cardwidth + xspace) * (i % 2) && mousex < xblank + (cardwidth + xspace) * (i % 2) + cardwidth) {
						if (mousey >= yblank + (cardheight + yspace) * (i / 2) && mousey < yblank + (cardheight + yspace) * (i / 2) + cardheight) {
							return i;
						}
					}

				}
			}
		}
		else if (teban == 1) {//player 1's turn
			if (mousex > 500 + xblank) {//if mousecursor is in player1 area
				for (int i = 0; i < (signed int)player[1].index_hold.size(); i++) {
					if (mousex >= 500 + xblank + (cardwidth + xspace) * (i % 2) && mousex < 500 + xblank + (cardwidth + xspace) * (i % 2) + cardwidth) {
						if (mousey >= yblank + (cardheight + yspace) * (i / 2) && mousey < yblank + (cardheight + yspace) * (i / 2) + cardheight) {
							return i;
						}
					}
				}
			}
		}
	}

	return -1;//still no card selected
}

//プレイヤーが選んだ札に対する処理全般
int GameScene::Trash(int index) {

	//samemonthcardには、その札がfieldで何番目なのかが入る
	samemonthcard.clear();

	//場札を走査して、月が一致する札が何枚あるか確認
	for (unsigned int i = 0; i < field.size(); i++) {
		if (card[player[teban].index_hold[index]].month == card[field[i]].month) {//月一致
			samemonthcard.push_back(i);
		}
	}
	
	if (samemonthcard.size() == 0) {
		//deck to field
		field.push_back(player[teban].index_hold[index]);//
		std::vector<byte>::iterator itr_temp3 = player[teban].index_hold.begin();
		itr_temp3 += index;
		player[teban].index_hold.erase(itr_temp3);

		return 0;
	}
	else if (samemonthcard.size() == 1) {
		//Get the card that player selected
		player[teban].index_get.push_back(player[teban].index_hold[index]);
		std::vector<byte>::iterator itr_temp1 = player[teban].index_hold.begin();
		itr_temp1 += index;
		player[teban].index_hold.erase(itr_temp1);

		//field to player
		player[teban].index_get.push_back(field[samemonthcard[0]]);
		std::vector<byte>::iterator itr_temp2 = field.begin();
		itr_temp2 += samemonthcard[0];
		field.erase(itr_temp2);

		return 0;
	}
	else if (samemonthcard.size() > 1) {
		cellkind = Cellkind::MULTICHOICE;
		return -1;
	}
	return -2;//error
}

//場札に同じ月の札が複数あった場合に行う選択
int GameScene::Choose() {
	if (click_left == 1) {
		for (int i = 0; i < (signed int)samemonthcard.size(); i++) {
			int num = samemonthcard[i];
			//
			if (mousex >= 160 + xblank + (cardwidth + 23) * (num % 4) && mousex < 160 + xblank + (cardwidth + 23) * (num % 4) + cardwidth) {
				if (mousey >= 40 + yblank + (cardheight + 12) * (num / 4) && mousey < 40 + yblank + (cardheight + 12) * (num / 4) + cardheight) {
					return num;
				}
			}
		}		
	}
	return -1;
}

//山札から引いて場札と照らしあわせる処理全般
void GameScene::DeckToField() {
	for (unsigned int i = 0; i < field.size(); i++) {
		if (card[deck.at(0)].month == card[field[i]].month) {//月一致
			//deck to player
			player[teban].index_get.push_back(deck.at(0));
			std::vector<byte>::iterator itr_temp1 = deck.begin();
			deck.erase(itr_temp1);

			//field to player
			player[teban].index_get.push_back(field[i]);
			std::vector<byte>::iterator itr_temp2 = field.begin();
			itr_temp2 += i;
			field.erase(itr_temp2);
			return;
		}
	}
	//deck to field
	field.push_back(deck.at(0));
	std::vector<byte>::iterator itr = deck.begin();
	deck.erase(itr);
}

int GameScene::YakuHantei() {

	unsigned int yaku = 0;

	int nowscore = 0;

	unsigned int kasu = 0;
	unsigned int tan = 0;
	unsigned int tane = 0;
	unsigned int hikari = 0;
	
	bool is43 = false;

	unsigned int num_akatan = 0;
	unsigned int num_aotan = 0;
	unsigned int num_inosikachou = 0;

	for (unsigned int i = 0; i < player[teban].index_get.size(); i++) {
		int index = player[teban].index_get[i];
		if (card[index].kind == 0)kasu++;
		else if (card[index].kind == 5)tan++;
		else if (card[index].kind == 10)tane++;
		else if (card[index].kind == 20)hikari++;

		if (index == 43)is43 = true;//If No.43:"Yanagi and OnnonoDouhuu" exists

		if (index == 2)num_akatan++;
		if (index == 6)num_akatan++;
		if (index == 10)num_akatan++;

		if (index == 22)num_aotan++;
		if (index == 34)num_aotan++;
		if (index == 36)num_aotan++;

		if (index == 23)num_inosikachou++;
		if (index == 27)num_inosikachou++;
		if (index == 35)num_inosikachou++;
	}

	if (kasu >= 10) {
		nowscore += 1 + kasu - 10;
		yaku += 0x0001;
	}
	if (tan >= 5) {
		nowscore += 1 + tan - 5;
		yaku += 0x0002;
	}
	if (tane >= 5) {
		nowscore += 1 + tane - 5;
		yaku += 0x0004;
	}

	if (hikari == 3 && !is43) {
		nowscore += 5;
		yaku += 0x0008;
	}
	else if (hikari == 4) {
		if (is43) {
			nowscore += 8;
			yaku += 0x0010;
		}
		else {
			nowscore += 10;
			yaku += 0x0020;
		}
	}
	else if (hikari == 5) {
		nowscore += 15;
		yaku += 0x0040;
	}
	
	if (num_akatan == 3) {
		nowscore += 5 + tan - 3;
		yaku += 0x0080;
	}
	if (num_aotan == 3) {
		nowscore += 5 + tan - 3;
		yaku += 0x0100;
	}
	if (num_inosikachou == 3) {
		nowscore += 5;
		yaku += 0x0200;
	}

	player[teban].num_kasu = kasu;
	player[teban].num_tan = tan;
	player[teban].num_tane = tane;

	int i = 0;

	if (nowscore > player[teban].nowscore) {
		player[teban].nowscore = nowscore;
		player[teban].yaku = yaku;
		return -1;
	}
	return 0;
}

/*タン５→タン６
面倒だからnum_kasuに保存して役判定後の差分で調べた。雑。

説明コメントを付ける
・シーンをどう作るか、流れ
・役判定の流れ、変数yaku

カードをシーン外に静止させて
*/