#include"utility.h"
#include"VSCPGameScene.h"

//Overriding GameScene
int VSCPGameScene::Update() {
	//セル表示処理
	//手札選択Select()
	////捨て札と場札を合わせる処理Trash()
	////山札から引いて場札を合わせる処理DecktoField()
	//終了処理

	//描画演出中は更新しない
	if (isdrawing)return 0;

	//こいこいしますか？もしくはどの札を選択しますか？という表示が出ている
	int fieldplace_choosed = -1;//switch~case文内だとLocal変数の初期化がされないのでここに；ONLY"case MULTICHOICE"
	switch (cellkind)
	{
	case Cellkind::NONE:
		break;
	case Cellkind::KOIKOI:
		if (teban == 1) {//CPU's turn
			//こいこい
			//相手がリーチでなく、かつ最後のターン以外する
			if (player[0].yaku_reach == 0x0000 && player[1].index_hold.size() > 1) {
				cellkind = Cellkind::NONE;
				teban ^= 1;
			}
			//勝負
			else {
				player[teban].score += player[teban].nowscore;
				player[(teban + 1) % 2].score -= player[teban].nowscore;
				SceneManager::GetInstance()->CreateScene(SceneID::RESULT, SceneLayer::UPPER, gamenum, teban, player[0].score, player[1].score);
				return 0;
			}
		}
		else if (teban == 0) {//Player's turn
			if (click_left == 1) {
				//こいこい
				if (mousex <= SCREEN_WIDTH / 2) {//左側をクリック
					cellkind = Cellkind::NONE;
					teban ^= 1;
				}
				//勝負
				else if (mousex > SCREEN_WIDTH / 2) {//右側をクリック
					player[teban].score += player[teban].nowscore;
					player[(teban + 1) % 2].score -= player[teban].nowscore;
					SceneManager::GetInstance()->CreateScene(SceneID::RESULT, SceneLayer::UPPER, gamenum, teban, player[0].score, player[1].score);
					return 0;
				}
			}
		}
		return 0;//表示が出ている間は手札選択にいけない
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
				return 0;
			}

			teban ^= 1;
			cellkind = Cellkind::NONE;
		}
		return 0;//表示が出ている間は手札選択にいけない
	default:
		break;
	}

	//手札選択
	holdplace_selected = Select();//normally returns -1, not 0

	if (holdplace_selected != -1) {

		if (TrashorGet(holdplace_selected) != 0)return 0;//エラーが出たり複数選択になったらもう１ループする

		DeckDraw();

		if (YakuHantei() != 0) {
			cellkind = Cellkind::KOIKOI;
			return 0;
		}

		teban ^= 1;
	}

	//役なしで終了
	if ((player[0].index_hold).size() == 0 && (player[1].index_hold).size() == 0) {
		SceneManager::GetInstance()->CreateScene(SceneID::RESULT, SceneLayer::UPPER, gamenum, teban, player[0].score, player[1].score);
		return 0;
	}
	return 0;
}

