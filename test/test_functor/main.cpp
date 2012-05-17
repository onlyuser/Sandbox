#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

typedef std::pair<int, std::string> PAIR_T;

struct LessThan
{
    bool operator()(const PAIR_T& x, const PAIR_T& y) const
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
    std::ifstream myFile(argv[1]);
    if(!myFile.is_open())
        return 1;
    std::vector<PAIR_T> myVec;
    while(!myFile.eof())
    {
        char buf[256];
        myFile.getline(buf, 256);
        if(myFile.eof())
            break;
        std::cout << "line: " << buf << std::endl;
        std::stringstream ss;
        ss << buf;
        int x;
        ss >> x;
        std::string s;
        ss >> s;
        myVec.push_back(PAIR_T(x, s));
    }
    myFile.close();
    std::sort(myVec.begin(), myVec.end(), LessThan());
    for(std::vector<PAIR_T>::iterator p = myVec.begin(); p != myVec.end(); ++p)
    {
        int x = (*p).first;
        std::cout << "int " << x << std::endl;
        std::string s = (*p).second;
        std::cout << "string " << s << std::endl;
    }
    return 0;
}
