#ifndef KERO_SPSC_TX_H
#define KERO_SPSC_TX_H

#include "queue.h"
#include <memory>

namespace kero {
namespace spsc {

template <typename T>
  requires std::movable<T>
class Tx {
public:
  Tx(const std::shared_ptr<Queue<T>>& queue) : queue_{queue} {}

  Tx(Tx&&) = default;
  ~Tx() = default;
  auto operator=(Tx&&) -> Tx& = default;

  Tx(const Tx&) = delete;
  auto operator=(const Tx&) -> Tx& = delete;

  auto Send(T&& value) const noexcept -> void {
    queue_->Enqueue(std::move(value));
  }

private:
  std::shared_ptr<Queue<T>> queue_;
};

} // namespace spsc
} // namespace kero

#endif // KERO_SPSC_TX_H
