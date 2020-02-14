#include"Choko.h"

unsigned int keystate_left;
unsigned int keystate_up;
unsigned int keystate_right;
unsigned int keystate_down;
unsigned int keystate_1;
unsigned int keystate_2;
unsigned int keystate_3;
unsigned int keystate_4;
unsigned int keystate_5;
unsigned int keystate_6;
unsigned int keystate_7;
unsigned int keystate_8;
unsigned int keystate_9;
unsigned int keystate_10;

int mousex;
int mousey;
unsigned int click_left;
unsigned int click_right;

int graph_cursor;

double GetFps(){
	static double fps = 0.f;
	static unsigned int counter = 0;
	static unsigned int time = 0;
	static unsigned int time_save = 0;
	if (counter == 0)
		time_save = GetNowCount();
	if (counter == 60){
		time = GetNowCount();//�~���b���x����int�^
		fps = 1000.0f / ((time - time_save) / 60.0f);
		counter = 0;
	}
	else{
		counter++;
	}//ifelse���͕���Dcounter��0��������else�ȉ��͔�΂����

	return fps;
}

void FpsControll(){
	static unsigned int counter = 0;
	static unsigned int time = 0;
	if (counter == 30){//every 30 frames
		while ((GetNowCount() - time) < 500){}
		counter = 0;
	}
	if (counter == 0)time = GetNowCount();
	counter++;
}

void InputReceive(){
	int joypad1input = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	if ((joypad1input & PAD_INPUT_LEFT) == 0){
		keystate_left = 0;
	}
	else{
		keystate_left++;
	}
	if ((joypad1input & PAD_INPUT_UP) == 0){
		keystate_up = 0;
	}
	else{
		keystate_up++;
	}

	if ((joypad1input & PAD_INPUT_RIGHT) == 0){
		keystate_right = 0;
	}
	else{
		keystate_right++;
	}

	if ((joypad1input & PAD_INPUT_DOWN) == 0){
		keystate_down = 0;
	}
	else{
		keystate_down++;
	}

	if ((joypad1input & PAD_INPUT_1) == 0){
		keystate_1 = 0;
	}
	else{
		keystate_1++;
	}
	if ((joypad1input & PAD_INPUT_2) == 0){
		keystate_2 = 0;
	}
	else{
		keystate_2++;
	}
	if ((joypad1input & PAD_INPUT_3) == 0){
		keystate_3 = 0;
	}
	else{
		keystate_3++;
	}
	if ((joypad1input & PAD_INPUT_4) == 0){
		keystate_4 = 0;
	}
	else{
		keystate_4++;
	}
	if ((joypad1input & PAD_INPUT_5) == 0){
		keystate_5 = 0;
	}
	else{
		keystate_5++;
	}
	if ((joypad1input & PAD_INPUT_6) == 0){
		keystate_6 = 0;
	}
	else{
		keystate_6++;
	}
	if ((joypad1input & PAD_INPUT_7) == 0){
		keystate_7 = 0;
	}
	else{
		keystate_7++;
	}
	if ((joypad1input & PAD_INPUT_8) == 0){
		keystate_8 = 0;
	}
	else{
		keystate_8++;
	}
	if ((joypad1input & PAD_INPUT_9) == 0){
		keystate_9 = 0;
	}
	else{
		keystate_9++;
	}
	if ((joypad1input & PAD_INPUT_10) == 0){
		keystate_10 = 0;
	}else{
		keystate_10++;
	}

	GetMousePoint(&mousex, &mousey);

	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0){
		click_left++;
	}else{
		click_left = 0;
	}

	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0) {
		click_right++;
	}else {
		click_right = 0;
	}
}

void InputDistribute(unsigned int& keystate, unsigned int keyconfig){
	switch (keyconfig){
	case 1:keystate = keystate_1; break;
	case 2:keystate = keystate_2; break;
	case 3:keystate = keystate_3; break;
	case 4:keystate = keystate_4; break;
	case 5:keystate = keystate_5; break;
	case 6:keystate = keystate_6; break;
	case 7:keystate = keystate_7; break;
	case 8:keystate = keystate_8; break;
	case 9:keystate = keystate_9; break;
	case 10:keystate = keystate_10; break;
	default:break;
	}
}

