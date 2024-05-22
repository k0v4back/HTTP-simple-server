FROM gcc:latest as build

WORKDIR /http_server

RUN apt-get update && \
apt-get install -y \
    cmake

COPY src/ ./src/
COPY libs/ ./libs/
COPY CMakeLists.txt .

WORKDIR /http_server/build

RUN cmake ../ && \
    cmake --build .

FROM ubuntu:latest

WORKDIR /http_server

COPY --from=build /http_server/build/http_server .

EXPOSE 8080

ENTRYPOINT [ "./http_server" ]