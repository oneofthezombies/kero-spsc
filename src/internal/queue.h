#ifndef KERO_SPSC_INTERNAL_QUEUE_H
#define KERO_SPSC_INTERNAL_QUEUE_H

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

template <typename T>
  requires std::movable<T>
class Queue {
public:
  Queue() noexcept {
    auto node = new Node<T>();
    head_.store(node, std::memory_order_relaxed);
    tail_ = node;
  }

  ~Queue() noexcept {
    while (auto node = head_.load(std::memory_order_relaxed)) {
      head_.store(node->next, std::memory_order_relaxed);
      delete node;
    }
  }

  Queue(Queue&&) = default;
  Queue(const Queue&) = default;
  auto operator=(Queue&&) -> Queue& = default;
  auto operator=(const Queue&) -> Queue& = default;

  auto Enqueue(T&& data) noexcept -> void {
    auto node = new Node<T>(std::move(data));
    tail_->next = node;
    tail_ = node;
  }

  auto TryDequeue() noexcept -> std::optional<T> {
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
  std::atomic<Node<T>*> head_{};
  Node<T>* tail_{};
};

} // namespace internal
} // namespace spsc
} // namespace kero

#endif // KERO_SPSC_INTERNAL_QUEUE_H
