# Kero SPSC

A memory-safe "single producer single consumer queue" implementation written in C++.  

## Why I Created This

Memory safety in programming languages is no longer an option. This is an area that must be met.  
However, the status of the C++ programming language in the current compiler industry is absolute. So I wrote the code in C++, but developed it to meet memory safety features.  

## Features

### Memory-Safe

- Only `std::movable` type items are allowed.  
- Even if either `Tx` or `Rx` is destroyed, the life of the queue is guaranteed to be `std::shared_ptr`.

### Lock-Free

- It guarantees a single producer and a single consumer to be the `Tx`, `Rx` type of `Channel`.  

> [!NOTE]  
> Is This Wait-Free?  
> It does not guarantee constant time because `operator new`, `operator delete` are called within the internal `Enqueue()` and `TryDequeue()` methods.

## Usage

```cpp
auto [tx, rx] = kero::spsc::Channel<std::unique_ptr<int>>::Builder{}.Build();
std::atomic<bool> done{false};

std::thread producer([&done, tx = std::move(tx)] {
  for (int i = 0; i < 10000; ++i) {
    tx.Send(std::make_unique<int>(i));
  }
  done.store(true, std::memory_order_release);
});

std::thread consumer([&done, rx = std::move(rx)] {
  while (auto data = rx.TryReceive()) {
    if (!data) {
      if (done.load(std::memory_order_acquire)) {
        break;
      }
    }
    std::cout << **data << std::endl;
  }
});
```

## For Contributors

This project was developed based on Bazel.  
Please ensure `bazel` command and after calling the `bazel --version` command, check whether the version is higher than `7.0.0`.  

You can get IDE support by running the command below.  

```sh
bazel run @hedron_compile_commands//:refresh_all
```

For more information, please refer to the link below.  

https://github.com/hedronvision/bazel-compile-commands-extractor

## References

[February 2024 Whitehouse Technical Report](https://www.whitehouse.gov/wp-content/uploads/2024/02/Final-ONCD-Technical-Report.pdf)  
