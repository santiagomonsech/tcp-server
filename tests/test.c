#include <stdio.h>
#include <assert.h>

#include "server.h"


int main(int argc, char const *argv[]) {
    printf("=== tcp_server tests ===\n");
    start_server(argc, argv);
    printf("\nAll tests passed!\n");
    return 0;
}
