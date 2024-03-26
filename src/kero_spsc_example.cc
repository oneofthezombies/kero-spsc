#include "kero_spsc.h"
#include <iostream>
#include <memory>
#include <thread>

auto main() -> int {
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

  producer.join();
  consumer.join();
  return 0;
}
