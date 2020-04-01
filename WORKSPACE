workspace(name = "cryptfile")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "rules_foreign_cc",
    strip_prefix = "rules_foreign_cc-master",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/master.zip",
)

load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies([
    # "//:built_cmake_toolchain",
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
    strip_prefix = "cryptopp-CRYPTOPP_8_2_0",
    sha256 = "1e5052e356bdefdfc742d0cfd480381a15d08d3a78c8a0383139b9fbe84d0dd5",
    urls = ["https://github.com/weidai11/cryptopp/archive/CRYPTOPP_8_2_0.zip"],
)

http_archive(
    name = "cxxopts",
    build_file_content = all_content,
    strip_prefix = "cxxopts-2.2.0",
    sha256 = "f9640c00d9938bedb291a21f9287902a3a8cee38db6910b905f8eba4a6416204",
    urls = ["https://github.com/jarro2783/cxxopts/archive/v2.2.0.zip"],
)

# boost
# load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

# git_repository(
#     name = "com_github_nelhage_rules_boost",
#     commit = "9f9fb8b2f0213989247c9d5c0e814a8451d18d7f",
#     remote = "https://github.com/nelhage/rules_boost",
#     shallow_since = "1570056263 -0700",
# )

# load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
# boost_deps()