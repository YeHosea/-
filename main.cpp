#include <iostream>
#include <vector>
#include <Windows.h>
#include <cstring>
#include <map>
#include <stack>
using namespace std;
struct MNode{
	struct SNode{		
		int flag;			//��־�Ƿ�Ϊѧ��
		string xuexiao;		//ѧУ
		string xueyuan;		//ѧԺ
		string zhuanye;		//רҵ
		string banji;		//�༶
		string xuehao;		//ѧ��
		string name;		//����
		string xingbie;		//�Ա�
		string ywchengji;		//���ĳɼ�
		string sxchengji;		//��ѧ�ɼ�
	}info;
	vector<MNode*>children;
	MNode *parent;
}node;
int posx,posy,zongy;
int id;//���ӵ�ѧ�����
int addflag=0;	//addflag=1 �����Ѵ��ڣ�������� addflag=2 ������ӳɹ�
int modifyflag=0;
int queryflag=0;
map<string,int>mapxuexiao;
map<string,int>mapxueyuan;
map<string,int>mapzhuanye;
map<string,int>mapbanji;
map<string,int>mapxuehao;
map<string,int>mapname;

void gotoxy(short x, short y)
{
	_COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
string MNodeinfo(MNode *node){
	if(node->info.name!="")			return node->info.name;
	else if(node->info.xuexiao!="")	return node->info.xuexiao;
	else if(node->info.xueyuan!="")	return node->info.xueyuan;
	else if(node->info.zhuanye!="")	return node->info.zhuanye;
	else if(node->info.banji!="")	return node->info.banji;
}
class MTree{
private:
	MNode *root;
public:
	void init(MNode *root);
	void insertSNode(MNode *node,int depth);
	void putChild_M(MNode *node,MNode *parent);
	void putChild_S(MNode *node,MNode *parent);
	void putChildren(vector<MNode*> nodes, MNode *parent);
	MNode *findMNode(MNode *root,string e,int flag,int depth);
	void queryName(MNode *node,MNode *e,int depth,int predepth);
	void deleteName(MNode *root,string e);
	void modifyName(MNode *root,string e,int f,string g);
}tree;
stack<MNode*>restack;
int vis[10];
void printbian(int posx){
	gotoxy(0+posx,1);
	for(int i=1;i<=31;i++)cout<<"*";
	for(int j=2;j<=zongy+1+2;j++){
		gotoxy(0+posx,j);cout<<"*";
		gotoxy(30+posx,j);cout<<"*";
	}
	gotoxy(0+posx,zongy+1+3);
	for(int i=1;i<=31;i++)cout<<"*";
}
void printtree(MNode *root,int ceng){
	for(int i=ceng;i<=9;i++)vis[i]=0;
	vector<MNode *>nodes=root->children;
	gotoxy(posx,posy);
	posy++;
	for(int i=1;i<=ceng-1;i++){
		if(i>=1&&vis[i-1]!=1){
			cout<<"��";
		}
		else cout<<" ";
		cout<<"   ";//3���ո�
	}
	if(ceng!=0)cout<<"����";//��������
	cout<<' '<<MNodeinfo(root)<<'\n';
	for(auto i:nodes){
		auto x=nodes.back();
		if(MNodeinfo(i)==MNodeinfo(x))vis[ceng]=1;
		printtree(i,ceng+1);
		vis[ceng]=0;
	}
}
void copyMNode(MNode *newroot,MNode *root){
	newroot->parent=root->parent;
	newroot->info=root->info;
	vector<MNode *>nodes=root->children;
	for(int i=0;i<nodes.size();i++){
		MNode *newMNode=new MNode;
		//newMNode->info=nodes[i]->info;
		//newMNode->parent=nodes[i]->parent;
		newroot->children.push_back(newMNode);
		copyMNode(newMNode,root->children[i]);
	}
}
//����ʼ��
void MTree::init(MNode *root) { this->root = root; }

//����һ���ӽڵ�
void MTree::putChild_M(MNode *node, MNode *parent) {
    parent->children.push_back(node);
    node->parent = parent;
}

//����һ��ѧ���ڵ�
void MTree::putChild_S(MNode *node, MNode *parent) {
    parent->children.push_back(node);
    node->parent = parent;
}

//����һ�����Ϊdepth�Ľڵ�
void MTree::insertSNode(MNode *node,int depth){
	MNode *s;
	addflag=0;//�жϴ˽ڵ��Ƿ��Ѿ�����
	if(depth>=1)s=findMNode(root,node->info.xuexiao,(depth==1)?1:0,1);
	if(depth>=2)s=findMNode(s,node->info.xueyuan,(depth==2)?1:0,2);
	if(depth>=3)s=findMNode(s,node->info.zhuanye,(depth==3)?1:0,3);
	if(depth>=4)s=findMNode(s,node->info.banji,(depth==4)?1:0,4);
	if(depth>=5)s=findMNode(s,node->info.name,(depth==5)?1:0,5);
	//else putChild_S(node->parent,s->parent);
}

//Ѱ��ѧ����Ӧ��Ϣ�������Ϣ�����ڣ����½�һ���ڵ�
MNode *MTree::findMNode(MNode *root,string e,int flag,int depth){
	vector<MNode *> nodes = root->children;
	for(int i=0;i<nodes.size();i++){
		if(MNodeinfo(nodes[i])==e){	//�ҵ��ѽ����Ľڵ�
			if(flag==1)addflag=1;	//����ýڵ��ڶ�Ӧ������ҵ���ͬ���ֵĽڵ㣬����Ϊ�ýڵ��Ѿ�����
			return nodes[i];		//���ظô��ڵĽڵ�
		}
	}
	//�ýڵ㲻���ڣ��½�һ���ڵ�
	MNode *a=new MNode;
	switch(depth){//�����½��ڵ����ȣ��ж��������ͣ���Ǹ����ƴ��ڣ�����Ӧ��ֵ
		case 1:a->info.xuexiao=e;	mapxuexiao[e]=1;	break;
		case 2:a->info.xueyuan=e;	mapxueyuan[e]=1;	break;
		case 3:a->info.zhuanye=e;	mapzhuanye[e]=1;	break;
		case 4:a->info.banji=e;		mapbanji[e]=1;		break;
		case 5:a->info.name=e;		mapname[e]=1;		break;
	}
	//���ýڵ���뵽���У������ظýڵ�
	putChild_M(a,root);
	return a;
}
void MTree::deleteName(MNode *root,string e){
	mapname[e]=0;
	vector<MNode *>nodes=root->children;
	if(nodes.size()==0){
		if(root->info.name==e){
			vector<MNode *>node=(root->parent)->children;
			for(auto i=node.begin();i!=node.end();){
				if((*i)->info.name==e){
					i=node.erase(i);
				}
				else{
					i++;
				}
			}
			(root->parent)->children=node;
			return;
		}
	}
	else{
		for(auto i:nodes){
			deleteName(i,e);
		}
	}
}
void MTree::modifyName(MNode *root,string e,int f,string g){
	if(modifyflag==1)return;
	vector<MNode *>nodes=root->children;
	MNode *newNode=new MNode;
	if(nodes.size()==0){
		if(root->info.name==e){
			newNode->info=root->info;
			switch(f){
				case 1:newNode->info.xuexiao=g;		break;
				case 2:newNode->info.xueyuan=g;		break;
				case 3:newNode->info.zhuanye=g;		break;
				case 4:newNode->info.banji=g;		break;
				case 5:newNode->info.xuehao=g;		break;
				case 6:newNode->info.xingbie=g;		break;
				case 7:newNode->info.ywchengji=g;	break;
				case 8:newNode->info.sxchengji=g;	break;
				case 9:{
					mapname[newNode->info.name]=0;
					newNode->info.name=g;			break;
				}
			}
		}
		tree.deleteName(root->parent,e);
		modifyflag=1;
		tree.insertSNode(newNode,5);
		return;
	}
	else{
		for(auto i:nodes){
			modifyName(i,e,f,g);
		}
	}
}

void MTree::queryName(MNode *root,MNode *e,int depth,int predepth){
	vector<MNode *>nodes=root->children;
	if(queryflag!=0)return;
	for(auto i:nodes){
		switch(predepth){
			case 1:{
				if(i->info.xuexiao==e->info.xuexiao){
					if(depth==predepth){queryflag=1;
						system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"ԴĿ¼"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
						posx=40,posy=2;gotoxy(posx+1,posy);cout<<"��ѯ�����"<<endl;posy++;printtree(i,0);printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
					}
					else queryName(i,e,depth,predepth+1);
				}
				break;
			}
			case 2:{
				if(i->info.xueyuan==e->info.xueyuan){
					if(depth==predepth){queryflag=1;
						system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"ԴĿ¼"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
						posx=40,posy=2;gotoxy(posx+1,posy);cout<<"��ѯ�����"<<endl;posy++;printtree(i,0);printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
					}
					else queryName(i,e,depth,predepth+1);
				}
				break;
			}
			case 3:{
				if(i->info.zhuanye==e->info.zhuanye){
					if(depth==predepth){queryflag=1;
						system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"ԴĿ¼"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
						posx=40,posy=2;gotoxy(posx+1,posy);cout<<"��ѯ�����"<<endl;posy++;printtree(i,0);printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
					}
					else queryName(i,e,depth,predepth+1);
				}
				break;
			}
			case 4:{
				if(i->info.banji==e->info.banji){
					if(depth==predepth){queryflag=1;
						system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"ԴĿ¼"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
						posx=40,posy=2;gotoxy(posx+1,posy);cout<<"��ѯ�����"<<endl;posy++;printtree(i,0);printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
					}
					else queryName(i,e,depth,predepth+1);
				}
				break;
			}
			case 5:{
				if(i->info.name==e->info.name){
					if(depth==predepth){queryflag=1;
						system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"ԴĿ¼"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
						posx=40,posy=2;gotoxy(posx+1,posy);cout<<"��ѯ�����"<<endl;
						posy++;printtree(i,0);
						posy++;gotoxy(posx+1,posy);cout<<"������"<<i->info.name<<endl;
						posy++;gotoxy(posx+1,posy);cout<<"ѧ�ţ�"<<i->info.xuehao<<endl;
						posy++;gotoxy(posx+1,posy);cout<<"�Ա�"<<i->info.xingbie<<endl;
						posy++;gotoxy(posx+1,posy);cout<<"���ĳɼ���"<<i->info.ywchengji<<endl;
						posy++;gotoxy(posx+1,posy);cout<<"��ѧ�ɼ���"<<i->info.sxchengji<<endl;
						printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
					}
					else return;
				}
				break;
			}
		}
	}
	if(queryflag==1||queryflag==-1)return;
	switch(predepth){
		case 1:{
			queryflag=-1;
			system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"���ǰ"<<endl;posy++;printtree(&node,0);zongy=max(zongy,posy);printbian(0);
			posx=40,posy=2;gotoxy(posx+1,posy);cout<<"��ѯʧ��"<<endl;
			posy++;gotoxy(posx+1,posy);cout<<"��ѯ��ѧУ�� ";
			posy++;gotoxy(posx+1,posy);cout<<e->info.xuexiao<<" ������";
			printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
			break;
		}
		case 2:{
			queryflag=-1;
			system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"���ǰ"<<endl;posy++;printtree(&node,0);zongy=max(zongy,posy);printbian(0);
			posx=40,posy=2;gotoxy(posx+1,posy);cout<<"��ѯʧ��"<<endl;
			posy++;gotoxy(posx+1,posy);cout<<"��ѯ��ѧԺ�� ";
			posy++;gotoxy(posx+1,posy);cout<<e->info.xuexiao<<' '<<e->info.xueyuan<<" ������";
			printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
			break;
		}
		case 3:{
			queryflag=-1;
			system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"���ǰ"<<endl;posy++;printtree(&node,0);zongy=max(zongy,posy);printbian(0);
			posx=40,posy=2;gotoxy(posx+1,posy);cout<<"��ѯʧ��"<<endl;
			posy++;gotoxy(posx+1,posy);cout<<"��ѯ��רҵ�� ";
			posy++;gotoxy(posx+1,posy);cout<<e->info.xuexiao<<' '<<e->info.xueyuan<<' '<<e->info.zhuanye<<" ������";
			printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
			break;
		}
		case 4:{
			queryflag=-1;
			system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"���ǰ"<<endl;posy++;printtree(&node,0);zongy=max(zongy,posy);printbian(0);
			posx=40,posy=2;gotoxy(posx+1,posy);cout<<"��ѯʧ��"<<endl;
			posy++;gotoxy(posx+1,posy);cout<<"��ѯ�İ༶�� ";
			posy++;gotoxy(posx+1,posy);cout<<e->info.xuexiao<<' '<<e->info.xueyuan;
			posy++;gotoxy(posx+1,posy);cout<<e->info.zhuanye<<' '<<e->info.banji<<" ������";
			printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
			break;
		}
		case 5:{
			queryflag=-1;
			system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"���ǰ"<<endl;posy++;printtree(&node,0);zongy=max(zongy,posy);printbian(0);
			posx=40,posy=2;gotoxy(posx+1,posy);cout<<"��ѯʧ��"<<endl;
			posy++;gotoxy(posx+1,posy);cout<<"��ѯ�������� ";
			posy++;gotoxy(posx+1,posy);cout<<e->info.xuexiao<<' '<<e->info.xueyuan;
			posy++;gotoxy(posx+1,posy);cout<<e->info.zhuanye<<' '<<e->info.banji<<' '<<e->info.xuehao<<" ������";
			printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
			break;
		}
	}
}
int main(){
	id=7;
	MNode lastnode;
	node.parent=nullptr;
	node.info.name=" ";
	tree.init(&node);

	MNode stu[10];
	stu[1].info.flag=1;
	stu[1].info.xuexiao="�㽭��ѧ";			 stu[1].info.xueyuan="�����";
	stu[1].info.zhuanye="�ƿ�";				stu[1].info.banji="1��";
	stu[1].info.xuehao="19015001";			stu[1].info.xingbie="��";
	stu[1].info.ywchengji="100";			stu[1].info.sxchengji="99";
	stu[1].info.name="������";				tree.insertSNode(&stu[1],5);

	stu[2].info.flag=1;
	stu[2].info.xuexiao="�㽭��ѧ";		stu[2].info.xueyuan="���簲ȫ";
	stu[2].info.zhuanye="����";			stu[2].info.banji="2��";
	stu[2].info.xuehao="19017002";		stu[2].info.xingbie="Ů";
	stu[2].info.ywchengji="30";			stu[2].info.sxchengji="79";
	stu[2].info.name="��ɣ";			tree.insertSNode(&stu[2],5);
	
	stu[3].info.flag=1;
	stu[3].info.xuexiao="�㽭��ѧ";		stu[3].info.xueyuan="���簲ȫ";
	stu[3].info.zhuanye="�Ű�";			stu[3].info.banji="3��";
	stu[3].info.xuehao="19015003";		stu[3].info.xingbie="Ů";
	stu[3].info.ywchengji="90";			stu[3].info.sxchengji="87";
	stu[3].info.name="����";			tree.insertSNode(&stu[3],5);

	stu[4].info.flag=1;
	stu[4].info.xuexiao="����";			stu[4].info.xueyuan="�˹�����";
	stu[4].info.zhuanye="������";		stu[4].info.banji="1��";
	stu[4].info.xuehao="19021001";		stu[4].info.xingbie="Ů";
	stu[4].info.ywchengji="95";			stu[4].info.sxchengji="67";
	stu[4].info.name="С��";			tree.insertSNode(&stu[4],5);

	stu[5].info.flag=1;
	stu[5].info.xuexiao="����";			stu[5].info.xueyuan="�˹�����";
	stu[5].info.zhuanye="���ܳ�";		stu[5].info.banji="1��";
	stu[5].info.xuehao="19021002";		stu[5].info.xingbie="��";
	stu[5].info.ywchengji="91";			stu[5].info.sxchengji="79";
	stu[5].info.name="С��";			tree.insertSNode(&stu[5],5);

	stu[6].info.flag=1;
	stu[6].info.xuexiao="�㽭��ѧ";			 stu[6].info.xueyuan="�����";
	stu[6].info.zhuanye="�ƿ�";				stu[6].info.banji="1��";
	stu[6].info.xuehao="19017002";			stu[6].info.xingbie="��";
	stu[6].info.ywchengji="100";			stu[6].info.sxchengji="100";
	stu[6].info.name="СҶ";				tree.insertSNode(&stu[6],5);

	stu[7].info.flag=1;
	stu[7].info.xuexiao="�㽭��ѧ";			 stu[7].info.xueyuan="�����";
	stu[7].info.zhuanye="�ƿ�";				stu[7].info.banji="1��";
	stu[7].info.xuehao="19017003";			stu[7].info.xingbie="��";
	stu[7].info.ywchengji="80";				stu[7].info.sxchengji="70";
	stu[7].info.name="СƤ";				tree.insertSNode(&stu[7],5);

	stu[8].info.flag=1;
	stu[8].info.xuexiao="�廪��ѧ";			 stu[8].info.xueyuan="�����";
	stu[8].info.zhuanye="�ƿ�";				stu[8].info.banji="1��";
	stu[8].info.xuehao="19016001";			stu[8].info.xingbie="��";
	stu[8].info.ywchengji="91";				stu[8].info.sxchengji="90";
	stu[8].info.name="С��";				
	//tree.queryName(&node,"wang");
	//tree.queryName(&node,"li");
	system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"Ŀ¼"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
	while(1){
		gotoxy(1,zongy+7);cout<<" �����������ʽ��1.��� 2.ɾ�� 3.�޸� 4.��ѯ 5.���µ�ǰĿ¼ 6.���� 7.�������� : ";
		int x;
		while(cin>>x&&(x!=1&&x!=2&&x!=3&&x!=4&&x!=5&&x!=6&&x!=7)){
			cout<<" ���벻���Ϲ�������������: ";
		}
		switch(x){
			case 1:{
				addshang:
				cout<<" ��������ӵ���Ϣ��1.ѧУ 2.ѧԺ 3.רҵ 4.�༶ 5.�Զ�����һ����ѧ�� 6.�ֶ�����һ����ѧ��:";
				int addInfo;
				while(cin>>addInfo&&(addInfo!=1&&addInfo!=2&&addInfo!=3&&addInfo!=4&&addInfo!=5&&addInfo!=6&&addInfo!=7)){
					cout<<"���벻���Ϲ�������������: ";
				}
				MNode *newnode=new MNode;
				addflag=0;
				switch(addInfo){//������ӵ���Ϣ���಻ͬ������
					case 1:{
						cout<<" ��������Ҫ��ӵ�ѧУ(������һ����������re��:";
						cin>>newnode->info.xuexiao;
						if(newnode->info.xuexiao=="re")goto addshang;
						break;
					}
					case 2:{
						cout<<" ��������Ҫ��ӵ�ѧԺ������ѧУ(������һ����������re��:";cin>>newnode->info.xuexiao;if(newnode->info.xuexiao=="re")goto addshang;
						cout<<" ��������Ҫ��ӵ�ѧԺ(������һ����������re����";cin>>newnode->info.xueyuan;if(newnode->info.xueyuan=="re")goto addshang;
						break;
					}
					case 3:{
						cout<<" ��������Ҫ��ӵ�רҵ������ѧУ(������һ����������re��:";cin>>newnode->info.xuexiao;if(newnode->info.xuexiao=="re")goto addshang;
						cout<<" ��������Ҫ��ӵ�רҵ������ѧԺ(������һ����������re��:";cin>>newnode->info.xueyuan;if(newnode->info.xueyuan=="re")goto addshang;
						cout<<" ��������Ҫ��ӵ�רҵ(������һ����������re����";cin>>newnode->info.zhuanye;if(newnode->info.zhuanye=="re")goto addshang;
						break;
					}
					case 4:{
						cout<<" ��������Ҫ��Ӱ༶������ѧУ(������һ����������re��:";cin>>newnode->info.xuexiao;if(newnode->info.xuexiao=="re")goto addshang;
						cout<<" ��������Ҫ��Ӱ༶������ѧԺ(������һ����������re��:";cin>>newnode->info.xueyuan;if(newnode->info.xueyuan=="re")goto addshang;
						cout<<" ��������Ҫ��Ӱ༶������רҵ(������һ����������re��:";cin>>newnode->info.zhuanye;if(newnode->info.zhuanye=="re")goto addshang;
						cout<<" ��������Ҫ��Ӱ༶(������һ����������re����";cin>>newnode->info.banji;if(newnode->info.banji=="re")goto addshang;
						break;
					}
					case 5:{
						newnode->info=stu[++id].info;
						break;
					}
					case 6:{
						newnode->info.flag=1;
						cout<<" ��������Ҫ���ѧ��������ѧУ(������һ����������re��:";cin>>newnode->info.xuexiao;if(newnode->info.xuexiao=="re")goto addshang;
						cout<<" ��������Ҫ���ѧ��������ѧԺ(������һ����������re��:";cin>>newnode->info.xueyuan;if(newnode->info.xueyuan=="re")goto addshang;
						cout<<" ��������Ҫ���ѧ��������רҵ(������һ����������re��:";cin>>newnode->info.zhuanye;if(newnode->info.zhuanye=="re")goto addshang;
						cout<<" ��������Ҫ���ѧ�������ڰ༶(������һ����������re��:";cin>>newnode->info.banji;if(newnode->info.banji=="re")goto addshang;
						cout<<" ��������Ҫ���ѧ��(������һ����������re����";cin>>newnode->info.name;if(newnode->info.banji=="re")goto addshang;
					}
				};
				if(addflag==1){//�ýڵ��Ѿ�����
					system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"���ǰ"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
					posx=40,posy=2;gotoxy(posx+1,posy);cout<<"�ýڵ��Ѿ����ڣ��޷����"<<endl;posy++;printbian(39);zongy=posy;gotoxy(33,zongy/2);cout<<"--->";
					break;
				}

				//��������
				MNode *reMNode=new MNode;
				node.parent=nullptr;
				copyMNode(reMNode,&node);
				restack.push(reMNode);

				system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"���ǰ"<<endl;posy++;printtree(&node,0);zongy=max(zongy,posy);printbian(0);
				tree.insertSNode(newnode,addInfo);
				posx=40,posy=2;gotoxy(posx+1,posy);cout<<"��Ӻ�"<<endl;posy++;printtree(&node,0);zongy=max(zongy,posy);printbian(39);gotoxy(33,zongy/2);cout<<"--->";
				break;
			}
			case 2:{
				cout<<" ������ɾ��ѧ�������� ";//��������
				string deleteName;
				while(cin>>deleteName&&mapname[deleteName]==0){
					cout<<endl<<" ��ѧ�������ڣ�����������: ";
				}
				//��������
				MNode *reMNode=new MNode;
				node.parent=nullptr;
				copyMNode(reMNode,&node);
				restack.push(reMNode);

				system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"�޸�ǰ"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
				tree.deleteName(&node,deleteName);
				posx=40,posy=2;gotoxy(posx+1,posy);cout<<"�޸ĺ�"<<endl;posy++;printbian(39);printtree(&node,0);zongy=posy;gotoxy(33,zongy/2);cout<<"--->";
				break;
			}	
			case 3:{
				cout<<" �������޸���Ϣ��ѧ�������� ";
				string modifyName;
				while(cin>>modifyName){
					if(mapname[modifyName]==0){
						cout<<endl;
						cout<<" ��ѧ�������ڣ�����������: ";
					}
					else break;
				}
				cout<<" �������޸ĵ���Ϣ��1.ѧУ 2.ѧԺ 3.רҵ 4.�༶ 5.ѧ�� 6.�Ա� 7.���ĳɼ� 8.��ѧ�ɼ� 9.������ ";
				int modifyInfo;
				while(cin>>modifyInfo&&(modifyInfo!=1&&modifyInfo!=2&&modifyInfo!=3&&modifyInfo!=4&&modifyInfo!=5&&modifyInfo!=6&&modifyInfo!=7&&modifyInfo!=8&&modifyInfo!=9)){
					cout<<"���벻���Ϲ�������������: ";
				}
				cout<<endl<<" �������޸�����Ϊ�� ";
				string modifyContent;
				cin>>modifyContent;
				modifyflag=0;
				//��������
				MNode *reMNode=new MNode;
				node.parent=nullptr;
				copyMNode(reMNode,&node);
				restack.push(reMNode);

				system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"�޸�ǰ"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
				tree.modifyName(&node,modifyName,modifyInfo,modifyContent);
				posx=40,posy=2;gotoxy(posx+1,posy);cout<<"�޸ĺ�"<<endl;posy++;printtree(&node,0);printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
				break;
			}
			case 4:{
				cout<<" ��������Ҫ��ѯ����Ϣ��1.ѧУ 2.ѧԺ 3.רҵ 4.�༶ 5.������ ";
				int queryInfo;
				while(cin>>queryInfo&&(queryInfo!=1&&queryInfo!=2&&queryInfo!=3&&queryInfo!=4&&queryInfo!=5&&queryInfo!=6)){
					cout<<" ���벻���Ϲ�������������: ";
				}
				MNode *newnode=new MNode;//�½�һ���ڵ㣬�����в�ѯ����Ϣ��ֵ������Ȼ����в�ѯ
				queryflag=0;
				switch(queryInfo){//������ӵ���Ϣ���಻ͬ������
					case 1:{
						cout<<" ��������Ҫ��ѯ��ѧУ��";
						cin>>newnode->info.xuexiao;
						tree.queryName(&node,newnode,1,1);
						break;
					}
					case 2:{
						cout<<" ��������Ҫ��ѯ��ѧԺ������ѧУ��";
						cin>>newnode->info.xuexiao;
						cout<<" ��������Ҫ��ѯ��ѧԺ��";
						cin>>newnode->info.xueyuan;
						tree.queryName(&node,newnode,2,1);
						break;
					}
					case 3:{
						cout<<" ��������Ҫ��ѯ��רҵ������ѧУ��";
						cin>>newnode->info.xuexiao;
						cout<<" ��������Ҫ��ѯ��רҵ������ѧԺ��";
						cin>>newnode->info.xueyuan;
						cout<<" ��������Ҫ��ѯ��רҵ��";
						cin>>newnode->info.zhuanye;
						tree.queryName(&node,newnode,3,1);
						break;
					}
					case 4:{
						cout<<" ��������Ҫ��ѯ�İ༶������ѧУ��";
						cin>>newnode->info.xuexiao;
						cout<<" ��������Ҫ��ѯ�İ༶������ѧԺ��";
						cin>>newnode->info.xueyuan;
						cout<<" ��������Ҫ��ѯ�İ༶������רҵ��";
						cin>>newnode->info.zhuanye;
						cout<<" ��������Ҫ��ѯ�İ༶��";
						cin>>newnode->info.banji;
						tree.queryName(&node,newnode,4,1);
						break;
					}
					case 5:{
						cout<<" ��������Ҫ��ѯ������������ѧУ��";
						cin>>newnode->info.xuexiao;
						cout<<" ��������Ҫ��ѯ������������ѧԺ��";
						cin>>newnode->info.xueyuan;
						cout<<" ��������Ҫ��ѯ������������רҵ��";
						cin>>newnode->info.zhuanye;
						cout<<" ��������Ҫ��ѯ�İ༶��";
						cin>>newnode->info.banji;
						cout<<" ��������Ҫ��ѯ��������";
						cin>>newnode->info.name;
						tree.queryName(&node,newnode,5,1);
						break;
					}
				};
				delete(newnode);//��ֹ�ڴ�й©
				break;
			}
			case 5:{
				system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"����Ŀ¼"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
				break;
			}
			case 6:{
				if(restack.size()==0){
					system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"����ǰ"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
					posx=40,posy=2;gotoxy(posx+1,posy);cout<<"�޲������Գ���"<<endl;posy++;printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
					break;
				}
				MNode *cheMNode=restack.top();
				restack.pop();
				system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"����ǰ"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
				node.parent=nullptr;node.children.clear();
				copyMNode(&node,cheMNode);
				posx=40,posy=2;gotoxy(posx+1,posy);cout<<"������"<<endl;posy++;
				printtree(&node,0);
				printbian(39);zongy=max(zongy,posy);gotoxy(33,zongy/2);cout<<"--->";
				break;
			}
			case 7:{
				system("cls");posx=2,posy=2;gotoxy(posx,posy);cout<<"���Ŀ¼"<<endl;posy++;printtree(&node,0);zongy=posy;printbian(0);
				cout<<endl<<"��������"<<endl;
				goto ends;
			}
		}
	}
	ends:
	system("pause");
}