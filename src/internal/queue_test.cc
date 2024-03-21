#include "queue.h"
#include <gtest/gtest.h>

TEST(QueueTest, Sequential) {
  kero::spsc::Queue<int> queue{};
  queue.Enqueue(1);
  queue.Enqueue(2);
  ASSERT_EQ(*queue.TryDequeue(), 1);
  ASSERT_EQ(*queue.TryDequeue(), 2);
  ASSERT_EQ(queue.TryDequeue(), std::nullopt);
}

TEST(QueueTest, Concurrent) {
  kero::spsc::Queue<std::unique_ptr<int>> queue{};
  std::thread enqueue_thread([&queue] {
    for (int i = 0; i < 1000; ++i) {
      queue.Enqueue(std::make_unique<int>(i));
    }
  });
  std::thread dequeue_thread([&queue] {
    for (int i = 0; i < 1000; ++i) {
      auto data = queue.TryDequeue();
      while (!data) {
        data = queue.TryDequeue();
      }
      ASSERT_EQ(**data, i);
    }
  });
  enqueue_thread.join();
  dequeue_thread.join();
}

TEST(QueueTest, MultipleConcurrent) {
  std::vector<std::thread> threads{};
  for (int i = 0; i < 100; ++i) {
    auto queue = std::make_shared<kero::spsc::Queue<std::unique_ptr<int>>>();
    std::thread enqueue_thread([queue] {
      for (int i = 0; i < 10000; ++i) {
        queue->Enqueue(std::make_unique<int>(i));
      }
    });
    std::thread dequeue_thread([queue] {
      for (int i = 0; i < 10000; ++i) {
        auto data = queue->TryDequeue();
        while (!data) {
          data = queue->TryDequeue();
        }
        ASSERT_EQ(**data, i);
      }
    });
    threads.emplace_back(std::move(enqueue_thread));
    threads.emplace_back(std::move(dequeue_thread));
  }

  for (auto& thread : threads) {
    thread.join();
  }
}
