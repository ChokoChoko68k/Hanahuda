/*
CPU対戦専用ゲームシーン
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
	byte FilterCandidate(std::vector<byte> playerplace_candidate);//候補を絞り込む。点数の大きいもの・光札のある月//Filterといいつつ返すのは唯一つ
	byte FieldBigger();
private:

};
