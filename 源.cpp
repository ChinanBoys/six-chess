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
void copy(int board1[19][19], int board2[19][19]);
int markslef(int* result);
int markenemy(int *result);

void analyse(int thearray[19], int side, int *result);
int* analyse1(int board[19][19], int side);//x,yΪ��ǰ����λ��
void analyse_horizontal(int board[19][19], int*result, int side);//"  ��  "
void analyse_vertical(int board[19][19], int*result, int side);//"  ح  "
void analyse_oblique1(int board[19][19], int*result, int side);//"  \  "
void analyse_oblique2(int board[19][19], int*result, int side);//"  /  "
bool surround(int board[19][19], int x, int y, int side);//�����жϸ�����Χ�������ǲ��ǿ�

bool surround(int board[19][19], int x, int y, int side)//��Χ�����ӷ���ture��ȫ�ǿհ׷���false
{
	for (int i = -3; i < 4; i++)
	{
		if (x + i > 18)
			break;
		for (int j = -3; j < 4; j++)
		{
			while (x+i < 0)
				i += 1;
			while (y+j < 0)
				j += 1;
			if (i == 0 & j == 0)
				j += 1;
			if (y + j > 18)
				break;
			if (board[x + i][y + j] == WHITE || board[x + i][y + j] == BLACK)
				return true;
		}
	}
	return false;
}

void analyse_horizontal(int board[19][19], int*result, int side)
{
	int thearray[19];
	for (int x = 0; x < 19; x++)
	{
		for (int y = 0; y < 19; y++)
		{
			if (board[x][y] == side)
			{
				for (int l = 0; l < 19; l++)
					thearray[l] = board[x][l];
				analyse(thearray, side, result);
				break;
			}
		}
	}
}
void analyse_vertical(int board[19][19], int*result, int side)
{
	int thearray[19];
	for (int x = 0; x < 19; x++)
	{
		for (int y = 0; y < 19; y++)
		{
			if (board[y][x] == side)
			{
				for (int l = 0; l < 19; l++)
					thearray[l] = board[l][x];
				analyse(thearray, side, result);
				break;
			}
		}
	}
}
void analyse_oblique1(int board[19][19], int*result, int side)//"  \  "
{
	int thearray[19];
	for (int x = 5; x < 19; x++)
	{
		int judge = 0;
		for (int k = 0; k < 19; k++)
			thearray[k] = -1;
		for (int y = 0; y <= x; y++)
		{
			if (board[y][18 - x + y] == side)
			{
				judge = 1;
				break;
			}
		}
		if (judge == 1)
		{
			for (int y = 0; y <= x; y++)
				thearray[y] = board[y][18 - x + y];
			analyse(thearray, side, result);
		}
	}
	for (int x = 1; x < 14; x++)
	{
		int judge = 0;
		for (int k = 0; k < 19; k++)
			thearray[k] = -1;
		for (int y = 0; y <= 18 - x; y++)
		{
			if (board[x + y][y] == side)
			{
				judge = 1;
				break;
			}
		}
		if (judge == 1)
		{
			for (int y = 0; y <= 18 - x; y++)
				thearray[y] = board[x + y][y];
			analyse(thearray, side, result);
		}
	}
}
void analyse_oblique2(int board[19][19], int*result, int side)//"  /  "
{
	int thearray[19];
	for (int x = 5; x < 19; x++)
	{
		int judge = 0;
		for (int k = 0; k < 19; k++)
			thearray[k] = -1;
		for (int y = 0; y <= x; y++)
		{
			if (board[y][x - y] == side)
			{
				judge = 1;
				break;
			}
		}
		if (judge == 1)
		{
			for (int y = 0; y <= x; y++)
				thearray[y] = board[y][x - y];
			analyse(thearray, side, result);
		}
	}
	for (int x = 1; x < 14; x++)
	{
		int judge = 0;
		for (int k = 0; k < 19; k++)
			thearray[k] = -1;
		for (int y = 0; y <= 18 - x; y++)
		{
			if (board[x + y][18 - y] == side)
			{
				judge = 1;
				break;
			}
		}
		if (judge == 1)
		{
			for (int y = 0; y <= 18 - x; y++)
				thearray[y] = board[x + y][18 - y];
			analyse(thearray, side, result);
		}
	}
}

