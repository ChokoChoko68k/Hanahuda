#pragma once

struct Pos {
	int x;
	int y;
};

//typedef int animid;

class Animation
{
	//variables
	//initializer
	//functions
	//con&des
public:
	Animation();
	~Animation();

	//Animation Registraion; returns animid
	int Register(int x_start,int y_start, int x_terminal, int y_terminal,int interval,int graph);

	//Execute Animation
	bool Exe();

	//Delete Animation Registraion
	bool Unregister(int animid);

private:
	Pos start{0};
	Pos terminal{0};
	int graph{0};
	int interval{0};
	int animid{0};

	unsigned int timer{0};
};

Animation::Animation()
{
}

Animation::~Animation()
{
}
