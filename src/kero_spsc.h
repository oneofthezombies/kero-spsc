#ifndef KERO_SPSC_KERO_SPSC_H
#define KERO_SPSC_KERO_SPSC_H

#include <atomic>
#include <optional>

namespace kero {
namespace spsc {

namespace internal {

template <typename T>
  requires std::movable<T>
struct Node {
  T data{};
  Node* next{};

  Node(T&& data) : data(std::move(data)) {}

  Node() = default;
  ~Node() = default;
  auto operator=(Node&&) -> Node& = default;

  Node(const T&) = delete;
  auto operator=(const Node&) -> Node& = delete;
};

} // namespace internal

template <typename T>
  requires std::movable<T>
class Queue {
public:
  Queue() {
    auto node = new internal::Node<T>();
    head_.store(node, std::memory_order_relaxed);
    tail_ = node;
  }

  ~Queue() {
    while (auto node = head_.load(std::memory_order_relaxed)) {
      head_.store(node->next, std::memory_order_relaxed);
      delete node;
    }
  }

  Queue(Queue&&) = default;
  Queue(const Queue&) = default;
  auto operator=(Queue&&) -> Queue& = default;
  auto operator=(const Queue&) -> Queue& = default;

  auto enqueue(T&& data) -> void {
    auto node = new internal::Node<T>(std::move(data));
    tail_->next = node;
    tail_ = node;
  }

  auto try_dequeue() -> std::optional<T> {
    auto head = head_.load(std::memory_order_acquire);
    auto head_next = head->next;
    if (head_next == nullptr) {
      return std::nullopt;
    }

    auto data = std::move(head_next->data);
    head_.store(head_next, std::memory_order_release);
    delete head;
    return data;
  }

private:
  std::atomic<internal::Node<T>*> head_{};
  internal::Node<T>* tail_{};
};

} // namespace spsc
} // namespace kero

#endif // KERO_SPSC_KERO_SPSC_H
