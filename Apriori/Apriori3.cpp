#include<bits/stdc++.h>
#include<string>
using namespace std;
#define dbg(x) cout<<#x<<"="<<x<<endl;
fstream ifile,ofile;
fstream Result_File;
const int threshold = 2;
int curr_l_no=1;
string filename = "T";
const int num_ids=5;
const int num_trans = 9;
int cnt;  // from recently read file
int curr_count=num_ids;  // to write to next file
int fdi=1;
int fdo=2;

// Final, Used Prune, Not generated All li's and Generated Result_File Finally.

struct trans_ids
{
	int id[num_ids+1];
	int count;
	trans_ids()
	{
		count=0;
	}	
};

trans_ids id_arr[10000];   	 // read from Li data file  -->> 0 based indexing
trans_ids out_id_arr [10000];  // write to Li+1 data file  -->> 0 based indexing
int bitmap[num_trans+1][num_ids+1];  // bitmap of initial transactional data  -->> 1 based indexing

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


void read_file()
{
	string st;
	st=filename;
	st.append(to_string(fdi));
	fdi=fdo;
	st.append(".txt");
//	cout<<" file "<<st<<endl; 
	ifile.open(st.c_str(), ios::in);
	if(!ifile) cout<<"err";
	string s;
//	cout<<"okjih";
	ifile>>s;  ifile>>s;  //ifile>>s; cout<<s; //ifile>>s; cout<<s; ifile>>s; cout<<s;
	cnt=0;
	int ct=0;
	while(!ifile.eof())
	{
		for(int i=0;i<curr_l_no;i++)
		{
			ifile>>s;
//			cout<<"s "<<s;
			id_arr[cnt].id[i]=to_int(s.substr(1,s.length()-1));
		}
		ifile>>s;
		id_arr[cnt].count = to_int(s);
		if(id_arr[cnt].count>=threshold) cnt++;
	}
	cnt--;
//	for(int i=0;i<cnt;i++)
//	{
//		for(int j=0;j<curr_l_no;j++)
//		{
//			cout<<id_arr[i].id[j]<<" ";
//		}
//		cout<<id_arr[i].count<<"\n";
//	}
	ifile.close();
}

void write_file()
{
	string st;
	st=filename;
	st.append(to_string(fdo));
	fdo=fdi;
	st.append(".txt"); 
	ofile.open(st.c_str(), ios::out);
	if(!ofile) cout<<"err";
	string s;
	ofile<<"IDs"<<"\t\tT_Count"<<endl;
	for(int i=0;i<curr_count;i++)
	{
		for(int j=0;j<curr_l_no;j++)
		{
			ofile<<"I"<<out_id_arr[i].id[j]<<" ";
			Result_File<<"I"<<out_id_arr[i].id[j]<<" ";
		}
		ofile<<"\t\t"<<out_id_arr[i].count<<"\n";
		Result_File<<"\t\t"<<out_id_arr[i].count<<"\n";
	}
	ofile.close();
}


void bmp()   // bitmap generation --> AllElectronics.cpp
{
	for(int i=0;i<=num_trans;i++)
	{
		for(int j=0;j<=num_ids;j++)
			bitmap[i][j]=0;
	}
	ifile.open("AllElectronics.txt", ios::in);
	string s;
	ifile>>s; ifile>>s; ifile>>s; ifile>>s;  ifile>>s; 
//	 ifile>>s; cout<<s;  ifile>>s; cout<<s;
 	ifile>>s;  // Read T001
 	
	for(int i=1;i<=num_trans;i++)
	{
		if(!ifile.eof())
		 ifile>>s; // Read first Item
		 while(!ifile.eof()&&s[0]=='I')
		 {
		 	bitmap[i][to_int(s.substr(1,s.size()-1))]=1;
		 	ifile>>s;
		 }
	}
	cout<<endl<<endl;
//	for(int i=1;i<=num_trans;i++)
//	{
//		for(int j=1;j<=num_ids;j++)
//			cout<<bitmap[i][j]<<" ";
//		cout<<endl;
//	}
	ifile.close();
	
}

bool prune(trans_ids obj)
{
	int flg=0;
	// generate subsets of size k-1
	int subset[num_ids];
	int ct = 0;
	for(int i=0;i<curr_l_no+1;i++)
	{
		ct = 0;
		for(int j=0;j<curr_l_no+1;j++)
		{
			if(j!=i)
			{
				subset[ct++] = obj.id[j];
			}
		}
		// subset contains a subset of k items, check its presence in input_file
		flg=0;
		int p;
		for(int k=0;k<cnt&&!flg;k++)
		{
			for(p=0;p<curr_l_no;p++)
			{
//				cout<<id_arr[k].id[p];
				if(subset[p]==id_arr[k].id[p]);
				else break;
			}
			if(p==curr_l_no) flg=1;
		}
		//cout<<" returning false "<<id_arr[k].id[p]<<" "<<subset[p]<<" ";
		if(!flg)
			return false;
	}
	return true;
}


