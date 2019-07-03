#include "readBMP.h"
pixel *picture_data;
WORD filetype;
header BMP_head;
info_header BMP_info;
pixel line;
block *bl;
int *current;
int xdiv,ydiv;
BYTE *data;
int len;

bool readBMP(char *filename)
{
	std::ifstream fp(filename,std::fstream::binary);
	if(!fp)
		return 0;
	fp.read((char *)&filetype,sizeof(filetype));
	if(filetype!=0x4d42)
	{
		std::cout<<"请选择正确的24位BMP图片文件"<<std::endl;
		return 0;
	}
	fp.read((char *)&BMP_head,sizeof(header));
	//printf("%ld\n",BMP_head.doc_size);
	fp.read((char *)&BMP_info,sizeof(info_header));
	//std::cout<<BMP_info.width<<" "<<BMP_info.height<<std::endl;
	//std::cout<<BMP_info.xpels<<" "<<BMP_info.ypels<<std::endl;
	//std::cout<<BMP_info.data_size<<std::endl;
	int size=BMP_info.height*BMP_info.width;
	picture_data=new pixel[size];
	int counter=0;
	int len=counter*3;
	int add=0;
	if(len%4!=0)
		add=4-(len%4);
	for(int i=0;i<size;i++)
	{
		fp.read((char*)&picture_data[i],sizeof(pixel));
		counter++;
		if(counter==BMP_info.width)
		{
			fp.seekg(add,std::ios::cur);
			counter=0;
		}
	}
	fp.close();
	return true;
}

void divide_pic(int n,int m,char *filename)
{
	bl=new block[m*n+1];
	current=new int[m*n+1];
	for(int i=1;i<=m*n;i++)
		current[i]=i;
	line.r=0x00;
	line.b=0x00;
	line.g=0x00;
	BYTE useless=0x00;
	char temp[]="temp.bmp";
	std::ofstream fout(temp,std::fstream::binary);
	long width=BMP_info.width;
	long height=BMP_info.height;
	height=height/(long)m;
	ydiv=(int)height;
	height=height*(long)m;
	int real_height=(int)height;
	int del=(int)(width%(long)n);
	width=width/(long)n;
	xdiv=(int)width;
	for(int i=1;i<=m*n;i++)
	{
		bl[i].point=new pixel*[ydiv+1];
		for(int j=1;j<ydiv+1;j++)
			bl[i].point[j]=new pixel[xdiv+1];
	}
	width=width*(long)n;
	int real_width=(int)width;
	DWORD doc_size=54;
	width+=(long)n-1;
	int add=(int)(width*3%4);
	if(add!=0)
		add=4-add;
	height+=(long)m-1;
	fout.write((char *)&filetype,sizeof(filetype));
	DWORD datasize=((DWORD)width*3+(DWORD)add)*(DWORD)height;
	doc_size+=datasize;
	BMP_head.doc_size=doc_size;
	fout.write((char *)&BMP_head,sizeof(header));
	BMP_info.height=height;
	BMP_info.width=width;
	BMP_info.data_size=datasize;
	fout.write((char*)&BMP_info,sizeof(info_header));
	int cursor=0;
	int i=1,j=1;
	int xcounter=0,ycounter=0;
	int block_num;
	int xcursor,ycursor;
	while(i<real_height)
	{
		j=1;
		block_num=((i-1)/ydiv)*n;
		ycursor=((i-1)%ydiv)+1;
		ycounter++;
		xcounter=0;
		while(j<real_width)
		{
			//block_num+=(j-1)/xdiv+1;
			xcursor=((j-1)%xdiv)+1;
			fout.write((char*)&picture_data[cursor],sizeof(pixel));
			pixelcpy(bl[block_num+(j-1)/xdiv+1].point[ycursor][xcursor],picture_data[cursor]);
			cursor++;
			j++;
			xcounter++;
			if(xcounter==xdiv)
			{
				fout.write((char*)&line,sizeof(pixel));
				xcounter=0;
			}
		}
		fout.write((char*)&picture_data[cursor],sizeof(pixel));
		//block_num+=(j-1)/xdiv+1;
		xcursor=((j-1)%xdiv)+1;
		pixelcpy(bl[block_num+(j-1)/xdiv+1].point[ycursor][xcursor],picture_data[cursor]);
		cursor++;
		for(int k=0;k<add;k++)
			fout.write((char*)&useless,sizeof(BYTE));
		if(ycounter==ydiv)
		{
			for(int k=0;k<(int)(width*3+add);k++)
				fout.write((char*)&useless,sizeof(BYTE));
			ycounter=0;
		}
		cursor+=del;
		i++;
	}
	j=1;
	ycounter++;
	xcounter=0;
	block_num=(m-1)*n;
	ycursor=ydiv;
	while(j<real_width)
	{
		//block_num+=(j-1)/xdiv+1;
		xcursor==((j-1)%xdiv)+1;
		fout.write((char*)&picture_data[cursor],sizeof(pixel));
		pixelcpy(bl[block_num+(j-1)/xdiv+1].point[ycursor][xcursor],picture_data[cursor]);
		cursor++;
		j++;
		xcounter++;
		if(xcounter==xdiv)
		{
			fout.write((char*)&line,sizeof(pixel));
			xcounter=0;
		}
	}
	xcursor=((j-1)%xdiv)+1;
	pixelcpy(bl[block_num+(j-1)/xdiv+1].point[ycursor][xcursor],picture_data[cursor]);
	fout.write((char*)&picture_data[cursor],sizeof(pixel));
	cursor++;
	for(int k=0;k<add;k++)
		fout.write((char*)&useless,sizeof(BYTE));
	fout.close();
	for(int k=1;k<=ydiv;k++)
		for(int t=1;t<=xdiv;t++)
		{
			bl[n].point[k][t].b=0xc0;
			bl[n].point[k][t].g=0xc0;
			bl[n].point[k][t].r=0xc0;
		}
	std::ifstream fin(temp,std::fstream::binary);
	fin.seekg(0,std::ios::end);
	len=fin.tellg();
	data=new BYTE[len];
	fin.seekg(0,std::ios::beg);
	fin.read((char*)data,54);
	int length=(int)BMP_info.data_size/(int)BMP_info.height;
	for(int k=0;k<BMP_info.height;k++)
		fin.read((char*)(data+54+k*length),length);
	fin.close();
	//std::cout<<BMP_info.data_size<<" "<<len<<std::endl;
}

