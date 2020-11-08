#pragma once

#include <memory>

namespace phase::signals {
namespace Sig {
// TODO: Extend signal set.
// Note: Should be the same order as sig_mapping (see cpp).
enum Sig
{
    interupt = 0
  , terminate = 1
};

namespace detail {
struct AllSignalsTag {};
struct NoSignalsTag {};
} // namespace phase::signals::Sig::detail

constexpr detail::AllSignalsTag all;
constexpr detail::NoSignalsTag none;
} // namespace phase::signals::Sig

class SignalMask
{
    struct Impl;
    SignalMask();

    std::unique_ptr<Impl> m_impl;
public:
    SignalMask(std::initializer_list<Sig::Sig> signals);
    SignalMask(Sig::detail::AllSignalsTag);
    SignalMask(Sig::detail::NoSignalsTag);

    SignalMask(const SignalMask&) = delete;
    SignalMask& operator =(const SignalMask&) = delete;

    ~SignalMask();

    void lock() noexcept;
    void unlock() noexcept;
};
} // namespace phase::signals
