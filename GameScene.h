/*
ゲームシーン
カードの実体は、プレイヤーや山札や場札と別に作っておく
カードの受け渡しは、実体そのものではなく0~47のインデックスの数字を各vectorにpush_back&eraseしていく
*/
#pragma once
#include"Scene.h"
#include"Choko.h"
#include<vector>

typedef unsigned char byte;

class GameScene : public Scene {
protected:

	enum class Cellkind { NONE, KOIKOI, MULTICHOICE };

	//札配置位置設定定数
	const byte xblank = 40;//The left edge of window-The leftmost card space
	const byte xspace = 30;//The interval of cards
	const byte yblank = 15;//The upper edge of window-The uppermost card space
	const byte yspace = 16;//The interval of cards
	const byte cardwidth = 94;
	const byte cardheight = 152;

	//ゲーム設定
	//const byte num_dealfuda = 8;

	//札クラス
	struct Card {
		byte month;
		byte kind;
		int graph;
	};

	//プレイヤークラス
	class Player {
	private:
		
	public:
		int score;//ゲームを通じての持ち点：初期値２０
		int nowscore;//そのラウンドで取得し点：初期値０；「勝負」するまで持ち点に加えられない
		//役判定用
		unsigned int yaku;
		unsigned int yaku_reach;//手役が完成したらyaku_reachからは消える//この仕様で迷った。
		int num_kasu;
		int num_tan;
		int num_tane;
		//手札に持っているカードのインデックス：0から47
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

	std::vector<byte> deck;//山札
	std::vector<byte> field;//場札

	bool isdrawing;

	Cellkind cellkind;
	byte gamenum;//何回戦目か
	byte teban;//手番は誰か

	int graph_back;
	int graph_ura;

	int holdplace_selected;//byteにしたいが-1を入れるのでint;符号なしに-1して255出すエラーやりました

	std::vector<byte> samemonthcard;//その札がfieldで何番目なのかが入る。出札と同じ月の札が場に複数あった場合に使う

	virtual void Init(int _gamenum, int _teban, int _score_player0, int _score_player1);
	virtual int CardDataLoad();
	virtual void Shuffle();
	virtual bool IsDealOK();//fieldに渡す８枚のうち同じ月が３枚以上になっていたらリセットする
	virtual void Deal();//配る
	virtual int Select();//手札から場に出す札を選択
	virtual int Trash(int _cardindex);//手札から場に捨てる：todo;場に同じ月のものが複数あった場合：出来たが書き方がヒドイ
	virtual int Choose();//場に同じ月のものが複数あった場合の、場札選択
	virtual void DeckDraw();
	virtual int YakuHantei();//役が出来たり、役の更新があったときに0以外を返す
public:
	GameScene() {};//なぜかデフォコンが無いとで継承先でエラーが出る
	GameScene(int _gamenum, int _teban, int _score_player0, int _score_player1) {
		Init(_gamenum, _teban, _score_player0, _score_player1);
	};
	virtual ~GameScene() {};

	virtual int Update();
	virtual void Draw();

};

//deckにcardのアドレスをコピーしたい：deckをvectorにしたら型がちがって代入できなかった
//std::vector<Card>* std::vector<Card*> の違いがわからん;前者はベクターのメンバ関数は使えない
/*
stage1からstage2に移るとき、シーンって作り直すもの？？？

*/