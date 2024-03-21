#include "queue.h"
#include <gtest/gtest.h>

TEST(QueueTest, SequentialEnqueueDequeue) {
  kero::spsc::internal::Queue<int> queue{};
  queue.Enqueue(1);
  queue.Enqueue(2);
  ASSERT_EQ(*queue.TryDequeue(), 1);
  ASSERT_EQ(*queue.TryDequeue(), 2);
  ASSERT_EQ(queue.TryDequeue(), std::nullopt);
}
