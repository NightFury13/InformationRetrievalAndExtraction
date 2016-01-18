#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <sstream> 
#include <iostream> 
#include <cstdio>
#include <unordered_map>
#include <regex>
#include <fstream>
#include "stemmer.cpp"

using namespace std;
#define TOTAL_DOCS 14162149
#define TITLE_WEIGHT 100000000

map<int, int> docWords;
set<string> stopWordsSet;
const char* fieldEnum[] = {"category", "title", "reference", "link", "infobox", "text"};
vector<string> fields (fieldEnum, fieldEnum + 6);
string path="";
void loadDocWords()
{

    ifstream docWordsFile(path + "/docWordsFile");
    if(docWordsFile.is_open())
    {
        string line;
        while (getline(docWordsFile, line))
        {
            stringstream lineStream(line);
            string cell;

            int count=0,id=0;
            while(getline(lineStream, cell, ' '))
            {
                if(count)
                {
                    docWords[id]=stoi(cell);
                    break;
                }
                id=stoi(cell);
                count++;
            }

        }
        docWordsFile.close();
    }
}

void loadStopWords()
{
    ifstream stopWordsFile("src/stop_words.txt");
    string line;
    if(stopWordsFile.is_open())
    {
        while(getline(stopWordsFile,line))
        {
            stringstream lineStream(line);
            string cell;
            while(getline(lineStream,cell,' '))
            {
                stopWordsSet.insert(cell);
            }

        }
        stopWordsFile.close();
    }    
}


vector <int> queryCat;
void getAllAns(vector<string> & v)
{
    map<long long,double> tiw;
    int i,j,k;
    typedef pair<double, long long> pairs;
    set <pairs > P;
    for(i=0;i<v.size();i++)
    {   

        string word = v[i];

        string fileName = path+"/files/", newFileName, offsetFileName;
        char first = word[0];
        char second = word[1];
        fileName += first;
        fileName += second;
        fileName += "/";
        map<long long ,long long> freq;
	map<long long,long long> cnt;
        for(int k=0;k<fields.size();k++)
        {
            if(queryCat.size() && queryCat[i]!=k)
                continue;
            newFileName = fileName+fields[k];
            offsetFileName = newFileName + ".index"; 

            ifstream offsetFile(offsetFileName);

            string line;
            if(offsetFile.is_open())
            {
                while (getline(offsetFile, line))
                {
                    stringstream lineStream(line);
                    string cell;

                    int count=0,offset,flag=0;

                    while(getline(lineStream, cell, ' '))
                    {
                        if(count==0)
                        {
                            if(word!=cell)
                                break;
                            flag=1;
                        }

                        if(count)
                            offset=stoll(cell);
                        count++;

                    }
                    if(flag)
                    {


                        // Iterating over posting list int this
                        // category
                        ifstream newFile(newFileName);
                        if(newFile.is_open())
                        {
                            newFile.seekg (offset, newFile.beg);
                            getline(newFile, line);
                            if(line.size()>6000000)
                                continue;
                            stringstream lineStream(line);
                            string cell;
                            while(getline(lineStream, cell, '#'))
                            {
                                if(cell.size() &&  cell[0]>='0' &&cell[0]<='9')
                                {
                                    stringstream lineStream(cell);
                                    string new_cell;
                                    long long count=0,page,f=0;
                                    while(getline(lineStream, cell, '-'))
                                    {
                                        if(count)
                                        {
                                            f=stoll(cell);
                                            if(k==1)
                                                f*=TITLE_WEIGHT;
                                        }                                            
                                        else
                                            page=stoll(cell);

                                        count++;
                                    }
                                    if(freq.find(page)!=freq.end())
                                        freq[page]+=f;
                                    else
                                        freq[page]=f;
                				    cnt[page]+=1;

                                }

                            }
                            newFile.close();
                        }


                    }
                }


                offsetFile.close();
            }

        }
    	int l=v.size();
        double idf=0;
        if(freq.size())
            idf=(float)log((float)TOTAL_DOCS/(float)freq.size());
        for(auto it=freq.begin();it!=freq.end();it++)
        {
   	    if(cnt[(*it).first]<l)
		    continue;
	    if(tiw.find((*it).first)!=tiw.end())
                tiw[(*it).first]+=((double)freq[(*it).first]*idf);
            else
                tiw[(*it).first]=((double)freq[(*it).first]*idf);
        }

    }
    if(tiw.size()==0)
    {
        cout << "No Answer" << endl;
        return;
    }
    
    string titleFileName = path+ "/titleFile";
    string line;
    ifstream titleFile(titleFileName);
    for(auto it=tiw.begin();it!=tiw.end();it++)
    {
        if(P.size()>10)
            P.erase(P.begin());
        else
        {
            pairs m = make_pair((*it).second,(*it).first);

            //titleFile.seekg(167*m.second, titleFile.beg);
            //getline(titleFile, line);
            //stringstream lineStream(line);
            //string cell;
            //int flag=0;
            /*
            while(getline(lineStream, cell, ' '))
            {
                if((cell[0]<='Z' && cell[0]>='A' )|| (cell[0]<='z' && cell[0]>='a'))
                {
                    flag=1;
                }
            }
            */
            //if(flag)
            P.insert(m);
        }
    }

    for(auto it=P.end();it!=P.begin();)
    {
        it--;
        pairs m = *it;

        titleFile.seekg (167*m.second, titleFile.beg);
        getline(titleFile, line);
        stringstream lineStream(line);
        string cell;
        int flag=0;
        while(getline(lineStream, cell, ' '))
        {
            if((cell[0]<='Z' && cell[0]>='A' )|| (cell[0]<='z' && cell[0]>='a'))
            {
                cout << cell << ' ' ;
                flag=1;
            }
            else if(flag)
                break;
        }
        cout << endl;
    }
    P.clear();
    titleFile.close();

}

