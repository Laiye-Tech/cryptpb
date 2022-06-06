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
        "@github_cxxopts//:cxxopts",
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
    defines = [
        "LAIYE_MODEL_ENCRYPT_KEY=\\\"$(LAIYE_MODEL_ENCRYPT_KEY)\\\"",
    ],
    includes = [
        "cryptopp/include",
        "cxxopts/include",
    ],
    linkshared = True,
    deps = [
        ":cryptopp",
        "@github_cxxopts//:cxxopts",
    ],
)

cc_test(
    name = "libcrypt_test",
    size = "small",
    srcs = ["libcrypt_test.cc"],
    linkopts = [
        "-ldl",
    ],
    deps = [
        ":cryptopp",
        "@com_google_googletest//:gtest_main",
    ],
)
