#include"utility.h"
#include"GameScene.h"

//そもそも各札に、一月の四枚は0から3,二月の四枚は4から7,.....十二月の四枚は44から47と、0から47のインデックスが割り振ってある

void GameScene::Init(int _gamenum, int _teban, int _score_player0, int _score_player1) {

	CardDataLoad();

	graph_back = LoadGraph("graphic/BG.png");

	//各札に0~47の番号を割り振ってあり、"「その番号」.png"の画像ファイルが存在することが前提。
	card[0].graph = LoadGraph("graphic/0.png");
	card[1].graph = LoadGraph("graphic/1.png");
	card[2].graph = LoadGraph("graphic/2.png");
	card[3].graph = LoadGraph("graphic/3.png");
	card[4].graph = LoadGraph("graphic/4.png");
	card[5].graph = LoadGraph("graphic/5.png");
	card[6].graph = LoadGraph("graphic/6.png");
	card[7].graph = LoadGraph("graphic/7.png");
	card[8].graph = LoadGraph("graphic/8.png");
	card[9].graph = LoadGraph("graphic/9.png");
	card[10].graph = LoadGraph("graphic/10.png");
	card[11].graph = LoadGraph("graphic/11.png");
	card[12].graph = LoadGraph("graphic/12.png");
	card[13].graph = LoadGraph("graphic/13.png");
	card[14].graph = LoadGraph("graphic/14.png");
	card[15].graph = LoadGraph("graphic/15.png");
	card[16].graph = LoadGraph("graphic/16.png");
	card[17].graph = LoadGraph("graphic/17.png");
	card[18].graph = LoadGraph("graphic/18.png");
	card[19].graph = LoadGraph("graphic/19.png");
	card[20].graph = LoadGraph("graphic/20.png");
	card[21].graph = LoadGraph("graphic/21.png");
	card[22].graph = LoadGraph("graphic/22.png");
	card[23].graph = LoadGraph("graphic/23.png");
	card[24].graph = LoadGraph("graphic/24.png");
	card[25].graph = LoadGraph("graphic/25.png");
	card[26].graph = LoadGraph("graphic/26.png");
	card[27].graph = LoadGraph("graphic/27.png");
	card[28].graph = LoadGraph("graphic/28.png");
	card[29].graph = LoadGraph("graphic/29.png");
	card[30].graph = LoadGraph("graphic/30.png");
	card[31].graph = LoadGraph("graphic/31.png");
	card[32].graph = LoadGraph("graphic/32.png");
	card[33].graph = LoadGraph("graphic/33.png");
	card[34].graph = LoadGraph("graphic/34.png");
	card[35].graph = LoadGraph("graphic/35.png");
	card[36].graph = LoadGraph("graphic/36.png");
	card[37].graph = LoadGraph("graphic/37.png");
	card[38].graph = LoadGraph("graphic/38.png");
	card[39].graph = LoadGraph("graphic/39.png");
	card[40].graph = LoadGraph("graphic/40.png");
	card[41].graph = LoadGraph("graphic/41.png");
	card[42].graph = LoadGraph("graphic/42.png");
	card[43].graph = LoadGraph("graphic/43.png");
	card[44].graph = LoadGraph("graphic/44.png");
	card[45].graph = LoadGraph("graphic/45.png");
	card[46].graph = LoadGraph("graphic/46.png");
	card[47].graph = LoadGraph("graphic/47.png");

	graph_ura = LoadGraph("graphic/ura.png");

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

void GameScene::Update() {
	//セル表示処理
	//手札選択Select()
	////捨て札と場札を合わせる処理Trash()
	////山札から引いて場札を合わせる処理DecktoField()
	//終了処理

	//描画演出中は更新しない
	if (isdrawing)return;

	//こいこいしますか？もしくはどの札を選択しますか？という表示が出ている
	int fieldplace_choosed = -1;//switch~case文内だとLocal変数の初期化がされないのでここに；ONLY"case MULTICHOICE"
	switch (cellkind)
	{
	case Cellkind::NONE:
		break;
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
		fieldplace_choosed = Choose();

		if (fieldplace_choosed != -1) {
			//Trash関数からコピペ
			//Get the card that player selected
			player[teban].index_get.push_back(player[teban].index_hold[holdplace_selected]);//本当はここにholdplace_selectedつまりプレイヤーが選んでおいたインデックスが入らなければならない。しかしこいこい判定直後１ループするせいでSelect()により-1が入る
			std::vector<byte>::iterator itr_temp1 = player[teban].index_hold.begin();
			itr_temp1 += holdplace_selected;//
			player[teban].index_hold.erase(itr_temp1);

			//deck to player
			player[teban].index_get.push_back(field[fieldplace_choosed]);
			std::vector<byte>::iterator itr_temp2 = field.begin();
			itr_temp2 += fieldplace_choosed;
			field.erase(itr_temp2);

			DeckDraw();

			if (YakuHantei() != 0) {
				cellkind = Cellkind::KOIKOI;
				return;
			}

			teban ^= 1;
			cellkind = Cellkind::NONE;
		}
		return;//表示が出ている間は手札選択にいけない
	default:
		break;
	}

	//手札選択
	holdplace_selected = Select();//normally returns -1, not 0

	if (holdplace_selected != -1) {

		if (Trash(holdplace_selected) != 0)return;//エラーが出たり複数選択になったらもう１ループする

		DeckDraw();

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
	//Draw BackGroundImage
	DrawGraph(0, 0, graph_back, FALSE);

	//Draw Cards Player0 holds
	for (int i = 0; i < (signed int)player[0].index_hold.size(); i++) {
		DrawGraph(SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 3 * (cardwidth + xspace) + i * (cardwidth + xspace), yblank + (cardheight + yspace) * 3, card[player[0].index_hold[i]].graph, FALSE);
	}

	//Draw Field Cards
	for (int i = 0; i < (signed int)field.size(); i++) {
		if (i < 10) {
			DrawGraph(SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 1 * (cardwidth + xspace) + (i / 2) * (cardwidth + xspace), yblank + (1/*if 0, same y coordinate as player1's hand*/ + i % 2) * (cardheight + yspace), card[field[i]].graph, FALSE);
		}
		else if (i <= 10 && i < 12) {
			DrawGraph(SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 2 * (cardwidth + xspace) + (i / 2) * (cardwidth + xspace), yblank + (1 + i % 2) * (cardheight + yspace), card[field[i]].graph, FALSE);
		}
		else {
			DrawGraph(SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 1 * (cardwidth + xspace) + (i / 2) * (cardwidth + xspace), yblank + (1 + i % 2) * (cardheight + yspace), card[field[i]].graph, FALSE);
		}
	}

	//Draw Cards Player1 holds
	for (int i = 0; i < (signed int)player[1].index_hold.size(); i++) {//CPなので手札は見せない
		DrawGraph(SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 3 * (cardwidth + xspace) + i * (cardwidth + xspace), yblank + (cardheight + yspace) * 0, graph_ura, FALSE);
	}

	//Draw Every Player's gotten cards
	for (int i = 0; i < (signed int)player[0].index_get.size(); i++) {
		DrawGraph(SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 3 * (cardwidth + xspace) + 6 * (cardwidth + xspace) + cardwidth / 2/*The place semi-rightmost card existed +half of cardwidth*/ + (i % 8) * cardwidth / 3, yblank + (cardheight + yspace) * (2 + i / 8), card[player[0].index_get[i]].graph, FALSE);
	}
	for (int i = 0; i < (signed int)player[1].index_get.size(); i++) {
		DrawGraph(SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 3 * (cardwidth + xspace) + 6 * (cardwidth + xspace) + cardwidth / 2/*The place semi-rightmost card existed +half of cardwidth*/ + (i % 8) * cardwidth / 3, yblank + (cardheight + yspace) * (0 + i / 8), card[player[1].index_get[i]].graph, FALSE);
	}

	//Draw Cell
	int nowyakunum = 0;//use only in KOIKOI
	unsigned int yaku = player[teban].yaku;//use only in KOIKOI
	switch (cellkind)
	{
	case GameScene::Cellkind::NONE:
		break;
	case GameScene::Cellkind::KOIKOI:
		DrawBox(40, yblank + (cardheight + yspace) * 1, 250/*not to overlap field*/, yblank + (cardheight + yspace) * 2 + cardheight, BLACK, TRUE);//x座標は画面中央を基準に真ん中揃え
		yaku = player[teban].yaku;

		//for debug
		DrawExtendFormatStringToHandle(SCREEN_WIDTH / 2 - 120 + 45, 30, 1.0, 1.0, WHITE, fonthandle, "手番=%d  役=%d", teban, yaku);

		nowyakunum = 0;//現在できている役の数。描画の位置調整も兼ねる
		//&演算子は順位が==より低い。括弧を付ける。
		if ((yaku & 0x0001) == 0x0001) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "カス　　%d文", 1 + player[teban].num_kasu - 10); nowyakunum++; }
		if ((yaku & 0x0002) == 0x0002) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "タン　　%d文", 1 + player[teban].num_tan - 5); nowyakunum++; }
		if ((yaku & 0x0004) == 0x0004) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "タネ　　%d文", 1 + player[teban].num_tane - 5); nowyakunum++; }
		if ((yaku & 0x0008) == 0x0008) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "三光　　%d文", 5); nowyakunum++; }
		if ((yaku & 0x0010) == 0x0010) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "雨四光　　%d文", 8); nowyakunum++; }
		if ((yaku & 0x0020) == 0x0020) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "四光　　%d文", 10); nowyakunum++; }
		if ((yaku & 0x0040) == 0x0040) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "五光　　%d文", 15); nowyakunum++; }
		if ((yaku & 0x0080) == 0x0080) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "赤タン　　%d文", 5 + player[teban].num_tan - 3); nowyakunum++; }
		if ((yaku & 0x0100) == 0x0100) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "青タン　　%d文", 5 + player[teban].num_tan - 3); nowyakunum++; }
		if ((yaku & 0x0200) == 0x0200) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "猪鹿蝶　　%d文", 5); nowyakunum++; }

		DrawExtendFormatStringToHandle(40+20, yblank + (cardheight + yspace) * 2, 1.0, 1.0, WHITE, fonthandle, "こいこいしますか？");
		DrawExtendFormatStringToHandle(40+20, yblank + (cardheight + yspace) * 2 + 30, 1.0, 1.0, WHITE, fonthandle, "はい　　　いいえ");
		break;
	case GameScene::Cellkind::MULTICHOICE:
		DrawBox(180, 400, 460, 460, BLACK, TRUE);
		DrawExtendFormatStringToHandle(200, 420, 1.0, 1.0, WHITE, fonthandle, "どれを取りますか？");
		break;
	default:
		break;
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
			if (chardata[i] == ',' || chardata[i] == '\n') {
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

//返り値は「そのカードの手札のなかで何番目に置いてあるか」。札のインデックスではない
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
void GameScene::DeckDraw() {
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

	int kasu = 0;
	int tan = 0;
	int tane = 0;
	int hikari = 0;
	
	bool is11 = false;//If No.11:"Sakura and Maku" exists
	bool is31 = false;//If No.31:"Susuki and Tsuki" exists
	bool is35 = false;//If No.35:"Kiku and Sake" exists
	bool is43 = false;//If No.43:"Yanagi and OnnonoDouhuu" exists

	int num_akatan = 0;
	int num_aotan = 0;
	int num_inosikachou = 0;

	//scanning&count
	for (int i = 0; i < player[teban].index_get.size(); i++) {
		int index = player[teban].index_get[i];
		if (card[index].kind == 0)kasu++;
		else if (card[index].kind == 5)tan++;
		else if (card[index].kind == 10)tane++;
		else if (card[index].kind == 20)hikari++;

		if (index == 11)is11 = true;
		if (index == 31)is31 = true;
		if (index == 35)is35 = true;
		if (index == 43)is43 = true;

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

	//decision
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
	if (hikari == 3 && !is43) {//三光
		nowscore += 5;
		yaku += 0x0008;
	}
	else if (hikari == 4) {
		if (is43) {
			nowscore += 8;//雨四光
			yaku += 0x0010;
		}
		else {
			nowscore += 10;//四光
			yaku += 0x0020;
		}
	}
	else if (hikari == 5) {
		nowscore += 15;//五光
		yaku += 0x0040;
	}
	
	if (num_akatan == 3) {
		nowscore += 5 + tan - 3;//赤短
		yaku += 0x0080;
	}
	if (num_aotan == 3) {
		nowscore += 5 + tan - 3;//青短
		yaku += 0x0100;
	}
	if (num_inosikachou == 3) {
		nowscore += 5;//猪鹿蝶
		yaku += 0x0200;
	}
	/*
	if (is35) {
		if(is11){
			nowscore += 3;//花見酒
			yaku += 0x0400;
		}
		if(is31){
			nowscore += 3;//月見酒
			yaku += 0x0800;
		}
	}
	*/

	player[teban].num_kasu = kasu;
	player[teban].num_tan = tan;
	player[teban].num_tane = tane;

	//new or renew yaku
	if (nowscore > player[teban].nowscore) {
		player[teban].nowscore = nowscore;
		player[teban].yaku = yaku;
		return 1;
	}
	//no or no renew yaku
	return 0;
}

/*タン５→タン６
面倒だからnum_kasuに保存して役判定後の差分で調べた。雑。

同じブランチ内での更新をCommit
これ更新させてくれ！ってのがPull Request
いいだろう更新を受け入れるのがmerge


fieldplace_choosed=samemonthcardには、その札がfieldで何番目なのかが入る
holdplace_selectedは「そのカードの手札のなかで何番目に置いてあるか」
どちらも札のインデックスではない。バグの温床だ
*/
