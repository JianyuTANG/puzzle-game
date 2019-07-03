#include "interaction.h"
int counter;
int *status_temp;
linklist leaderboard;

bool judge(int m,int n)
{
	for(int i=1;i<=m*n;i++)
		if(current[i]!=i)
			return false;
	return true;
}

void interact(char ctr)
{
	switch (ctr)
	{
	case 'W':
		if((blank-1)/n==m-1) 
		{
			std::cout<<"非法移动，当前已在最顶部"<<std::endl;
			return;
		}
		swap(*(current+blank),*(current+blank+n));
		writeblock(n,m,current[blank],blank);
		writeblock(n,m,current[blank+n],blank+n);
		blank+=n;
		break;
	case 'D':
		if((blank-1)%n==n-1)
		{
			std::cout<<"非法移动，当前已在最右侧"<<std::endl;
			return;
		}
		swap(current[blank],current[blank+1]);
		writeblock(n,m,current[blank],blank);
		writeblock(n,m,current[blank+1],blank+1);
		blank++;
		break;
	case 'S':
		if((blank-1)/n==0)
		{
			std::cout<<"非法移动，当前已在最底部"<<std::endl;
			return;
		}
		swap(*(current+blank),*(current+blank-n));
		writeblock(n,m,current[blank],blank);
		writeblock(n,m,current[blank-n],blank-n);
		blank-=n;
		break;
	case 'A':
		if((blank-1)%n==0)
		{
			std::cout<<"非法移动，当前已在最左侧"<<std::endl;
			return;
		}
		swap(current[blank],current[blank-1]);
		writeblock(n,m,current[blank],blank);
		writeblock(n,m,current[blank-1],blank-1);
		blank--;
		break;
	case 'R':
		for(int i=1;i<=m*n;i++)
			current[i]=originstatus[i];
		blank=n;
		break;
	case 'G':
		{
			char output_filename[256];
			printf("请输入您要保存图片的完整路径与名称:\n");
			std::cin>>output_filename;
			std::ofstream fout1(output_filename,std::fstream::binary);
			fout1.write((char*)data,len);
			fout1.close();
			printf("保存成功！\n");
			break;
		}
	case 'I':{
		printf("请输入进度名称(英文字符):\n");
		char progress[256];
		std::cin>>progress;
		int k=0;
		for(;progress[k]!='\0';k++);
		progress[k]='.';
		k++;
		progress[k]='t';
		k++;
		progress[k]='x';
		k++;
		progress[k]='t';
		k++;
		progress[k]='\0';
		FILE *fprogress;
		fprogress=fopen(progress,"w");
		for(k=1;k<=m*n;k++)
			fprintf(fprogress,"%d ",current[k]);
		fprintf(fprogress,"%d",blank);
		fclose(fprogress);
		printf("保存进度成功！\n");
		break;
			 }
	case 'O':{
		printf("请输入要读取的进度名称(英文字符):\n");
		char progress[256];
		std::cin>>progress;
		int k=0;
		for(;progress[k]!='\0';k++);
		progress[k]='.';
		k++;
		progress[k]='t';
		k++;
		progress[k]='x';
		k++;
		progress[k]='t';
		k++;
		progress[k]='\0';
		FILE *fprogress;
		fprogress=fopen(progress,"r");
		for(k=1;k<=m*n;k++)
			fscanf(fprogress,"%d ",&current[k]);
		fscanf(fprogress,"%d",&blank);
		fclose(fprogress);
		for(int i=1;i<=m*n;i++)
			writeblock(n,m,current[i],i);
		printf("读取进度成功！\n");
		break;
			 }
	case 'F':
		autosolve();
		break;
	case 'B':{
		leaderboard.showboard();
		break;
			 }
	default:
		break;
	}	
}

bool sovablility()
{
	if(inverted_sequence_num()%2==0)
		return true;
	else
		return false;
}

int inverted_sequence_num()
{
	int answer=0;
	for(int i=1;i<m*n;i++)
		for(int j=i+1;j<=m*n;j++)
			if(current[i]>current[j])
				answer++;
	return answer;
}

int search1(int target)
{
	for(int i=1;i<=m*n;i++)
		if(status_temp[i]==target)
			return i;
}

