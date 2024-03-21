cc_library(
    name = "kero_spsc",
    srcs = [
        "src/channel.h",
        "src/internal/queue.h",
        "src/rx.h",
        "src/tx.h",
    ],
    hdrs = [
        "src/kero_spsc.h",
    ],
    copts = ["-std=c++20"],
    includes = ["src"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "kero_spsc_test",
    srcs = ["src/kero_spsc_test.cc"],
    copts = ["-std=c++20"],
    deps = [
        ":kero_spsc",
        "@googletest//:gtest_main",
    ],
)
