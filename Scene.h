#pragma once

enum class SceneID { TITLE, FUSUMA, GAME,VSCPGAME, RESULT };

enum class SceneLayer { DOWNER, UPPER };

//�V�[��
class Scene {
private:
	//�R�s�[�֎~
	Scene(const Scene& other);//�R�s�R��;�������ɂƂ����肵�Ă�private�Ȃ̂ŌĂׂȂ�
	Scene& operator=(const Scene& other);//������Z�q�I�[�o�[���[�h;������Ă�private�Ȃ̂ŌĂׂȂ�//�Ȃ�intellisense���x�����H�H
	Scene(const Scene&& other);//�R�s�R��;�������ɂƂ����肵�Ă�private�Ȃ̂ŌĂׂȂ�
	Scene& operator=(const Scene&& other);//������Z�q�I�[�o�[���[�h;������Ă�private�Ȃ̂ŌĂׂȂ�//�Ȃ�intellisense���x�����H�H

public:
	Scene() {};
	virtual int Update() = 0;
	virtual void Draw() = 0;
	virtual ~Scene() {};
};

//�e�V�[���Ǘ��B��w�V�[���ƍ��w�V�[��������A���w�V�[�������݂��Ă���ꍇ�ɂ͒�w�V�[���̍X�V�̓X�L�b�v�����B�܂��A���w�V�[������ɕ`�悳���BSAI��GIMP�Ȃǂ̊G�`���\�t�g�̃��C���\�����Q�l�ɂ����B

//�V�[���Ԃŏ����󂯓n���ɂ́A��USceneManager��AcrossSceneData�ɕۑ������̂��ACreateScene�֐��ɓn���B�n���Ȃ��ꍇ�ACreateScene�֐��ɂ̓f�t�H���g������
class SceneManager {
private:
	//�R�s�[�֎~
	SceneManager(const SceneManager& other);
	SceneManager& operator=(const SceneManager& other);
	SceneManager(const SceneManager&& other);
	SceneManager& operator=(const SceneManager&& other);

	//�R���X�g���N�^��private��
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
	static void CreateInstance() {//����̃I�u�W�F�N�g����Ƃ��鑊�ΓI�ȎQ�ƁE�E�E����Ȃ��Ȃ�
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

	bool isvscp;//���U���g��ʂ���A���Ă�������GameScene�����̂�VSCPGameScene�����̂��ۑ����Ă�������
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