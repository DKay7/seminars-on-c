#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Usage: %s <BINARY_FILE_PATH>\n", argv[0]);
        return EXIT_SUCCESS;
    }

    FILE *user_file = fopen(argv[1], "rb"); // Reading in binary mode
    if (!user_file) {
        printf("Error opening `%s` file\n", argv[1]);
        return EXIT_FAILURE;
    }

    // determine size
    size_t current = ftell(user_file);
    fseek(user_file, 0L, SEEK_END);
    size_t size = ftell(user_file);
    fseek(user_file, current, SEEK_SET);

    // allocating space and reading
    unsigned char *binary_buffer = (unsigned char *)calloc(size, sizeof(char));
    fread(binary_buffer, sizeof(char), size, user_file);

    // closing file after reading
    fclose(user_file);
    

    // printing data from user file:
    for (size_t idx = 0; idx < size; ++idx) {
        if (idx % 16 == 0)
            printf("\n%07zX ", idx);

        printf("%x ", binary_buffer[idx]);
    }

    printf("\n");

    free(binary_buffer);

    return 0;
}
