#include <vector>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <stdlib.h>

class input_parser
{
public:
    void register_unit_callback(std::function<void (std::string,std::string)> cb)
    {
        unit_callback_ = cb;
    }

    void register_input_callback(std::function<void (int)>  cb)
    {
        input_callback_ = cb;
    }
    void register_connection_callback(std::function<void (std::string, std::string, std::string, std::string, std::string, std::string)> cb)
    {
        connection_callback_ = cb;
    }

    void register_value_callback(std::function<void (std::string, std::string, std::string)> cb)
    {
        value_callback_ = cb;
    }

    void process()
    {
        int units_num = get_section_title_line();

        std::cout<<"the units_num is "<<units_num<<std::endl;
        for(int i = 0; i < units_num; ++i)
        {
            std::string name,type;
            get_section_internal_line(name,type);
            if(unit_callback_)
                unit_callback_(name,type);
        }

        //process the INPUTS line
        int inputs_num = get_section_title_line();
        if(input_callback_)
            input_callback_(inputs_num);

        int connections_num = get_section_title_line();

        for(int i = 0; i < connections_num; ++i) {
            std::string from, to;
            get_section_internal_line(from, to);
            assert(from != "result");
            std::vector<std::string> from_strs = split(from, '/');
            assert(from_strs.size() >= 2);
            std::vector<std::string> to_strs = split(to, '/');
            assert(from_strs.size() >= 1);

            //the first parameter is from_strs[0]
            std::string second = from_strs[0] == "input" ? "" : from_strs[1];
            std::string third = from_strs[0] == "input" ? from_strs[1] : from_strs[2];

            std::string fifth = "";
            std::string sixth = "";
            if (to_strs[0] == "input") {
                sixth = to_strs[1];
            } else if (to_strs[0] != "result") {
                fifth = to_strs[1];
                sixth = to_strs[2];
            }

            if (connection_callback_)
                connection_callback_(from_strs[0], second, third, to_strs[0], fifth, sixth);
        }
        int values_num = get_section_title_line();
        for(int i = 0;i < values_num;++i)
        {
            std::string input,value;
            get_section_internal_line(input,value);
            std::vector<std::string> strs = split(input,'/');
            assert(strs.size() == 2);
            if(value_callback_)
                value_callback_(strs[0],strs[1],value);
        }


    }

private:
    template <typename Out>
    void split(const std::string & s,char delim,Out result)
    {
        std::stringstream ss;
        ss.str(s);
        std::string item;
        while(std::getline(ss,item,delim))
        {
            *(result++) = item;
        }
    }

    std::vector<std::string> split(const std::string &s,char delim)
    {
        std::vector<std::string> elems;
        split(s,delim,std::back_inserter(elems));
        return elems;
    }

    int get_section_title_line()
    {
        std::string line;
        std::getline(std::cin,line);
        std::vector<std::string> words = split(line,' ');
        assert(words.size() == 2);
        return atoi(words[1].c_str());
    }

    void get_section_internal_line(std::string& a,std::string &b)
    {
        std::string line;
        std::getline(std::cin,line);
        std::vector<std::string> words = split(line,' ');
        assert(words.size() == 3);
        a = words[0];
        b = words[2];
    }

private:
    std::function<void (std::string, std::string)> unit_callback_;
    std::function<void (int n)> input_callback_;

    std::function<void (std::string,std::string,std::string,std::string,std::string,std::string)>  connection_callback_;
    std::function<void (std::string,std::string,std::string)> value_callback_;

};


struct unit
{
    unit():unit_name(""),unit_type(""),input_num(0) {}
    std::string unit_name;
    std::string unit_type;
    int      input_num;
};

static unit o_unit;

void solve_unit(std::string a,std::string b)
{
    o_unit.unit_name = a;
    o_unit.unit_type = b;
}

void solve_input(int a)
{
    o_unit.input_num = a;
}

void solve_connection(std::string a,std::string b,std::string c,std::string d,std::string e,std::string f)
{
    
}

void solve_value(std::string a,std::string b,std::string c)
{
    int value = atoi(c.c_str());
    int index = atoi(b.c_str());
    
}

int main()
{
    input_parser  o_in;
    o_in.register_unit_callback(solve_unit);
    o_in.register_input_callback(solve_input);
    o_in.register_connection_callback(solve_connection);
    o_in.register_value_callback(solve_value);
    o_in.process(value);
    return 0;
}
