#include<bits/stdc++.h>
#include<string>
using namespace std;
fstream ifile,ofile;
const int num_ids = 20; //5;
const int num_trans=20; //9;
const int min_sup=5; //2;

#define dbg(x) cout<<#x<<"="<<x<<endl;

struct fptree
{
	int item;
	int count;
	fptree *child[num_ids];    // children -> max can be num_ids
  	fptree *pptr;		// parent
  	fptree *next;      //  next in the header list
  	int nc;      // number of children
	fptree()
	{
		item=-1;
		count=0;
		for(int i=0;i<num_ids;i++)
			child[i]=NULL;
		pptr=NULL;
		next=NULL;
		nc=0;
	}
};

typedef fptree* ftptr;

struct ids
{
	int id;
	int count;
	ids()
	{
		count=0;
	}	
};


ftptr creat(int val);
string to_string(int a);
int to_int(string s);
bool cmpr(ids a, ids b);
void get_data(ids trans_for_ids[]);
set<int> get_next_trans();
vector<int> asc_order(set<int>a, ids trans_for_ids[]);
void add_branch(ftptr root, vector<int>&arr, int ind,fptree header_list[]);
void create_tree(ftptr root,fptree *header_list, int l1_count, ids trans_for_ids[]);
void init_header_list(fptree header_list[],int &l1_count, ids trans_for_ids[]);
void add_to_header_list(ftptr node,fptree header_list[]);
void print_headers(int l1_count, fptree *header_list);
void create_pattern_base(fptree header_list[],int l1_count,vector<int>item);
void fp_mine(vector<vector<int> >&patt_base, vector<int> item);
void subset(vector<int>&items, vector<int>s, vector<vector<int> >&subs,int i);
void fp_create(ftptr &treeb, vector<vector<int> >patt_base,fptree header_list[],int local_count);


ftptr creat(int val)
{
	ftptr fp = new fptree();
	fp->item = val;
	return fp;	
}


string to_string(int a)
{
	string s="";
	while(a)
	{
		s+=char(a%10+'0');
		a=a/10;
	}
	string t="";
	for(int i=s.length()-1;i>=0;i--)
	{
		t+=s[i];
	}
	return t;
}

int to_int(string s)
{
	int a=0;
	for(int i=0;i<s.size();i++)
	{
		a = a*10 + int(s[i]-'0');
	}
	return a;
}



bool cmpr(ids a, ids b)
{
	return a.count>b.count;
}

void get_data(ids trans_for_ids[])
{
	for(int i=1;i<=num_ids;i++)
	{
		trans_for_ids[i].id = i ;
		trans_for_ids[i].count=0;
	}
	string s;
	ifile>>s; ifile>>s; ifile>>s; ifile>>s; ifile>>s;
	int tno;
	int ino;
	while(!ifile.eof())
	{
		ifile>>s;
		if(s[0]=='T')
		{
			tno = to_int(s.substr(3,s.length()-3));
		}
		else if(s[0]=='I')
		{
			trans_for_ids[to_int(s.substr(1,s.length()-1))].count++;
		}
		if(ifile.eof()) break;
	}
	sort(trans_for_ids+1,trans_for_ids+num_ids+1,cmpr);	
	
	trans_for_ids[2].id = 1;
	trans_for_ids[3].id = 3;

	for(int i=1;i<=num_ids;i++)
	{
		ofile<<trans_for_ids[i].id<<"\t\t"; 
		ofile<<trans_for_ids[i].count<<endl; 
	}
}

set<int> get_next_trans()
{
	set<int>arr;
	arr.clear();
	string s;
	ifile>>s;
	while(!ifile.eof())
	{
		if(s[0]=='T')
		{
			break;
		}
		else if(s[0]=='I')
		{
		arr.insert(to_int(s.substr(1,s.length()-1)));
		}
		if(ifile.eof()) break;
		ifile>>s;
	}
	return arr;
}

