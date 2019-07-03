#pragma once
#include <fstream>
#include <iostream>
typedef unsigned char BYTE;  
typedef unsigned short WORD;  
typedef unsigned long DWORD;  

struct header        //图片头
{
	DWORD doc_size;
	WORD reserved_1;
	WORD reserved_2;
	DWORD data_start;
};

struct info_header    //图片信息头
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

struct pixel          //像素RGB
{
	BYTE b;
	BYTE g;
	BYTE r;
};

struct block          //存储每一块的信息
{
	pixel **point;
};

extern block *bl;

bool readBMP(char *filename);     //读图

void divide_pic(int n,int m,char *filename);  //分割

void pixelcpy(pixel &a,pixel &b);      //拷贝像素信息

extern int *current;

void swap(int &a,int &b);         

void writeblock(int n,int m,int block_num,int pos);    //写入块的信息

void randomize(int m,int n);     //打乱

extern BYTE *data;

extern int len;