#pragma once

enum class SceneID { TITLE, FUSUMA, GAME,VSCPGAME, RESULT };

enum class SceneLayer { DOWNER, UPPER };

//シーン
class Scene {
private:
	//コピー禁止
	Scene(const Scene& other);//コピコン;仮引数にとったりしてもprivateなので呼べない
	Scene& operator=(const Scene& other);//代入演算子オーバーロード;代入してもprivateなので呼べない//なぜintellisenseが警告を？？
	Scene(const Scene&& other);//コピコン;仮引数にとったりしてもprivateなので呼べない
	Scene& operator=(const Scene&& other);//代入演算子オーバーロード;代入してもprivateなので呼べない//なぜintellisenseが警告を？？

public:
	Scene() {};
	virtual int Update() = 0;
	virtual void Draw() = 0;
	virtual ~Scene() {};
};

//各シーン管理。低層シーンと高層シーンがあり、高層シーンが存在している場合には低層シーンの更新はスキップされる。また、高層シーンが上に描画される。SAIやGIMPなどの絵描きソフトのレイヤ構造を参考にした。

//シーン間で情報を受け渡すには、一旦SceneManagerのAcrossSceneDataに保存したのち、CreateScene関数に渡す。渡さない場合、CreateScene関数にはデフォルト引数が
class SceneManager {
private:
	//コピー禁止
	SceneManager(const SceneManager& other);
	SceneManager& operator=(const SceneManager& other);
	SceneManager(const SceneManager&& other);
	SceneManager& operator=(const SceneManager&& other);

	//コンストラクタをprivateに
	SceneManager() {
		downerscene = nullptr;
		upperscene = nullptr;

		gamenum = 0;
		teban = 0;
		score_player0 = 20;
		score_player1 = 20;
		isvscp = false;
	};
	static SceneManager* pointer_instance;

	//Across Scene Data
	int gamenum;
	int teban;
	int score_player0;
	int score_player1;
	//int nextsceneid;

public:
	static void CreateInstance() {//特定のオブジェクトを基準とする相対的な参照・・・じゃなくなる
		if (pointer_instance == nullptr) {
			pointer_instance = new SceneManager();
		}
	}

	static SceneManager* GetInstance() { return pointer_instance; }

	static void DeleteInstance() {
		if (pointer_instance != nullptr) {
			delete pointer_instance;
			pointer_instance = nullptr;
		}
	}

private:
	Scene* downerscene;
	Scene* upperscene;

	//bool isskipdownerscene;
public:
	int Update();

	void Draw();

	//void ChangeSkipFlag(bool _isskipdownerscene) { isskipdownerscene = _isskipdownerscene; }

	void CreateScene(SceneID _sceneid, SceneLayer _layer, int _gamenum = 0, int _teban = 0 , int _score_player0 = 20, int _score_player1 = 20);

	void DeleteScene(SceneLayer _layer);

	bool isvscp;//リザルト画面から帰ってきたあとGameSceneを作るのかVSCPGameSceneを作るのか保存しておくため
};


/*
Scenes *pointer_nextchildscene;
if (pointer_childscene != NULL) {
pointer_nextchildscene = pointer_childscene->Update(this);

if (pointer_nextchildscene != pointer_childscene) {
delete pointer_childscene;
pointer_childscene = pointer_nextchildscene;
}
}

if (pointer_childscene != NULL) pointer_childscene->Draw(this);

*/