vector<int> asc_order(set<int>a, ids trans_for_ids[])
{
	vector<int>arr;
	arr.clear();
	for(int i=1;i<=num_ids&&trans_for_ids[i].count>=min_sup;i++)
	{
		if(a.find(trans_for_ids[i].id)!=a.end())
		{
			arr.push_back(trans_for_ids[i].id);
		}
	}
	return arr;
}


void add_to_header_list(ftptr node,fptree header_list[],int l1_count)
{
	for(int i=0;i<l1_count;i++)
	{
		if(node->item==header_list[i].item)
		{
			ftptr p=&header_list[i];
			while(p->next!=NULL)
			{
				p=p->next;
			}
			p->next = node;
			return;			
		}
	}
	cout<<"\n ------- ERROR !!!! ------- "<<endl;
}



void add_branch(ftptr root, vector<int>&arr, int ind, int item_count,fptree header_list[],int l1_count)
{
	if(ind>=arr.size()) return;
	
	if(root->nc==0)
	{
		root->child[root->nc] = new fptree();
		root->child[root->nc]->item = arr[ind];
		root->child[root->nc]->count = item_count;
		root->child[root->nc]->pptr = root;
		add_to_header_list(root->child[root->nc],header_list,l1_count);
		root->nc++;
		ind++;
		add_branch(root->child[root->nc-1],arr,ind,item_count,header_list,l1_count);
		return;
	}
	else
	{
		for(int i=0;i<root->nc;i++)
		{
			if(root->child[i]->item==arr[ind])
			{
					root->child[i]->count+=item_count;
					ind++;
					add_branch(root->child[i],arr,ind,item_count,header_list,l1_count);
					return;	
			}
		}
		root->child[root->nc] = new fptree();
		root->child[root->nc]->item = arr[ind];
		root->child[root->nc]->count = item_count;
		root->child[root->nc]->pptr = root;
		add_to_header_list(root->child[root->nc],header_list,l1_count);
		root->nc++;
		ind++;
		add_branch(root->child[root->nc-1],arr,ind,item_count,header_list,l1_count);
		return;
	}
}


void print_tree(ftptr root)
{
	cout<<root->item<<" "<<root->count<<endl;
	for(int i=0;i<root->nc;i++)
	{
		print_tree(root->child[i]);
	}
}

void trim_tree(ftptr root,fptree header_list[], int &l1_count)
{	
	int x = root->nc;
	for(int i=0;i<x;i++)
	{
		if(root->child[i]->count<min_sup)
		{
			root->nc--;
//			for(int k=0;k<l1_count;k++)
//			{
//				if(root->child[i]->item==header_list[k].item)
//				{
//					header_list[k].item=-90;
//					l1_count--;
//					break;
//				}	
//			}
			root->child[i]=NULL;
		}
		else
			trim_tree(root->child[i],header_list,l1_count);
	}
}



void print_headers(int l1_count, fptree *header_list)
{
	for(int i=0;i<l1_count;i++)
	{
		cout<<header_list[i].item<<" -> ";
		ftptr p = header_list[i].next;
		while(p)
		{
			cout<<p->item<<" "<<p->count;
			p=p->next;
			if(p) cout<<" , ";
		}
		cout<<endl;
	}
}


void create_tree(ftptr root,fptree *header_list, int l1_count, ids trans_for_ids[])
{
	while(!ifile.eof())
	{
		set<int>a = get_next_trans();
 		vector<int>arr = asc_order(a,trans_for_ids);
// 		cout<<"\n next transaction is : "<<endl;
// 		for(int i=0;i<arr.size();i++)
// 			cout<<arr[i]<<" ";
// 		cout<<endl;
		// add this transaction to tree
		add_branch(root,arr,0,1,header_list,l1_count);
//		cout<<"\n After adding branch Tree is : "<<endl;
//		print_tree(::root);
	}
}



void init_header_list(fptree header_list[],int &l1_count, ids trans_for_ids[])
{
	int j=0;
	//initialise list
	for(int i=1;i<=num_ids&&trans_for_ids[i].count>=min_sup;i++,j++)
	{
		header_list[j].item =  trans_for_ids[i].id;
		header_list[j].next = NULL;
		header_list[j].count = trans_for_ids[i].count;
	}
	l1_count = j;
}


