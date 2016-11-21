#include<bits/stdc++.h>
#include<math.h>
using namespace std;
#define dbg(x) cout<<#x<<"="<<x<<endl;
struct dtree
{
	string attr; // attribute name
	int nc; //no of children
	dtree *chptr[10];  // no of children = no of distinct values of attr attribute
	dtree *pptr;
};

typedef dtree* dtptr;
dtptr root;

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

vector<string>attr_list;
vector<vector<string> >tuples;
map<string, set<string> >distinct_values;  // attr -> distinct values

void get_data(vector<string>&attr_list, vector<vector<string> >&tuples)
{
	fstream file;
	file.open("DataBase.txt", ios::in);
	string str;
	file>>str;
	file>>str;
	while(str!="1")
	{
		attr_list.push_back(str);
		file>>str;
	}
	int i=2;
	file>>str;
	set<string> set_val;
	set_val.clear();
	vector<string>values;
	while(!file.eof())
	{
		if(str==to_string(i))
		{
			file>>str;	
			tuples.push_back(values);
			values.clear();
			i++;
		}
		else
		{
			values.push_back(str);
			file>>str;
		}
	}
	values.push_back(str);
	tuples.push_back(values);
	file.close();
	for(int i=0;i<attr_list.size();i++)
	{
//		cout<<attr_list[i]<<"\t";
		distinct_values[attr_list[i]] = set_val; 
	}
	cout<<endl;
	for(int i=0;i<tuples.size();i++)
	{
		for(int j=0;j<tuples[0].size();j++)
		{
//			cout<<tuples[i][j]<<"\t";
			if( distinct_values[attr_list[j]].find(tuples[i][j]) == distinct_values[attr_list[j]].end())
			{
				distinct_values[attr_list[j]].insert(tuples[i][j]);
			}
		}
		cout<<endl;
	}
//	for(map<string,set<string> >::iterator it = distinct_values.begin(); it!= distinct_values.end(); it++)
//	{
//		cout<<(it->first)<<" : "<<endl;
//		for(set<string>::iterator jt = it->second.begin(); jt !=it->second.end(); jt++)
//		{
//			cout<<*(jt)<<" , ";
//		}
//		cout<<endl;
//	}
}


double calc_info_d(vector<int> &rem_tuples, map<string, int>&clas_cnt)
{
	double info_d = 0.0;
	int total = rem_tuples.size();
//	dbg(total);
	// info_d calculation
	for(map<string,int>::iterator it=clas_cnt.begin(); it!=clas_cnt.end(); it++)
	{
//		dbg(it->second);
		info_d += ((double(it->second))/double(rem_tuples.size()))*(log2((double(it->second)/double(rem_tuples.size()))));
	}
	info_d *= -1;
	return info_d;
}



void get_rem_data(int ind, string attr, string val, vector<int>&rem_attr_new, vector<int>& rem_tuples_new , vector<int>&rem_attr, vector<int>&rem_tuples )
{
	rem_attr_new = rem_attr;
	rem_attr_new.erase(rem_attr_new.begin()+ind);
	int index=-1;
	for(int i=0;i<attr_list.size();i++)
	{
		if(attr_list[i]==attr)
		{
			index=i;
			break;
		}
	}
	if(index==-1)
	{
		cout<<"ERROR!! "<<endl;
		exit(0);
	}
	
	for(int i=0;i<rem_tuples.size();i++)
	{
		if(tuples[rem_tuples[i]][index]==val)
		{
			rem_tuples_new.push_back( rem_tuples[i] );
		}
	}
	// done ...
}


int attr_selection( vector<int>&rem_attr, vector<int>&rem_tuples,  vector<string>&attr_list, vector<vector<string> >&tuples, map<string,int>&clas_cnt)
{
	double info_d = calc_info_d(rem_tuples, clas_cnt);
	dbg(info_d);
	int best_attr = -1;
	double gain = INT_MIN;
	double gain_max=INT_MIN;
	double info_a_d, info_a_n;
	map<string, int> ccnt;
	for(int i=0;i<rem_attr.size();i++)
	{
//		dbg("you");
		info_a_d = 0;
		ccnt.clear();
		vector<int>rem_attr_new;
		vector<int>rem_tuples_new;
		for(set<string>::iterator it = distinct_values[attr_list[rem_attr[i]]].begin(); it!=distinct_values[attr_list[rem_attr[i]]].end(); it++ )
		{
			info_a_n = 0.0;
			string tmp;
			get_rem_data(i, attr_list[rem_attr[i]], *it , rem_attr_new, rem_tuples_new, rem_attr, rem_tuples);
//			dbg("it");
			for(int i=1;i<rem_tuples_new.size();i++)
			{
				tmp = tuples[rem_tuples_new[i]][attr_list.size()-1]; 
				if(ccnt.find(tmp) == ccnt.end())
				{
					ccnt[tmp] = 1;
				}
				else
				{
					ccnt[tmp]++;
				}
			}
			info_a_n = calc_info_d(rem_tuples_new, ccnt);
			info_a_d += info_a_n*(double(rem_tuples_new.size())/double(rem_tuples.size()));				
		}		
		gain = info_d - info_a_d;
		dbg(gain);
		if(gain > gain_max)
		{
			gain_max = gain;
			best_attr = i;  // i is index in rem_attr which itself holds indices of attr_list
		}	
	}
	if(best_attr==-1)
	{
		cout<<" \n\n Not Possible To Reach Me !!"<<endl;
		exit(0);
	}
	return best_attr;
}


