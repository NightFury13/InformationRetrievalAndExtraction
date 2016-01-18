#include<bits/stdc++.h>

using namespace std;

#define pb push_back

long long int chk[10000] = {0};
bool compareRows (vector<string> one,vector<string> two) 
{ 
	for(int i=0;i<one.size();i++)
	{
		if(one[i] != two[i])
			return (one[i] > two[i]);
	}

	return true; 
}

int check(int total)
{
	for(int i=1;i <= total;i++)
		if(chk[i] != -1)
			return 1;
	return -1;
}

priority_queue<pair<vector<string>,int>,vector<pair<vector<string>,int> >,greater<pair<vector<string>,int> > > heap;
priority_queue<pair<vector<string>,int>,vector<pair<vector<string>,int> > > heap1;
vector<int> col;
vector<int> sizes;
vector<int> col_order;
vector<vector<string> > colVals;
vector<string> col_srt;
long long int num_cols;
int asc=0;

int find_next(int fl,int total)
{
	if(chk[fl] != -1)
		return fl;
	for(int i=1;i<=total;i++)
	{
		if(chk[i] != -1)
			return i;
	}
	return -1;
}
void mergeAllBlocks(long long int num_blocks,long long int block_lines)
{
	FILE *fp[10000]; 
	FILE *fp_out = fopen("output.txt","w+");
	for(long long int i=1;i<=num_blocks;i++)
	{
		char fname[1000];
		sprintf(fname,"%lld.txt",i);
		fp[i] = fopen(fname,"r");
	}
	char line[1000];
	for(long long int i=1;i<=num_blocks;i++)
	{
		if(fgets(line,sizeof line,fp[i]) == NULL)
		{
			chk[i] = -1;
			continue;
		}
		vector<string> temp_vec;
		string s = "";
		long long int flag = 0;
		long long int idx = 0;
		std::istringstream ss(line);
		std::string token;
		while(std::getline(ss, token, ' ')) 
		{
			if(strcmp(token.c_str(),"")!=0)
				temp_vec.push_back(token);
		}
		string last = temp_vec[temp_vec.size()-1];
		last = last.substr(0,last.length()-1);

		temp_vec.pop_back();
		temp_vec.push_back(last);
		if(asc == 1)
			heap.push(make_pair(temp_vec,i));
		else
			heap1.push(make_pair(temp_vec,i));
	}
	while (1)
	{
		if(check(num_blocks) == -1)
			break;
		vector<string> temp_vec;
		if(asc == 1)
			temp_vec = heap.top().first;
		else
			temp_vec = heap1.top().first;
		char line[1000];
		strcpy(line,temp_vec[0].c_str());
		for(long long int j=1;j<temp_vec.size();j++)
		{
			strcat(line," ");
			strcat(line,temp_vec[j].c_str());
		}
		strcat(line,"\n");
		string stringLine="";
		vector <string> headDecl;
		long long int colCheck=0;
		while(colCheck<num_cols)
		{
			long long int l;
			for(l = 0; l<temp_vec.size();l++)
			{
				if(col_order[l]-1==colCheck)
					break;
			}
			stringLine += temp_vec[l];
			if(colCheck!=num_cols-1)
				stringLine +=" ";
			colCheck++;
		}
		stringLine+="\n";
		fputs(stringLine.c_str(),fp_out);
		if(asc == 1)
			heap.pop();
		else
			heap1.pop();

		long long int fl;
		if(asc == 1)
			fl =  find_next(heap.top().second,num_blocks);
		else
			fl =  find_next(heap1.top().second,num_blocks);

		if(fl == -1)
		{
			if(asc == 1)
			{
				while(!heap.empty())
				{
					vector<string> temp_vec3 = heap.top().first;
					strcpy(line,temp_vec3[0].c_str());	
					for(long long int j=1;j<temp_vec3.size();j++)
					{   
						strcat(line," ");
						strcat(line,temp_vec3[j].c_str());
					}
					strcat(line,"\n");
					string stringLine="";
					vector <string> headDecl;
					long long int colCheck=0;
					for(long long int k = 0 ;k<temp_vec3.size(); k++)
					{
						long long int l;
						for(long long int l = 0; l< temp_vec3.size();l++)
						{
							if(col_order[l]-1==colCheck)
							{
								break;
							}
						}
						stringLine += temp_vec[l];
						if(colCheck!=num_cols-1)
							stringLine +=" ";
						colCheck++;
					}
					stringLine+="\n";
					//cout<<stringLine;
					fputs(stringLine.c_str(),fp_out);
					heap.pop();
				}
			}
			else
			{
				while(!heap1.empty())
				{
					vector<string> temp_vec3 = heap1.top().first;
					strcpy(line,temp_vec3[0].c_str());	
					for(long long int j=1;j<temp_vec3.size();j++)
					{   
						strcat(line,"  ");
						strcat(line,temp_vec3[j].c_str());
					}
					strcat(line,"\n");
					string stringLine="";
					vector <string> headDecl;
					long long int colCheck=0;
					for(long long int k = 0 ;k<temp_vec3.size(); k++)
					{
						long long int l;
						for(l = 0; l< temp_vec3.size();l++)
						{
							if(col_order[l]-1==colCheck)
							{
								break;
							}
							stringLine += temp_vec[l];
							if(colCheck!=num_cols-1)
								stringLine +=" ";
							colCheck++;
						}
					}
					stringLine+="\n";
					fputs(stringLine.c_str(),fp_out);
					heap1.pop();
				}
			}
			break;

		}
		else
		{
			if(fgets(line,sizeof line,fp[fl]) == NULL)
				chk[fl] = -1;
			else
			{
				vector<string> temp_vec2;
				string s = "";
				long long int flag = 0;
				long long int idx = 0;
				
				std::istringstream ss(line);
				std::string token;
				while(std::getline(ss, token, ' ')) 
				{
					if(strcmp(token.c_str(),"")!=0)
						temp_vec2.push_back(token);
				}
				string last = temp_vec2[temp_vec2.size()-1];
				last = last.substr(0,last.length()-1);
				temp_vec2.pop_back();
				temp_vec2.push_back(last);
				if(asc == 1)
					heap.push(make_pair(temp_vec2,fl));
				else
					heap1.push(make_pair(temp_vec2,fl));

			}

		}

	}
	return;
}

