/*
�Q�[���V�[��
�J�[�h�̎��̂́A�v���C���[��R�D���D�ƕʂɍ���Ă���
�J�[�h�̎󂯓n���́A���̂��̂��̂ł͂Ȃ�0~47�̃C���f�b�N�X�̐������evector��push_back&erase���Ă���
*/
#pragma once
#include"Scene.h"
#include"Choko.h"
#include<vector>

typedef unsigned char byte;

class GameScene : public Scene {
protected:

	enum class Cellkind { NONE, KOIKOI, MULTICHOICE };

	//�D�z�u�ʒu�ݒ�萔
	const byte xblank = 40;//The left edge of window-The leftmost card space
	const byte xspace = 30;//The interval of cards
	const byte yblank = 15;//The upper edge of window-The uppermost card space
	const byte yspace = 16;//The interval of cards
	const byte cardwidth = 94;
	const byte cardheight = 152;

	//�Q�[���ݒ�
	//const byte num_dealfuda = 8;

	//�D�N���X
	struct Card {
		byte month;
		byte kind;
		int graph;
	};

	//�v���C���[�N���X
	class Player {
	private:
		
	public:
		int score;//�Q�[����ʂ��Ă̎����_�F�����l�Q�O
		int nowscore;//���̃��E���h�Ŏ擾���_�F�����l�O�G�u�����v����܂Ŏ����_�ɉ������Ȃ�
		//�𔻒�p
		unsigned int yaku;
		unsigned int yaku_reach;//���������������yaku_reach����͏�����//���̎d�l�Ŗ������B
		int num_kasu;
		int num_tan;
		int num_tane;
		//��D�Ɏ����Ă���J�[�h�̃C���f�b�N�X�F0����47
		std::vector<byte> index_hold;
		std::vector<byte> index_get;

		Player() {
			score = 20;
			nowscore = 0;
			yaku = 0; 
			yaku_reach = 0;
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

	Button button[2];

	std::vector<byte> deck;//�R�D
	std::vector<byte> field;//��D

	bool isdrawing;

	Cellkind cellkind;
	byte gamenum;//�����ڂ�
	byte teban;//��Ԃ͒N��

	int graph_back;
	int graph_ura;

	int holdplace_selected;//byte�ɂ�������-1������̂�int;�����Ȃ���-1����255�o���G���[���܂���

	std::vector<byte> samemonthcard;//���̎D��field�ŉ��ԖڂȂ̂�������B�o�D�Ɠ������̎D����ɕ����������ꍇ�Ɏg��

	virtual void Init(int _gamenum, int _teban, int _score_player0, int _score_player1);
	virtual int CardDataLoad();
	virtual void Shuffle();
	virtual bool IsDealOK();//field�ɓn���W���̂������������R���ȏ�ɂȂ��Ă����烊�Z�b�g����
	virtual void Deal();//�z��
	virtual int Select();//��D�����ɏo���D��I��
	virtual int Trash(int _cardindex);//��D�����Ɏ̂Ă�Ftodo;��ɓ������̂��̂������������ꍇ�F�o���������������q�h�C
	virtual int Choose();//��ɓ������̂��̂������������ꍇ�́A��D�I��
	virtual void DeckDraw();
	virtual int YakuHantei();//�����o������A���̍X�V���������Ƃ���0�ȊO��Ԃ�
public:
	GameScene() {};//�Ȃ����f�t�H�R���������ƂŌp����ŃG���[���o��
	GameScene(int _gamenum, int _teban, int _score_player0, int _score_player1) {
		Init(_gamenum, _teban, _score_player0, _score_player1);
	};
	virtual ~GameScene() {};

	virtual int Update();
	virtual void Draw();

};

//deck��card�̃A�h���X���R�s�[�������Fdeck��vector�ɂ�����^���������đ���ł��Ȃ�����
//std::vector<Card>* std::vector<Card*> �̈Ⴂ���킩���;�O�҂̓x�N�^�[�̃����o�֐��͎g���Ȃ�
/*
stage1����stage2�Ɉڂ�Ƃ��A�V�[�����č�蒼�����́H�H�H

*/