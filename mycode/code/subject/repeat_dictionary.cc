#include <iostream>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include <assert.h>
using namespace std;

template <typename Out>
void split_(const string& s,char delim,Out result)
{
    stringstream ss;
    ss.str(s);
    string item;
    while(getline(ss,item,delim))
    {
        *(result++) = item;
    }
}

vector<string> split(const string &s,char delim)
{
    vector<string> elems;
    split_(s,delim,std::back_inserter(elems));
    return elems;
}

void get_section_in(string &s,string &b)
{
    string line;
    getline(cin,line);
    vector<string> words = split(line,' ');
    s = words[0];
    b = words[2];
}

int main()
{
    string a,value;
    get_section_in(a,value);
    vector<string> strs = split(a,'/');
    assert(strs.size() == 2);

    cout<<strs[0].c_str()<<"  "<<strs[1].c_str()<<endl;
}