void VSCPGameScene::Draw() {
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
#ifdef _DEBUG
		DrawGraph(SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 3 * (cardwidth + xspace) + i * (cardwidth + xspace), yblank + (cardheight + yspace) * 0, card[player[1].index_hold[i]].graph, FALSE);
#else
		DrawGraph(SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 3 * (cardwidth + xspace) + i * (cardwidth + xspace), yblank + (cardheight + yspace) * 0, graph_ura, FALSE);
#endif
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
		DrawBox(40, yblank + (cardheight + yspace) * 1, 40 + 300, yblank + (cardheight + yspace) * 2 + cardheight, BLACK, TRUE);//x座標は画面中央を基準に真ん中揃え
		yaku = player[teban].yaku;

		nowyakunum = 0;//現在できている役の数。描画の位置調整も兼ねる
		//&演算子は順位が==より低い。括弧を付ける。
		if ((yaku & 0x0001) == 0x0001) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "カス　　%d文", 1 + player[teban].num_kasu - 10); nowyakunum++; }
		if ((yaku & 0x0002) == 0x0002) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "タン　　%d文", 1 + player[teban].num_tan - 5); nowyakunum++; }
		if ((yaku & 0x0004) == 0x0004) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "タネ　　%d文", 1 + player[teban].num_tane - 5); nowyakunum++; }
		if ((yaku & 0x0008) == 0x0008) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "三光　　%d文", 5); nowyakunum++; }
		if ((yaku & 0x0010) == 0x0010) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "雨四光　　%d文", 8); nowyakunum++; }
		if ((yaku & 0x0020) == 0x0020) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "四光　　%d文", 10); nowyakunum++; }
		if ((yaku & 0x0040) == 0x0040) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "五光　　%d文", 15); nowyakunum++; }
		if ((yaku & 0x0080) == 0x0080) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "赤タン　　%d文", 5 + player[teban].num_tan - 3); nowyakunum++; }
		if ((yaku & 0x0100) == 0x0100) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "青タン　　%d文", 5 + player[teban].num_tan - 3); nowyakunum++; }
		if ((yaku & 0x0200) == 0x0200) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "猪鹿蝶　　%d文", 5); nowyakunum++; }

		DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 2, 1.0, 1.0, WHITE, fonthandle, "こいこいしますか？");
		DrawBox(SCREEN_WIDTH / 2 - 60 - 10, yblank + (cardheight + yspace) * 2 + 30 - 10, SCREEN_WIDTH / 2 - 60 - 10 + 80, yblank + (cardheight + yspace) * 2 + 30 - 10 + 30, BLACK, TRUE);
		DrawExtendFormatStringToHandle(SCREEN_WIDTH / 2 - 60, yblank + (cardheight + yspace) * 2 + 30, 1.0, 1.0, WHITE, fonthandle, "はい");
		DrawBox(SCREEN_WIDTH / 2 + 60 - 10, yblank + (cardheight + yspace) * 2 + 30 - 10, SCREEN_WIDTH / 2 + 60 - 10 + 80, yblank + (cardheight + yspace) * 2 + 30 - 10 + 30, BLACK, TRUE);
		DrawExtendFormatStringToHandle(SCREEN_WIDTH / 2 + 60, yblank + (cardheight + yspace) * 2 + 30, 1.0, 1.0, WHITE, fonthandle, "いいえ");
		break;
	case GameScene::Cellkind::MULTICHOICE:
		DrawBox(180, 400, 460, 460, BLACK, TRUE);
		DrawExtendFormatStringToHandle(200, 420, 1.0, 1.0, WHITE, fonthandle, "どれを取りますか？");
		break;
	default:
		break;
	}
}

//返り値は「そのカードの手札のなかで何番目に置いてあるか」。札のインデックスではない
int VSCPGameScene::Select() {
	if (teban == 1) {//CPU's turn
		//TODO
		//候補
		std::vector<byte> playerplace_candidate;

		//もし手札に同じ月の札が２枚あり、かつ場札にあるならその月を優先
		playerplace_candidate=DiscardSameMonth();
		if (playerplace_candidate.size() != 0) {
			return FilterCandidate(playerplace_candidate);
		}
		
		//場札のうちの点数の大きい方を使う:kindが20ならそれ、そうでなければ10->5->0と優先度合いを下げる//場札じゃなくてプレイヤーの札になっとるTODO
		return FieldBigger();

		//自分が何の役を目指すかに沿う:int nerauyakuを作っておく、nerauyakuは4T目終了時点で最も近いものに決定
		//TowardYaku();
	}
	else if (click_left == 1) {//player's turn
		if (teban == 0) {
			if (yblank + (cardheight + yspace) * 3 <= mousey && mousey <= yblank + (cardheight + yspace) * 3 + cardheight) {//if mousecursor is near player1 area
				for (int i = 0; i < (signed int)player[0].index_hold.size(); i++) {
					if (SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 3 * (cardwidth + xspace) + i * (cardwidth + xspace) <= mousex && mousex <= SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 3 * (cardwidth + xspace) + i * (cardwidth + xspace) + cardwidth) {
						return i;
					}
				}
			}
		}
	}
	return -1;//still no card selected
}

