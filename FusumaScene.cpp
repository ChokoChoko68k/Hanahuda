#include"utility.h"
#include"FusumaScene.h"
#include"Choko.h"

FusumaScene::FusumaScene() {
	count = 0;
	leftx_limit = 0;
	rightx_limit = SCREEN_WIDTH;

	isclosing = true;

	waitcount = 0;

	graph_fusuma_left = LoadGraph("graphic/Fusuma_left.jpg");
	graph_fusuma_right = LoadGraph("graphic/Fusuma_right.jpg");
};

FusumaScene::~FusumaScene() {
};

int FusumaScene::Update() {

	leftx_limit = count;
	rightx_limit = SCREEN_WIDTH - count;

	if (count == SCREEN_WIDTH / 2) {
		if (waitcount == 48) {
			if (SceneManager::GetInstance()->isvscp)SceneManager::GetInstance()->CreateScene(SceneID::VSCPGAME, SceneLayer::DOWNER);
			else SceneManager::GetInstance()->CreateScene(SceneID::GAME, SceneLayer::DOWNER);
			
			isclosing = false;
		}
		waitcount++;
	}

	if (count < SCREEN_WIDTH / 2) {
		if (isclosing)count += 16;//shutterring speed
	}
	if (count <= SCREEN_WIDTH / 2) {
		if (!isclosing)count -= 16;//opening speed
	}


	//‚Ó‚·‚Ü‘S•”ŠJ‚¢‚½
	if (!isclosing && count == 0) {
		SceneManager::GetInstance()->DeleteScene(SceneLayer::UPPER);
		return 0;
	}
	return 0;
}

void FusumaScene::Draw() {
	DrawGraph(-SCREEN_WIDTH/2+leftx_limit, 0, graph_fusuma_left, FALSE);
	DrawGraph(rightx_limit, 0, graph_fusuma_right, FALSE);
}