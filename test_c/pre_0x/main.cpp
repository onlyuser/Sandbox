#include "stdio.h"

int main(int argc, char** argv)
{
    int i = 0;
    printf("%d %d %d\n", ++i, ++i, ++i);

    return 0;
}

// .LBB2:
//     .loc 1 5 0
//     movl    $0, -4(%rbp)
//     .loc 1 6 0
//     addl    $1, -4(%rbp)
//     addl    $1, -4(%rbp)
//     addl    $1, -4(%rbp)
//     movl    -4(%rbp), %ecx
//     movl    -4(%rbp), %edx
//     movl    -4(%rbp), %eax
//     movl    %eax, %esi
//     movl    $.LC0, %edi
//     movl    $0, %eax
//     call    printf
//     .loc 1 8 0
//     movl    $0, %eax
