#ifndef KERO_SPSC_INTERNAL_RX_H
#define KERO_SPSC_INTERNAL_RX_H

#include "queue.h"
#include <memory>

namespace kero {
namespace spsc {

template <typename T>
  requires std::movable<T>
class Rx {
public:
  Rx(const std::shared_ptr<Queue<T>>& queue) : queue_{queue} {}

  Rx(Rx&&) = default;
  ~Rx() = default;
  auto operator=(Rx&&) -> Rx& = default;

  Rx(const Rx&) = delete;
  auto operator=(const Rx&) -> Rx& = delete;

  auto TryReceive() const noexcept -> std::optional<T> {
    return queue_->TryDequeue();
  }

private:
  std::shared_ptr<Queue<T>> queue_;
};

} // namespace spsc
} // namespace kero

#endif // KERO_SPSC_INTERNAL_RX_H
