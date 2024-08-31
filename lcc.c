#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For unlink function
#include <sys/stat.h>  // For file existence checking

// Function to print usage information
void print_usage(const char *progname) {
    printf("Usage: %s [options] source_file -o output_file\n", progname);
    printf("Options:\n");
    printf("  --static          Link statically with musl libc\n");
    printf("  --help            Display this help message\n");
    printf("  --version         Display version information\n");
}

// Function to print version information
void print_version() {
    printf("LCC - Lightweight C Compiler\n");
    printf("Version 1.0\n");
}

// Function to check if a file exists
int file_exists(const char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

// Function to build a static binary using musl
void build_static_binary(const char *source_file, const char *output_file) {
    char cmd[1024];

    // Check if the source file exists
    if (!file_exists(source_file)) {
        fprintf(stderr, "Error: Source file '%s' does not exist.\n", source_file);
        exit(1);
    }

    // Compile source file to object file with musl libc
    snprintf(cmd, sizeof(cmd), "musl-gcc -c %s -o temp.o", source_file);
    if (system(cmd) != 0) {
        fprintf(stderr, "Error: Compilation failed.\n");
        exit(1);
    }

    // Link the object file statically
    snprintf(cmd, sizeof(cmd), "musl-gcc -static -o %s temp.o", output_file);
    if (system(cmd) != 0) {
        fprintf(stderr, "Error: Linking failed.\n");
        unlink("temp.o");
        exit(1);
    }

    // Clean up temporary object file
    unlink("temp.o");
}

int main(int argc, char **argv) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    int static_link = 0;
    const char *source_file = NULL;
    const char *output_file = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "--version") == 0) {
            print_version();
            return 0;
        } else if (strcmp(argv[i], "--static") == 0) {
            static_link = 1;
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else {
            source_file = argv[i];
        }
    }

    if (!source_file || !output_file) {
        fprintf(stderr, "Error: Invalid arguments. Please provide source and output filenames.\n");
        print_usage(argv[0]);
        return 1;
    }

    if (static_link) {
        build_static_binary(source_file, output_file);
        printf("Compilation successful, output: %s\n", output_file);
    } else {
        fprintf(stderr, "Error: Static linking is required.\n");
        return 1;
    }

    return 0;
}

