#include"utility.h"
#include"GameScene.h"

//���������e�D�ɁA�ꌎ�̎l����0����3,�񌎂̎l����4����7,.....�\�񌎂̎l����44����47�ƁA0����47�̃C���f�b�N�X������U���Ă���

void GameScene::Init(int _gamenum, int _teban, int _score_player0, int _score_player1) {

	CardDataLoad();

	graph_back = LoadGraph("graphic/BG.png");

	//�e�D��0~47�̔ԍ�������U���Ă���A"�u���̔ԍ��v.png"�̉摜�t�@�C�������݂��邱�Ƃ��O��B
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
	//�Z���\������
	//��D�I��Select()
	////�̂ĎD�Ə�D�����킹�鏈��Trash()
	////�R�D��������ď�D�����킹�鏈��DecktoField()
	//�I������

	//�`�扉�o���͍X�V���Ȃ�
	if (isdrawing)return;

	//�����������܂����H�������͂ǂ̎D��I�����܂����H�Ƃ����\�����o�Ă���
	int fieldplace_choosed = -1;//switch~case��������Local�ϐ��̏�����������Ȃ��̂ł����ɁGONLY"case MULTICHOICE"
	switch (cellkind)
	{
	case Cellkind::NONE:
		break;
	case Cellkind::KOIKOI:
		if (click_left == 1) {
			//��������
			if (mousex >= 0 && mousex < 320 && mousey >= 0 && mousey < 480) {
				cellkind = Cellkind::NONE;
				teban ^= 1;
			}
			//����
			else if (mousex >= 320 && mousex < 640 && mousey >= 0 && mousey < 480) {
				player[teban].score += player[teban].nowscore;
				player[(teban + 1) % 2].score -= player[teban].nowscore;
				SceneManager::GetInstance()->CreateScene(SceneID::RESULT, SceneLayer::UPPER, gamenum, teban, player[0].score, player[1].score);
				return;
			}
		}
		return;//�\�����o�Ă���Ԃ͎�D�I���ɂ����Ȃ�
	case Cellkind::MULTICHOICE:
		fieldplace_choosed = Choose();

		if (fieldplace_choosed != -1) {
			//Trash�֐�����R�s�y
			//Get the card that player selected
			player[teban].index_get.push_back(player[teban].index_hold[holdplace_selected]);//�{���͂�����holdplace_selected�܂�v���C���[���I��ł������C���f�b�N�X������Ȃ���΂Ȃ�Ȃ��B�����������������蒼��P���[�v���邹����Select()�ɂ��-1������
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
		return;//�\�����o�Ă���Ԃ͎�D�I���ɂ����Ȃ�
	default:
		break;
	}

	//��D�I��
	holdplace_selected = Select();//normally returns -1, not 0

	if (holdplace_selected != -1) {

		if (Trash(holdplace_selected) != 0)return;//�G���[���o���蕡���I���ɂȂ���������P���[�v����

		DeckDraw();

		if (YakuHantei() != 0) {
			cellkind = Cellkind::KOIKOI;
			return;
		}

		teban ^= 1;
	}

	//���Ȃ��ŏI��
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
	for (int i = 0; i < (signed int)player[1].index_hold.size(); i++) {//CP�Ȃ̂Ŏ�D�͌����Ȃ�
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
		DrawBox(40, yblank + (cardheight + yspace) * 1, 250/*not to overlap field*/, yblank + (cardheight + yspace) * 2 + cardheight, BLACK, TRUE);//x���W�͉�ʒ�������ɐ^�񒆑���
		yaku = player[teban].yaku;

		//for debug
		DrawExtendFormatStringToHandle(SCREEN_WIDTH / 2 - 120 + 45, 30, 1.0, 1.0, WHITE, fonthandle, "���=%d  ��=%d", teban, yaku);

		nowyakunum = 0;//���݂ł��Ă�����̐��B�`��̈ʒu���������˂�
		//&���Z�q�͏��ʂ�==���Ⴂ�B���ʂ�t����B
		if ((yaku & 0x0001) == 0x0001) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�J�X�@�@%d��", 1 + player[teban].num_kasu - 10); nowyakunum++; }
		if ((yaku & 0x0002) == 0x0002) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�^���@�@%d��", 1 + player[teban].num_tan - 5); nowyakunum++; }
		if ((yaku & 0x0004) == 0x0004) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�^�l�@�@%d��", 1 + player[teban].num_tane - 5); nowyakunum++; }
		if ((yaku & 0x0008) == 0x0008) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�O���@�@%d��", 5); nowyakunum++; }
		if ((yaku & 0x0010) == 0x0010) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�J�l���@�@%d��", 8); nowyakunum++; }
		if ((yaku & 0x0020) == 0x0020) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�l���@�@%d��", 10); nowyakunum++; }
		if ((yaku & 0x0040) == 0x0040) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�܌��@�@%d��", 15); nowyakunum++; }
		if ((yaku & 0x0080) == 0x0080) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�ԃ^���@�@%d��", 5 + player[teban].num_tan - 3); nowyakunum++; }
		if ((yaku & 0x0100) == 0x0100) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�^���@�@%d��", 5 + player[teban].num_tan - 3); nowyakunum++; }
		if ((yaku & 0x0200) == 0x0200) { DrawExtendFormatStringToHandle(40, 10 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�������@�@%d��", 5); nowyakunum++; }

		DrawExtendFormatStringToHandle(40+20, yblank + (cardheight + yspace) * 2, 1.0, 1.0, WHITE, fonthandle, "�����������܂����H");
		DrawExtendFormatStringToHandle(40+20, yblank + (cardheight + yspace) * 2 + 30, 1.0, 1.0, WHITE, fonthandle, "�͂��@�@�@������");
		break;
	case GameScene::Cellkind::MULTICHOICE:
		DrawBox(180, 400, 460, 460, BLACK, TRUE);
		DrawExtendFormatStringToHandle(200, 420, 1.0, 1.0, WHITE, fonthandle, "�ǂ�����܂����H");
		break;
	default:
		break;
	}
}

int GameScene::CardDataLoad() {

	static const int LIMIT_OF_DIGITS = 4;//���͂����x��pattern�̌����̌��E�{�P;null������
	char chardata[LIMIT_OF_DIGITS];

	int fp = FileRead_open("hanahuda.csv");
	if (fp == NULL)return -1;

	int datanum = 0, ordernum = 0;
	while (true) {
		//��P�ʂɕ���
		for (int i = 0; i<LIMIT_OF_DIGITS; i++) {
			chardata[i] = FileRead_getc(fp);//FileRead�n�̊֐��͓����Ƀ|�C���^�������Ă��āA�Ăяo���x�Ɍ����Ȃ��Ƃ���Ń|�C���^������Ă�������
			if (chardata[i] == ',' || chardata[i] == '\n') {
				chardata[i] = '\0';
				break;
			}
			if (chardata[i] == EOF)goto EXFILE;
		}
		//����𐮐��^or���������_�^�ɕϊ���enemydata�ɓ����
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

	bool isused[48] = {false};//{false,}48�͖ʓ|

	for (int i = 0; i < 48; i++) {
		isused[i] = false;
	}

	for (int i = 0; i < 48; i++) {
		//�g�p����ĂȂ��J�[�h�̔ԍ��������_���ŒT�����Ă�G�g�p����Ă�����ׂ̃J�[�h�𒲂ׂ�
		int index = GetRand(47);
		while (isused[index] == true) {
			index = (index + 1) % 48;
		}
		isused[index] = true;

		deck.push_back(index);
	}
}

//�z�肩�����������G�ԎD�̃��[����A��D�ɓ�������3������ƃQ�[�������܂��i�܂Ȃ��B���������邽�߂̔���
bool GameScene::IsDealOK() {
	//field�ɓn���W���̂�����������3���ɂȂ��Ă����烊�Z�b�g
	byte month_exist[12] = {};//����Łu�������v�ɂȂ��Ă���̂��낤��->�Ȃ��Ƃ�

	for (int i = 0; i < 8; i++) {
		//field�ɓ���e�D�̃C���f�b�N�X�͊���(���̃J�X�D���O�Ƃ��ĂS�V�̋˂ɖP���܂Ŋ���U���Ă���)�̂ŁA��������o����
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

//�Ԃ�l�́u���̃J�[�h�̎�D�̂Ȃ��ŉ��Ԗڂɒu���Ă��邩�v�B�D�̃C���f�b�N�X�ł͂Ȃ�
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

//�v���C���[���I�񂾎D�ɑ΂��鏈���S��
int GameScene::Trash(int index) {

	//samemonthcard�ɂ́A���̎D��field�ŉ��ԖڂȂ̂�������
	samemonthcard.clear();

	//��D�𑖍����āA������v����D���������邩�m�F
	for (unsigned int i = 0; i < field.size(); i++) {
		if (card[player[teban].index_hold[index]].month == card[field[i]].month) {//����v
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

//��D�ɓ������̎D�������������ꍇ�ɍs���I��
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

//�R�D��������ď�D�ƏƂ炵���킹�鏈���S��
void GameScene::DeckDraw() {
	for (unsigned int i = 0; i < field.size(); i++) {
		if (card[deck.at(0)].month == card[field[i]].month) {//����v
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
	if (hikari == 3 && !is43) {//�O��
		nowscore += 5;
		yaku += 0x0008;
	}
	else if (hikari == 4) {
		if (is43) {
			nowscore += 8;//�J�l��
			yaku += 0x0010;
		}
		else {
			nowscore += 10;//�l��
			yaku += 0x0020;
		}
	}
	else if (hikari == 5) {
		nowscore += 15;//�܌�
		yaku += 0x0040;
	}
	
	if (num_akatan == 3) {
		nowscore += 5 + tan - 3;//�ԒZ
		yaku += 0x0080;
	}
	if (num_aotan == 3) {
		nowscore += 5 + tan - 3;//�Z
		yaku += 0x0100;
	}
	if (num_inosikachou == 3) {
		nowscore += 5;//������
		yaku += 0x0200;
	}
	/*
	if (is35) {
		if(is11){
			nowscore += 3;//�Ԍ���
			yaku += 0x0400;
		}
		if(is31){
			nowscore += 3;//������
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

/*�^���T���^���U
�ʓ|������num_kasu�ɕۑ����Ė𔻒��̍����Œ��ׂ��B�G�B

�����u�����`���ł̍X�V��Commit
����X�V�����Ă���I���Ă̂�Pull Request
�������낤�X�V���󂯓����̂�merge


fieldplace_choosed=samemonthcard�ɂ́A���̎D��field�ŉ��ԖڂȂ̂�������
holdplace_selected�́u���̃J�[�h�̎�D�̂Ȃ��ŉ��Ԗڂɒu���Ă��邩�v
�ǂ�����D�̃C���f�b�N�X�ł͂Ȃ��B�o�O�̉�����
*/
