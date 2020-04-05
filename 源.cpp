#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>


#define BLACK 0
#define WHITE 1
#define EMPTY 2

struct Point { //��ṹ
	int x, y;
};
struct Step { //���ṹ
	Point first, second;
	int value;
};
int Board[19][19];//�洢������Ϣ����Ԫ��ֵΪ BLACK, WHITE, EMPTY ֮һ

/*����Ϊ����д�ĺ���*/

struct directs {
	//directX��ʾÿ�������м���������liveX��ʾÿ����������0����1����2
	int direct[4];
	int live[4];
};

struct point {
	int x;
	int y;
	int mark;
};

//��������
directs compute(int board[29][29], int x, int y, int side);
int markslef(directs s);
int markenemy(directs s);
void copy(int board1[29][29], int board2[29][29]);
void initialize(int board[29][29]);


point* AI(int board[29][29], int side) {
	int i, j, k;
	int marks[361] = { 0 };//i*19+j         ���е�ķ�ֵ
	int temp_max_mark = 0;
	int temp_max_j = 0;
	point *max=new point[15];//��ֵ��ߵ�15����
	int enemy_side;
	int temp_board[29][29];

	if (side == BLACK)enemy_side = WHITE;
	else enemy_side = BLACK;

	for (i = 0; i < 15; i++) {
		max[i].x = 0;
		max[i].y = 0;
		max[i].mark = 0;
	}

	for (i = 5; i < 24; i++) {
		for (j = 5; j < 24; j++) {
			if (board[i][j] == EMPTY) { //ֻ�������̵Ŀ�λ
				marks[(i - 5) * 19 + (j - 5)] = markslef(compute(board, i, j, side)) + markenemy(compute(board, i, j, enemy_side));
			}
		}
	}

	for (i = 0; i < 15; i++) { //��361������ѡ��15�����ģ�����¼����
		for (j = i; j < 361; j++) {
			if (marks[j] > temp_max_mark) {
				temp_max_mark = marks[j];
				temp_max_j = j; //��������
			}
		}
		marks[i] = temp_max_mark;
		max[i].mark = temp_max_mark;
		max[i].x = temp_max_j / 19; //����ȡ��
		max[i].y = temp_max_j % 19;
		temp_max_mark = 0;
		marks[temp_max_j] = 0;//������ѭ���ҵ������ֵ��marks�����й��㣬��������ѭ���ٴ��ҵ������
	}
	//������ɵ�һ����ѡ��
	//���¾�����δд���󲹳�

	return max;
}

//��������
int* AItree(int board[29][29], int side, point* max)
{
	int *result=new int[4];
	point *maxtree=new point[15];//�ҵ�����������15����
	for (int i = 0; i < 15; i++)
		maxtree[i].mark = maxtree[i].x = maxtree[i].y = 0;//����ֵ0
	int a = board[0][0];
	for (int i = 0; i < 15; i++)
	{
		int chessboard_tree[29][29] = { 0 };
		initialize(chessboard_tree);
		
		for (int p = 0; p < 29; p++)
		{
			for (int q = 0; q < 29; q++)
				chessboard_tree[p][q] = board[p][q];
		}
		chessboard_tree[max[i].x+5][max[i].y+5] = side;

		int marks[361] = { 0 };
		for (int k = 5; k < 24; k++) {
			for (int l = 5; l < 24; l++) {
				if (chessboard_tree[k][l] == EMPTY) { //ֻ�������̵Ŀ�λ
					marks[(k - 5) * 19 + (l - 5)] = markslef(compute(chessboard_tree, k, l, side)) + markenemy(compute(chessboard_tree, k, l, 1-side));
				}
			}
		}
		point tempmax;
		int aitree_temp_max = 0;
		for (int k = 0; k < 361; k++)//�ҵ��������ĵ�
		{
			if (marks[k] > aitree_temp_max)
			{
				aitree_temp_max = marks[k];
				tempmax.x = k / 19; 
				tempmax.y = k % 19;
				tempmax.mark = marks[k];
			}
		}
		maxtree[i].x = tempmax.x;
		maxtree[i].y = tempmax.y;
		maxtree[i].mark = tempmax.mark;
	}
	int aitree_temp_max = 0;
	for (int i = 0; i < 15; i++)//�ҵ����ķ����͵������������
	{
		if (max[i].mark + maxtree[i].mark > aitree_temp_max)
		{
			result[0] = max[i].x;
			result[1] = max[i].y;
			result[2] = maxtree[i].x;
			result[3] = maxtree[i].y;
			aitree_temp_max = max[i].mark + maxtree[i].mark;
		}
	}
	return result;
}

