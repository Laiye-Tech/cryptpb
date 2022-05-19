load("@rules_foreign_cc//tools/build_defs:make.bzl", "make")
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")
load("@rules_cc//cc:defs.bzl", "cc_binary")

make(
    name = "cryptopp",
    lib_source = "@cryptopp//:all",
    make_commands = [
        "make -j$(nproc) -k -C $EXT_BUILD_ROOT/external/cryptopp",
        "make -C $EXT_BUILD_ROOT/external/cryptopp install PREFIX=$INSTALLDIR",
    ],
    static_libraries = ["libcryptopp.a"],
)

cmake_external(
    name = "cxxopts",
    headers_only = True,
    lib_source = "@cxxopts//:all",
    make_commands = [
        "make -j$(nproc)",
        "make install",
    ],
)

cc_binary(
    name = "cryptfile",
    srcs = [
        "cryptfile.cc",
        "fileutils.h",
        "functions.h",
        "functions-header.h",
    ],
    includes = [
        "cryptopp/include",
        "cxxopts/include",
    ],
    deps = [
        ":cryptopp",
        ":cxxopts",
    ],
)

cc_binary(
    name = "libcryptfile.so",
    srcs = [
        "cryptfile.cc",
        "fileutils.h",
        "functions.h",
        "functions-header.h",
    ],
    includes = [
        "cryptopp/include",
        "cxxopts/include",
    ],
    linkshared = True,
    deps = [
        ":cryptopp",
        ":cxxopts",
    ],
)

cc_test(
    name = "libcrypt_test",
    size = "small",
    srcs = ["libcrypt_test.cc"],
    deps = [
        ":cryptopp",
        "@com_google_googletest//:gtest_main",
    ],
)