void analyse(int thearray[19], int side, int *result)
{
	//1&&2������������
	int judge_continuous = 0;//�����ж϶��ٸ�ͬ����������
	for (int i = 0; i < 19; i++)
	{
		if (thearray[i] == side)
		{
			judge_continuous += 1;
			if (judge_continuous == 6)
			{
				result[0] += 1;
				for (int j = 0; j < 6; j++)
					thearray[i - j] = 1 - side;
			}
			if (judge_continuous > 6 && ((i < 18 && thearray[i + 1] != side) || i == 18))
			{
				result[1] += 1;
				for (int j = 0; j < judge_continuous; j++)
				{
					if (thearray[i - j] == side)
						thearray[i - j] = -1;
				}
			}
		}
		else
			judge_continuous = 0;
	}

	//3����5
	judge_continuous = 0;//�ж�ֵ����
	for (int i = 0; i < 19; i++)
	{
		if (i > 5 && judge_continuous == 5)
		{
			if (thearray[i] == EMPTY && thearray[i - 6] == EMPTY)//��ͷΪ��
			{
				result[2] += 1;
				for (int j = 1; j < 6; j++)
				{
					if (thearray[i - j] == side)
						thearray[i - j] = -1;
				}
			}
		}
		if (thearray[i] == side)
			judge_continuous += 1;
		else
			judge_continuous = 0;
	}
	//4����5�ڣ�__OO__OOO__
	judge_continuous = 0;//�ж�ֵ����
	for (int i = 0; i < 19; i++)
	{
		int recover_i = i;
		int judgeempty = 0;
		if (thearray[i] == side || thearray[i] == EMPTY)
		{
			if (thearray[i] == EMPTY)
				judgeempty += 1;
			i += 1;
			judge_continuous += 1;
			if (i > 14)
				break;
			int tempi = i + 5;
			for (i; i < tempi; i++)
			{
				if (thearray[i] == side || thearray[i] == EMPTY)
				{
					judge_continuous += 1;
					if (thearray[i] == EMPTY)
						judgeempty += 1;
				}
				else if (thearray[i] == -1)
				{
					while (i < 18 && thearray[i + 1] != -1)
						i += 1;
					judge_continuous = 0;
					goto here4;
				}
				else
				{
					i = recover_i;
					break;
				}
			}
		}
		if (judge_continuous == 6 && judgeempty == 1)
		{
			result[3] += 1;
			for (int j = 1; j < 7; j++)
			{
				if (thearray[i - j] == side)
					thearray[i - j] = -1;
			}
		}
		else
		{
			judge_continuous = 0;
			i = recover_i;
		}
	here4:		i = i;
	}
	//5����5
	judge_continuous = 0;//�ж�ֵ����
	for (int i = 0; i < 19; i++)
	{
		int recover_i = i;
		int judgeempty = 0;
		if (thearray[i] == 1 - side)
		{
			i += 1;
			if (i > 14)
				break;
			int tempi = i + 5;
			for (i; i < tempi; i++)
			{
				if (thearray[i] == side || thearray[i] == EMPTY)
					judge_continuous += 1;
				else if (thearray[i] == -1)
				{
					while (i < 18 && thearray[i + 1] != -1)
						i += 1;
					judge_continuous = 0;
					goto here5;
				}
				else
				{
					i = recover_i;
					break;
				}
			}
		}
		if (i < 19 && judge_continuous == 5 && thearray[i] == 1 - side)
		{
			result[4] += 1;
			for (int j = 1; j < 6; j++)
			{
				if (thearray[i - j] == side)
					thearray[i - j] = -1;
			}
		}
		else
		{
			judge_continuous = 0;
			i = recover_i;
		}
	here5:i = i;
	}
	//6����4
	judge_continuous = 0;//�ж�ֵ����
	for (int i = 0; i < 19; i++)
	{
		if (i > 4 && judge_continuous == 4)
		{
			if (thearray[i] == EMPTY && thearray[i - 5] == EMPTY)//��ͷΪ��
			{
				result[5] += 1;//����Ҫ��������Ϊ����ͬʱ����������4�������������ж�
				for (int j = 1; j < 5; j++)
				{
					if (thearray[i - j] == side)
						thearray[i - j] = -1;
				}
			}
		}
		if (thearray[i] == side)
			judge_continuous += 1;
		else
			judge_continuous = 0;
	}
	//7����4��__OO____OO__����
	judge_continuous = 0;//�ж�ֵ����
	for (int i = 0; i < 19; i++)
	{

		int recover_i = i;
		int judgeempty = 0;
		if (thearray[i] == side || thearray[i] == EMPTY)
		{
			if (thearray[i] == EMPTY)
				judgeempty += 1;
			i += 1;
			judge_continuous += 1;
			if (i > 14)
				break;
			int tempi = i + 5;
			for (i; i < tempi; i++)
			{
				if (thearray[i] == side || thearray[i] == EMPTY)
				{
					judge_continuous += 1;
					if (thearray[i] == EMPTY)
						judgeempty += 1;
				}
				else if (thearray[i] == -1)
				{
					while (i < 18 && thearray[i + 1] != -1)
						i += 1;
					judge_continuous = 0;
					goto here7;
				}
				else
				{
					i = recover_i;
					break;
				}
			}
		}
		if (judge_continuous == 6 && judgeempty == 2)
		{
			result[6] += 1;
			for (int j = 1; j < 7; j++)
			{
				if (thearray[i - j] == side)
					thearray[i - j] = -1;
			}
		}
		else
		{
			judge_continuous = 0;
			i = recover_i;
		}
	here7:i = i;
	}
	//8����4
	judge_continuous = 0;//�ж�ֵ����
	for (int i = 0; i < 19; i++)
	{
		int recover_i = i;
		int judgeempty = 0;
		if (thearray[i] == 1 - side)
		{
			i += 1;
			if (i > 14)
				break;
			int tempi = i + 4;
			for (i; i < tempi; i++)
			{
				if (thearray[i] == side || thearray[i] == EMPTY)
					judge_continuous += 1;
				else if (thearray[i] == -1)
				{
					while (i < 18 && thearray[i + 1] != -1)
						i += 1;
					judge_continuous = 0;
					goto here8;
				}
				else
				{
					i = recover_i;
					break;
				}
			}
		}
		if (i < 19 && judge_continuous == 4 && thearray[i] == 1 - side)
		{
			result[7] += 1;
			for (int j = 1; j < 5; j++)
			{
				if (thearray[i - j] == side)
					thearray[i - j] = -1;
			}
		}
		else
		{
			judge_continuous = 0;
			i = recover_i;
		}
	here8:i = i;
	}
	//9����3
	judge_continuous = 0;//�ж�ֵ����
	for (int i = 0; i < 19; i++)
	{
		if (i > 3 && judge_continuous == 3)
		{
			if (thearray[i] == EMPTY && thearray[i - 4] == EMPTY)//��ͷΪ��
			{
				result[8] += 1;//����Ҫ��������Ϊ����ͬʱ����������3�������������ж�
				for (int j = 1; j < 4; j++)
				{
					if (thearray[i - j] == side)
						thearray[i - j] = -1;
				}
			}
		}
		if (thearray[i] == side)
			judge_continuous += 1;
		else
			judge_continuous = 0;
	}
	//10������3��__O__O__O__     __OO____O__
	judge_continuous = 0;//�ж�ֵ����
	for (int i = 0; i < 19; i++)
	{
		int recover_i = i;
		int judgeempty = 0;
		if (thearray[i] == EMPTY)
		{
			i += 1;
			if (i > 13)
				break;
			int tempi = i + 5;
			for (i; i < tempi; i++)
			{
				if (thearray[i] == side || thearray[i] == EMPTY)
				{
					judge_continuous += 1;
					if (thearray[i] == EMPTY)
						judgeempty += 1;
				}
				else if (thearray[i] == -1)
				{
					while (i < 18 && thearray[i + 1] != -1)
						i += 1;
					judge_continuous = 0;
					goto here10;
				}
				else
				{
					i = recover_i;
					break;
				}
			}
		}
		if (judge_continuous == 5 && judgeempty == 2 && thearray[i] == EMPTY)
		{
			result[9] += 1;
			for (int j = 1; j < 6; j++)
			{
				if (thearray[i - j] == side)
					thearray[i - j] = -1;
			}
		}
		else
		{
			judge_continuous = 0;
			i = recover_i;
		}
	here10:i = i;
	}
	//11����3��__O__OO__
	judge_continuous = 0;//�ж�ֵ����
	for (int i = 0; i < 19; i++)
	{
		int recover_i = i;
		int judgeempty = 0;
		if (thearray[i] == side || thearray[i] == EMPTY)
		{
			if (thearray[i] == EMPTY)
				judgeempty += 1;
			i += 1;
			judge_continuous += 1;
			if (i > 14)
				break;
			int tempi = i + 5;
			for (i; i < tempi; i++)
			{
				if (thearray[i] == side || thearray[i] == EMPTY)
				{
					judge_continuous += 1;
					if (thearray[i] == EMPTY)
						judgeempty += 1;
				}
				else if (thearray[i] == -1)
				{
					while (i < 18 && thearray[i + 1] != -1)
						i += 1;
					judge_continuous = 0;
					goto here11;
				}
				else
				{
					i = recover_i;
					break;
				}
			}
		}
		if (judge_continuous == 6 && judgeempty == 3)
		{
			result[10] += 1;
			for (int j = 1; j < 7; j++)
			{
				if (thearray[i - j] == side)
					thearray[i - j] = -1;
			}
		}
		else
		{
			judge_continuous = 0;
			i = recover_i;
		}
	here11:i = i;
	}
	//12����3
	judge_continuous = 0;//�ж�ֵ����
	for (int i = 0; i < 19; i++)
	{
		int recover_i = i;
		int judgeempty = 0;
		if (thearray[i] == 1 - side)
		{
			i += 1;
			if (i > 15)
				break;
			int tempi = i + 3;
			for (i; i < tempi; i++)
			{
				if (thearray[i] == side || thearray[i] == EMPTY)
					judge_continuous += 1;
				else if (thearray[i] == -1)
				{
					while (i < 18 && thearray[i + 1] != -1)
						i += 1;
					judge_continuous = 0;
					goto here12;
				}
				else
				{
					i = recover_i;
					break;
				}
			}
		}
		if (i < 19 && judge_continuous == 3 && thearray[i] == 1 - side)
		{
			result[11] += 1;
			for (int j = 1; j < 4; j++)
			{
				if (thearray[i - j] == side)
					thearray[i - j] = -1;
			}
		}
		else
		{
			judge_continuous = 0;
			i = recover_i;
		}
	here12:i = i;
	}
	//13����2��__OO__
	judge_continuous = 0;//�ж�ֵ����
	for (int i = 0; i < 19; i++)
	{
		if (i > 2 && judge_continuous == 2)
		{
			if (thearray[i] == EMPTY && thearray[i - 3] == EMPTY)//��ͷΪ��
			{
				result[12] += 1;//����Ҫ��������Ϊ����ͬʱ����������3�������������ж�
				for (int j = 1; j < 3; j++)
				{
					if (thearray[i - j] == side)
						thearray[i - j] = -1;
				}
			}
		}
		if (thearray[i] == side)
			judge_continuous += 1;
		else
			judge_continuous = 0;
	}
	//14����2��������X��������O
	judge_continuous = 0;//�ж�ֵ����
	for (int i = 0; i < 19; i++)
	{
		int recover_i = i;
		int judgeempty = 0;
		if (thearray[i] == side || thearray[i] == EMPTY)
		{
			if (thearray[i] == EMPTY)
				judgeempty += 1;
			i += 1;
			judge_continuous += 1;
			if (i > 14)
				break;
			int tempi = i + 5;
			for (i; i < tempi; i++)
			{
				if (thearray[i] == side || thearray[i] == EMPTY)
				{
					judge_continuous += 1;
					if (thearray[i] == EMPTY)
						judgeempty += 1;
				}
				else if (thearray[i] == -1)
				{
					while (i < 18 && thearray[i + 1] != -1)
						i += 1;
					judge_continuous = 0;
					goto here14;
				}
				else
				{
					i = recover_i;
					break;
				}
			}
		}
		if (judge_continuous == 6 && judgeempty == 4)
		{
			result[13] += 1;
			for (int j = 1; j < 7; j++)
			{
				if (thearray[i - j] == side)
					thearray[i - j] = -1;
			}
		}
		else
		{
			judge_continuous = 0;
			i = recover_i;
		}
	here14:i = i;
	}
	//15����2
	judge_continuous = 0;//�ж�ֵ����
	for (int i = 0; i < 19; i++)
	{
		int recover_i = i;
		int judgeempty = 0;
		if (thearray[i] == 1 - side)
		{
			i += 1;
			if (i > 16)
				break;
			int tempi = i + 2;
			for (i; i < tempi; i++)
			{
				if (thearray[i] == side || thearray[i] == EMPTY)
					judge_continuous += 1;
				else if (thearray[i] == -1)
				{
					while (i < 18 && thearray[i + 1] != -1)
						i += 1;
					judge_continuous = 0;
					goto here15;
				}
				else
				{
					i = recover_i;
					break;
				}
			}
		}
		if (i < 19 && judge_continuous == 2 && thearray[i] == 1 - side)
		{
			result[14] += 1;
			for (int j = 1; j < 3; j++)
			{
				if(thearray[i-j]==side)
				thearray[i - j] = -1;
			}
		}
		else
		{
			judge_continuous = 0;
			i = recover_i;
		}
	here15:i = i;
	}
}




