/*
CPU�ΐ��p�Q�[���V�[��
*/
#pragma once
#include"GameScene.h"
#include"utility.h"
#include<vector>

class VSCPGameScene :public GameScene
{
public:
	VSCPGameScene(int _gamenum, int _teban, int _score_player0, int _score_player1) {
		Init(_gamenum, _teban, _score_player0, _score_player1);
	};
	virtual ~VSCPGameScene() {};

	//for Overide
	void Update();
	void Draw();
	int Select();
	int Choose();

	//for CPAI
	std::vector<byte> DiscardSameMonth();
	//int VSCPGameScene::TowardYaku();
	byte FilterCandidate(std::vector<byte> playerplace_candidate);//�����i�荞�ށB�_���̑傫�����́E���D�̂��錎//Filter�Ƃ����Ԃ��̂͗B���
	byte FieldBigger();
private:

};
