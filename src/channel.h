#ifndef KERO_SPSC_CHANNEL_H
#define KERO_SPSC_CHANNEL_H

#include "rx.h"
#include "tx.h"

namespace kero {
namespace spsc {

template <typename T>
  requires std::movable<T>
struct Channel {
  class Builder {
  public:
    Builder() = default;
    ~Builder() = default;

    Builder(Builder&&) = delete;
    Builder(const Builder&) = delete;
    auto operator=(Builder&&) -> Builder& = delete;
    auto operator=(const Builder&) -> Builder& = delete;

    auto Build() -> Channel<T> {
      auto queue = std::make_shared<typename internal::Queue<T>>();
      auto tx = Tx<T>{queue};
      auto rx = Rx<T>{queue};
      return Channel<T>{std::move(tx), std::move(rx)};
    }
  };

  Tx<T> tx;
  Rx<T> rx;

  Channel(Channel&&) = default;
  ~Channel() = default;
  auto operator=(Channel&&) -> Channel& = default;

  Channel(const Channel&) = delete;
  auto operator=(const Channel&) -> Channel& = delete;

private:
  Channel(Tx<T>&& tx, Rx<T>&& rx) : tx{std::move(tx)}, rx{std::move(rx)} {}
};

} // namespace spsc
} // namespace kero

#endif // KERO_SPSC_CHANNEL_H