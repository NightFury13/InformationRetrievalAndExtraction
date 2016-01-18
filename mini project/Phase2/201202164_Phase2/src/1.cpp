// #include<bits/stdc++.h>

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <sstream> 
#include <cstdio>
#include <unordered_map>
#include <regex>
#include <fstream>
#include "stemmer.cpp"
#include <sys/stat.h>

using namespace std;

long long words =0;
unordered_map<int,int>  pageMap;
set<string> stopWordsSet;
map<string, long long> fileOffset;

class WordDetail
{
    public:
        // vector of pageId,Frequency
        map <int, int > category;
        map <int,int> title;
        map <int,int>  infobox;
        map <int,int> text;
        map <int,int> link;
        map <int,int> reference;
        map <int,int> all;
        bool isStopWord;
        int count;
        WordDetail()
        {
            isStopWord=false;
            count=0;

        }
        void cleanData()
        {
            all.erase( all.begin(), all.end() );
            reference.erase( reference.begin(), reference.end() );
            infobox.erase( infobox.begin(), infobox.end() );
            category.erase( category.begin(), category.end() );
            text.erase(text.begin(), text.end() );
            link.erase(link.begin(), link.end());
            title.erase(title.begin(), title.end());
            count=0;
        }
        ~WordDetail()
        {
            cleanData();
        }
};

unordered_map<string,WordDetail>  index1;