int* analyse1(int board[19][19], int side)//x,yΪ��ǰ����λ��
{
	int *result = new int[15];//15������
	for (int i = 0; i < 15; i++)
		result[i] = 0;
	analyse_horizontal(board, result, side);
	analyse_vertical(board, result, side);
	analyse_oblique1(board, result, side);
	analyse_oblique2(board, result, side);
	return result;
}




point* AI(int board[19][19], int side) {
	int i, j, k;
	int marks[361] = { 0 };//i*19+j         ���е�ķ�ֵ
	int temp_max_mark = 0;
	int temp_max_j = 0;
	point *max = new point[360];//��ֵ��ߵ�15����
	int enemy_side;
	int temp_board[19][19];

	if (side == BLACK)enemy_side = WHITE;
	else enemy_side = BLACK;

	for (i = 0; i < 360; i++) {
		max[i].x = 0;
		max[i].y = 0;
		max[i].mark = -1;
	}
	int maxcount = 0;
	for (i = 0; i < 19; i++) {
		for (j = 0; j < 19; j++) {
			if (board[i][j] == EMPTY&&surround(board,i,j,side)) { //ֻ�������̵Ŀ�λ
				{
					int boardtemp[19][19];
					copy(board, boardtemp);
					int mark_enemy1 = markenemy(analyse1(boardtemp, enemy_side));
					boardtemp[i][j] = side;
					int mark_self = markslef(analyse1(boardtemp, side));
					boardtemp[i][j] = enemy_side;
					int mark_enemy2 = markenemy(analyse1(boardtemp, enemy_side));
					max[maxcount].mark = mark_self + mark_enemy2-mark_enemy1;
					max[maxcount].x = i;
					max[maxcount].y = j;
					maxcount += 1;
				}
			}
		}
	}


	return max;
}