void blocksort(char fname[100])
{
    int i,j;

    colVals.clear();
	FILE *fp = fopen(fname,"r");
	char line[1000];
	while(fgets(line,sizeof line,fp) != NULL)
	{
		string lin = "";
		lin+=line;
		lin = lin.substr(0,lin.length()-1);
		vector<string> temp_vec;
		for(long long int i=0;i<col_order.size();i++)
		{
			string s="";
			long long int cno = col_order[i]-1;
			std::istringstream ss(lin);
			std::string token;
			for(long long int j=0;j<=cno;j++)
			{
				//cout<<"+";
				std::getline(ss, token, ' ');
			    //std::cout << token << '\n';
			}
			s += token;
			temp_vec.push_back(s);
		}
		colVals.push_back(temp_vec);
	}
	fclose(fp);
    


	fp = fopen(fname,"w");
	if(asc == 1)
		sort(colVals.begin(),colVals.end());
	else
		sort(colVals.begin(),colVals.end(),compareRows);

	for(long long int i=0;i<colVals.size();i++)
	{
		strcpy(line,colVals[i][0].c_str());
		for(long long int j=1;j<colVals[i].size();j++)
		{
			strcat(line,"  ");
			strcat(line,colVals[i][j].c_str());
		}
		string tempString = "";
		tempString += line;
		tempString +='\n';
		fputs(tempString.c_str(),fp);
	}
	fclose(fp);
}

int main(int argc,char *argv[])
{
	int i,j,k;

    string cell,line;
    ifstream metafile ("metadata.txt");
    map<string, int> md;
    map<int, int> md_size;
    
    if(metafile.is_open())
    {
        int index=1;
        while ( getline (metafile,line) )
        {
            

            line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

            int count=0;
            string key;
            int val;
            stringstream lineStream(line);
            while(getline(lineStream,cell,','))
            {

                if(count==0)
                    key=cell;
                else
                    val=stoi(cell);
                
                cell.erase(remove_if(cell.begin(), cell.end(), ::isspace), cell.end());
                count++;

            }
            md[key]=index;
            md_size[index]=val;            
            index++;

        }
        metafile.close();
    }

    
    string inputFile = argv[1];
    string outFile = argv[2];

    int mem = stoi(argv[3]);

	if(strcmp(argv[4],"asc") == 0)
		asc = 1;
	else if(strcmp(argv[4],"desc") == 0)
		asc = 0;
	else
	{
		cout << "INVALID ARGUMENTS" << endl;
		return 0;
	}

    map<int,bool> flagger;
    
    for(auto i=md.begin(); i!=md.end(); i++)
    {
        flagger[i->second] = false;
    }

	for(int i=5;i<argc;i++)
	{
		string temp = argv[i];
        flagger[md[temp]]=true;
        col_order.pb(md[temp]);
	}

    /*
    for(auto i=md.begin(); i!=md.end(); i++)
    {
        cout << i->first<< " " << i->second << endl; 
    }
    */

    for(auto i=flagger.begin();i!=flagger.end();i++)
    {
        if(i->second==false)
        {
            col_order.pb(i->first);
            flagger[i->first]=true;
        }
    }
    
    /*
    for( i=0;i<col_order.size();i++)
    {
        cout << col_order[i] << endl;
    }
    */


    ifstream iFile (inputFile);
    long long lines=0;
    long long lineSize=0;

    if(iFile.is_open())
    {
        string line;
        while ( getline (iFile,line) )
        {
                    lines++;
                    lineSize=line.size();
        }
        iFile.close();
    }

    long long total_data = lines*lineSize;
    long long num_blocks = total_data/(mem*1024*1024);
    if((total_data%(mem*1024*1024)) != 0)
    {
               num_blocks++;
    }
    
    long long block_lines = lines/num_blocks;
    ifstream inFile("input.txt");
    if(inFile.is_open())
    {

	for(int i=1;i<=num_blocks;i++)
	{
        string line;
        ofstream outFile;
        stringstream ss;
        ss << i;
        string fName = ss.str();
        fName = fName + ".txt";
        outFile.open(fName);
		int cnt = 0;
		while(cnt < block_lines)
		{
            if( getline (inFile,line) )
            {
                outFile << line <<endl;
            }
			cnt++;
		}
	}

    }
    num_cols = flagger.size();

	for(int i=1;i<=num_blocks;i++)
	{
		char fname[100];
		sprintf(fname,"%d.txt",i);
		blocksort(fname);
	}

    mergeAllBlocks(num_blocks,block_lines);
	
    for(int i=1;i<=num_blocks;i++)
	{
		char fname[100];
		sprintf(fname,"%d.txt",i);
        if( remove( fname ) != 0 )
                perror( "Error deleting file" );

    }

}
