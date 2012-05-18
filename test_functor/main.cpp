#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

typedef std::pair<int, std::string> pair_t;

struct LessThan
{
    bool operator()(const pair_t& x, const pair_t& y) const
    {
        return x.first > y.first;
    }
};

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cout << "expect 1 arg" << std::endl;
        return 1;
    }
    std::cout << "before sort:" << std::endl;
    std::ifstream myFile(argv[1]);
    if(!myFile.is_open())
        return 1;
    std::vector<pair_t> myVec;
    while(!myFile.eof())
    {
        char buf[256];
        myFile.getline(buf, 256);
        if(myFile.eof())
            break;
        std::cout << buf << std::endl;
        std::stringstream ss;
        ss << buf;
        int key;
        std::string value;
        ss >> key >> value;
        myVec.push_back(pair_t(key, value));
    }
    myFile.close();
    std::cout << std::endl;
    std::cout << "after sort:" << std::endl;
    std::sort(myVec.begin(), myVec.end(), LessThan());
    for(std::vector<pair_t>::iterator p = myVec.begin(); p != myVec.end(); ++p)
        std::cout << (*p).first << ' ' << (*p).second << std::endl;
    return 0;
}
