#include"utility.h"
#include"GameScene.h"
#include"Choko.h"

//���������e�D�ɁA�ꌎ�̎l����0����3,�񌎂̎l����4����7,.....�\�񌎂̎l����44����47�ƁA0����47�̃C���f�b�N�X������U���Ă���

GameScene::GameScene(int _gamenum, int _teban, int _score_player0, int _score_player1) {

	CardDataLoad();

	graph_back = LoadGraph("graphic/a0007_001048_m.png");

	//�e�D��0~47�̔ԍ�������U���Ă���A"�u���̔ԍ��v.png"�̉摜�t�@�C�������݂��邱�Ƃ��O��B
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
	//�Z���\������
	//��D�I��Select()
	////�̂ĎD�Ə�D�����킹�鏈��Trash()
	////�R�D��������ď�D�����킹�鏈��DecktoField()
	//�I������

	//�`�扉�o���͍X�V���Ȃ�
	if (isdrawing)return;

	//�����������܂����H�������͂ǂ̎D��I�����܂����H�Ƃ����\�����o�Ă���
	int num_choosed = -1;//switch~case��������Local�ϐ��̏�����������Ȃ��̂ł����ɁGONLY"case MULTICHOICE"
	switch (cellkind)
	{
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
		num_choosed = Choose();

		if (num_choosed != -1) {
			//TODO:�ǂ�����܂����H�ƌ����Ă���ɂ��ւ�炸�������v���Ȃ����̂�I�ԂƃG���[

			//Trash�֐�����R�s�y
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
			return;//�\�����o�Ă���Ԃ͎�D�I���ɂ����Ȃ�
		}
		break;
	case Cellkind::NONE:
	default:
		break;
	}

	//��D�I��
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

	//���Ȃ��ŏI��
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

	//todo:��ޕʂɃ\�[�g;���
	for (unsigned int i = 0; i < (player[0].index_get).size(); i++) {
		DrawGraph(30 + (i % 12) * 21, 320 + 70 * (i / 12), card[player[0].index_get[i]].graph, TRUE);//30�͓K��
	}

	for (unsigned int i = 0; i < (player[1].index_get).size(); i++) {
		DrawGraph(640 - cardwidth - 30 - (i % 12) * 21, 320 + 70 * (i / 12), card[player[1].index_get[i]].graph, TRUE);
	}

	if (cellkind == Cellkind::KOIKOI) {
		DrawBox(180, 50, 460, 460, BLACK, TRUE);
		unsigned int yaku = player[teban].yaku;

		DrawExtendFormatStringToHandle(100, 30, 1.0, 1.0, WHITE, fonthandle, "���=%d  ��=%d", teban, yaku);

		int nowyakunum = 0;
		//&���Z�q�͏��ʂ�==���Ⴂ�B���ʂ�t����B
		if ((yaku & 0x0001) == 0x0001) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�J�X�@�@%d��", 1 + player[teban].num_kasu - 10); nowyakunum++; }
		if ((yaku & 0x0002) == 0x0002) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�^���@�@%d��", 1 + player[teban].num_tan - 5); nowyakunum++; }
		if ((yaku & 0x0004) == 0x0004) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�^�l�@�@%d��", 1 + player[teban].num_tane - 5); nowyakunum++; }
		if ((yaku & 0x0008) == 0x0008) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�O���@�@%d��", 5); nowyakunum++; }
		if ((yaku & 0x0010) == 0x0010) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�J�l���@�@%d��", 8); nowyakunum++; }
		if ((yaku & 0x0020) == 0x0020) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�l���@�@%d��", 10); nowyakunum++; }
		if ((yaku & 0x0040) == 0x0040) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�܌��@�@%d��", 15); nowyakunum++; }
		if ((yaku & 0x0080) == 0x0080) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�ԃ^���@�@%d��", 5 + player[teban].num_tan - 3); nowyakunum++; }
		if ((yaku & 0x0100) == 0x0100) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�^���@�@%d��", 5 + player[teban].num_tan - 3); nowyakunum++; }
		if ((yaku & 0x0200) == 0x0200) { DrawExtendFormatStringToHandle(200, 70 + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�������@�@%d��", 5); nowyakunum++; }

		DrawExtendFormatStringToHandle(200, 400, 1.0, 1.0, WHITE, fonthandle, "�����������܂����H");
		DrawExtendFormatStringToHandle(200, 430, 1.0, 1.0, WHITE, fonthandle, "�͂��@�@�@������");
	}
	else if (cellkind == Cellkind::MULTICHOICE) {
		DrawBox(180, 400, 460, 460, BLACK, TRUE);
		DrawExtendFormatStringToHandle(200, 420, 1.0, 1.0, WHITE, fonthandle, "�ǂ�����܂����H");
	}
	if (keystate_2 == 1) {
		int i = 0;
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
			if (chardata[i] == ';' || chardata[i] == '\n') {
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

//�R�D��������ď�D�ƏƂ炵���킹�鏈���S��
void GameScene::DeckToField() {
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

/*�^���T���^���U
�ʓ|������num_kasu�ɕۑ����Ė𔻒��̍����Œ��ׂ��B�G�B

�����R�����g��t����
�E�V�[�����ǂ���邩�A����
�E�𔻒�̗���A�ϐ�yaku

�J�[�h���V�[���O�ɐÎ~������
*/