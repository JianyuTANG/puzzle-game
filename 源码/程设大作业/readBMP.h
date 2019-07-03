#pragma once
#include <fstream>
#include <iostream>
typedef unsigned char BYTE;  
typedef unsigned short WORD;  
typedef unsigned long DWORD;  

struct header        //ͼƬͷ
{
	DWORD doc_size;
	WORD reserved_1;
	WORD reserved_2;
	DWORD data_start;
};

struct info_header    //ͼƬ��Ϣͷ
{
	DWORD size;
	long width;
	long height;
	WORD planes;
	WORD color_bit;
	DWORD compression;
	DWORD data_size;
	long xpels;
	long ypels;
	DWORD colors;
	DWORD important_colors;
};

struct pixel          //����RGB
{
	BYTE b;
	BYTE g;
	BYTE r;
};

struct block          //�洢ÿһ�����Ϣ
{
	pixel **point;
};

extern block *bl;

bool readBMP(char *filename);     //��ͼ

void divide_pic(int n,int m,char *filename);  //�ָ�

void pixelcpy(pixel &a,pixel &b);      //����������Ϣ

extern int *current;

void swap(int &a,int &b);         

void writeblock(int n,int m,int block_num,int pos);    //д������Ϣ

void randomize(int m,int n);     //����

extern BYTE *data;

extern int len;