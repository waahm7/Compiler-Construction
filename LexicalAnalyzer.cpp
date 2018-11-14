#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

string convertEscapeSequences(string str){
    string result;
    for (int i=0;i<str.length();i++) {
        char c = str[i];
        if (c == '\\' &&i<str.length()-1){
            switch (str[++i]) {
               //  case '\\':
                //     c = '\\';
                 //    break;
                case 'n':
                    c = '\n';
                    break;
                case 't':
                    c = '\t';
                    break;
                case 'b':
                    c='\b';
                    break;
                case 'r':
                    c='\r';
                    break;
				
				case 'a':
                    c='\a';
                    break;
				case 'f':
                    c='\f';
                    break;
				case 'v':
                    c='\v';
                    break;	
				default:
					c='\\';
					i--;
					break;

            }
        }
        result += c;
    }
    return result;
}
void printChar(char ch){
	switch(ch){
		case '\n':
			cout<<"\\n";
			break;
		case '\0':
			cout<<"\\0";
			break;	
		case '\a':
			cout<<"\\a";
			break;
		case '\b':
			cout<<"\\b";
			break;
		case '\f':
			cout<<"\\f";
			break;
		case '\r':
			cout<<"\\r";
			break;
		case '\t':
			cout<<"\\t";
			break;
		case '\v':
			cout<<"\\v";
			break;
		default:
			cout<<ch;	
			break;
	}
}
int getNumberOfStatesFromFile(){
	ifstream in("input.txt");
	int count=0;
	string str;
	getline(in,str);
	while(!in.eof()){
		if(str.length()>0)
			count++;
	//	cout<<str<<endl;	
		getline(in,str);

	}
	if(str.length()>0)
		count++;
	in.close();
	return count-1;
}
int stringToInt(string str){
	stringstream ss(str); 
  	int num = 0; 
    ss >> num; 
	return num;
}
vector<int> splitString(string str){
	istringstream iss(str);
	vector<std::string> result((istream_iterator<string>(iss)),
	istream_iterator<string>());
	vector<int> resultInteger;
	// for(int i=0;i<result.size();i++)
	// 	cout<<result[i]<<' ';
	// cout<<endl;
	for(int i=0;i<result.size();i++)
		resultInteger.push_back(stringToInt(result[i]));
	
	return resultInteger;
	
}
void displayFA(vector<int> columns,int** FA,int r,int c){
	for(int i=0;i<columns.size();i++)
		cout<<columns[i]<<' ';
	cout<<endl;
	for(int i=0;i<r;i++){
		for(int j=0;j<c;j++)
			if(FA[i][j]!=-1)
				cout<<FA[i][j]<<' ';
		cout<<endl;	
	}
}
void init(int** FA,int r,int c){
	for(int i=0;i<r;i++)
		for(int j=0;j<c;j++)
			FA[i][j]=-1;
}

int isUnique(vector<vector<int> >& compressed, int**FA, int column){
	if(compressed.size()<1)
		return -1;
	for(int i=0;i<compressed[0].size();i++){
		int a1=0;
		int a2=0;
		bool equal=true;
		for(;a2<compressed.size()&&equal;a2++){
			if(FA[a1++][column]!=compressed[a2][i])
				equal=false;
		}
		if(equal)
			return i;
	}
	return -1;

}
void insertInToCompressed(vector<vector<int> >& compressed, int**FA, int column){
	for(int i=0;i<compressed.size();i++){
			compressed[i].push_back(FA[i][column]);
	}
	cout<<endl;
}
void writeCompressed(vector<vector<int> >& compressed){
	ofstream out("compressed.txt");
	for(int i=0;i<compressed.size();i++){
		for(int j=0;j<compressed[i].size();j++){
			
			out<<compressed[i][j]<<' ';
		}
		out<<endl;	
	}
}
void displayMap(int map[]){
	for(int i=0;i<129;i++)
		cout<<map[i]<<' ';
	cout<<endl;	
}
void printLexeme(string str){
	for(int i=0;i<str.length();i++)
		printChar(str[i]);
		
}
void printTokken(string str,int& FS,int& LS,int& CS,int& LSFS,int& remember,int &LN){
	if(LSFS!=-1)
		FS=remember+1;
	else 
		FS=LS+1;

	printLexeme(str.substr(LS,FS-LS));
	if(LSFS!=-1)
		cout<<'\t'<<LSFS<<endl;	
	else
		cout<<"\terror\tline no:"<<LN<<endl;	

	CS=0;
	LSFS=-1;
	LS=FS;

}
int main(){

	ifstream in("FA.txt");
	ifstream input("input.txt");
	
	int states=getNumberOfStatesFromFile();
	int** FA;
	string str;
	FA=new int*[states];
	for(int i=0;i<states;i++)
		FA[i]=new int[129];
	
	init(FA,states,129);
	getline(in,str);
	vector<int> columns=splitString(str);
	columns[columns.size()-1]=128;
	for(int i=0;i<states;i++){
		getline(in,str);
	//	cout<<str<<endl;
		vector<int> values=splitString(str);
		for(int j=0;j<values.size();j++){
			//cout<<values[j]<<':'<<columns[j]<<endl;
		
			FA[i][columns[j]]=values[j];
		}		
	}
	//displayFA(columns,FA,states,129);
	//cout<<"\n\n";
	vector<vector<int> > compressed;
	for(int i=0;i<states;i++){
		vector<int> temp;
		compressed.push_back(temp);
	}	
	//cout<<compressed.size()<<endl;
	//now time to compress it
	int map[129];
	for(int i=0;i<129;i++){
		//check is ith comlumn is unique or not
		int unique=isUnique(compressed,FA,i);
		if(unique==-1){
				insertInToCompressed(compressed,FA,i);	
				map[i]=compressed[0].size()-1;
		}else
			map[i]=unique;
	}
	//displayMap(map);
	writeCompressed(compressed);
	
	
	
	
		
	
	int finalStates[]={1,2,5,6,7};
	int LS=0,LSFS=-1,remember=-1,CS=0,FS=0,LN=0;
	char ch;
	str="";
	stringstream buffer;
	buffer << input.rdbuf();
	str=buffer.str();
	
	
	str=convertEscapeSequences(str);
	
	int k=0;
	while(FS<str.length()){
	
		CS=compressed[CS][map[(int)str[FS]]];			
		if(CS!=-1){
		
			if(compressed[CS][compressed[0].size()-1]!=0){
					LSFS=compressed[CS][compressed[0].size()-1];                                                                                                                                                                                                                                                                                                                                                            
					remember=FS;
					if(str[FS]=='\n'){
						LN++;
					}

			}
			FS++;
			if(FS>=str.length())
				printTokken(str,FS,LS,CS,LSFS,remember,LN);

		}else
				printTokken(str,FS,LS,CS,LSFS,remember,LN);

	}

	in.close();
	input.close();
	//destructor	
	for(int i=0;i<states;i++)
		delete FA[i];
	delete FA;


	return 0;
}