void pixelcpy(pixel &a,pixel &b)
{
	a.b=b.b;
	a.g=b.g;
	a.r=b.r;
}

void randomize(int m,int n)
{
	int newcursor,cursor;
	for(cursor=1;cursor<=m*n;cursor++)
	{
		newcursor=rand()%(m*n+1-cursor)+cursor;
		swap(*(current+cursor),*(current+newcursor));
	}
	for(cursor=1;cursor<=m*n;cursor++)
	{
		if(current[cursor]==n)
			break;
	}
	swap(current[cursor],current[n]);
	for(int i=1;i<=m*n;i++)
		writeblock(n,m,current[i],i);
	/*
	std::ofstream fp("temp.bmp",std::fstream::binary);
	fp.write((char*)data,len);
	fp.close();
	*/
}	 

void writeblock(int n,int m,int block_num,int pos)
{
	int xcursor,ycursor;
	ycursor=(pos-1)/n;
	xcursor=(pos-1)%n;
	xcursor*=(xdiv+1)*3;
	ycursor*=ydiv+1;
	int cursor=0;
	//fp.write((char*)&filetype,sizeof(filetype));
	//fp.write((char *)&BMP_head,sizeof(header));
	//fp.write((char*)&BMP_info,sizeof(info_header));
	cursor+=54;
	int length=(int)BMP_info.data_size/(int)BMP_info.height;
	//fp.seekp(ycursor*length,std::ios::cur);
	cursor+=ycursor*length;
	for(int i=1;i<=ydiv;i++)
	{
		//fp.seekp(xcursor,std::ios::cur);
		cursor+=xcursor;
		for(int j=1;j<=xdiv;j++)
		{
			//fp.write((char*)&bl[block_num].point[i][j],sizeof(pixel));
			data[cursor]=bl[block_num].point[i][j].b;
			cursor++;
			data[cursor]=bl[block_num].point[i][j].g;
			cursor++;
			data[cursor]=bl[block_num].point[i][j].r;
			cursor++;
		}
		//fp.seekp(-xdiv,std::ios::cur);
		cursor-=3*xdiv;
		//fp.seekp(-xcursor,std::ios::cur);
		cursor-=xcursor;
		//fp.seekp(length,std::ios::cur);
		cursor+=length;
	}
	cursor=0;
}

void swap(int &a,int &b)
{
	int temp=a;
	a=b;
	b=temp;
}
