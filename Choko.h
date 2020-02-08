#pragma once
#include"DxLib.h"

extern unsigned int keystate_left;
extern unsigned int keystate_up;
extern unsigned int keystate_right;
extern unsigned int keystate_down;
extern unsigned int keystate_1;
extern unsigned int keystate_2;
extern unsigned int keystate_3;
extern unsigned int keystate_4;
extern unsigned int keystate_5;
extern unsigned int keystate_6;
extern unsigned int keystate_7;
extern unsigned int keystate_8;
extern unsigned int keystate_9;
extern unsigned int keystate_10;

extern int mousex;
extern int mousey;
extern unsigned int click_left;
extern unsigned int click_right;

double GetFps();
void FpsControll();
void InputReceive();
void InputDistribute(unsigned int& keystate, unsigned int keyconfig);
int CursorLoad(const char* filename);
void CursorDraw();
void Wait();

inline int SignRand(int argument){
	return GetRand(argument * 2) - argument;//between -argument and +argument
}

inline double SignRand(double argument){
	return ((argument * 2 * GetRand(10000) / 10000.0) - argument);
}

class Button
{
public:
	Button() {};
	Button(int arg_x, int arg_y, int arg_width, int arg_height, int arg_graph) {
		x = arg_x;
		y = arg_y;
		width = arg_width;
		height = arg_height;
		graph = arg_graph;
	};
	virtual ~Button() {};

	void GraphLoad(const char* imagefilename) {
		graph = LoadGraph(imagefilename);
	}

	bool IsPressed() {
		if (click_left == 1) {
			if (mousex >= x && mousex < x + width) {
				if (mousey >= y && mousey < y + height) {
					return true;
				}
			}
		}
		else return false;
	}

	int Draw() { return DrawGraph(x, y, graph, TRUE); }
	int Draw(int arg_x, int arg_y) { return DrawGraph(arg_x, arg_y, graph, TRUE); }
private:
	int x{0};
	int y{0};
	int width{0};
	int height{0};
	int graph{0};
};