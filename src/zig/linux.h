// #ifndef A_H_
// #define A_H_

// #define _POSIX_C_SOURCE 200809L
// #define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "m-string.h"

void zig_install(const char* install_path)
{
    const char* working_dir = "/home/doccaico/Downloads";

    if (chdir(working_dir) == -1) {
        fprintf(stderr, "Error: not found '%s'\n", working_dir);
        exit(1);
    }

    // char pathname[256];
    //
    // // memset(pathname, '\0', PATHNAME_SIZE);
    // getcwd(pathname, 256);
    // fprintf(stdout,"現在のファイルパス:%s\n", pathname);
    FILE* stream;
    const int max_buffer = 128;
    char buffer[max_buffer];

    // string_t data;
    // string_init(data);

    stream = popen("cat index.json | jq -r '.master.\"x86_64-linux\".tarball'", "r");
    if (stream) {
        fgets(buffer, max_buffer, stream);
        pclose(stream);
    }
    printf("%s\n", buffer);
}

// #endif // A_H_

    // if (stream) {
    //     while (!feof(stream)) {
    //         if (fgets(buffer, max_buffer, stream) != NULL) {
    //             // data.append(buffer);
    //             string_cat_str(data, buffer);
    //         }
    //     }
    //     pclose(stream);
    // }