//��������
int* AItree(int board[19][19], int side, point* max)
{
	int *result = new int[4];
	int enemy_side = 1 - side;
	point *maxtree = new point[360];//�ҵ�����������15����
	for (int i = 0; i < 360; i++)
		maxtree[i].mark = maxtree[i].x = maxtree[i].y = -1;//����ֵ0
	for (int i = 0; i < 360; i++)
	{
		int chessboard_tree[19][19] = { 0 };
		copy(board, chessboard_tree);
		if (max[i].mark >= 0)
			chessboard_tree[max[i].x][max[i].y] = side;
		else
			break;
		int marks[361] = { 0 };
		for (int k = 0; k < 19; k++) {
			for (int l = 0; l < 19; l++) {
				if (chessboard_tree[k][l] == EMPTY&&surround(chessboard_tree,k,l,side)) { //ֻ�������̵Ŀ�λ
					if (k == 4 && l == 9&&max[i].x==4&&max[i].y==4)
						k = k;
					int boardtemp[19][19];
					copy(chessboard_tree, boardtemp);
					int mark_enemy1 = markenemy(analyse1(boardtemp, enemy_side));
					boardtemp[k][l] = side;
					int mark_self = markslef(analyse1(boardtemp, side));
					boardtemp[k][l] = enemy_side;
					int mark_enemy2 = markenemy(analyse1(boardtemp, enemy_side));
					marks[k * 19 + l] = mark_self + mark_enemy2 - mark_enemy1;
				}
			}
		}
		point tempmax;
		tempmax.mark = tempmax.x = tempmax.y = 0;
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
	for (int i = 0; i < 360; i++)//�ҵ����ķ����͵������������
	{
		if (max[i].mark >= 0)
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
		else
			break;
	}
	return result;
}

int markslef(int* result) { //����result�ڵĸ����Դ��(�Լ�)
/*1.����:10000
2.����:10000
3.��5��128
4.��5��31
5.��5:0
6.��4:1024
7:��4:512
8��4:0
9.��3:256
10.����3��32
11.��3��32
12.��3��0
13.��2��16
14.��2��3
15.��2: 0
*/
	int sum = 0;
	sum += result[0] * 10000;
	sum += result[1] * 10000;
	sum += result[2] * 128;
	sum += result[3] * 31;
	sum += result[4] * 0;
	sum += result[5] * 1024;
	sum += result[6] * 512;
	sum += result[7] * 0;
	sum += result[8] * 256;
	sum += result[9] * 32;
	sum += result[10] * 31;
	sum += result[11] * 0;
	sum += result[12] * 16;
	sum += result[13] * 3;
	sum += result[14] * 0;
	return sum;
}

int markenemy(int *result) { //����result�ڵĸ����Դ�֣����ˣ�
/*1.����:5000
2.����:5000
3.��5��2048
4.��5��1024
5.��5:0
6.��4:1024
7:��4:512
8��4:0
9.��3:256
10.����3��32
11.��3��32
12.��3��0
13.��2��16
14.��2��3
15.��2: 0
*/
	int sum = 0;
	sum += result[0] * 5000;
	sum += result[1] * 5000;
	sum += result[2] * 2048;
	sum += result[3] * 1024;
	sum += result[4] * 0;
	sum += result[5] * 1024;
	sum += result[6] * 512;
	sum += result[7] * 0;
	sum += result[8] * 256;
	sum += result[9] * 32;
	sum += result[10] * 31;
	sum += result[11] * 0;
	sum += result[12] * 16;
	sum += result[13] * 3;
	sum += result[14] * 0;
	return sum;
}


void copy(int board1[19][19], int board2[19][19]) {
	int i, j;
	for (i = 0; i < 19; i++) {
		for (j = 0; j < 19; j++) {
			board2[i][j] = board1[i][j];
		}
	}
}

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
			/*******/		printf("name gkdtest��\n");		/**ֻ�޸�����壬��Ҫɾ��name�ո�****/
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
			temp = AItree(Board, side, AI(Board, side));
			step.first.x = temp[0];
			step.first.y = temp[1];
			step.second.x = temp[2];
			step.second.y = temp[3];

			Board[step.first.x][step.first.y] = computerSide;

			//���ɵ�2������λ��step.second.x��step.second.y	


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



