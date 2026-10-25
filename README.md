# HTTP-simple-server

A simple http server written for the purpose of learning C++ and working with the network

## Clone

```bash
git clone
git submodule update --init --recursive
```

## Compile

### Debug:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j$(nproc)
```

### Release:
```bash
mkdir build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

## Run

```
./build/bin/http_server
```

And enter the address in the web browser: `http://127.0.0.1:7373`