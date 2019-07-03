#pragma once
#include "readBMP.h"

extern int blank,m,n;

bool judge(int m,int n);       //��ԭ�ж�

void interact(char ctr);      //���ܲ���Ӧ�û��������Ҫ����

bool sovablility();//�ɽ����ж�

int inverted_sequence_num();   //��������

void autosolve();        //�Զ����
                                     //���������������Զ������Ҫ���õĺ���
void normalsolve(int pos);
void normalmove(int pos,int to);
void blankmove_left(int i,int j);
void blankmove_right(int i,int j);
void blankmove_up(int i,int j);
void moveastep(char ctr);

void show();           //���״̬

extern int counter;

extern int *originstatus,*status_temp;


struct linknode                   //����ڵ� �������а�Ĵ洢
{
	char name[256];
	double time;
	linknode *pre,*next;
};

class linklist                 //������
{
public:
	void add(char *n,double t); //����ڵ�
	int num;
	linknode *head;
	linklist();
	void save();              //���ļ�����������ñ���
	void buildboard();        //����
	void showboard();         //��ʾ
};

extern linklist leaderboard;

int display(int k);