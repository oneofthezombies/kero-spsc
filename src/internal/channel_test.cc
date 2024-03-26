#include "channel.h"
#include <gtest/gtest.h>

TEST(ChannelTest, Sequential) {
  auto [tx, rx] = kero::spsc::Channel<std::unique_ptr<int>>::Builder{}.Build();
  tx.Send(std::make_unique<int>(1));
  ASSERT_FALSE(rx.IsEmpty());
  ASSERT_EQ(**rx.TryReceive(), 1);
  ASSERT_TRUE(rx.IsEmpty());
  ASSERT_EQ(rx.TryReceive(), std::nullopt);
}

TEST(ChannelTest, Concurrent) {
  auto [tx, rx] = kero::spsc::Channel<std::unique_ptr<int>>::Builder{}.Build();
  std::thread send_thread([&tx] {
    for (int i = 0; i < 1000; ++i) {
      tx.Send(std::make_unique<int>(i));
    }
  });
  std::thread receive_thread([&rx] {
    for (int i = 0; i < 1000; ++i) {
      while (rx.IsEmpty()) {
      }
      ASSERT_FALSE(rx.IsEmpty());
      auto data = rx.TryReceive();
      ASSERT_EQ(**data, i);
    }
    ASSERT_TRUE(rx.IsEmpty());
  });
  send_thread.join();
  receive_thread.join();
}

TEST(ChannelTest, MultipleConcurrent) {
  std::vector<std::thread> threads{};
  for (int i = 0; i < 10000; ++i) {
    auto [tx, rx] =
        kero::spsc::Channel<std::unique_ptr<int>>::Builder{}.Build();
    std::thread send_thread([tx = std::move(tx)] {
      for (int i = 0; i < 10000; ++i) {
        tx.Send(std::make_unique<int>(i));
      }
    });
    std::thread receive_thread([rx = std::move(rx)] {
      for (int i = 0; i < 10000; ++i) {
        while (rx.IsEmpty()) {
        }
        ASSERT_FALSE(rx.IsEmpty());
        auto data = rx.TryReceive();
        while (!data) {
          data = rx.TryReceive();
        }
        ASSERT_EQ(**data, i);
      }
      ASSERT_TRUE(rx.IsEmpty());
    });
    threads.emplace_back(std::move(send_thread));
    threads.emplace_back(std::move(receive_thread));
  }

  for (auto& thread : threads) {
    thread.join();
  }
}
