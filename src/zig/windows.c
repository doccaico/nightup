#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // chdir
#include <sys/stat.h> // stat

#include "m-string.h"

void zig_install(const char* install_path)
{
    // 作業ディレクトリを変更する
    const char* working_dir = "/home/doccaico/Downloads";
    if (chdir(working_dir) == -1) {
        fprintf(stderr, "Error: failed to chdir '%s'\n", working_dir);
        exit(1);
    }

    // index.jsonを取得する
    system("curl -sSOL https://ziglang.org/download/index.json");
    fprintf(stdout, "Download (index.json) is done\n");

    // ダウンロードURLを取得する
    FILE* stream;
    const int max_buf_size = 128;
    char buf[max_buf_size];
    stream = popen("cat index.json | jq -r '.master.\"x86_64-linux\".tarball'", "r");
    if (stream) {
        fgets(buf, max_buf_size, stream);
        pclose(stream);
    }
    string_t url;
    string_init_set_str(url, buf);
    string_strim(url);
    fprintf(stdout, "Download URL => '%s'\n", string_get_cstr(url));

    // ダウンロード (tar.xz)
    string_t cmd;
    string_init(cmd);
    string_printf(cmd, "curl -sSOL %s", string_get_cstr(url));
    system(string_get_cstr(cmd));
    string_reset(cmd);
    fprintf(stdout, "Download (tar.xz) is done\n");

    // 解凍
    size_t i = string_search_rchar(url, '/');
    const char* tarname = &string_get_cstr(url)[i + 1];
    string_printf(cmd, "tar xf %s", tarname);
    system(string_get_cstr(cmd));
    string_reset(cmd);
    fprintf(stdout, "Extraction is done\n");

    // インストール先のディレクトリが存在したら削除する
    struct stat st = {0};
    if (stat(install_path, &st) != -1) {
        string_printf(cmd, "rm -rf %s", install_path);
        system(string_get_cstr(cmd));
        string_reset(cmd);
        fprintf(stdout, "Removed: %s\n", install_path);
    }

    // 移動
    string_t src;
    string_init_set_str(src, tarname);
    i  = string_search_str(src, ".tar.xz");
    memcpy(buf, string_get_cstr(src), i);
    buf[i] = '\0';
    string_printf(cmd, "mv -f %s %s", buf, install_path);
    system(string_get_cstr(cmd));
    string_reset(cmd);
    fprintf(stdout, "Moved: %s\n", install_path);

    // tar.xzとindex.jsonを削除する
    string_printf(cmd, "rm %s", tarname);
    system(string_get_cstr(cmd));
    string_reset(cmd);
    string_printf(cmd, "rm %s", "index.json");
    system(string_get_cstr(cmd));
    fprintf(stdout, "Unnecessary files deleted\n");

    // 文字列をfreeする
    string_clear(src);
    string_clear(cmd);
    string_clear(url);
}
