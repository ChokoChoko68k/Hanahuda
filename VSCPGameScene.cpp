#include"utility.h"
#include"VSCPGameScene.h"

//Overriding GameScene
int VSCPGameScene::Update() {
	//�Z���\������
	//��D�I��Select()
	////�̂ĎD�Ə�D�����킹�鏈��Trash()
	////�R�D��������ď�D�����킹�鏈��DecktoField()
	//�I������

	//�`�扉�o���͍X�V���Ȃ�
	if (isdrawing)return 0;

	//�����������܂����H�������͂ǂ̎D��I�����܂����H�Ƃ����\�����o�Ă���
	int fieldplace_choosed = -1;//switch~case��������Local�ϐ��̏�����������Ȃ��̂ł����ɁGONLY"case MULTICHOICE"
	switch (cellkind)
	{
	case Cellkind::NONE:
		break;
	case Cellkind::KOIKOI:
		if (teban == 1) {//CPU's turn
			//��������
			//���肪���[�`�łȂ��A���Ō�̃^�[���ȊO����
			if (player[0].yaku_reach == 0x0000 && player[1].index_hold.size() > 1) {
				cellkind = Cellkind::NONE;
				teban ^= 1;
			}
			//����
			else {
				player[teban].score += player[teban].nowscore;
				player[(teban + 1) % 2].score -= player[teban].nowscore;
				SceneManager::GetInstance()->CreateScene(SceneID::RESULT, SceneLayer::UPPER, gamenum, teban, player[0].score, player[1].score);
				return 0;
			}
		}
		else if (teban == 0) {//Player's turn
			if (click_left == 1) {
				//��������
				if (mousex <= SCREEN_WIDTH / 2) {//�������N���b�N
					cellkind = Cellkind::NONE;
					teban ^= 1;
				}
				//����
				else if (mousex > SCREEN_WIDTH / 2) {//�E�����N���b�N
					player[teban].score += player[teban].nowscore;
					player[(teban + 1) % 2].score -= player[teban].nowscore;
					SceneManager::GetInstance()->CreateScene(SceneID::RESULT, SceneLayer::UPPER, gamenum, teban, player[0].score, player[1].score);
					return 0;
				}
			}
		}
		return 0;//�\�����o�Ă���Ԃ͎�D�I���ɂ����Ȃ�
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
				return 0;
			}

			teban ^= 1;
			cellkind = Cellkind::NONE;
		}
		return 0;//�\�����o�Ă���Ԃ͎�D�I���ɂ����Ȃ�
	default:
		break;
	}

	//��D�I��
	holdplace_selected = Select();//normally returns -1, not 0

	if (holdplace_selected != -1) {

		if (TrashorGet(holdplace_selected) != 0)return 0;//�G���[���o���蕡���I���ɂȂ���������P���[�v����

		DeckDraw();

		if (YakuHantei() != 0) {
			cellkind = Cellkind::KOIKOI;
			return 0;
		}

		teban ^= 1;
	}

	//���Ȃ��ŏI��
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
	for (int i = 0; i < (signed int)player[1].index_hold.size(); i++) {//CP�Ȃ̂Ŏ�D�͌����Ȃ�
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
		DrawBox(40, yblank + (cardheight + yspace) * 1, 40 + 300, yblank + (cardheight + yspace) * 2 + cardheight, BLACK, TRUE);//x���W�͉�ʒ�������ɐ^�񒆑���
		yaku = player[teban].yaku;

		nowyakunum = 0;//���݂ł��Ă�����̐��B�`��̈ʒu���������˂�
		//&���Z�q�͏��ʂ�==���Ⴂ�B���ʂ�t����B
		if ((yaku & 0x0001) == 0x0001) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�J�X�@�@%d��", 1 + player[teban].num_kasu - 10); nowyakunum++; }
		if ((yaku & 0x0002) == 0x0002) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�^���@�@%d��", 1 + player[teban].num_tan - 5); nowyakunum++; }
		if ((yaku & 0x0004) == 0x0004) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�^�l�@�@%d��", 1 + player[teban].num_tane - 5); nowyakunum++; }
		if ((yaku & 0x0008) == 0x0008) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�O���@�@%d��", 5); nowyakunum++; }
		if ((yaku & 0x0010) == 0x0010) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�J�l���@�@%d��", 8); nowyakunum++; }
		if ((yaku & 0x0020) == 0x0020) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�l���@�@%d��", 10); nowyakunum++; }
		if ((yaku & 0x0040) == 0x0040) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�܌��@�@%d��", 15); nowyakunum++; }
		if ((yaku & 0x0080) == 0x0080) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�ԃ^���@�@%d��", 5 + player[teban].num_tan - 3); nowyakunum++; }
		if ((yaku & 0x0100) == 0x0100) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�^���@�@%d��", 5 + player[teban].num_tan - 3); nowyakunum++; }
		if ((yaku & 0x0200) == 0x0200) { DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 1 + 10/* 10 is between box ceil and string ceil*/ + nowyakunum * 30, 1.0, 1.0, WHITE, fonthandle, "�������@�@%d��", 5); nowyakunum++; }

		DrawExtendFormatStringToHandle(40 + 10, yblank + (cardheight + yspace) * 2, 1.0, 1.0, WHITE, fonthandle, "�����������܂����H");
		DrawBox(SCREEN_WIDTH / 2 - 60 - 10, yblank + (cardheight + yspace) * 2 + 30 - 10, SCREEN_WIDTH / 2 - 60 - 10 + 80, yblank + (cardheight + yspace) * 2 + 30 - 10 + 30, BLACK, TRUE);
		DrawExtendFormatStringToHandle(SCREEN_WIDTH / 2 - 60, yblank + (cardheight + yspace) * 2 + 30, 1.0, 1.0, WHITE, fonthandle, "�͂�");
		DrawBox(SCREEN_WIDTH / 2 + 60 - 10, yblank + (cardheight + yspace) * 2 + 30 - 10, SCREEN_WIDTH / 2 + 60 - 10 + 80, yblank + (cardheight + yspace) * 2 + 30 - 10 + 30, BLACK, TRUE);
		DrawExtendFormatStringToHandle(SCREEN_WIDTH / 2 + 60, yblank + (cardheight + yspace) * 2 + 30, 1.0, 1.0, WHITE, fonthandle, "������");
		break;
	case GameScene::Cellkind::MULTICHOICE:
		DrawBox(180, 400, 460, 460, BLACK, TRUE);
		DrawExtendFormatStringToHandle(200, 420, 1.0, 1.0, WHITE, fonthandle, "�ǂ�����܂����H");
		break;
	default:
		break;
	}
}

