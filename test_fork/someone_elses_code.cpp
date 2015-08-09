#include <signal.h> // raise
#include <stdio.h> // printf
#include <unistd.h> // sleep

static bool corrupted_program_state = false;

void enter_corrupted_program_state() {
    corrupted_program_state = true;
}

bool someone_elses_code_with_undesired_side_effect(int *result, int a, int b) {
    if(!result) {
        return false;
    }
    *result = a + b; // someone_elses result
    corrupted_program_state = true; // undesired side effect (yeah, too bad it happens)
    sleep(3); // simulate a delay
    return true;
}

void someone_elses_code_manifesting_undesired_side_effect() {
    printf("Success!\n"); // someone_elses result
    if(corrupted_program_state) {
        raise(SIGSEGV); // manifest undesired side effect
    }
}
