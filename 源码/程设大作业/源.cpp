#include "interaction.h"
#include <chrono>
using namespace std;
using namespace chrono;


int *a,blank,*originstatus;
int n,m;


void show()                               //输出当前状态矩阵
{
	int j=(m-1)*n;
	int i=1;
	while(i<=n&&j>=0)
	{
		if((j+i)!=blank)
		{
			cout<<display(*(current+i+j))<<" ";
			if(display(*(current+i+j))<10)
				cout<<" ";
		}
		else
			cout<<"-1 ";
		if(i==n)
		{
			cout<<endl;
			i=1;
			j-=n;
		}
		else
			i++;
	}
}

int main()
{
	char *filename;
	filename=new char[200];
	cout<<"请输入读入24位BMP图片的完整路径"<<endl;       //读入路径
	cin>>filename;
	bool flag=readBMP(filename);                         //读图
	while(!flag)                                         //错误陷阱，如果读入失败或文件格式有问题，重新读入
	{
		cin>>filename;
		flag=readBMP(filename);
	}
	cout<<"请设置游戏难度，输入m和n，以空格隔开，分别代表行数和列数"<<endl;
	cin>>n>>m;                      //m行n列!
	divide_pic(n,m,"new.bmp");                           //分割图形
	originstatus=new int[m*n+1];                         //当前状态
	status_temp=new int[m*n+1];
	randomize(m,n);                                      //随机打乱
	blank=n;
	cout<<endl;
	show();                                              //显示当前状态矩阵

	if(sovablility())                                    //判断可解性
	{
		cout<<"当前状态是可解的！请开始游戏！"<<endl;
	}
	else
	{
		cout<<"当前状态是不可解的"<<endl<<"是否需要重新打乱？需要请输入Y回车，否则请输入N回车"<<endl;
		char choose;
		cin>>choose;
		if(choose=='Y')
		{
			while(!sovablility())                      //不可解则再次打乱直到可解为止
			{
				randomize(m,n);
			}
			cout<<endl;
			show();
			cout<<"当前状态是可解的了！请开始游戏！"<<endl;
		}
		else
			cout<<endl<<"请开始游戏！"<<endl;
	}
	leaderboard.buildboard();                         //读入排行榜
	auto start = system_clock::now();                 //计时开始
	char ctr;
	for(int i=1;i<=m*n;i++)
		originstatus[i]=current[i];//当前状态
	cout<<endl;
	while(cin>>ctr)                                   //开始接受输入
	{
		interact(ctr);                                //交互函数，对用户输入做出反应
		if(ctr=='P')    //退出
			return 0;
		if(ctr!='F'&&ctr!='B')
			show();
		if((ctr=='W'||ctr=='A'||ctr=='S'||ctr=='D')&&judge(m,n))        //判断是否已复原
		{
			auto end   = system_clock::now();
			auto duration = duration_cast<microseconds>(end - start);
			cout<<"You have successfully restored the picture! Congratulation!"<<endl;
			double usertime=double(duration.count()) * microseconds::period::num / microseconds::period::den;
			cout<<"您用时"<<usertime<<endl;                               //输出用时
			cout<<"请输入您的姓名（限英文字符，以回车结束）"<<endl;       //加入排行榜
			char username[256];
			cin>>username;
			leaderboard.add(username,usertime);
			leaderboard.save();
			cout<<endl;
		}

	}
	return 0;
}