#include <stdio.h> // fprintf, popen, pclose
#include <stdlib.h> // exit, system
#include <unistd.h> // chdir
#include <sys/stat.h> // stat

#include "m-string.h"

void odin_install(const char* install_path)
{
    // 作業ディレクトリを変更する
    const char* working_dir = "/home/doccaico/Downloads";
    if (chdir(working_dir) == -1) {
        fprintf(stderr, "Error: failed to chdir '%s'\n", working_dir);
        exit(1);
    }

    // nightly.jsonを取得する
    system("curl -sSOL https://f001.backblazeb2.com/file/odin-binaries/nightly.json");
    fprintf(stdout, "Download (nightly.json) is done\n");

    // ダウンロードURLを取得する
    FILE* stream;
    const int max_buf_size = 128;
    char buf[max_buf_size];
    stream = popen("cat nightly.json | jq -r '.last_updated'", "r");
    if (stream) {
        fgets(buf, max_buf_size, stream);
        pclose(stream);
    }
    string_t url;
    string_init_set_str(url, buf);
    string_strim(url);
    // "2014-01-01"を取得する
    size_t i = string_search_char(url, 'T');
    memcpy(buf, string_get_cstr(url), i);
    buf[i] = '\0';
    const char* date = buf;
    // ファイル名を設定する
    string_t filename;
    string_init(filename);
    string_printf(filename, "odin-linux-amd64-nightly%%2B%s.tar.gz", date);
    // 最終的なダウンロード先のURLを作成する
    string_t dl_url;
    string_init(dl_url);
    string_printf(dl_url, "https://f001.backblazeb2.com/file/odin-binaries/nightly/%s", string_get_cstr(filename));
    fprintf(stdout, "Download URL => '%s'\n", string_get_cstr(dl_url));

    // ダウンロード (tar.gz)
    string_t cmd;
    string_init(cmd);
    string_printf(cmd, "curl -sSOL %s", string_get_cstr(dl_url));
    system(string_get_cstr(cmd));
    string_reset(cmd);
    fprintf(stdout, "Download (tar.gz) is done\n");

    // 解凍
    string_printf(cmd, "mkdir -p odin && tar xzf %s -C odin --strip-components=1", string_get_cstr(filename));
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
    string_printf(cmd, "mv -f odin %s", install_path);
    system(string_get_cstr(cmd));
    string_reset(cmd);
    fprintf(stdout, "Moved: %s\n", install_path);

    // tar.gzとnightly.jsonを削除する
    string_printf(cmd, "rm %s", string_get_cstr(filename));
    system(string_get_cstr(cmd));
    system("rm nightly.json");
    fprintf(stdout, "Unnecessary files deleted\n");

    // 文字列をfreeする
    string_clear(filename);
    string_clear(dl_url);
    string_clear(cmd);
    string_clear(url);
}