void insertWord(string category,string new_cell,int page)
{
    if(index1.count(new_cell))
    {
        if(index1[new_cell].isStopWord==false)
        {
            if(category=="category")
            {
                if(index1[new_cell].category.count(page))
                {
                    index1[new_cell].category[page]++;
                }
                else
                {
                    index1[new_cell].category.insert({page,1});
                }

            }
            else if(category=="title")
            {

                if(index1[new_cell].title.count(page))
                {
                    index1[new_cell].title[page]++;
                }
                else
                {
                    index1[new_cell].title.insert({page,1});
                }
            }
            else if(category=="infobox")
            {

                if(index1[new_cell].infobox.count(page))
                {
                    index1[new_cell].infobox[page]++;
                }
                else
                {
                    index1[new_cell].infobox.insert({page,1});
                }
            }
            else if(category=="link")
            {
                if(index1[new_cell].link.count(page))
                {
                    index1[new_cell].link[page]++;
                }
                else
                {
                    index1[new_cell].link.insert({page,1});
                }

            }
            else if(category=="reference")
            {

                if(index1[new_cell].reference.count(page))
                {
                    index1[new_cell].reference[page]++;
                }
                else
                {
                    index1[new_cell].reference.insert({page,1});
                }
            }
            else if(category=="text")
            {

                if(index1[new_cell].text.count(page))
                {
                    index1[new_cell].text[page]++;
                }
                else
                {
                    index1[new_cell].text.insert({page,1});
                }
            }
            if(index1[new_cell].all.count(page))
            {
                index1[new_cell].all[page]++;
            }
            else
            {
                index1[new_cell].all.insert({page,1});
            }
        }
        else
            return;

    }
    else
    {
        WordDetail wordDetail;
        if(category=="text")
            wordDetail.text.insert({page,1});
        else if(category=="category")
            wordDetail.category.insert({page,1});
        else if(category=="link")
            wordDetail.link.insert({page,1});
        else if(category=="reference")
            wordDetail.reference.insert({page,1});
        else if(category=="title")
            wordDetail.title.insert({page,1});
        else
            wordDetail.all.insert({page,1});

        index1.insert({new_cell,wordDetail});

    }
    if(category=="all")
        index1[new_cell].count++;

}
const char* fieldEnum[] = {"category", "title", "reference", "link", "infobox", "text"};
vector<string> fields (fieldEnum, fieldEnum + 6);
void createIndexFiles()
{
    int i,j;
    for(i=0;i<26;i++)
    {
        for(j=0;j<26;j++)
        {
            string fileName = "Index/files/", newFileName;
            char first='a'+ i;
            char second = 'a' + j;
            fileName+=first;
            fileName+=second;

            const int dir_err = mkdir(fileName.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            fileName+="/";
            for(int k=0;k<fields.size();k++)
            {
                newFileName=fileName+fields[k];
                ofstream newFile(newFileName);
                newFile << "\n" << endl;
                newFile.close();
            }
        }
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

    }
}

void writeToIndex()
{
    for(auto i=index1.begin();i!=index1.end();i++)
    {
        if(i->first.size() <= 1 || i->first[0]>'z' || i->first[0]<'a' || i->first[1]<'a' || i->first[1]>'z')
            continue;

        string fileName = "Index/files/", newFileName, temp;

        char first=i->first[0];
        char second = i->first[1];
        fileName+=first;
        fileName+=second;
        fileName+="/";

        newFileName=fileName+"title";
        ofstream newFile(newFileName, std::ios_base::app);

        if(i->second.title.size())
        {
            newFile << i->first << " #";
            //newFile << "Title:" <<endl;
            for(auto j=i->second.title.begin();j!=i->second.title.end();j++)
            {
                newFile << j->first << '-' << j->second << "#";
            }
            newFile <<endl;
        }
        newFile.close();


        newFileName=fileName+"infobox";
        newFile.open(newFileName, std::ios_base::app);

        if(i->second.infobox.size())
        {
            newFile << i->first << " #";
            //newFile << "Infobox:" <<endl;
            for(auto j=i->second.infobox.begin();j!=i->second.infobox.end();j++)
            {
                newFile << j->first << '-' << j->second << "#";
            }
            newFile  << endl;
        }
        newFile.close();

        newFileName=fileName+"text";
        newFile.open(newFileName, std::ios_base::app);
        if(i->second.text.size())
        {
            newFile << i->first << " #";
            //newFile << "Text:" <<endl;
            for(auto j=i->second.text.begin();j!=i->second.text.end();j++)
            {
                newFile << j->first << '-' << j->second << "#";
            }
            newFile  << endl;
        }
        newFile.close();

        newFileName=fileName+"link";
        newFile.open(newFileName, std::ios_base::app);
        if(i->second.link.size())
        {
            newFile << i->first << " #";
            //newFile << "Link:" <<endl;
            for(auto j=i->second.link.begin();j!=i->second.link.end();j++)
            {
                newFile << j->first << '-' << j->second << "#";
            }
            newFile << "#" << endl;
        }
        newFile.close();

        newFileName=fileName+"reference";
        newFile.open(newFileName, std::ios_base::app);
        if(i->second.reference.size())
        {
            newFile << i->first << " #";
            //newFile << "Reference:" <<endl;
            for(auto j=i->second.reference.begin();j!=i->second.reference.end();j++)
            {
                newFile << j->first << ' ' << j->second << "#";
            }
            newFile << endl;
        }
        newFile.close();

        newFileName=fileName+"category";
        newFile.open(newFileName, std::ios_base::app);

        if(i->second.category.size())
        {
            newFile << i->first << " #";
            //newFile << "Category:" <<endl;
            for(auto j=i->second.category.begin();j!=i->second.category.end();j++)
            {
                newFile << j->first << '-' << j->second << "#";
            }
            newFile  <<endl;
        }
        newFile.close();

    }
}



void sortBlocks()
{
    int i,j,k;
    string prev="";
    for(i=0;i<26;i++)
    {
        for(j=0;j<26;j++)
        {
            string fileName = "Index/files/", newFileName, offsetFileName;
            char first='a'+ i;
            char second = 'a' + j;
            fileName+=first;
            fileName+=second;
            fileName+="/";

            for(int k=0;k<fields.size();k++)
            {
                newFileName=fileName+fields[k];
                offsetFileName = newFileName + ".index";
                ifstream newFile(newFileName);
                vector<pair<string, string> > sortedIndex;

                if(newFile.is_open())
                {
                    string line;
                    while(getline(newFile,line))
                    {
                        stringstream lineStream(line);
                        string cell;

                        int count=0;
                        string word="" , posting="";
                        while(getline(lineStream, cell, ' '))
                        {
                            if(count)
                                posting=cell;
                            else
                                word=cell;
                            count++;
                        }
                        sortedIndex.push_back({word,posting});
                    }
                    newFile.close();
                }

                sort(sortedIndex.begin(), sortedIndex.end());
                ofstream newOutFile(newFileName);
                ofstream offsetFile(offsetFileName);
                prev="";

                if(newOutFile.is_open() && offsetFile.is_open())
                {
                    for(int l=0;l<sortedIndex.size();l++)
                    {
                        if(prev == sortedIndex[l].first)
                        {
                            newOutFile << ' ' << sortedIndex[l].second;	
                        } 		
                        else
                        {	
                            if(l!=0)
                                newOutFile << endl;
                            long long offset = newOutFile.tellp();
                            newOutFile << sortedIndex[l].second;
                            offsetFile << sortedIndex[l].first << ' ' << offset <<endl;

                        }

                        prev=sortedIndex[l].first;

                    }

                    newOutFile.close();
                    offsetFile.close();
                }

            }
        }
    }
}

bool isRoman(const string& input)
{
    std::regex rgx("^M{0,4}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})$");
    std::smatch match;

    if (regex_match(input, rgx))
        return true;
    else
        return false;
}

int main()
{
    ofstream titleFile("Index/titleFile");
    createIndexFiles();
    loadStopWords();

    unordered_map<int, string>  titleToPage;
    int i,j,m,n;
    ifstream parsedFile("out.txt");
    int page=0;
    string title;
    int title_flag=0,category_flag=0,reference_flag=0,infobox_flag=0,link_flag=0,text_flag=0,pid_flag=0;
    int a[10]={0};
    /*
       0 map <int, int > category;
       1 map <int,int> title;
       2 map <int,int>  infobox;
       3 map <int,int> text;
       4 map <int,int> link;
       5 map <int,int> reference;
       */
    long long count=-1, block=0, doc_words=0;
    ofstream docWords("Index/docWordsFile");

    if(parsedFile.is_open())
    {
        string line;
        while(getline(parsedFile,line))
        {
            //TODO: Unmodularized code

            // PAGE ID
            if(pid_flag && line.find("&&pid&&")!=-1)
            {
                pid_flag=0;
                continue;
            }

            if(line.find("&&pid&&:")!=-1)
            {

                pid_flag=1;
                block++;

                if(block>500)
                {
                    block=0;
                    writeToIndex();
                    index1.clear();
                }

                continue;
            }

            // TITLE
            if(title_flag && line.find("&&title&&")!=-1)
            {

                title_flag=0;
                continue;
            }
            if(line.find("&&title&&:")!=-1)
            {
                count++;
                title_flag=1;
                continue;
            }

            // CATEGORY
            if(category_flag && line.find("&&category&&")!=-1)
            {
                category_flag=0;
                continue;
            }
            if(line.find("&&category&&:")!=-1)
            {
                category_flag=1;
                continue;
            }

            // TEXT
            if(text_flag && line.find("&&text&&")!=-1)
            {
                text_flag=0;
                continue;
            }
            if(line.find("&&text&&:")!=-1)
            {
                text_flag=1;
                continue;
            }

            // LINK
            if(link_flag && line.find("&&link&&")!=-1)
            {
                link_flag=0;
                continue;
            }
            if(line.find("&&link&&:")!=-1)
            {
                link_flag=1;
                continue;
            }

            // REFERENCE
            if(reference_flag && line.find("&&reference&&")!=-1)
            {
                reference_flag=0;
                continue;
            }
            if(line.find("&&reference&&:")!=-1)
            {
                reference_flag=1;
                continue;
            }

            // INFOBOX
            if(infobox_flag && line.find("&&infobox&&")!=-1)
            {
                infobox_flag=0;
                continue;
            }
            if(line.find("&&infobox&&:")!=-1)
            {
                infobox_flag=1;
                continue;
            }

            // Index Data
            stringstream lineStream(line);
            string cell;
            // Store current page title wihout modification
            if(title_flag)
            {
                title = line;
                string fileNum = std::to_string(count);
                fileNum.resize(15,' ');
                title.resize(150,' ');
                if(titleFile.is_open())
                {   
                    titleFile << fileNum << ' ' << title << endl;
                }

                if(docWords.is_open() && count!=0)
                {
                    docWords << count-1 << ' ' << doc_words << endl;
                    doc_words=0;
                }
            }

            while(getline(lineStream,cell,' '))
            {


                string new_cell="";
                if(pid_flag)
                {
                    for(i=0;i<cell.size();i++)
                    {
                        if(cell[i]>='0' && cell[i]<='9')
                        {
                            new_cell+=cell[i];
                        }

                    }
                    if(new_cell.size())
                    {
                        page=stoi(new_cell);
                        pageMap[count]=page;
                    }
                }
                else
                {
                    /*
                       if(isRoman(cell))
                       {
                       cell="rx" + cell + "rx";
                       }
                       */
                    for(i=0;i<cell.size();i++)
                    {
                        if(cell[i]<='Z'&&cell[i]>='A')
                        {
                            cell[i]='a'+(char)(cell[i]-'A');
                        }
                        if(cell[i]>='a' && cell[i]<='z')
                        {
                            new_cell+=cell[i];
                        }
                    }


                    // Check if the word is a stop word
                    if(stopWordsSet.find(new_cell)!=stopWordsSet.end())
                        continue;

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
                    if(title_flag)
                    {
                        insertWord("title",new_cell,count);
                    }
                    else if(category_flag)
                    {
                        insertWord("category",new_cell,count);
                    }
                    else if(text_flag)
                    {
                        insertWord("text",new_cell,count);
                    }
                    else if(infobox_flag)
                    {
                        insertWord("infobox",new_cell,count);
                    }
                    else if(link_flag)
                    {
                        insertWord("link",new_cell,count);
                    }
                    else if(reference_flag)
                    {
                        insertWord("reference",new_cell,count);
                    }
                    //insertWord("all",new_cell,page);
                    words++;
                    doc_words++;


                }

            }
        }
        parsedFile.close();
    }
    if(docWords.is_open() && count!=0)
    {
        docWords << count << ' ' << doc_words << endl;
        doc_words=0;
    }
    titleFile.close();
    docWords.close();
    ofstream newFile("flagFile", std::ios_base::app);

    sortBlocks();



    return 0;



}
