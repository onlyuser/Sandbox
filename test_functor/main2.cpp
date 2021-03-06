#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

typedef std::pair<int, std::string> pair_t;

struct FindKey : std::unary_function<pair_t, bool>
{
    pair_t::first_type m_key;

    FindKey(pair_t::first_type key)
    {
        m_key = key;
    }

    bool operator()(const pair_t& x) const
    {
        return x.first == m_key;
    }
};

int main(int argc, char** argv)
{
    if(argc != 2) {
        std::cout << "expect 1 arg" << std::endl;
        return 1;
    }
    std::cout << "input:" << std::endl;
    std::ifstream myFile(argv[1]);
    if(!myFile.is_open()) {
        return 1;
    }
    std::vector<pair_t> myVec;
    while(!myFile.eof()) {
        char buf[256];
        myFile.getline(buf, 256);
        if(myFile.eof()) {
            break;
        }
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
    std::cout << "output:" << std::endl;
    for(std::vector<pair_t>::iterator p = myVec.begin(); p != myVec.end(); p++) {
        std::vector<pair_t>::iterator q = std::find_if(myVec.begin(), myVec.end(), FindKey((*p).first));
        if(q != myVec.end()) {
            std::cout << (*q).first << ' ' << (*q).second << std::endl;
        }
    }
    return 0;
}
