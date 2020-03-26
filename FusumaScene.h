#pragma once
#include"Scene.h"

class FusumaScene : public Scene {
private:
	int graph_fusuma_left;
	int graph_fusuma_right;

	int count;
	int leftx_limit;
	int rightx_limit;

	bool isclosing;

	int waitcount = 0;

public:
	FusumaScene();
	~FusumaScene();

	int Update();
	void Draw();
};
