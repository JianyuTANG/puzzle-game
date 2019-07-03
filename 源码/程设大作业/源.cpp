#include "interaction.h"
#include <chrono>
using namespace std;
using namespace chrono;


int *a,blank,*originstatus;
int n,m;


void show()                               //�����ǰ״̬����
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
	cout<<"���������24λBMPͼƬ������·��"<<endl;       //����·��
	cin>>filename;
	bool flag=readBMP(filename);                         //��ͼ
	while(!flag)                                         //�������壬�������ʧ�ܻ��ļ���ʽ�����⣬���¶���
	{
		cin>>filename;
		flag=readBMP(filename);
	}
	cout<<"��������Ϸ�Ѷȣ�����m��n���Կո�������ֱ��������������"<<endl;
	cin>>n>>m;                      //m��n��!
	divide_pic(n,m,"new.bmp");                           //�ָ�ͼ��
	originstatus=new int[m*n+1];                         //��ǰ״̬
	status_temp=new int[m*n+1];
	randomize(m,n);                                      //�������
	blank=n;
	cout<<endl;
	show();                                              //��ʾ��ǰ״̬����

	if(sovablility())                                    //�жϿɽ���
	{
		cout<<"��ǰ״̬�ǿɽ�ģ��뿪ʼ��Ϸ��"<<endl;
	}
	else
	{
		cout<<"��ǰ״̬�ǲ��ɽ��"<<endl<<"�Ƿ���Ҫ���´��ң���Ҫ������Y�س�������������N�س�"<<endl;
		char choose;
		cin>>choose;
		if(choose=='Y')
		{
			while(!sovablility())                      //���ɽ����ٴδ���ֱ���ɽ�Ϊֹ
			{
				randomize(m,n);
			}
			cout<<endl;
			show();
			cout<<"��ǰ״̬�ǿɽ���ˣ��뿪ʼ��Ϸ��"<<endl;
		}
		else
			cout<<endl<<"�뿪ʼ��Ϸ��"<<endl;
	}
	leaderboard.buildboard();                         //�������а�
	auto start = system_clock::now();                 //��ʱ��ʼ
	char ctr;
	for(int i=1;i<=m*n;i++)
		originstatus[i]=current[i];//��ǰ״̬
	cout<<endl;
	while(cin>>ctr)                                   //��ʼ��������
	{
		interact(ctr);                                //�������������û�����������Ӧ
		if(ctr=='P')    //�˳�
			return 0;
		if(ctr!='F'&&ctr!='B')
			show();
		if((ctr=='W'||ctr=='A'||ctr=='S'||ctr=='D')&&judge(m,n))        //�ж��Ƿ��Ѹ�ԭ
		{
			auto end   = system_clock::now();
			auto duration = duration_cast<microseconds>(end - start);
			cout<<"You have successfully restored the picture! Congratulation!"<<endl;
			double usertime=double(duration.count()) * microseconds::period::num / microseconds::period::den;
			cout<<"����ʱ"<<usertime<<endl;                               //�����ʱ
			cout<<"������������������Ӣ���ַ����Իس�������"<<endl;       //�������а�
			char username[256];
			cin>>username;
			leaderboard.add(username,usertime);
			leaderboard.save();
			cout<<endl;
		}

	}
	return 0;
}