// rem_attr ->indices of reamining attributes for this subtree
// rem_tuple ->indices of tuples in the subdataset
dtptr create_tree(dtree *&root, vector<int>rem_attr, vector<int>rem_tuples,  vector<string>&attr_list, vector<vector<string> >&tuples)
{
	if(rem_tuples.size()==0)
	{
		cout<<"What!! How!!"<<endl;
		exit(0);
	}
//	dbg(rem_attr.size());
//	dbg(rem_tuples.size());
	root = new dtree();
	// if all tuples belong to the same class already ...
	int check = 1;
	map<string,int>clas_cnt;
	
	string clas = tuples[rem_tuples[0]][attr_list.size()-1];
	clas_cnt[clas] = 1;
	string tmp;
	for(int i=1;i<rem_tuples.size();i++)
	{
		// check for all same or not
		tmp = tuples[rem_tuples[i]][attr_list.size()-1]; 
		if(tmp != clas)
		{
			check=0;
		}
		// get count of all attributes
		if(clas_cnt.find(tmp) == clas_cnt.end())
		{
			clas_cnt[tmp] = 1;
		}
		else
		{
			clas_cnt[tmp]++;
		}
			
	}
	if(check==1)	// leaf node -> all in same class
	{
		dbg("all same");
		dbg(clas);
		root->attr = clas;
		root->nc = 0;
		return root;
	}
	// all are not in same class
	// check if rem_attr is empty
	int max_count=0;
	string max_str;
	if(rem_attr.size()==0)
	{
		dbg("rem attr = 0 so assign majority");
		// yes, so assign one with majority of class
		for( map<string,int>::iterator it = clas_cnt.begin(); it!=clas_cnt.end(); it++)
		{
			if(it->second > max_count)
			{
				max_str = it->first;
				max_count = it->second;
			}
		}
		root->attr = max_str;
		root->nc = 0;
		return root;
	}
	// else --> recursion for the tree
	dbg(" selection starts ..");
	
//	for( map<string,int>::iterator it=clas_cnt.begin(); it!=clas_cnt.end();it++)
//	{
//		cout<<it->first<<" : "<<it->second<<endl;
//	}
//	
	int selected_attr_ind  = attr_selection(rem_attr, rem_tuples, attr_list, tuples, clas_cnt);
	dbg(" selection ends..");
	string sel_attr = attr_list[rem_attr[selected_attr_ind]];
	cout<<"\n\n Selection is : "<<sel_attr<<endl;
	root->attr = sel_attr;
	root->nc=0;	

	for(map<string,set<string> >::iterator it = distinct_values.begin(); it!= distinct_values.end(); it++)
	{
		if(it->first==sel_attr)
		{
			for(set<string>::iterator jt = it->second.begin(); jt !=it->second.end(); jt++)
			{
				dbg("going for childern");
				dbg(*jt);
				
				vector<int>nav;
				vector<int>ntv;
				get_rem_data(selected_attr_ind, sel_attr, *jt, nav, ntv, rem_attr, rem_tuples);
				if(ntv.size()!=0)
				{
					root->nc++;
					root->chptr[root->nc-1] = create_tree(root->chptr[root->nc-1], nav, ntv, attr_list, tuples );
					root->chptr[root->nc-1]->pptr = root;
				}
			}
			break;
		}
	}
	return root;
}




int main()
{
//	dbg((9/14)*log2(9/14));
	root = NULL;
	get_data(attr_list, tuples);
	vector<int>lst1;
	vector<int>lst2;
	for(int i=0;i<attr_list.size()-1;i++)
		lst1.push_back(i);
	for(int j=0;j<tuples.size();j++)
		lst2.push_back(j);
		
	create_tree(root, lst1, lst2,  attr_list, tuples);	
	return 0;
}
