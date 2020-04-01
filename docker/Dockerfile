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
ENV BAZEL_VERSION 2.0.0
WORKDIR /
RUN mkdir /bazel && \
    cd /bazel && \
    curl -H "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36" -fSsL -O https://github.com/bazelbuild/bazel/releases/download/$BAZEL_VERSION/bazel-$BAZEL_VERSION-installer-linux-x86_64.sh && \
    curl -H "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36" -fSsL -o /bazel/LICENSE.txt https://raw.githubusercontent.com/bazelbuild/bazel/master/LICENSE && \
    chmod +x bazel-*.sh && \
    ./bazel-$BAZEL_VERSION-installer-linux-x86_64.sh && \
    cd / && \
    rm -f /bazel/bazel-$BAZEL_VERSION-installer-linux-x86_64.sh

FROM base_build as binary_build

WORKDIR /src/workspace

ADD . .

RUN bazel --output_user_root=/tmp/build_output \
    build \
    --color=yes \
    --verbose_failures \
    --output_filter=DONT_MATCH_ANYTHING \
    //cryptfile && \
    cp bazel-bin/cryptfile/cryptfile /usr/local/bin/

FROM ubuntu:18.04

COPY --from=binary_build /src/workspace/bazel-bin/cryptfile/cryptfile /usr/local/bin/

ENTRYPOINT [ "/usr/local/bin/cryptfile" ]