int markslef(directs s) { //����directs�ڵĸ����Դ��(�Լ�)
/*��X��0��
  ��1��4�� ��1��2��
  ��2��16�� ��2��3��
  ��3��64�� ��3��32��
  ��4��512�� ��4��52��
  ��5��512�� ��5��256��
  6��9999��
*/
	int sum = 0;
	int i;
	for (i = 0; i < 4; i++) {
		if (s.live[i] == 0) {//��
			if (s.direct[i] >= 6)sum += 9999;
			else sum += 0;
		}
		else if (s.live[i] == 1) { //��
			if (s.direct[i] == 1)sum += 2;
			else if (s.direct[i] == 2)sum += 3;
			else if (s.direct[i] == 3)sum += 32;
			else if (s.direct[i] == 4)sum += 52;
			else if (s.direct[i] == 5)sum += 256;
			else if (s.direct[i] >= 6)sum += 9999;
		}
		else { //��
			if (s.direct[i] == 1)sum += 4;
			else if (s.direct[i] == 2)sum += 16;
			else if (s.direct[i] == 3)sum += 64;
			else if (s.direct[i] == 4)sum += 512;
			else if (s.direct[i] == 5)sum += 512;
			else if (s.direct[i] >= 6)sum += 9999;
		}
	}
	return sum;
}

int markenemy(directs s) { //����directs�ڵĸ����Դ�֣����ˣ�
/*��X��0��
  ��1��4�� ��1��2��
  ��2��16�� ��2��3��
  ��3��64�� ��3��32��
  ��4��256�� ��4��52��
  ��5��512�� ��5��1024��
  6��5000��
*/
	int sum = 0;
	int i;
	for (i = 0; i < 4; i++) {
		if (s.live[i] == 0) {//��
			if (s.direct[i] >= 6)sum += 9999;
			else sum += 0;
		}
		else if (s.live[i] == 1) { //��
			if (s.direct[i] == 1)sum += 2;
			else if (s.direct[i] == 2)sum += 3;
			else if (s.direct[i] == 3)sum += 32;
			else if (s.direct[i] == 4)sum += 52;
			else if (s.direct[i] == 5)sum += 1024;
			else if (s.direct[i] >= 6)sum += 5000;
		}
		else { //��
			if (s.direct[i] == 1)sum += 4;
			else if (s.direct[i] == 2)sum += 16;
			else if (s.direct[i] == 3)sum += 64;
			else if (s.direct[i] == 4)sum += 256;
			else if (s.direct[i] == 5)sum += 512;
			else if (s.direct[i] >= 6)sum += 5000;
		}
	}
	return sum;
}

void initialize(int board[29][29]) { //�������������ǰ����б߽�����ĵ�ĳ�ʼֵ��Ϊ3
	int i, j;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 29; j++) {
			board[i][j] = 3;
		}
	}
	for (i = 24; i < 29; i++) {
		for (j = 0; j < 29; j++) {
			board[i][j] = 3;
		}
	}
	for (i = 5; i < 24; i++) {
		for (j = 0; j < 5; j++) {
			board[i][j] = 3;
		}
	}
	for (i = 5; i < 24; i++) {
		for (j = 24; j < 29; j++) {
			board[i][j] = 3;
		}
	}
}

directs compute(int board[29][29], int x, int y, int side) { //�ж����Ӻ��ж��ٸ������������������ĸ�����
	//�����ӵ�Ϊ���ģ�������Χ
	int i;
	int flag = 0;
	int side1, side2;
	directs s;
	s.live[0] = 0;
	s.live[1] = 0;
	s.live[2] = 0;
	s.live[3] = 0;

	//����
	side1 = 0;
	side2 = 0;

	for (i = 1; i <= 5; i++) {
		if (board[x][y - i] == side) {
			side1++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x][y - i] == EMPTY) {
		s.live[0]++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x][y + i] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x][y + i] == EMPTY) {
		s.live[0]++;
	}
	s.direct[0] = 1 + side1 + side2;


	//����
	side1 = 0;
	side2 = 0;

	for (i = 1; i <= 5; i++) {
		if (board[x - i][y] == side) {
			side1++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x - i][y] == EMPTY) {
		s.live[1]++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x + i][y] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x + i][y] == EMPTY) {
		s.live[1]++;
	}
	s.direct[1] = 1 + side1 + side2;

	//б�� ���ϵ�����
	side1 = 0;
	side2 = 0;

	for (i = 1; i <= 5; i++) {
		if (board[x - i][y - i] == side) {
			side1++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x - i][y - i] == EMPTY) {
		s.live[2]++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x + i][y + i] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x + i][y + i] == EMPTY) {
		s.live[2]++;
	}
	s.direct[2] = 1 + side1 + side2;

	//б�� ���µ�����
	side1 = 0;
	side2 = 0;

	for (i = 1; i <= 5; i++) {
		if (board[x + i][y - i] == side) {
			side1++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x + i][y - i] == EMPTY) {
		s.live[3]++;
	}

	for (i = 1; i <= 5; i++) {
		if (board[x - i][y + i] == side) {
			side2++;
			if (i == 5)i++;
		}
		else break;
	}
	if (board[x - i][y + i] == EMPTY) {
		s.live[3]++;
	}
	s.direct[3] = 1 + side1 + side2;

	return s;
}

void copy(int board1[29][29], int board2[29][29]) {
	int i, j;
	for (i = 0; i < 29; i++) {
		for (j = 0; j < 29; j++) {
			board2[i][j] = board1[i][j];
		}
	}
}

