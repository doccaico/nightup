#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define INI_IMPLEMENTATION
#include "ini.h"

#ifdef _WIN32
    #include "zig/windows.c"
    #include "odin/windows.c"
    #include "v/windows.c"
#elif __linux__
    #include "zig/linux.c"
    #include "odin/linux.c"
    #include "v/linux.c"
#else
    #error Unsupported Os.
#endif

#define NIGHTUP_INI ".nightup"

void help_and_exit(void)
{
    fprintf(stderr, "Usage:\n nightup LANG\n");
    fprintf(stderr, "\nSupported languages:\n");
    fprintf(stderr, "\tzig, odin, v\n");
    exit(1);
}

char* get_install_path(const char* home_dir, const char* lang)
{
    FILE* fp = fopen(home_dir, "r");
    fseek(fp, 0, SEEK_END);
    long int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* data = malloc( size + 1);
    fread(data, 1, size, fp);
    data[size] = '\0';
    fclose(fp);

    ini_t* ini = ini_load(data, NULL);
    free(data);

#ifdef _WIN32
    char *section_name = "Windows";
#elif __linux__
    char *section_name = "Linux";
#else
    #error Unsupported Os.
#endif

    int section = ini_find_section(ini, section_name, 0);
    int index = ini_find_property(ini, section, lang, 0);
    const char* install_path = ini_property_value(ini, section, index);

    size_t len = strlen(install_path) + 1;
    char* result = malloc(len);
    memcpy(result, install_path, len);
    result[len - 1] = '\0';

    ini_destroy(ini);

    return result;
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        help_and_exit();
    }
    char home_dir[64];

#ifdef _WIN32
    sprintf(home_dir, "%s\\%s", getenv("USERPROFILE"), NIGHTUP_INI);
#elif __linux__
    sprintf(home_dir, "%s/%s", getenv("HOME"), NIGHTUP_INI);
#else
    #error Unsupported Os.
#endif

    char* install_path;

    if (strcmp(argv[1], "zig") == 0) {
        install_path = get_install_path(home_dir, "zig");
        zig_install(install_path);
    } else if (strcmp(argv[1], "odin") == 0) {
        install_path = get_install_path(home_dir, "odin");
        odin_install(install_path);
    } else if (strcmp(argv[1], "v") == 0) {
        install_path = get_install_path(home_dir, "v");
        v_install(install_path);
    } else {
        help_and_exit();
    }

    free(install_path);

    return 0;
}