int main(int argc, char* argv[])
{
    loadStopWords();
    //loadDocWords();
    long long n,i;
    string line;
    path=argv[1];

    // Get the number of queries (extra read)
    vector<string> query;
    getline(cin,line);
    int total=stoll(line);
    while (total-- && getline(cin, line))
    {
        stringstream lineStream(line);
        string cell;

        while(getline(lineStream, cell, ' '))
        {

            string new_cell="";
            for(i=0;i<cell.size();i++)
            {
                if(cell[i]<='Z' && cell[i]>='A')
                {
                    cell[i]='a'+(char)(cell[i]-'A');
                }
                if((cell[i]>='a' && cell[i]<='z' )|| cell[i]==':')
                {
                    new_cell += cell[i];
                }
            }

            new_cell.erase(remove_if(new_cell.begin(), new_cell.end(), ::isspace), new_cell.end());
            if(new_cell.size()<=1 || stopWordsSet.find(new_cell)!=stopWordsSet.end())
                continue;

            if(new_cell[1]==':')
            {
                if(new_cell[0]=='c')
                    queryCat.push_back(0);
                else if(new_cell[0]=='t')
                    queryCat.push_back(1);
                else if(new_cell[0]=='r')
                    queryCat.push_back(2);
                else if(new_cell[0]=='e')
                    queryCat.push_back(3);
                else if(new_cell[0]=='i')
                    queryCat.push_back(4);
                else if(new_cell[0]=='b')
                    queryCat.push_back(5);

                new_cell=new_cell.substr(2);
            }
            char word[1000];
            if(new_cell.size()>100)
                continue;
            for(i=0;i<new_cell.size();i++)
            {
                word[i]=new_cell[i];
            }
            word[i]='\0';
            int len = stem(word, 0, strlen(word) - 1) + 1;
            new_cell = new_cell.substr(0,len);

            query.push_back(new_cell);
        }
        getAllAns(query);
        query.clear();
        queryCat.clear();
    }
}
