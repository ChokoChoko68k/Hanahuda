#include"utility.h"
#include"VSCPGameScene.h"

//Overriding GameScene
void VSCPGameScene::Update() {
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
		if (teban == 1) {//CPU's turn
			//TODO:YakuHanteiの情報をプレイヤーが持っているようにするe.g.赤短何枚とか
			/*for (int i = 0; i < player[0].index_get.size(); i++) {
				if (card[player[0].index_get[i]].kind == 20) {

				}
			}*/
			//勝負
			player[teban].score += player[teban].nowscore;
			player[(teban + 1) % 2].score -= player[teban].nowscore;
			SceneManager::GetInstance()->CreateScene(SceneID::RESULT, SceneLayer::UPPER, gamenum, teban, player[0].score, player[1].score);
			return;
		}

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

void VSCPGameScene::Draw() {
	//Draw BackGroundImage
	DrawGraph(0, 0, graph_back, TRUE);

	for (unsigned int i = 0; i < (player[0].index_hold).size(); i++) {
		DrawGraph(xblank + (cardwidth + xspace) * (i % 2), yblank + (cardheight + yspace) * (i / 2), card[player[0].index_hold[i]].graph, TRUE);
	}

	for (unsigned int i = 0; i < field.size(); i++) {
		DrawGraph(160 + xblank + (cardwidth + 23) * (i % 4), 40 + yblank + (cardheight + 12) * (i / 4), card[field[i]].graph, TRUE);
	}

	for (unsigned int i = 0; i < (player[1].index_hold).size(); i++) {//CPなので手札は見せない
		DrawGraph(500 + xblank + (cardwidth + xspace) * (i % 2), yblank + (cardheight + yspace) * (i / 2), graph_ura, TRUE);
	}

	//Draw Every Player's gotten cards todo:月別にソート;後回し
	for (unsigned int i = 0; i < (player[0].index_get).size(); i++) {
		DrawGraph(30 + (i % 12) * 21, 320 + 70 * (i / 12), card[player[0].index_get[i]].graph, TRUE);//30は適当
	}
	for (unsigned int i = 0; i < (player[1].index_get).size(); i++) {
		DrawGraph(640 - cardwidth - 30 - (i % 12) * 21, 320 + 70 * (i / 12), card[player[1].index_get[i]].graph, TRUE);
	}

	//Draw Cell
	int nowyakunum = 0;//use only in KOIKOI
	unsigned int yaku = player[teban].yaku;//use only in KOIKOI
	switch (cellkind)
	{
	case GameScene::Cellkind::NONE:
		break;
	case GameScene::Cellkind::KOIKOI:
		DrawBox(SCREEN_WIDTH / 2 - 200, 50, SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT - 50, BLACK, TRUE);//x座標は画面中央を基準に真ん中揃え
		yaku = player[teban].yaku;

		DrawExtendFormatStringToHandle(SCREEN_WIDTH / 2 - 120+45, 30, 1.0, 1.0, WHITE, fonthandle, "手番=%d  役=%d", teban, yaku);

		nowyakunum = 0;
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



		//もし手札に同じ月の札が２枚あり、かつ場札にあるなら
		int month_saved[8];//一時置き場、配られる札の数
		for (int i = 0; i < player[1].index_hold.size(); i++) {
			month_saved[i] = card[player[1].index_hold[i]].month;
		}
		//quick sort
		sort(month_saved, 8);

		//手札に同じ月の札が複数あるか判定
		for (int i = 0; i < 8; i++) {//month_savedの何番目か
			int month_detected = 0;
			if (month_detected == month_saved[i]) {//順番に並べた手札で一つ前の札と月が同じ＝＝同じ月の札が複数ある
				//場札の月と一致しているか
				for (int j = 0; j < field.size(); j++) {
					if (card[field[j]].month == month_detected) {
						//一致していればその月の手札を全部候補に
						for (int k = 0; k < player[1].index_hold.size(); k++) {//kは手札の何番目か
							if (card[player[1].index_hold[k]].month == month_detected) {
								playerplace_candidate.push_back(k);
							}
						}
					}
				}
			}
			else {
				month_detected = month_saved[i];
			}
		}
		//候補のうちの点数の大きい方を使う:kindが20ならそれ、そうでなければ10->5->0と優先度合いを下げる
		if (playerplace_candidate.size() != 0) {
			for (int l = 0; l < playerplace_candidate.size(); l++) {
				if (card[playerplace_candidate[l]].kind == 20)return l;
			}
			for (int l = 0; l < playerplace_candidate.size(); l++) {
				if (card[playerplace_candidate[l]].kind == 10)return l;
			}
			for (int l = 0; l < playerplace_candidate.size(); l++) {
				if (card[playerplace_candidate[l]].kind == 5)return l;
			}
			//カス札しかないなら、光札のある月をねらう
			for (int i = 0; i < playerplace_candidate.size(); i++) {
				if (card[playerplace_candidate[i]].month == 1
					|| card[playerplace_candidate[i]].month == 3
					|| card[playerplace_candidate[i]].month == 8
					|| card[playerplace_candidate[i]].month == 11
					|| card[playerplace_candidate[i]].month == 12) {
					return i;
				}
			}
			//睦月・弥生・葉月・霜月・師走もないなら最初の候補札で
			return playerplace_candidate[0];
		}





		//自分が何の役を目指すかに沿う:int nerauyakuを作っておく、nerauyakuは4T目終了時点で最も近いものに決定
		//if(nerauyaku == kou)...
		//return;
		/*/候補のうちの点数の大きい方を使う:kindが20ならそれ、そうでなければ10->5->0と優先度合いを下げる
		if (playerplace_candidate.size() != 0) {
			for (int l = 0; l < playerplace_candidate.size(); l++) {
				if (card[playerplace_candidate[l]].kind == 20)return l;
			}
			for (int l = 0; l < playerplace_candidate.size(); l++) {
				if (card[playerplace_candidate[l]].kind == 10)return l;
			}
			for (int l = 0; l < playerplace_candidate.size(); l++) {
				if (card[playerplace_candidate[l]].kind == 5)return l;
			}
			//カス札しかないなら、光札のある月をねらう
			for (int i = 0; i < playerplace_candidate.size(); i++) {
				if (card[playerplace_candidate[i]].month == 1
					|| card[playerplace_candidate[i]].month == 3
					|| card[playerplace_candidate[i]].month == 8
					|| card[playerplace_candidate[i]].month == 11
					|| card[playerplace_candidate[i]].month == 12) {
					return i;
				}
			}
			//睦月・弥生・葉月・霜月・師走もないなら最初の候補札で
			return playerplace_candidate[0];
		}*/






		//場札・手札を見て点数の大きい方を使う
		for (int i = 0; i < field.size(); i++) {
			if (card[field[i]].kind == 20) {
				for (int k = 0; k < player[1].index_hold.size(); k++) {//kは手札の何番目か
					if (card[player[1].index_hold[k]].month == card[field[i]].month) {
						return k;
					}
				}
			}
		}
		for (int l = 0; l < player[1].index_hold.size(); l++) {
			if (card[player[1].index_hold[l]].kind == 20)return l;
		}
		for (int i = 0; i < field.size(); i++) {
			if (card[field[i]].kind == 10) {
				for (int k = 0; k < player[1].index_hold.size(); k++) {//kは手札の何番目か
					if (card[player[1].index_hold[k]].month == card[field[i]].month) {
						return k;
					}
				}
			}
		}
		for (int l = 0; l < player[1].index_hold.size(); l++) {
			if (card[player[1].index_hold[l]].kind == 10)return l;
		}
		for (int i = 0; i < field.size(); i++) {
			if (card[field[i]].kind == 5) {
				for (int k = 0; k < player[1].index_hold.size(); k++) {//kは手札の何番目か
					if (card[player[1].index_hold[k]].month == card[field[i]].month) {
						return k;
					}
				}
			}
		}
		for (int l = 0; l < player[1].index_hold.size(); l++) {
			if (card[player[1].index_hold[l]].kind == 5)return l;
		}
		//カス札しかないなら、光札のある月をねらう
		for (int i = 0; i < player[1].index_hold.size(); i++) {
			if (card[player[1].index_hold[i]].month == 1
				|| card[player[1].index_hold[i]].month == 3
				|| card[player[1].index_hold[i]].month == 8
				|| card[player[1].index_hold[i]].month == 11
				|| card[player[1].index_hold[i]].month == 12) {
				return i;
			}
		}
		//睦月・弥生・葉月・霜月・師走もないなら最初の札で
		return 0;
	}
	if (click_left == 1) {
		if (teban == 0) {//player's turn
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
	}
	return -1;//still no card selected
}

//場札に同じ月の札が複数あった場合に行う選択
int VSCPGameScene::Choose() {
	if (teban == 1) {//CPU's turn
		//TODO
		return 0;
	}
	if (click_left == 1) {
		if (teban == 0) {//player's turn
			for (int i = 0; i < (signed int)samemonthcard.size(); i++) {
				int num = samemonthcard[i];
				if (mousex >= 160 + xblank + (cardwidth + 23) * (num % 4) && mousex < 160 + xblank + (cardwidth + 23) * (num % 4) + cardwidth) {
					if (mousey >= 40 + yblank + (cardheight + 12) * (num / 4) && mousey < 40 + yblank + (cardheight + 12) * (num / 4) + cardheight) {
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

*/