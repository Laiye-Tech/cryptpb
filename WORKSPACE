workspace(name = "cryptfile")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "rules_foreign_cc",
    sha256 = "c2cdcf55ffaf49366725639e45dedd449b8c3fe22b54e31625eb80ce3a240f1e",
    strip_prefix = "rules_foreign_cc-0.1.0",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/0.1.0.zip",
)

load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies([
])

http_archive(
    name = "bazel_skylib",
    sha256 = "97e70364e9249702246c0e9444bccdc4b847bed1eb03c5a3ece4f83dfe6abc44",
    urls = [
        "https://mirror.bazel.build/github.com/bazelbuild/bazel-skylib/releases/download/1.0.2/bazel-skylib-1.0.2.tar.gz",
        "https://github.com/bazelbuild/bazel-skylib/releases/download/1.0.2/bazel-skylib-1.0.2.tar.gz",
    ],
)

load("@bazel_skylib//:workspace.bzl", "bazel_skylib_workspace")

bazel_skylib_workspace()

# Group the sources of the library so that CMake rule have access to it
all_content = """filegroup(name = "all", srcs = glob(["**"]), visibility = ["//visibility:public"])"""

http_archive(
    name = "cryptopp",
    build_file_content = all_content,
    sha256 = "c934d2c427a0ef197ea989a00f7b6d866d110dd55257d2944d0513b382b7e2b4",
    strip_prefix = "cryptopp-CRYPTOPP_5_6_5",
    urls = ["https://github.com/weidai11/cryptopp/archive/CRYPTOPP_5_6_5.zip"],
)

http_archive(
    name = "github_cxxopts",
    sha256 = "1eefdf5af3ba0c66458258de05df2a113262ad5e85cac489de0a456088e9f9b0",
    strip_prefix = "cxxopts-3.0.0",
    urls = ["https://github.com/jarro2783/cxxopts/archive/v3.0.0.zip"],
)

http_archive(
    name = "com_google_googletest",
    sha256 = "b4870bf121ff7795ba20d20bcdd8627b8e088f2d1dab299a031c1034eddc93d5",
    strip_prefix = "googletest-release-1.11.0",
    url = "https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz",
)