int getx(int pos)
{
	int x=(pos-1)%n+1;
	return x;
}

int gety(int pos)
{
	int y;
	y=(pos-1)/n;
	return y;
}

int getpos(int i,int j)
{
	return (i-1)*n+j;
}

void autosolve()
{
	for(int i=1;i<=m*n;i++)
		status_temp[i]=current[i];
	int tempblank=blank;
	counter=0;
	int i=m,j=1;
	int targetpos,blankpos=blank;
	printf("步骤如下：\n");
	for(;i>2;i--)
	{
		for(j=1;j<n;j++)
		{
			targetpos=search1((i-1)*n+j);
			if(targetpos==getpos(i,j))
				continue;
			normalsolve(targetpos);
		}
		targetpos=search1(i*n);
		if(targetpos==i*n)
			continue;
		normalmove(targetpos,(i-1)*n);
		if(blank==i*n)
		{
			moveastep('w');
			continue;
		}
		blankmove_left(i-2,n);
		moveastep('w');
		moveastep('a');
		moveastep('s');
		moveastep('s');
		moveastep('d');
		moveastep('w');
		moveastep('a');
		moveastep('w');
		moveastep('d');
		moveastep('s');
		moveastep('s');
		moveastep('a');
		moveastep('w');
	}
	//show();
	//std::cout<<std::endl;
	for(j=1;j<n-1;j++)
	{
		normalsolve(search1(n+j));
		targetpos=search1(j);
		if(targetpos==j)
			continue;
		//show();
		//std::cout<<std::endl;
		normalmove(targetpos,j+1);
		//show();
		//std::cout<<std::endl;
		if(blank==j)
		{
			moveastep('a');
			continue;
		}
		blankmove_right(0,j+1);
		//show();
		//std::cout<<std::endl;
		moveastep('d');
		moveastep('d');
		moveastep('s');
		moveastep('a');
		moveastep('w');
		moveastep('a');
		moveastep('s');
		moveastep('d');
		moveastep('d');
		moveastep('w');
		moveastep('a');
	}
	normalsolve(search1(n+j));
	normalsolve(search1(j));
	if(status_temp[n]!=n)
		moveastep('w');
	printf("\n共用步数：%d\n",counter);
	blank=tempblank;
}

void normalsolve(int pos)
{
	normalmove(pos,status_temp[pos]);
}

void normalmove(int pos,int to)
{
	int i,j;
	i=gety(pos);
	j=getx(pos);
	int ti=gety(to);
	int tj=getx(to);
	if(j<tj)
	{
		blankmove_right(i,j);
		while(j<tj-1)
		{
			moveastep('d');
			j++;
			blankmove_right(i,j);
		}
		moveastep('d'); 
		j++;
	}
	else if(j>tj)
	{
		blankmove_left(i,j);
		while(j>tj+1)
		{
			moveastep('a');
			j--;
			blankmove_left(i,j);
		}
		moveastep('a');
		j--;
	}
	if(i<ti)
	{
		blankmove_up(i,j);
		while(i<ti-1)
		{
			moveastep('w');
			i++;
			blankmove_up(i,j);
		}
		moveastep('w');
		i++;
	}
	else if(i>ti)
	{
		blankmove_right(1,j);
		moveastep('w');
		moveastep('d');
		moveastep('s');
	}
}

