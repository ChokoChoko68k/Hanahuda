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
		int score;//ゲームを通じての持ち点：初期値２０
		int nowscore;//そのラウンドで取得し点：初期値０；「勝負」するまで持ち点に加えられない
		//役判定用
		unsigned int yaku;
		unsigned int num_kasu;
		unsigned int num_tan;
		unsigned int num_tane;
		//手札に持っているカードのインデックス：0から47
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
	//山札のインデックス
	std::vector<byte> deck;
	//場札のインデックス
	std::vector<byte> field;

	bool isdrawing;
	CELLKIND cellkind;
	byte gamenum;
	byte teban;

	int graph_back;

	int index_selected;

	//出札と同じ月の札が場に複数あった場合に使う
	std::vector<byte> samemonthcard;

	int CardDataLoad();
	void Shuffle();
	//fieldに渡す８枚のうち同じ月が３枚以上になっていたらリセット
	bool IsDealOK();
	//配る
	void Deal();
	//手札から場に出す札を選択
	int Select();
	//手札から場に捨てる：todo;場に同じ月のものが複数あった場合：出来たが書き方がヒドイ
	int Trash(int _cardindex);
	//場に同じ月のものが複数あった場合の、場札選択
	int Choose();
	void DeckToField();
	int YakuHantei();
public:
	GameScene(int _gamenum, int _teban, int _score_player0, int _score_player1);
	~GameScene();

	void Update();
	void Draw();

};

//deckにcardのアドレスをコピーしたい：deckをvectorにしたら型がちがって代入できなかった
//std::vector<Card>* std::vector<Card*> の違いがわからん;前者はベクターのメンバ関数は使えない
/*
stage1からstage2に移るとき、シーンって作り直すもの？？？

*/