int main()
{
	ftptr root;
	fptree header_list[num_ids];
	int l1_count=0;
	fptree t1,t2;
	ids trans_for_ids[num_ids+1];
	
	ifile.open("AllElectronics.txt",ios::in);
	ofile.open("CountFile.txt",ios::out);
	ofile<<"\tID"<<"\t\t"<<"Transactions"<<endl;
	get_data(trans_for_ids);
	ifile.close();
	ofile.close();
	
	root =  creat(-1);
	init_header_list(header_list,l1_count,trans_for_ids);
	ifile.open("AllElectronics.txt",ios::in);
	ofile.open("ResultFile.txt",ios::out);
	string s;
	ifile>>s; ifile>>s; ifile>>s; ifile>>s; ifile>>s;
	ifile>>s;    // read "T001"
	create_tree(root,header_list,l1_count,trans_for_ids);
	ifile.close();
//	print_tree(root);
//	cout<<"==============================================================="<<endl;
//	print_headers(l1_count, header_list);
//	cout<<"==============================================================="<<endl;
	vector<int>item;
	create_pattern_base(header_list,l1_count,item);
	return 0;
}


void add_to_patt_base(vector<int>&pb, ftptr node)
{
	node=node->pptr;
	while(node && node->item!=-1)
	{
		pb.push_back(node->item);
		node = node->pptr;
	}
}


void create_pattern_base(fptree header_list[],int l1_count,vector<int>item)
{
	vector<vector<int> >patt_base;	
	for(int i=l1_count-1;i>=0;i--)
	{
		patt_base.clear();
		vector<int>pb;
//		dbg(header_list[i].item);
		ftptr tmp = header_list[i].next;
		while(tmp)
		{
			pb.clear();
			pb.push_back(tmp->count);	// count for current element in header linked list
			add_to_patt_base(pb,tmp);	// add the branch above this node to pattern base
			tmp=tmp->next;	
//			for(int j=0;j<pb.size();j++)
//			{
//				cout<<pb[j]<<" ";
//			}		
//			cout<<endl;		
			vector<int>pb2;
			for(int k=pb.size()-1;k>=0;k--)
				pb2.push_back(pb[k]);
			patt_base.push_back(pb2);	// pb2 has the path from root to that node/element (in header linked list) followed by last element as count
			// patt_base consists of pattern base corresponding to current item in header list
		}
		item.push_back(header_list[i].item);	// item contains the list of elements/items to be appended to final set generated in the end.
//		dbg("calling fp_mine");
//		for(int t=0;t<patt_base.size();t++)
//		{
//			for(int q=0;q<patt_base[t].size();q++)
//				cout<<patt_base[t][q]<<" ";
//			cout<<endl;
//		}
		fp_mine(patt_base,item);	// mine the pattern base for corresponding itemset -> item and pattern base -> patt_base
		// remove this item now...
		item.pop_back();
	}
	if(item.size()!=0)  // case where single item would have left out, so take care of it separately
	{
		vector<vector<int> >subs;
		vector<int>s;
		subset(item,s,subs,0);
		cout<<"==============================================================="<<endl;
		for(int w=0;w<item.size();w++)
		{
			cout<<item[w]<<" ";
			ofile<<item[w]<<" ";
		}
//		cout<<" -> ";
		cout<<endl;
		ofile<<endl;
		cout<<"==============================================================="<<endl;			
	}
}