void blankmove_up(int i,int j)
{
	if(blank==getpos(i+2,j))
		return;
	int bi=gety(blank);
	int bj=getx(blank);
	if(i==0&&j==n)
	{
		if(bj<j)
		{
			while(bj<j-1)
			{
				moveastep('a');
				bj++;
			}
			if(bi==i)
			{
				moveastep('s');
				bi++;
				moveastep('a');
				bj++;
			}
			else if(bi>i)
			{
				moveastep('a');
				bj++;
				while(bi>i+1)
				{
					moveastep('w');
					bi--;
				}
			}
		}
		else if(bj==j)
		{
			while(bi>i+1)
			{
				moveastep('w');
				bi--;
			}
		}
	}
	else if(i==0)
	{
		if(bi>i)
		{
			while(bi>i+1)
			{
				moveastep('w');
				bi--;
			}
			if(bj<j)
			{
				while(bj<j)
				{
					moveastep('a');
					bj++;
				}
			}
			else if(bj>j)
			{
				while(bj>j)
				{
					moveastep('d');
					bj--;
				}
			}
		}
		else if(bi==i)
		{
			moveastep('s');
			bi++;
			if(bj<j)
			{
				while(bj<j)
				{
					moveastep('a');
					bj++;
				}
			}
			else if(bj>j)
			{
				while(bj>j)
				{
					moveastep('d');
					bj--;
				}
			}
		}
	}
	else if(j==n)
	{
		if(bj<n)
		{
			while(bj<n-1)
			{
				moveastep('a');
				bj++;
			}
			if(bi>i)
			{
				while(bi>i+1)
				{
					moveastep('w');
					bi--;
				}
				moveastep('a');
				bj++;
			}
			else if(bi<=i)
			{
				while(bi<i+1)
				{
					moveastep('s');
					bi++;
				}
				moveastep('a');
				bj++;
			}
		}
		else if(bj==n)
		{
			if(bi>i)
			{
				while(bi>i+1)
				{
					moveastep('w');
					bi--;
				}
			}
			else
			{
				moveastep('d');
				bj--;
				while(bi<i+1)
				{
					moveastep('s');
					bi++;
				}
				moveastep('a');
				bj++;
			}
		}
	}
	else
	{
		if(bj<j)
		{
			while(bj<j-1)
			{
				moveastep('a');
				bj++;
			}
			if(bi>i)
			{
				moveastep('a');
				bj++;
				while(bi>i+1)
				{
					moveastep('w');
					bi--;
				}
			}
			else if(bi<i)
			{
				moveastep('a');
				bj++;
				moveastep('a');
				bj++;
				while(bi<i+1)
				{
					moveastep('s');
					bi++;
				}
				moveastep('d');
				bj--;
			}
			else if(bi==i)
			{
				moveastep('w');
				bi--;
				moveastep('a');
				bj++;
				moveastep('a');
				bj++;
				moveastep('s');
				bi++;
				moveastep('s');
				bi++;
				moveastep('d');
				bj--;
			}
		}
		else if(bj>j)
		{
			if(bi>i)
			{
				while(bi>i+1)
				{
					moveastep('w');
					bi--;
				}
				while(bj>j)
				{
					moveastep('d');
					bj--;
				}
			}
			else if(bi<=i)
			{
				while(bi<i+1)
				{
					moveastep('s');
					bi++;
				}
				while(bj>j)
				{
					moveastep('d');
					bj--;
				}
			}
		}
		else if(bj==j)
		{
			if(bi<i)
			{
				moveastep('a');
				bj++;
				while(bi<i+1)
				{
					moveastep('s');
					bi++;
				}
				moveastep('d');
				bj--;
			}
			else if(bi>i)
			{
				while(bi>i+1)
				{
					moveastep('w');
					bi--;
				}
			}
		}
	}
}

