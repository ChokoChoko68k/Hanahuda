#include"Animation.h"
#include<DxLib.h>

bool Animation::Exe() {
	DrawGraph(start.x+(terminal.x - start.x)/interval*timer, start.y + (terminal.y - start.y) / interval * timer,graph,FALSE);
	return true;
}