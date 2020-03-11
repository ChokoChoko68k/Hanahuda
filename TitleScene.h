#pragma once
#include"Scene.h"

class TitleScene :public Scene{
private:
	int graph_back;//title picture
public:
	TitleScene();
	~TitleScene();
	int Update();
	void Draw();
};
