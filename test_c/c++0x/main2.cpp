#include <iostream>

int main(int argc, char** argv)
{
    if(argc != 2) {
        std::cout << "Error: Need 1 arg!" << std::endl;
        return 1;
    }
    int x = atoi(argv[1]);
    std::cout << "The factorial of " << x << " is " << [](int n) -> float
                                                         {
                                                             float result = 1;
                                                             for(int i = 1; i <= n; i++) {
                                                                 result *= i;
                                                             }
                                                             return result;
                                                         }(x) << std::endl;
    return 0;
}
