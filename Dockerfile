# if you dont access use this 
# FROM vsomeip_build:v0 as builder
FROM registry.digitalocean.com/vehicle-plus/tcu_builder_packs:v8 as builder

RUN apk update && apk add --no-cache --virtual .second_build_dependency \
    binutils cmake curl gcc g++ git libtool make tar build-base linux-headers
RUN apk add --no-cache curl-dev 

COPY build_src build_src
RUN cd build_src; \
    mkdir build; \
    cd build; \
    cmake ..; \
    make

FROM alpine:3.17.2

RUN mkdir /composefiles
RUN apk add openssl --no-cache
COPY --from=builder /build_src/build /build_src/build
COPY --from=builder /build_src/ChainedCertficate.cer /build_src
COPY --from=builder /build_src/private_key.der /build_src
COPY --from=builder /usr/local/lib/libvsomeip3.so.3 /usr/local/lib
COPY --from=builder /usr/local/lib/libvsomeip3-cfg.so.3 /usr/local/lib
COPY --from=builder /usr/local/lib/libvsomeip3-sd.so.3 /usr/local/lib
COPY --from=builder /usr/lib/libstdc++.so.6 /usr/lib
COPY --from=builder /usr/lib/libboost_thread.so.1.63.0 /usr/lib
COPY --from=builder /usr/lib/libboost_system.so.1.63.0 /usr/lib
COPY --from=builder /usr/lib/libgcc_s.so.1 /usr/lib
COPY --from=builder /usr/lib/libboost_filesystem.so.1.63.0 /usr/lib
COPY --from=builder /usr/lib/libcurl.so.4 /usr/lib
COPY --from=builder /usr/lib/libnghttp2.so.14 /usr/lib
COPY --from=builder /usr/lib/libbrotlidec.so.1 /usr/lib
COPY --from=builder /usr/lib/libbrotlicommon.so.1 /usr/lib
COPY --from=builder /lib/libz.so.1 /libs
COPY --from=builder /lib/libcrypto.so.3 /lib
COPY --from=builder /lib/libssl.so.3 /lib

WORKDIR /build_src/build

ENTRYPOINT [ "./backend_gateway" ]
# COPY vsomeip.json /etc/vsomeip/vsomeip.json
# ENV VSOMEIP_CONFIGURATION=/etc/vsomeip/vsomeip.json