std::vector<byte> VSCPGameScene::DiscardSameMonth(){
	int month_saved[8];//一時置き場、配られる札の数
	for (int i = 0; i < player[1].index_hold.size(); i++) {
		month_saved[i] = card[player[1].index_hold[i]].month;
	}
	//quick sort
	sort(month_saved, 8);
	
	//手札に同じ月の札が複数あるか判定
	std::vector<byte>pp_candidate;//playerplace_candidateへ帰る
	int month_detected = 0;//month_savedをループで調べたとき、一枚前の札の月：month_saved_saved
	for (int i = 0; i < 8; i++) {//month_savedの何番目か
		if (month_detected == month_saved[i]) {//月を順番に並べた手札で一つ前の札と月が同じ＝＝同じ月の札が複数ある//手札が一枚でも減ると-858993460(初期化だけした値)が入る
			//場札の月と一致しているか
			for (int j = 0; j < field.size(); j++) {//手札のカードを一枚でも出した後だと配列に-858993460が入り、これを参照することになってしまっている
				if (card[field[j]].month == month_detected) {
					//一致していればその月の手札を全部候補に
					for (int k = 0; k < player[1].index_hold.size(); k++) {//kは手札の何番目か
						if (card[player[1].index_hold[k]].month == month_detected) {
							pp_candidate.push_back(k);
						}
					}
				}
			}
		}
		else {
			month_detected = month_saved[i];//次の月へ
		}
	}
	return pp_candidate;
}

//int VSCPGameScene::TowardYaku(){if(nerauyaku == kou)...};

byte VSCPGameScene::FilterCandidate(std::vector<byte> pp_c) {//候補を絞り込む。点数の大きいもの・光札のある月
	//得点の高い方を//TODO桜のタンと桜のカスで後者の方とりおった
	for (int l = 0; l < pp_c.size(); l++) {
		if (card[player[1].index_hold[pp_c[l]]].kind == 20)return pp_c[l];
	}
	for (int l = 0; l < pp_c.size(); l++) {
		if (card[player[1].index_hold[pp_c[l]]].kind == 10)return pp_c[l];
	}
	for (int l = 0; l < pp_c.size(); l++) {
		if (card[player[1].index_hold[pp_c[l]]].kind == 5)return pp_c[l];
	}
	//カス札しかないなら、光札のある月をねらう
	for (int i = 0; i < pp_c.size(); i++) {
		if (card[player[1].index_hold[pp_c[i]]].month == 1
			|| card[player[1].index_hold[pp_c[i]]].month == 3
			|| card[player[1].index_hold[pp_c[i]]].month == 8
			|| card[player[1].index_hold[pp_c[i]]].month == 11
			|| card[player[1].index_hold[pp_c[i]]].month == 12) {
			return pp_c[i];
		}
	}
	//カス札のみで睦月・弥生・葉月・霜月・師走もないなら最初の候補札でいいや
	return pp_c[0];
}
byte VSCPGameScene::FieldBigger() {
	//得点の高い方を
	for (int i = 0; i < field.size(); i++) {
		if (card[field[i]].kind == 20) {
			for (int j = 0; j < player[1].index_hold.size(); j++) {
				if(card[player[1].index_hold[j]].month == card[field[i]].month)return j;
			}
		}
	}
	for (int i = 0; i < field.size(); i++) {
		if (card[field[i]].kind == 10) {
			for (int j = 0; j < player[1].index_hold.size(); j++) {
				if (card[player[1].index_hold[j]].month == card[field[i]].month)return j;
			}
		}
	}
	for (int i = 0; i < field.size(); i++) {
		if (card[field[i]].kind == 5) {
			for (int j = 0; j < player[1].index_hold.size(); j++) {
				if (card[player[1].index_hold[j]].month == card[field[i]].month)return j;
			}
		}
	}
	for (int i = 0; i < field.size(); i++) {//カス札
		for (int j = 0; j < player[1].index_hold.size(); j++) {
			if (card[player[1].index_hold[j]].month == card[field[i]].month)return j;
		}
	}
	//月が一致しないので捨てることになる
	//場札に光札のある月を捨てたくない（盗られる可能性あるから）
	for (int i = 0; i < player[1].index_hold.size(); i++) {
		if (card[player[1].index_hold[i]].month != 1
			&& card[player[1].index_hold[i]].month != 3
			&& card[player[1].index_hold[i]].month != 8
			&& card[player[1].index_hold[i]].month != 11
			&& card[player[1].index_hold[i]].month != 12) {
			return i;
		}
	}
	//カス札のみで睦月・弥生・葉月・霜月・師走しかないなら最初の候補札でいいや
	return 0;
}

