load("@rules_foreign_cc//tools/build_defs:make.bzl", "make")
load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")
load("@rules_cc//cc:defs.bzl", "cc_binary")

make(
    name = "cryptopp",
    lib_source = "@cryptopp//:all",
    make_commands = [
        "export N_CORES=`python -c 'import multiprocessing as mp; print(mp.cpu_count())'`",
        "make -j$N_CORES -k -C $EXT_BUILD_ROOT/external/cryptopp",
        "make -C $EXT_BUILD_ROOT/external/cryptopp install PREFIX=$INSTALLDIR",
    ],
    static_libraries = ["libcryptopp.a"],
)

cmake_external(
    name = "cxxopts",
    lib_source = "@cxxopts//:all",
    make_commands = [
        "export N_CORES=`python -c 'import multiprocessing as mp; print(mp.cpu_count())'`",
        "make -j$N_CORES",
        "make install",
    ],
    headers_only = True,
)

cc_binary(
    name = "cryptfile",
    srcs = [
        "cryptfile.cc",
        "functions.h",
        "fileutils.h",
        "functions-header.h",
    ],
    deps = [
        ":cryptopp",
        ":cxxopts",
        # "@boost//:algorithm",
    ],
    includes = ["cryptopp/include","cxxopts/include"],
)
