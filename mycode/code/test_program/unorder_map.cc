#include <iostream>
#include <string>
#include <unordered_map>

typedef std::unordered_map<std::string,std::string>  stringmap;

void merge(stringmap & a,const stringmap & b)
{
    a.insert(b.begin(),b.end());
}

int main()
{
    std::unordered_map<std::string,int> mymap = {{"Mars",3000},{"Saturn",6000},{"Jasper",7000}};
    mymap.at("Mars") = 3396;
    mymap.at("Saturn") += 272;
    mymap.at("Jasper") = mymap.at("Saturn") + 9638;

    for(auto &x : mymap)
    {
        std::cout<<x.first<<": "<<x.second<<std::endl;
    }
    std::cout<<"mymap.size() is "<<mymap.size()<<std::endl;


    std::unordered_map<std::string,std::string> mymap2 = {{"Chinese","Beijing"},{"korean","Soar"},{"Australia","Canberra"},{"U.S.","Washington"},{"France","Paris"}};
    std::cout<<"mymap2 contains"<<std::endl;

    for(auto it = mymap2.begin();it != mymap2.end();++it)
    {
        std::cout<<" "<<it->first<<":"<<it->second<<std::endl;
    }
    std::cout<<std::endl;

    std::cout<<"mymap2 buckets contain:\n";
    for(unsigned int i = 0; i < mymap2.bucket_count();++i)
    {
        std::cout<<"bucket #"<<i<<"contains:";
        for(auto local_it = mymap2.begin(i);local_it != mymap2.end(i);++local_it)
        {
            std::cout<<" "<<local_it->first<<":"<<local_it->second;
        }
        std::cout<<std::endl;
    }


//buckets
    std::unordered_map<std::string,std::string> mymap3 = {
        {"us","United States"},
        {"uk","United Kingdom"},
        {"fr","France"},
        {"de","Germany"}
    };

    for(auto & x : mymap3){
        std::cout<<"Elem ["<<x.first<<":"<<x.second<<"]";
        std::cout<<"is in bucket #"<<mymap3.bucket(x.first)<<std::endl;
    }
///find
    std::unordered_map<std::string,double> mymap4 = {
        {"Burger",2.99},
        {"Fries",1.99},
        {"Soda",1.50}
    };

    // for(auto &x :{"Burger","Pizza","Salad","Soda"})
    // {
    //     if(mymap4.count(x) > 0)
    //     {
    //         std::cout<<"mymap4 has"<<x<<std::endl;
    //     }
    //     else
    //     {
    //         std::cout<<"mymap4 has no"<<x<<std::endl;
    //     }
    // }

    std::string aa = "Burger";
    std::unordered_map<std::string,double>::const_iterator got = mymap4.find(aa);
    if(got == mymap4.end())
    {
        std::cout<<"not found"<<std::endl;
    }
    else
    {
        std::cout<<got->first<<"is "<<got->second<<std::endl;
    }



    return 0;
}