void copy(int board1[19][19], int board2[29][29]) {
	int i, j;
	for (i = 0; i < 19; i++) {
		for (j = 0; j < 19; j++) {
			board2[i + 5][j + 5] = board1[i][j];
		}
	}
}


/*����Ϊ����д�ĺ���*/


int main()
{
	Step step;//��ʱ���ṹ
	char message[256];//ͨ����Ϣ����
	int computerSide;//����ִ����ɫ
	int start = 0;//�Ծֿ�ʼ���
	srand(int(time(0)));
	//�˴����ó�ʼ������
	//...

	while (1)	//������ѭ��
	{
		fflush(stdout);//��Ҫɾ������䣬�������������
		scanf("%s", message);//��ȡƽ̨������Ϣ
		//��������
		if (strcmp(message, "name?") == 0)//���սƽ̨���Ͷ���
		{
			fflush(stdin);
			/***********��"�����"��Ϊ��Ķ�����������6�����ֻ�12��Ӣ����ĸ�������޳ɼ�************/
			/*******/		printf("name gkd��\n");		/**ֻ�޸�����壬��Ҫɾ��name�ո�****/
			/***********��"�����"��Ϊ��Ķ�����������6�����ֻ�12��Ӣ����ĸ�������޳ɼ�************/
		}
		else if (strcmp(message, "new") == 0)//���������
		{
			int i, j;
			scanf("%s", message);//��ȡ����ִ����ɫ
			fflush(stdin);
			if (strcmp(message, "black") == 0)	computerSide = BLACK;  //ִ��
			else  computerSide = WHITE;   //ִ��

			for (i = 0; i < 19; ++i)   //��ʼ�����
				for (j = 0; j < 19; ++j)
					Board[i][j] = EMPTY;
			start = 1;

			if (computerSide == BLACK)
			{
				/**********���ɵ�һ���ŷ�����������step�ṹ�У���������Ϊ(step.first.x,step.first.y)**********/
				/****************************���·������룬���滻�ҵ�ʾ������******************************/

				//�˴�������ʦ�Ĵ���
				step.first.x = 9;
				step.first.y = 9;


				/******************************����������һ���������*******************************************/

				Board[step.first.x][step.first.y] = computerSide;//����������
				//printf("move %c%c@@\n", step.first.x + 'A', step.first.y + 'A');//����ŷ�
				printf("move %c%c\n", step.first.x + 'A', step.first.y + 'A');//����ŷ�
			}
		}
		else if (strcmp(message, "move") == 0)//����,���������
		{
			scanf("%s", message);//��ȡ���������ŷ�
			fflush(stdin);
			int len = strlen(message);

			step.first.x = message[0] - 'A';		step.first.y = message[1] - 'A';
			if (len == 4)
			{
				step.second.x = message[2] - 'A';		step.second.y = message[3] - 'A';
			}
			//�����������
			Board[step.first.x][step.first.y] = 1 - computerSide;
			//if(!(step.second.x==-1 && step.second.y==-1)) Board[step.second.x][step.second.y] = 1 - computerSide;
			if (len == 4) Board[step.second.x][step.second.y] = 1 - computerSide;
			/**********************************************************************************************************/
			/***�������ӵ����꣬������step�ṹ�У���һ������(step.first.x,step.first.y)����һ������(step.first.x,step.first.y)***/
			/**************************************���·������룬���滻�ҵ�ʾ������*****************************************/
			
			int next_x;
			int next_y;
			int side = computerSide;
			int * temp;
			temp = new int[4];

			//���ɵ�1������λ��step.first.x��step.first.y
			int chessboard[29][29] = { EMPTY };//������
			initialize(chessboard);
			copy(Board, chessboard); //����ϵͳ������
			temp=AItree( chessboard,side,AI(chessboard, side));
			step.first.x = temp[0];
			step.first.y = temp[1];
			step.second.x = temp[2];
			step.second.y = temp[3];
			/*next_x = temp[0];
			next_y = temp[1];
			chessboard[next_x][next_y] = computerSide;

			step.first.x = next_x - 5;
			step.first.y = next_y - 5;*/
			Board[step.first.x][step.first.y] = computerSide;

			//���ɵ�2������λ��step.second.x��step.second.y	

			/*initialize(chessboard);
			copy(Board, chessboard); //����ϵͳ������
			temp = AI(chessboard, side, 3);
			next_x = temp[0];
			next_y = temp[1];

			step.second.x = next_x - 5;
			step.second.y = next_y - 5;*/
			Board[step.second.x][step.second.y] = computerSide;
			

			/*****************************************������������******************************************************/
			/**********************************************************************************************************/

			printf("move %c%c%c%c\n", step.first.x + 'A', step.first.y + 'A', step.second.x + 'A', step.second.y + 'A');//����ŷ�
		}
		else if (strcmp(message, "error") == 0)//�ŷ�����
		{
			fflush(stdin);
		}
		else if (strcmp(message, "end") == 0)//�Ծֽ���
		{
			fflush(stdin);
			start = 0;
		}
		else if (strcmp(message, "quit") == 0)//�˳�����
		{
			fflush(stdin);
			printf("Quit!\n");
			break;
		}
	}
	return 0;
}