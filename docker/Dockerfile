FROM ubuntu:18.04 as base_build

RUN apt-get update && apt-get install -y --no-install-recommends \
        automake \
        build-essential \
        ca-certificates \
        curl \
        libcurl3-dev \
        libfreetype6-dev \
        libpng-dev \
        libtool \
        libzmq3-dev \
        mlocate \
        zip \
        unzip \
        pkg-config \
        cmake \
        software-properties-common \
        && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Set up Bazel
ENV BAZEL_VERSION 3.7.2
WORKDIR /
RUN mkdir /bazel && \
    cd /bazel && \
    curl -H "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36" -fSsL -O https://github.com/bazelbuild/bazel/releases/download/$BAZEL_VERSION/bazel-$BAZEL_VERSION-installer-linux-x86_64.sh && \
    # comment line below, because error: "curl: (7) Failed to connect to raw.githubusercontent.com port 443: Connection refused"
#    curl -H "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36" -fSsL -o /bazel/LICENSE.txt https://raw.githubusercontent.com/bazelbuild/bazel/master/LICENSE && \
    chmod +x bazel-*.sh && \
    ./bazel-$BAZEL_VERSION-installer-linux-x86_64.sh && \
    cd / && \
    rm -f /bazel/bazel-$BAZEL_VERSION-installer-linux-x86_64.sh

FROM base_build as binary_build

WORKDIR /src/workspace

ADD . .

ARG LAIYE_MODEL_ENCRYPT_KEY=""
RUN echo "Building with build options LAIYE_MODEL_ENCRYPT_KEY： ${LAIYE_MODEL_ENCRYPT_KEY}"

RUN bazel --output_user_root=/tmp/build_output \
    build \
    --color=yes \
    --verbose_failures \
    --output_filter=DONT_MATCH_ANYTHING \
    --define=LAIYE_MODEL_ENCRYPT_KEY=${LAIYE_MODEL_ENCRYPT_KEY} \
    cryptfile/... && \
    cp bazel-bin/cryptfile/cryptfile /usr/local/bin/

FROM ubuntu:18.04

COPY --from=binary_build /src/workspace/bazel-bin/cryptfile/cryptfile /usr/local/bin/
COPY --from=binary_build /src/workspace/bazel-bin/cryptfile/libcryptfile.so /opt/lib/

ENTRYPOINT [ "/usr/local/bin/cryptfile" ]