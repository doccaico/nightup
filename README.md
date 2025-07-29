# nightup
My updater tool written in C.

## Dependencies
- [jq](https://github.com/jqlang/jq)
- 7zip (7za)
- curl

## How to use
```sh
$ nightup [lang]
```

## Supported languages
- [zig](https://github.com/ziglang/zig)
- [odin](https://github.com/odin-lang/Odin)
- [v](https://github.com/vlang/v)
- [go](https://github.com/golang/go)

## Build
```sh
# Windows Native (w64devkit)
$ ./build.bat release

# WSL2 (or Linux Native?)
$ ./build.bash release
```

## Config(INI file)
```ini
; ~/.nightup
[Windows]
zig=C:\Langs\zig
odin=C:\Langs\odin
v=C:\Langs\v

[Linux]
zig=/home/doccaico/langs/zig
odin=/home/doccaico/langs/odin
v=/home/doccaico/langs/v
```