void blankmove_left(int i,int j)
{
	if(blank==getpos(i+1,j-1))
		return;
	int bi=gety(blank);
	int bj=getx(blank);
	if(i==0&&j==n)
	{
		if(bj<n)
		{
			while(bj<n-1)
			{
				moveastep('a');
				bj++;
			}
			if(bi>0)
			{
				while(bi>0)
				{
					moveastep('w');
					bi--;
				}
			}
		}
		else if(bj==n)
		{
			while(bi>1)
			{
				moveastep('w');
				bi--;
			}
			moveastep('d');
			bj--;
			moveastep('w');
			bi--;
		}
	}
	else if(i==0)
	{
		if(bj<j)
		{
			while(bi>0)
			{
				moveastep('w');
				bi--;
			}
			while(bj<j-1)
			{
				moveastep('a');
				bj++;
			}
		}
		else if(bj==j)
		{
			while(bi>1)
			{
				moveastep('w');
				bi--;
			}
			moveastep('d');
			bj--;
			moveastep('w');
			bi--;
		}
		else if(bj>j)
		{
			if(bi>0)
			{
				while(bi>1)
				{
					moveastep('w');
					bi--;
				}
				while(bj>j-1)
				{
					moveastep('d');
					bj--;
				}
				moveastep('w');
				bi--;
			}
			else if(bi==0)
			{
				moveastep('s');
				bi++;
				while(bj>j-1)
				{
					moveastep('d');
					bj--;
				}
				moveastep('w');
				bi--;
			}
		}
	}
	else if(j==n)
	{
		if(bj<n)
		{
			while(bj<n-1)
			{
				moveastep('a');
				bj++;
			}
			if(bi>i)
			{
				while(bi>i)
				{
					moveastep('w');
					bi--;
				}
			}
			else if(bi<i)
			{
				while(bi<i)
				{
					moveastep('s');
					bi++;
				}
			}
		}
		else if(bj==n)
		{
			if(bi>i)
			{
				while(bi>i+1)
				{
					moveastep('w');
					bi--;
				}
				moveastep('d');
				bj--;
				moveastep('w');
				bi--;
			}
			else if(bi<i)
			{
				moveastep('d');
				bj--;
				while(bi<i)
				{
					moveastep('s');
					bi++;
				}
			}
		}
	}
	else
	{
		if(bj<j)
		{
			while(bj<j-1)
			{
				moveastep('a');
				bj++;
			}
			if(bi>i)
			{
				while(bi>i)
				{
					moveastep('w');
					bi--;
				}
			}
			else if(bi<i)
			{
				while(bi<i)
				{
					moveastep('s');
					bi++;
				}
			}
		}
		else if(bj>j)
		{
			if(bi>=i)
			{
				while(bi>i-1)
				{
					moveastep('w');
					bi--;
				}
				while(bj>j-1)
				{
					moveastep('d');
					bj--;
				}
				moveastep('s');
				bi++;
			}
			else if(bi<i)
			{
				while(bj>j-1)
				{
					moveastep('d');
					bj--;
				}
				while(bi<i)
				{
					moveastep('s');
					bi++;
				}
			}
		}
		else if(bj==j)
		{
			if(bi>i)
			{
				while(bi>i+1)
				{
					moveastep('w');
					bi--;
				}
				moveastep('a');
				bj++;
				moveastep('w');
				bi--;
				moveastep('w');
				bi--;
				moveastep('d');
				bj--;
				moveastep('d');
				bj--;
				moveastep('s');
				bi++;
			}
			else if(bi<i)
			{
				moveastep('d');
				bj--;
				while(bi<i)
				{
					moveastep('s');
					bi++;
				}
			}
		}
	}
}

void blankmove_right(int i,int j)
{
	if(blank==getpos(i+1,j+1))
		return;
	int bi=gety(blank);
	int bj=getx(blank);
	if(i==0)
	{
		if(bj<j)
		{
			if(bi>0)
			{
				while(bi>1)
				{
					moveastep('w');
					bi--;
				}
				while(bj<j+1)
				{
					moveastep('a');
					bj++;
				}
				moveastep('w');
				bi--;
			}
			else
			{
				while(bj<j-1)
				{
					moveastep('a');
					bj++;
				}
				moveastep('s');
				bi++;
				moveastep('a');
				bj++;
				moveastep('a');
				bj++;
				moveastep('w');
				bi--;
			}
		}
		else if(bj>j)
		{
			while(bi>0)
			{
				moveastep('w');
				bi--;
			}
			while(bj>j+1)
			{
				moveastep('d');
				bj--;
			}
		}
		else if(bj==j)
		{
			while(bi>1)
			{
				moveastep('w');
				bi--;
			}
			moveastep('a');
			bj++;
			moveastep('w');
			bi--;
		}
	}
	else
	{
		if(bj<j)
		{
			if(bi>i)
			{
				while(bi>i+1)
				{
					moveastep('w');
					bi--;
				}
				while(bj<j+1)
				{
					moveastep('a');
					bj++;
				}
				moveastep('w');
				bi--;
			}
			else if(bi<i)
			{
				while(bi<i-1)
				{
					moveastep('s');
					bi++;
				}
				while(bj<j+1)
				{
					moveastep('a');
					bj++;
				}
				moveastep('s');
				bi++;
			}
			else if(bi==i)
			{
				moveastep('w');
				bi--;
				while(bj<j+1)
				{
					moveastep('a');
					bj++;
				}
				moveastep('s');
				bi++;
			}
		}
		else if(bj==j)
		{
			moveastep('a');
			bj++;
			if(bi>i)
			{
				while(bi>i)
				{
					moveastep('w');
					bi--;
				}
			}
			else if(bi<i)
			{
				while(bi<i)
				{
					moveastep('s');
					bi++;
				}
			}
		}
		else if(bj>j)
		{
			if(bi>i)
			{
				while(bi>i)
				{
					moveastep('w');
					bi--;
				}
			}
			else if(bi<i)
			{
				while(bi<i)
				{
					moveastep('s');
					bi++;
				}
			}
			while(bj>j+1)
			{
				moveastep('d');
				bj--;
			}
		}
	}
}

