#include <stdio.h> // fprintf, popen, pclose
#include <stdlib.h> // exit, system
#include <unistd.h> // chdir
#include <sys/stat.h> // stat

#include "m-string.h"

void go_install(const char* install_path)
{
    // 作業ディレクトリを変更する
    const char* working_dir = "/home/doccaico/Downloads";
    if (chdir(working_dir) == -1) {
        fprintf(stderr, "Error: failed to chdir '%s'\n", working_dir);
        exit(1);
    }

    // go.jsonを取得する
    system("curl -sSL https://go.dev/dl/?mode=json -o go.json");
    fprintf(stdout, "Download (go.json) is done\n");

    // ダウンロードURLを取得する
    FILE* stream;
    const int max_buf_size = 128;
    char buf[max_buf_size];
    // stream = popen("cat go.json | jq -r '.master.\"x86_64-linux\".tarball'", "r");
    stream = popen("cat go.json | jq -r '.[0].files[] | select(.os == \"windows\" and .arch == \"amd64\" and .kind == \"archive\").filename'", "r");
    if (stream) {
        fgets(buf, max_buf_size, stream);
        pclose(stream);
    }
    string_t filename;
    string_init_set_str(filename, buf);
    string_strim(filename);
    string_t url;
    string_init(url);
    string_printf(url, "https://go.dev/dl/%s", string_get_cstr(filename));
    fprintf(stdout, "Download URL => '%s'\n", string_get_cstr(filename));

    // ダウンロード (zip)
    string_t cmd;
    string_init(cmd);
    string_printf(cmd, "curl -sSOL %s", string_get_cstr(url));
    system(string_get_cstr(cmd));
    string_reset(cmd);
    fprintf(stdout, "Download (zip) is done\n");

    // 解凍
    const char* tarname = string_get_cstr(filename);
    string_printf(cmd, "7za x -aoa %s -bso0 -bsp0", tarname);
    system(string_get_cstr(cmd));
    string_reset(cmd);
    fprintf(stdout, "Extraction is done\n");

    // インストール先のディレクトリが存在したら削除する
    struct stat st;
    if (stat(install_path, &st) == 0) {
        string_printf(cmd, "rm -rf %s", install_path);
        system(string_get_cstr(cmd));
        string_reset(cmd);
        fprintf(stdout, "Removed: %s\n", install_path);
    }

    // 移動
    string_printf(cmd, "mv -f go %s", install_path);
    system(string_get_cstr(cmd));
    string_reset(cmd);
    fprintf(stdout, "Moved: %s\n", install_path);

    // zipとgo.jsonを削除する
    string_printf(cmd, "rm %s", tarname);
    system(string_get_cstr(cmd));
    system("rm go.json");
    fprintf(stdout, "Unnecessary files deleted\n");

    // 文字列をfreeする
    string_clear(filename);
    string_clear(cmd);
    string_clear(url);
}
