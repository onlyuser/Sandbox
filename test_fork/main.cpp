// REFERENCE:
// "How to get file descriptor of buffer in memory?"
// http://stackoverflow.com/questions/1558772/how-to-get-file-descriptor-of-buffer-in-memory

// CONCEPTS:
// * sandbox code that corrupts program state
// * convert in-memory buffer to FILE*
// * convert file descriptor to FILE*
// * pipe std-out from one process to std-in of another process
// * type traits

#include <stdio.h> // fdopen, fprintf, fscanf
#include <unistd.h> // close, fork, pipe
#include <iostream> // std::cout, std::endl
#include <stdlib.h> // atoi, exit
#include <signal.h> // raise, SIGSEGV

bool corrupted_program_state = false;

template<class T>
T do_calculation(T a, T b) {
    corrupted_program_state = true;
    return a + b;
}

template<class T>
void report_result(T x) {
    std::cout << "Result is: " << x << std::endl;
    if(corrupted_program_state) {
        raise(SIGSEGV);
    }
}

template<class T> struct pod_traits {};
template<> struct pod_traits<int>   { static char ptn[]; static int*   ptr_for_read(int& x)   { return &x; } };
template<> struct pod_traits<float> { static char ptn[]; static float* ptr_for_read(float& x) { return &x; } };
template<> struct pod_traits<char*> { static char ptn[]; static char*  ptr_for_read(char*& x) { return x; } };
char pod_traits<int>::ptn[]   = "%d";
char pod_traits<float>::ptn[] = "%f";
char pod_traits<char*>::ptn[] = "%s";

#define ENTER_SANDBOX \
    {                                       \
        int p[2];                           \
        pipe(p);                            \
        pid_t child_pid = fork();           \
        if(!child_pid) {                    \
            close(p[0]);                    \
            FILE* file = fdopen(p[1], "w"); \
            {

#define LEAVE_SANDBOX(x) \
            }                                             \
            fprintf(file, pod_traits<typeof(x)>::ptn, x); \
            exit(0);                                      \
        } else {                                          \
            close(p[1]);                                  \
            FILE* file = fdopen(p[0], "r");               \
            fscanf(file, pod_traits<typeof(x)>::ptn, pod_traits<typeof(x)>::ptr_for_read(x)); \
        }                                                 \
    }

int main(int argc, char** argv)
{
    if(argc != 3) {
        std::cout << "Error: Expect 2 arguments!" << std::endl;
        return 1;
    }

    int a_int = atoi(argv[1]);
    int b_int = atoi(argv[2]);

    float a_float = atof(argv[1]);
    float b_float = atof(argv[2]);

    //char* a_string = argv[1];
    //char* b_string = argv[2];

    // int result
    {
        int result = 0;
        ENTER_SANDBOX;
        result = do_calculation(a_int, b_int); // execute code that corrupts program state
        LEAVE_SANDBOX(result);
        report_result(result); // manifest crash if in corrupted program state
    }

    // float result
    {
        float result = 0;
        ENTER_SANDBOX;
        result = do_calculation(a_float, b_float); // execute code that corrupts program state
        LEAVE_SANDBOX(result);
        report_result(result); // manifest crash if in corrupted program state
    }

    // string result
    {
        char* result = new char[80];
        ENTER_SANDBOX;
        sprintf(result, "hello_world");
        corrupted_program_state = true; // execute code that corrupts program state
        LEAVE_SANDBOX(result);
        report_result(result); // manifest crash if in corrupted program state
    }

    return 0;
}
