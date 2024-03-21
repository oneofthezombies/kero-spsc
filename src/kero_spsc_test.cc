// #include "kero_spsc.h"
// #include <gtest/gtest.h>

// #include <memory>
// #include <thread>
// #include <vector>

// struct Dto {
//   int i{};

//   Dto(int i) : i(i) {}
// };

// TEST(QueueTest, PushAndPop) {
//   auto threads = std::vector<std::thread>{};

//   for (int ti = 0; ti < 1000; ++ti) {
//     std::shared_ptr<kero::spsc::Queue<std::unique_ptr<Dto>>> q =
//         std::make_shared<kero::spsc::Queue<std::unique_ptr<Dto>>>();
//     auto item_count = 100000;

//     auto t1 = std::thread([q, item_count] {
//       for (int i = 0; i < item_count; ++i) {
//         q->enqueue(std::unique_ptr<Dto>(new Dto{i}));
//       }
//     });
//     threads.push_back(std::move(t1));

//     auto t2 = std::thread([q, item_count] {
//       auto current = 0;
//       while (current < item_count) {
//         auto data = q->try_dequeue();
//         if (data) {
//           EXPECT_EQ((*data)->i, current);
//           ++current;
//         }
//       }
//     });
//     threads.push_back(std::move(t2));
//   }

//   for (auto& t : threads) {
//     t.join();
//   }
// }