int get_count_bitmap(int ids[], int n)
{
	int count=0;
	int p=1;
	for(int i=1;i<=num_trans;i++)
	{
		p=1;
		for(int j=0;j<=n&&p; j++)  // number of ids NOT the id numbers
		{
			p = p&bitmap[i][ids[j]];
		}
		if(p) count++;
	}
	return count;
}


void join()
{
	int f=1;
	int out_cnt=0;
	int p=0,k=0,i=0,j=0;
	for(i=0;i<cnt;i++)
	{
		for(j=i+1;j<cnt;j++)
		{
			f=1;
			// check for common in k-1 items
			for(k=0;k<curr_l_no-1;k++)
			{
				if(id_arr[i].id[k]!=id_arr[j].id[k]) 
				{
					f=0;
					break;
				}
			}
			// check if kth item in ith set is smaller than kth item in jth set
			if(f&&id_arr[i].id[k]<id_arr[j].id[k]) {}
			else f=0;
			// if flag then join is valid
			if(f)
			{
//				dbg(out_cnt);
//				dbg(f);
//				dbg(curr_l_no);
				for(k=0;k<curr_l_no;k++)
				{
//					dbg(k);
					out_id_arr[out_cnt].id[k]=id_arr[i].id[k];
				}
				out_id_arr[out_cnt].id[k]=id_arr[j].id[k-1];
				//===================== Prune step / Apriori Property =====================//
				if(prune(out_id_arr[out_cnt]))
				{
					out_id_arr[out_cnt].count = get_count_bitmap(out_id_arr[out_cnt].id,k);
//					cout<<" Prune Passed \n";
				}
				else
				{
					out_id_arr[out_cnt].count = 0;
//					cout<<" Prune Failed \n";
				}
			
				if(out_id_arr[out_cnt].count>=threshold)
					out_cnt++;	
			
			}
		}
	}
	curr_count = out_cnt;
	curr_l_no++;
}

int main2();

int main()
{
	Result_File.open("Result_File.txt",ios::out);
	Result_File<<"IDs"<<"\t\tT_Count"<<endl;
	main2();
	bmp();
	do
	{
		read_file();
		join();
		write_file();
	}while(curr_count>1);

	return 0;
}



struct ids
{
	string id;
	int count;
	ids()
	{
		count=0;
	}	
};



ids trans_for_ids[num_ids+1];

void get_data()
{
	for(int i=1;i<=num_ids;i++)
	{
		trans_for_ids[i].id="I";
//		trans_for_ids[i].id += char(i+'0');
		trans_for_ids[i].id.append(to_string(i));
		trans_for_ids[i].count=0;
	}
	string s;
	ifile>>s; ifile>>s; ifile>>s; ifile>>s; ifile>>s;
//	cout<<s;
	int tno;
	int ino;
	while(!ifile.eof())
	{
		ifile>>s;
		if(s[0]=='T')
		{
//			tno=(s[3]-'0');
			tno = to_int(s.substr(3,s.length()-3));
		}
		else if(s[0]=='I')
		{
//			cout<<"len "<<s.length()<<endl;
//			cout<<to_int(s.substr(1,s.length()-1))<<endl;
			trans_for_ids[to_int(s.substr(1,s.length()-1))].count++;
//			trans_for_ids[to_int(s.substr(1,s.length()-1))].trans.append(to_string(tno));
		}
		if(ifile.eof()) break;
	}
	trans_for_ids[to_int(s.substr(1,s.length()-1))].count--;	
	for(int i=1;i<=num_ids;i++)
	{
		if(trans_for_ids[i].count>threshold);
		{
			ofile<<trans_for_ids[i].id<<"\t\t"; 
			ofile<<trans_for_ids[i].count<<endl; 
			Result_File<<trans_for_ids[i].id<<"\t\t"; 
			Result_File<<trans_for_ids[i].count<<endl; 
		}
	}
}

int main2()
{
	ifile.open("AllElectronics.txt",ios::in);
	ofile.open("T1.txt",ios::out);
	ofile<<"IDs"<<"\t\t"<<"T_Count"<<endl;
	get_data();
	ifile.close();
	ofile.close();
	return 0;
}

