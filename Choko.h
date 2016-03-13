#pragma once
#include"DxLib.h"

//original boolean type define
typedef unsigned char Switch;
const Switch OFF = 0;
const Switch ON = 1;

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

inline double MultipleItself(double it){
	return it*it;
}
inline float MultipleItself(float it){
	return it*it;
}

/*#ifdef _DEBUG
#pragma comment(lib,"Choko_d.lib")
#else
#pragma comment(lib,"Choko_r.lib")
#endif*/