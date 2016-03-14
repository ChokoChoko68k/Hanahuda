#pragma once
#include"Scene.h"
#include<vector>

typedef unsigned char byte;

class GameScene : public Scene {
private:
	enum CELLKIND { NONE, KOIKOI, CHOICE };

	const byte xblank = 20;
	const byte xspace = 10;
	const byte yblank = 32;
	const byte yspace = 10;
	const byte cardwidth = 42;
	const byte cardheight = 63;

	struct Card {
		byte month;
		byte kind;
		int graph;
	};

	class Player {
	private:
		
	public:
		int score;//�Q�[����ʂ��Ă̎����_�F�����l�Q�O
		int nowscore;//���̃��E���h�Ŏ擾���_�F�����l�O�G�u�����v����܂Ŏ����_�ɉ������Ȃ�
		//�𔻒�p
		unsigned int yaku;
		unsigned int num_kasu;
		unsigned int num_tan;
		unsigned int num_tane;
		//��D�Ɏ����Ă���J�[�h�̃C���f�b�N�X�F0����47
		std::vector<byte> index_hold;
		std::vector<byte> index_get;

		Player() {
			score = 20;
			nowscore = 0;
			yaku = 0; 
			num_kasu = 0;
			num_tan = 0;
			num_tane = 0;
		};
		~Player() {};

		void Clear() {
			index_hold.clear();
			index_get.clear();
			nowscore = 0;
			yaku = 0;
			num_kasu = 0;
			num_tan = 0;
			num_tane = 0;
		}
	};

	Card card[48];
	Player player[2];
	//�R�D�̃C���f�b�N�X
	std::vector<byte> deck;
	//��D�̃C���f�b�N�X
	std::vector<byte> field;

	bool isdrawing;
	CELLKIND cellkind;
	byte gamenum;
	byte teban;

	int graph_back;

	int index_selected;

	//�o�D�Ɠ������̎D����ɕ����������ꍇ�Ɏg��
	std::vector<byte> samemonthcard;

	int CardDataLoad();
	void Shuffle();
	//field�ɓn���W���̂������������R���ȏ�ɂȂ��Ă����烊�Z�b�g
	bool IsDealOK();
	//�z��
	void Deal();
	//��D�����ɏo���D��I��
	int Select();
	//��D�����Ɏ̂Ă�Ftodo;��ɓ������̂��̂������������ꍇ�F�o���������������q�h�C
	int Trash(int _cardindex);
	//��ɓ������̂��̂������������ꍇ�́A��D�I��
	int Choose();
	void DeckToField();
	int YakuHantei();
public:
	GameScene(int _gamenum, int _teban, int _score_player0, int _score_player1);
	~GameScene();

	void Update();
	void Draw();

};

//deck��card�̃A�h���X���R�s�[�������Fdeck��vector�ɂ�����^���������đ���ł��Ȃ�����
//std::vector<Card>* std::vector<Card*> �̈Ⴂ���킩���;�O�҂̓x�N�^�[�̃����o�֐��͎g���Ȃ�
/*
stage1����stage2�Ɉڂ�Ƃ��A�V�[�����č�蒼�����́H�H�H

*/