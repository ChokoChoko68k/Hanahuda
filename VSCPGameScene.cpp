#include"utility.h"
#include"VSCPGameScene.h"

//Overriding GameScene
void VSCPGameScene::Update() {
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
		if (teban == 1) {//CPU's turn
			//��������
			//���Ȃ�

			//����
			player[teban].score += player[teban].nowscore;
			player[(teban + 1) % 2].score -= player[teban].nowscore;
			SceneManager::GetInstance()->CreateScene(SceneID::RESULT, SceneLayer::UPPER, gamenum, teban, player[0].score, player[1].score);
			return;
		}

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

void VSCPGameScene::Draw() {
	//Draw BackGroundImage
	DrawGraph(0, 0, graph_back, FALSE);

	//Draw Cards Player0 holds
	for (int i = 0; i < (signed int)player[0].index_hold.size(); i++) {
		DrawGraph(SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 3 * (cardwidth + xspace)+i* (cardwidth + xspace), yblank + (cardheight + yspace) * 3, card[player[0].index_hold[i]].graph, FALSE);
	}

	//Draw Field Cards
	for (int i = 0; i < (signed int)field.size(); i++) {
		if (i < 10) {
			DrawGraph(SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 1 * (cardwidth + xspace) + (i / 2) * (cardwidth + xspace), yblank + (1/*if 0, same y coordinate as player1's hand*/ + i % 2) * (cardheight + yspace) , card[field[i]].graph, FALSE);
		}
		else if (i<=10 && i < 12) {
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
		DrawGraph(SCREEN_WIDTH / 2 - xspace / 2 - cardwidth - 3 * (cardwidth + xspace) + 6 * (cardwidth + xspace) + cardwidth / 2/*The place semi-rightmost card existed +half of cardwidth*/ +(i % 8) * cardwidth / 3, yblank + (cardheight + yspace) * (2 + i / 8), card[player[0].index_get[i]].graph, FALSE);
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
		DrawBox(40, yblank + (cardheight + yspace)*1, 40+300, yblank + (cardheight + yspace) * 2+cardheight, BLACK, TRUE);//x���W�͉�ʒ�������ɐ^�񒆑���
		yaku = player[teban].yaku;

		//for debug
		DrawExtendFormatStringToHandle(SCREEN_WIDTH / 2 - 120+45, 30, 1.0, 1.0, WHITE, fonthandle, "���=%d  ��=%d", teban, yaku);

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

		DrawExtendFormatStringToHandle(40, yblank + (cardheight + yspace) * 2, 1.0, 1.0, WHITE, fonthandle, "�����������܂����H");
		DrawExtendFormatStringToHandle(40, yblank + (cardheight + yspace) * 2+30, 1.0, 1.0, WHITE, fonthandle, "�͂��@�@�@������");
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



		//������D�ɓ������̎D���Q������A����D�ɂ���Ȃ�
		int month_saved[8];//�ꎞ�u����A�z����D�̐�
		for (int i = 0; i < player[1].index_hold.size(); i++) {
			month_saved[i] = card[player[1].index_hold[i]].month;
		}
		//quick sort
		sort(month_saved, 8);

		//��D�ɓ������̎D���������邩����
		for (int i = 0; i < 8; i++) {//month_saved�̉��Ԗڂ�
			int month_detected = 0;
			if (month_detected == month_saved[i]) {//���Ԃɕ��ׂ���D�ň�O�̎D�ƌ������������������̎D����������
				//��D�̌��ƈ�v���Ă��邩
				for (int j = 0; j < field.size(); j++) {
					if (card[field[j]].month == month_detected) {
						//��v���Ă���΂��̌��̎�D��S������
						for (int k = 0; k < player[1].index_hold.size(); k++) {//k�͎�D�̉��Ԗڂ�
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
		//���̂����̓_���̑傫�������g��:kind��20�Ȃ炻��A�����łȂ����10->5->0�ƗD��x������������
		if (playerplace_candidate.size() != 0) {
			for (int l = 0; l < (signed int)playerplace_candidate.size(); l++) {
				if (card[playerplace_candidate[l]].kind == 20)return l;
			}
			for (int l = 0; l < (signed int)playerplace_candidate.size(); l++) {
				if (card[playerplace_candidate[l]].kind == 10)return l;
			}
			for (int l = 0; l < (signed int)playerplace_candidate.size(); l++) {
				if (card[playerplace_candidate[l]].kind == 5)return l;
			}
			//�J�X�D�����Ȃ��Ȃ�A���D�̂��錎���˂炤
			for (int i = 0; i < (signed int)playerplace_candidate.size(); i++) {
				if (card[playerplace_candidate[i]].month == 1
					|| card[playerplace_candidate[i]].month == 3
					|| card[playerplace_candidate[i]].month == 8
					|| card[playerplace_candidate[i]].month == 11
					|| card[playerplace_candidate[i]].month == 12) {
					return i;
				}
			}
			//�r���E�퐶�E�t���E�����E�t�����Ȃ��Ȃ�ŏ��̌��D��
			return playerplace_candidate[0];
		}





		//���������̖���ڎw�����ɉ���:int nerauyaku������Ă����Anerauyaku��4T�ڏI�����_�ōł��߂����̂Ɍ���
		//if(nerauyaku == kou)...
		//return;
		/*/���̂����̓_���̑傫�������g��:kind��20�Ȃ炻��A�����łȂ����10->5->0�ƗD��x������������
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
			//�J�X�D�����Ȃ��Ȃ�A���D�̂��錎���˂炤
			for (int i = 0; i < playerplace_candidate.size(); i++) {
				if (card[playerplace_candidate[i]].month == 1
					|| card[playerplace_candidate[i]].month == 3
					|| card[playerplace_candidate[i]].month == 8
					|| card[playerplace_candidate[i]].month == 11
					|| card[playerplace_candidate[i]].month == 12) {
					return i;
				}
			}
			//�r���E�퐶�E�t���E�����E�t�����Ȃ��Ȃ�ŏ��̌��D��
			return playerplace_candidate[0];
		}*/






		//��D�E��D�����ē_���̑傫�������g��
		for (int i = 0; i < (signed int)field.size(); i++) {
			if (card[field[i]].kind == 20) {
				for (int k = 0; k < player[1].index_hold.size(); k++) {//k�͎�D�̉��Ԗڂ�
					if (card[player[1].index_hold[k]].month == card[field[i]].month) {
						return k;
					}
				}
			}
		}
		for (int l = 0; l < (signed int)player[1].index_hold.size(); l++) {
			if (card[player[1].index_hold[l]].kind == 20)return l;
		}
		for (int i = 0; i < (signed int)field.size(); i++) {
			if (card[field[i]].kind == 10) {
				for (int k = 0; k < player[1].index_hold.size(); k++) {//k�͎�D�̉��Ԗڂ�
					if (card[player[1].index_hold[k]].month == card[field[i]].month) {
						return k;
					}
				}
			}
		}
		for (int l = 0; l < (signed int)player[1].index_hold.size(); l++) {
			if (card[player[1].index_hold[l]].kind == 10)return l;
		}
		for (int i = 0; i < (signed int)field.size(); i++) {
			if (card[field[i]].kind == 5) {
				for (int k = 0; k < player[1].index_hold.size(); k++) {//k�͎�D�̉��Ԗڂ�
					if (card[player[1].index_hold[k]].month == card[field[i]].month) {
						return k;
					}
				}
			}
		}
		for (int l = 0; l < (signed int)player[1].index_hold.size(); l++) {
			if (card[player[1].index_hold[l]].kind == 5)return l;
		}
		//�J�X�D�����Ȃ��Ȃ�A���D�̂��錎���˂炤
		for (int i = 0; i < (signed int)player[1].index_hold.size(); i++) {
			if (card[player[1].index_hold[i]].month == 1
				|| card[player[1].index_hold[i]].month == 3
				|| card[player[1].index_hold[i]].month == 8
				|| card[player[1].index_hold[i]].month == 11
				|| card[player[1].index_hold[i]].month == 12) {
				return i;
			}
		}
		//�r���E�퐶�E�t���E�����E�t�����Ȃ��Ȃ�ŏ��̎D��
		return 0;
	}
	if (click_left == 1) {//player's turn
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

//��D�ɓ������̎D�������������ꍇ�ɍs���I��
int VSCPGameScene::Choose() {
	if (teban == 1) {//CPU's turn
		//TODO
		return 0;
	}
	if (click_left == 1) {//player'sturn
		if (teban == 0) {
			for (int i = 0; i < (signed int)samemonthcard.size(); i++) {
				int num = samemonthcard[i];/////////////////////////////////
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

*/