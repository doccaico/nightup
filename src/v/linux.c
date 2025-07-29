#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // chdir
#include <sys/stat.h> // stat

#include "m-string.h"

void v_install(const char* install_path)
{
    // 作業ディレクトリを変更する
    const char* working_dir = "/home/doccaico/Downloads";
    if (chdir(working_dir) == -1) {
        fprintf(stderr, "Error: failed to chdir '%s'\n", working_dir);
        exit(1);
    }

    // latestを取得する
    system("curl -sSOL https://api.github.com/repos/vlang/v/releases/latest");
    fprintf(stdout, "Download (latest) is done\n");

    // ダウンロードURLを取得する
    FILE* stream;
    const int max_buf_size = 128;
    char buf[max_buf_size];
    stream = popen("cat latest | jq -r '.assets[0].browser_download_url'", "r");
    if (stream) {
        fgets(buf, max_buf_size, stream);
        pclose(stream);
    }
    string_t url;
    string_init_set_str(url, buf);
    string_strim(url);
    fprintf(stdout, "Download URL => '%s'\n", string_get_cstr(url));

    // ダウンロード (zip)
    string_t cmd;
    string_init(cmd);
    string_printf(cmd, "curl -sSOL %s", string_get_cstr(url));
    system(string_get_cstr(cmd));
    string_reset(cmd);
    fprintf(stdout, "Download (zip) is done\n");

    // 解凍
    system("7za x -aoa v_linux.zip -bso0 -bsp0");
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
    string_printf(cmd, "mv -f v %s", install_path);
    system(string_get_cstr(cmd));
    string_reset(cmd);
    fprintf(stdout, "Moved: %s\n", install_path);

    // zipとlatestを削除する
    system("rm v_linux.zip");
    system("rm latest");
    fprintf(stdout, "Unnecessary files deleted\n");

    // 文字列をfreeする
    string_clear(cmd);
    string_clear(url);
}