//場札に同じ月の札が複数あった場合に行う選択
int VSCPGameScene::Choose() {
	if (teban == 1) {//CPU's turn
		//TODO
		return samemonthcard[0];
	}
	if (click_left == 1) {//player'sturn
		if (teban == 0) {
			for (int i = 0; i < (signed int)samemonthcard.size(); i++) {
				int num = samemonthcard[i];
				if (num < 10) {
					if ((SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 1 * (cardwidth + xspace) + (num / 2) * (cardwidth + xspace)) <= mousex && mousex <= (SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 1 * (cardwidth + xspace) + (num / 2) * (cardwidth + xspace) + cardwidth) && yblank + (1 + num % 2) * (cardheight + yspace) <= mousey && mousey <= yblank + (1 + num % 2) * (cardheight + yspace) + cardheight) {//if choosed
						return num;
					}
				}
				else if (num <= 10 && num < 12) {
					if ((SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 2 * (cardwidth + xspace) + (num / 2) * (cardwidth + xspace)) <= mousex && mousex <= (SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 1 * (cardwidth + xspace) + (num / 2) * (cardwidth + xspace) + cardwidth) && yblank + (1 + num % 2) * (cardheight + yspace) <= mousey && mousey <= yblank + (1 + num % 2) * (cardheight + yspace) + cardheight) {//if choosed
						return num;
					}
				}
				else {
					if ((SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 1 * (cardwidth + xspace) + (num / 2) * (cardwidth + xspace)) <= mousex && mousex <= (SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 1 * (cardwidth + xspace) + (num / 2) * (cardwidth + xspace) + cardwidth) && yblank + (1 + num % 2) * (cardheight + yspace) <= mousey && mousey <= yblank + (1 + num % 2) * (cardheight + yspace) + cardheight) {//if choosed
						return num;
					}
				}
			}
		}
	}
	return -1;
}

/*
タイトル画面からこちらに来るのはいいのだが、一回リザルト画面に行くとSceneID::GAMEでつくってしまう
nextsceneidを持っておくか->bool isvscpで茶を濁す


CPU AI

case KOIKOI:
	//相手の得た札がリーチじゃない：光札２枚未満、タネ４枚未満、タン４枚未満、カス８枚未満ならこいこい
Select(){
	//もし手札に同じ月の札が２枚あり、かつ場札にあるなら
	////それの点数の大きい方を使う
	//候補が存在するなら判定してreturn;

	//自分が何の役を目指すかに沿う:int nerauyakuを作っておく、nerauyakuは4T目終了時点で最も近いものに決定：if(nerauyaku == kou)...
	//候補が存在するなら判定してreturn;

	//場札・手札を見て最も得点が高いやつ
	//カス札しかないなら光札を優先：松・桜・芒・柳・桐
	return 0;
}

Choose(){
	//自分が何の役を目指すかに沿う:int nerauyakuを作っておく、nerauyakuは4T目終了時点で最も近いものに決定
	//
	return;

	//選択札を見て最も得点が高いやつ
	return;
}



「候補をvectorに数個選ぶ→得点の高いor光札」という形式に抽象化できるのでは？？？


0221
KOIKOI判定でクリック判定を画面中央を境になるように
Add #ifdef _DEBUG in Draw() Cards Player1 holds
in VSCPGameScene::Choose(),made return properly
0223
FieldBigger(),Change kasuhudaProc 
Change unevitably discarding case algolrthm
FilterCandidate(), modify designated card index
0224
yaku_reach
*/