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
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ../
cmake --build . --config Debug
```

### Release:
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ../
cmake --build . --config Release
```

## Run

```
./http_server
```

And enter the address in the web browser: `http://127.0.0.1:7373`