void fp_mine(vector<vector<int> >&patt_base, vector<int> item)
{
//	label:
	if(patt_base.size()==1)
	{
		patt_base[0].erase(patt_base[0].end()-1);
//		dbg("size is 1 here" );
//		cout<<" case where tree has only one branch, simply generate all combinations prefixed/suffixed with the item vector "<<endl;
		vector<vector<int> >subs;
		vector<int>s;
		subset(patt_base[0],s,subs,0);
		cout<<"==============================================================="<<endl;
		for(int i=0;i<subs.size();i++)
		{
			for(int k=0;k<item.size();k++)
			{
				cout<<item[k]<<" ";
				ofile<<item[k]<<" ";
			}
//			cout<<" -> ";
			for(int j=0;j<subs[i].size();j++)
			{
				cout<<subs[i][j]<<" ";
				ofile<<subs[i][j]<<" ";
			}
			cout<<endl;
			ofile<<endl;
		}
//		exit(0);
		cout<<"==============================================================="<<endl;			
	}
	else
	{
//		dbg("will create tree");
		ftptr treeb = creat(-1);
		fptree local_header_list[num_ids];
		int local_count=0;
		int flg=1;
		for(int i=0;i<patt_base.size();i++)
		{
			for(int j=0;j<patt_base[i].size()-1;j++)	// last element in patt_base[i] ie patt_base[i][patt_base[i].size()-1] is the count of that pattern_base 
			{											//  , not any item hence that is not considered.
				flg=1;
				for(int k=0;k<local_count;k++)
				{
					if(local_header_list[k].item==patt_base[i][j])
					{
						flg=0;
						local_header_list[k].count+=patt_base[i][patt_base[i].size()-1];
					}
				}
				if(flg)
				{
					local_header_list[local_count].item=patt_base[i][j];
					local_header_list[local_count].next=NULL;
					local_header_list[local_count].count=patt_base[i][patt_base[i].size()-1];
					local_count++;
				}
			}
		}
		
		// Testing purpose...
		
//			cout<<"\n"<<" Generated local header list for the itemset : "<<endl;
//			for(int p=0;p<item.size();p++)
//				cout<<item[p]<<" ";
//			cout<<endl;
//			for(int p=0; p<local_count; p++)
//			{
//				cout<<" item : "<<local_header_list[p].item<<" count : "<<local_header_list[p].count<<"\n";
//			}
			
			int final_local_count = local_count;
//			dbg(final_local_count);
//			cout<<"After Pruning..."<<endl;
			int p;
			for(p=0; p<local_count&&local_header_list[p].count>=min_sup; p++)
			{
//				cout<<" item : "<<local_header_list[p].item<<" count : "<<local_header_list[p].count<<"\n";
			}
			final_local_count = p;
//			dbg(final_local_count);

		// Testing Part over ...
		
		fp_create(treeb, patt_base,local_header_list,local_count);
//		print_tree(treeb);
//		print_headers(final_local_count, local_header_list);
		trim_tree(treeb,local_header_list,local_count);
//		print_tree(treeb);
//		print_headers(final_local_count, local_header_list);
//		exit(0);
		if(treeb->nc==1)
		{
//			dbg("Only One Branch in Tree ");
			vector< vector< int> >pb_temp;
			vector<int>items_temp;
			for(int k=0;k<final_local_count;k++)
			{
				items_temp.push_back(local_header_list[k].item);
			}
			items_temp.push_back(treeb->count);	// simply pushed because count value is always to be poped from if-part ie all combinations generation code.
			pb_temp.push_back(items_temp);
			fp_mine(pb_temp,item);
		}
		else
		{
			create_pattern_base(local_header_list,final_local_count,item);
		}		

	}
	
}


void fp_create(ftptr &treeb, vector<vector<int> >patt_base,fptree header_list[],int local_count)
{
	for(int i=0;i<patt_base.size();i++)
	{
		int ct = patt_base[i][patt_base[i].size()-1];
		patt_base[i].erase(patt_base[i].end()-1);
//		dbg("Adding branch : ");
//		for(int j=0;j<patt_base[i].size();j++)
//			cout<<patt_base[i][j]<<" ";
//		cout<<endl;
		add_branch(treeb,patt_base[i],0,ct,header_list,local_count);
	}
}


void subset(vector<int>&items, vector<int>s, vector<vector<int> >&subs,int i)
{
	int set_size = pow(2,items.size());
	subs.clear();
	for(int cnt=0;cnt<set_size;cnt++)
	{
		s.clear();
		for(int j=0;j<items.size();j++)
		{
			if(cnt&(1<<j))
			{
				s.push_back(items[j]);
			}
		}
		subs.push_back(s);
	}
}










