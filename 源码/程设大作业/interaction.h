#pragma once
#include "readBMP.h"

extern int blank,m,n;

bool judge(int m,int n);       //复原判断

void interact(char ctr);      //接受并响应用户输入的主要函数

bool sovablility();//可解性判断

int inverted_sequence_num();   //求逆序数

void autosolve();        //自动求解
                                     //接下来几个都是自动求解需要调用的函数
void normalsolve(int pos);
void normalmove(int pos,int to);
void blankmove_left(int i,int j);
void blankmove_right(int i,int j);
void blankmove_up(int i,int j);
void moveastep(char ctr);

void show();           //输出状态

extern int counter;

extern int *originstatus,*status_temp;


struct linknode                   //链表节点 用于排行榜的存储
{
	char name[256];
	double time;
	linknode *pre,*next;
};

class linklist                 //链表类
{
public:
	void add(char *n,double t); //加入节点
	int num;
	linknode *head;
	linklist();
	void save();              //向文件种输出，永久保存
	void buildboard();        //载入
	void showboard();         //显示
};

extern linklist leaderboard;

int display(int k);