//�Ԃ�l�́u���̃J�[�h�̎�D�̂Ȃ��ŉ��Ԗڂɒu���Ă��邩�v�B�D�̃C���f�b�N�X�ł͂Ȃ�
int VSCPGameScene::Select() {
	if (teban == 1) {//CPU's turn
		//TODO
		//���
		std::vector<byte> playerplace_candidate;

		//������D�ɓ������̎D���Q������A����D�ɂ���Ȃ炻�̌���D��
		playerplace_candidate=DiscardSameMonth();
		if (playerplace_candidate.size() != 0) {
			return FilterCandidate(playerplace_candidate);
		}
		
		//��D�̂����̓_���̑傫�������g��:kind��20�Ȃ炻��A�����łȂ����10->5->0�ƗD��x������������//��D����Ȃ��ăv���C���[�̎D�ɂȂ��Ƃ�TODO
		return FieldBigger();

		//���������̖���ڎw�����ɉ���:int nerauyaku������Ă����Anerauyaku��4T�ڏI�����_�ōł��߂����̂Ɍ���
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
	int month_saved[8];//�ꎞ�u����A�z����D�̐�
	for (int i = 0; i < player[1].index_hold.size(); i++) {
		month_saved[i] = card[player[1].index_hold[i]].month;
	}
	//quick sort
	sort(month_saved, 8);
	
	//��D�ɓ������̎D���������邩����
	std::vector<byte>pp_candidate;//playerplace_candidate�֋A��
	int month_detected = 0;//month_saved�����[�v�Œ��ׂ��Ƃ��A�ꖇ�O�̎D�̌��Fmonth_saved_saved
	for (int i = 0; i < 8; i++) {//month_saved�̉��Ԗڂ�
		if (month_detected == month_saved[i]) {//�������Ԃɕ��ׂ���D�ň�O�̎D�ƌ������������������̎D����������//��D���ꖇ�ł������-858993460(���������������l)������
			//��D�̌��ƈ�v���Ă��邩
			for (int j = 0; j < field.size(); j++) {//��D�̃J�[�h���ꖇ�ł��o�����ゾ�Ɣz���-858993460������A������Q�Ƃ��邱�ƂɂȂ��Ă��܂��Ă���
				if (card[field[j]].month == month_detected) {
					//��v���Ă���΂��̌��̎�D��S������
					for (int k = 0; k < player[1].index_hold.size(); k++) {//k�͎�D�̉��Ԗڂ�
						if (card[player[1].index_hold[k]].month == month_detected) {
							pp_candidate.push_back(k);
						}
					}
				}
			}
		}
		else {
			month_detected = month_saved[i];//���̌���
		}
	}
	return pp_candidate;
}

//int VSCPGameScene::TowardYaku(){if(nerauyaku == kou)...};

byte VSCPGameScene::FilterCandidate(std::vector<byte> pp_c) {//�����i�荞�ށB�_���̑傫�����́E���D�̂��錎
	//���_�̍�������//TODO���̃^���ƍ��̃J�X�Ō�҂̕��Ƃ肨����
	for (int l = 0; l < pp_c.size(); l++) {
		if (card[player[1].index_hold[pp_c[l]]].kind == 20)return pp_c[l];
	}
	for (int l = 0; l < pp_c.size(); l++) {
		if (card[player[1].index_hold[pp_c[l]]].kind == 10)return pp_c[l];
	}
	for (int l = 0; l < pp_c.size(); l++) {
		if (card[player[1].index_hold[pp_c[l]]].kind == 5)return pp_c[l];
	}
	//�J�X�D�����Ȃ��Ȃ�A���D�̂��錎���˂炤
	for (int i = 0; i < pp_c.size(); i++) {
		if (card[player[1].index_hold[pp_c[i]]].month == 1
			|| card[player[1].index_hold[pp_c[i]]].month == 3
			|| card[player[1].index_hold[pp_c[i]]].month == 8
			|| card[player[1].index_hold[pp_c[i]]].month == 11
			|| card[player[1].index_hold[pp_c[i]]].month == 12) {
			return pp_c[i];
		}
	}
	//�J�X�D�݂̂Ŗr���E�퐶�E�t���E�����E�t�����Ȃ��Ȃ�ŏ��̌��D�ł�����
	return pp_c[0];
}
byte VSCPGameScene::FieldBigger() {
	//���_�̍�������
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
	for (int i = 0; i < field.size(); i++) {//�J�X�D
		for (int j = 0; j < player[1].index_hold.size(); j++) {
			if (card[player[1].index_hold[j]].month == card[field[i]].month)return j;
		}
	}
	//������v���Ȃ��̂Ŏ̂Ă邱�ƂɂȂ�
	//��D�Ɍ��D�̂��錎���̂Ă����Ȃ��i������\�����邩��j
	for (int i = 0; i < player[1].index_hold.size(); i++) {
		if (card[player[1].index_hold[i]].month != 1
			&& card[player[1].index_hold[i]].month != 3
			&& card[player[1].index_hold[i]].month != 8
			&& card[player[1].index_hold[i]].month != 11
			&& card[player[1].index_hold[i]].month != 12) {
			return i;
		}
	}
	//�J�X�D�݂̂Ŗr���E�퐶�E�t���E�����E�t�������Ȃ��Ȃ�ŏ��̌��D�ł�����
	return 0;
}

//��D�ɓ������̎D�������������ꍇ�ɍs���I��
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
�^�C�g����ʂ��炱����ɗ���̂͂����̂����A��񃊃U���g��ʂɍs����SceneID::GAME�ł����Ă��܂�
nextsceneid�������Ă�����->bool isvscp�Œ������


CPU AI

case KOIKOI:
	//����̓����D�����[�`����Ȃ��F���D�Q�������A�^�l�S�������A�^���S�������A�J�X�W�������Ȃ炱������
Select(){
	//������D�ɓ������̎D���Q������A����D�ɂ���Ȃ�
	////����̓_���̑傫�������g��
	//��₪���݂���Ȃ画�肵��return;

	//���������̖���ڎw�����ɉ���:int nerauyaku������Ă����Anerauyaku��4T�ڏI�����_�ōł��߂����̂Ɍ���Fif(nerauyaku == kou)...
	//��₪���݂���Ȃ画�肵��return;

	//��D�E��D�����čł����_���������
	//�J�X�D�����Ȃ��Ȃ���D��D��F���E���E䊁E���E��
	return 0;
}

Choose(){
	//���������̖���ڎw�����ɉ���:int nerauyaku������Ă����Anerauyaku��4T�ڏI�����_�ōł��߂����̂Ɍ���
	//
	return;

	//�I���D�����čł����_���������
	return;
}



�u����vector�ɐ��I�ԁ����_�̍���or���D�v�Ƃ����`���ɒ��ۉ��ł���̂ł́H�H�H


0221
KOIKOI����ŃN���b�N�������ʒ��������ɂȂ�悤��
Add #ifdef _DEBUG in Draw() Cards Player1 holds
in VSCPGameScene::Choose(),made return properly
0223
FieldBigger(),Change kasuhudaProc 
Change unevitably discarding case algolrthm
FilterCandidate(), modify designated card index
0224
yaku_reach
*/