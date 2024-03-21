cc_library(
    name = "kero_spsc",
    srcs = [
        "src/internal/channel.h",
        "src/internal/queue.h",
        "src/internal/rx.h",
        "src/internal/tx.h",
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
    srcs = [
        "src/internal/channel_test.cc",
        "src/internal/queue_test.cc",
    ],
    copts = ["-std=c++20"],
    deps = [
        ":kero_spsc",
        "@googletest//:gtest_main",
    ],
)
