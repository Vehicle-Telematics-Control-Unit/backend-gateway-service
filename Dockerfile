FROM vsomeip-alpine as builder

COPY build_src build_src

RUN apk add --no-cache curl-dev 

RUN cd build_src; \
    mkdir build; \
    cd build; \
    cmake ..; \
    make

FROM alpine:3.17.2
RUN apk add openssl
COPY --from=builder /build_src/build /build_src/build
COPY --from=builder /build_src/ChainedCertficate.cer /build_src
COPY --from=builder /build_src/private_key.der /build_src
COPY --from=builder /usr/local/lib/libvsomeip3.so.3 /usr/local/lib
COPY --from=builder /usr/local/lib/libvsomeip3-cfg.so.3 /usr/local/lib
COPY --from=builder /usr/local/lib/libvsomeip3-sd.so.3 /usr/local/lib
COPY --from=builder /usr/lib/libstdc++.so.6 /usr/lib
COPY --from=builder /usr/lib/libboost_thread.so.1.58.0 /usr/lib
COPY --from=builder /usr/lib/libboost_system.so.1.58.0 /usr/lib
COPY --from=builder /usr/lib/libgcc_s.so.1 /usr/lib
COPY --from=builder /usr/lib/libboost_filesystem.so.1.58.0 /usr/lib
COPY --from=builder /usr/lib/libcurl.so.4 /usr/lib
COPY --from=builder /usr/lib/libnghttp2.so.14 /usr/lib
COPY --from=builder /usr/lib/libbrotlidec.so.1 /usr/lib
COPY --from=builder /usr/lib/libbrotlicommon.so.1 /usr/lib
COPY --from=builder /lib/libz.so.1 /libs
COPY --from=builder /lib/libcrypto.so.3 /lib
COPY --from=builder /lib/libssl.so.3 /lib

# COPY vsomeip.json /etc/vsomeip/vsomeip.json
# ENV VSOMEIP_CONFIGURATION=/etc/vsomeip/vsomeip.json