void moveastep(char ctr)
{
	counter++;
	switch (ctr)
	{
	case 's':
		std::cout<<"W";
		break;
	case 'a':
		std::cout<<"D";
		break;
	case 'd':
		std::cout<<"A";
		break;
	case 'w':
		std::cout<<"S";
		break;
	default:
		break;
	}
	switch (ctr)
	{
	case 's':
		if((blank-1)/n==m-1)
		{
			std::cout<<"非法移动，当前已在最顶部"<<std::endl;
			return;
		}
		swap(*(status_temp+blank),*(status_temp+blank+n));
		blank+=n;
		break;
	case 'a':

		if((blank-1)%n==n-1)
		{
			std::cout<<"非法移动，当前已在最右侧"<<std::endl;
			return;
		}
		swap(status_temp[blank],status_temp[blank+1]);
		blank++;
		break;
	case 'w':

		if((blank-1)/n==0)
		{ 
			std::cout<<"非法移动，当前已在最底部"<<std::endl;
			return;
		}
		swap(*(status_temp+blank),*(status_temp+blank-n));
		blank-=n;
		break;
	case 'd':

		if((blank-1)%n==0)
		{
			std::cout<<"非法移动，当前已在最左侧"<<std::endl;
			return;
		}
		swap(status_temp[blank],status_temp[blank-1]);
		blank--;
		break;
	default:
		break;
	}	
}

void linklist::buildboard()
{
	char file_name[]="board.txt";
	FILE *fp;
	char buffer[356];
	num=0;
	head=NULL;
	linknode *tmp=new linknode;
	tmp->time=0;
	head=tmp;
	linknode *tmp2=new linknode;
	tmp2->time=10000000000;
	tmp2->next=NULL;
	tmp2->pre=head;
	head->pre=NULL;
	head->next=tmp2;
	if(fp=fopen(file_name,"r"))
	{
		while(fgets(buffer,355,fp))
		{
			char filename[356];
			double t;
			sscanf(buffer,"%s %lf",filename,&t);
			add(filename,t);
		}
	}
}

void linklist::add(char *n,double t)
{
	linknode *temp;
	temp=head;
	linknode *tmp=new linknode;
	strcpy(tmp->name,n);
	tmp->next=NULL;
	tmp->pre=NULL;
	tmp->time=t;
	while(temp)
	{
		if(temp->time<=t)
			temp=temp->next;
		else
		{
			temp->pre->next=tmp;
			tmp->pre=temp->pre;
			tmp->next=temp;
			temp->pre=tmp;
			break;
		}
	}
	num++;
}

void linklist::showboard()
{
	if(num==0)
	{
		printf("排行榜为空\n");
		return;
	}
	linknode *temp=head->next;
	int counter=1;
	printf("编号 姓名     用时\n");
	while(temp&&counter<=10)
	{
		if(temp->next)
		{
			printf("%d:  %s  %lf秒\n",counter,temp->name,temp->time);
			counter++;
		}
		else
			break;
		temp=temp->next;
	}
	printf("\n");
}

void linklist::save()
{
	if(num>0)
	{
		char file_name[]="board.txt";
		FILE *fp;
		if(fp=fopen(file_name,"w"))
		{
			linknode *temp=head->next;
			int counter=1;
			while(temp&&counter<=10)
			{
				if(temp->next)
				{
					fprintf(fp,"%s %lf\n",temp->name,temp->time);
					counter++;
				}
				else
					break;
				temp=temp->next;
			}
			fclose(fp);
		}
	}
}

linklist::linklist()
{
}

int display(int k)
{
	int answer;
	answer=(m-1-((k-1)/n))*n+(k-1)%n;
	return answer;
}