// REFERENCE:
// "C++ - Passing a C-style array with size information to a function"
// https://solarianprogrammer.com/2016/11/28/cpp-passing-c-style-array-with-size-information-to-function/?utm_source=feedburner&utm_medium=feed&utm_campaign=Feed%3A+SolarianProgrammer+%28Solarian+Programmer%29

#include <iostream>

// The array is passed as a reference to the function, keeping the size information (only for C++)
template<typename T, int N>
void func(T (&a)[N]) {
#if 1
    for(int i = 0; i < N; i++) {
        std::cout << a[i] << std::endl;
    }
#else
    // Here N stores the correct size information for the array
    // Basically N is equal to sizeof(my_array)/sizeof(my_array[0]) so the next code will work as expected:
    for(int i = 0; i < N; ++i) {
        a[i] = a[i] + i;
    }
#endif
}

int main() {
    // C-style array
    int my_array[5] = {1, 2, 3, 4, 5};

    // Pass the array to a function
    func(my_array);

    return 0;
}
