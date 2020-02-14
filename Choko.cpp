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
		time = GetNowCount();//ミリ秒精度だがint型
		fps = 1000.0f / ((time - time_save) / 60.0f);
		counter = 0;
	}
	else{
		counter++;
	}//ifelse文は分岐．counterが0だったらelse以下は飛ばされる

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
   * 軸要素の選択
   * 順に見て、最初に見つかった異なる2つの要素のうち、
   * 大きいほうの番号を返します。
   * 全部同じ要素の場合は -1 を返します。
   */
int pivot(int* a, int i, int j) {
	int k = i + 1;
	while (k <= j && a[i] == a[k]) k++;
	if (k > j) return -1;
	if (a[i] >= a[k]) return i;
	return k;
}

/*
 * パーティション分割
 * a[i]～a[j]の間で、x を軸として分割します。
 * x より小さい要素は前に、大きい要素はうしろに来ます。
 * 大きい要素の開始番号を返します。
 */
int partition(int* a, int i, int j, int x) {
	int l = i, r = j;

	// 検索が交差するまで繰り返します
	while (l <= r) {

		// 軸要素以上のデータを探します
		while (l <= j && a[l] < x)  l++;

		// 軸要素未満のデータを探します
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
 * クイックソート（再帰用）
 * 配列aの、a[i]からa[j]を並べ替えます。
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
 * ソート
 */
void sort(int* a, int size_a) {
	quickSort(a, 0, size_a - 1);
}

/*
配列を引数にとる
・配列変数の名前だけ渡す
・[]空括弧でうけとる;このからかっこが実はポインタ

SignRand関数とSelectUnused関数の返り値をconstにしたらerror LNK 2019 (定義が見つからない)が出た．

int FileReadLine(int filehandle,const int charmax){
char chara[charmax];//エラー：引数は毎度変わるからconstとはいえ無理
*/
/*
char* const FileReadLine(int filehandle){
char chara[100];
if (filehandle == NULL)return chara;//charaを返したところでアドレスから消える、staticは？
for (int i = 0; i < 100 && FileRead_getc(filehandle) != '\n'; i++)
chara[i] = FileRead_getc(filehandle);
return chara;//アドレスを返してんのに返り値にポインタ？
}

int FileReadSkip(const int filehandle,const int number_of_row){
if (filehandle == NULL)return -1;
for (int i = 0; i<number_of_row; i++)
while (FileRead_getc(filehandle) != '\n');
return 0;
}

int SelectUnused(unsigned int max, unsigned int arr[]){//実体はポインタだから const int const にしたら×?
for (unsigned int number = 0; number < max; number++){
if (arr[number] == OFF){
return number;
}
}
return -1;
}
*/