int CursorLoad(const char *filename) {
	int result = LoadGraph(filename);
	if (result == -1) {
		return -1;
	}else{
		graph_cursor = result;
		return 0;
	}
}

void CursorDraw() {
	DrawGraph(mousex, mousey, graph_cursor, TRUE);
}

/*
   * ���v�f�̑I��
   * ���Ɍ��āA�ŏ��Ɍ��������قȂ�2�̗v�f�̂����A
   * �傫���ق��̔ԍ���Ԃ��܂��B
   * �S�������v�f�̏ꍇ�� -1 ��Ԃ��܂��B
   */
int pivot(int* a, int i, int j) {
	int k = i + 1;
	while (k <= j && a[i] == a[k]) k++;
	if (k > j) return -1;
	if (a[i] >= a[k]) return i;
	return k;
}

/*
 * �p�[�e�B�V��������
 * a[i]�`a[j]�̊ԂŁAx �����Ƃ��ĕ������܂��B
 * x ��菬�����v�f�͑O�ɁA�傫���v�f�͂�����ɗ��܂��B
 * �傫���v�f�̊J�n�ԍ���Ԃ��܂��B
 */
int partition(int* a, int i, int j, int x) {
	int l = i, r = j;

	// ��������������܂ŌJ��Ԃ��܂�
	while (l <= r) {

		// ���v�f�ȏ�̃f�[�^��T���܂�
		while (l <= j && a[l] < x)  l++;

		// ���v�f�����̃f�[�^��T���܂�
		while (r >= i && a[r] >= x) r--;

		if (l > r) break;
		int t = a[l];
		a[l] = a[r];
		a[r] = t;
		l++; r--;
	}
	return l;
}

/*
 * �N�C�b�N�\�[�g�i�ċA�p�j
 * �z��a�́Aa[i]����a[j]����בւ��܂��B
 */
void quickSort(int* a, int i, int j) {
	if (i == j) return;
	int p = pivot(a, i, j);
	if (p != -1) {
		int k = partition(a, i, j, a[p]);
		quickSort(a, i, k - 1);
		quickSort(a, k, j);
	}
}

/*
 * �\�[�g
 */
void sort(int* a, int size_a) {
	quickSort(a, 0, size_a - 1);
}

/*
�z��������ɂƂ�
�E�z��ϐ��̖��O�����n��
�E[]�󊇌ʂł����Ƃ�;���̂��炩���������̓|�C���^

SignRand�֐���SelectUnused�֐��̕Ԃ�l��const�ɂ�����error LNK 2019 (��`��������Ȃ�)���o���D

int FileReadLine(int filehandle,const int charmax){
char chara[charmax];//�G���[�F�����͖��x�ς�邩��const�Ƃ͂�������
*/
/*
char* const FileReadLine(int filehandle){
char chara[100];
if (filehandle == NULL)return chara;//chara��Ԃ����Ƃ���ŃA�h���X���������Astatic�́H
for (int i = 0; i < 100 && FileRead_getc(filehandle) != '\n'; i++)
chara[i] = FileRead_getc(filehandle);
return chara;//�A�h���X��Ԃ��Ă�̂ɕԂ�l�Ƀ|�C���^�H
}

int FileReadSkip(const int filehandle,const int number_of_row){
if (filehandle == NULL)return -1;
for (int i = 0; i<number_of_row; i++)
while (FileRead_getc(filehandle) != '\n');
return 0;
}

int SelectUnused(unsigned int max, unsigned int arr[]){//���̂̓|�C���^������ const int const �ɂ�����~?
for (unsigned int number = 0; number < max; number++){
if (arr[number] == OFF){
return number;
}
}
return -1;
}
*/

