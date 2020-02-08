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
		//TODO
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
		DrawBox(180, 50, 460, 460, BLACK, TRUE);
		yaku = player[teban].yaku;

		DrawExtendFormatStringToHandle(100, 30, 1.0, 1.0, WHITE, fonthandle, "手番=%d  役=%d", teban, yaku);

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
*/