# nightup
My updater tool written in C.

## How to use
```sh
$ nightup [lang]
```

## Supported languages
- zig
- odin
- v

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
