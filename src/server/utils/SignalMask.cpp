#include "SignalMask.hpp"

#include <pthread.h>
#include <cassert>
#include <signal.h>

constexpr int sig_mapping[] = { SIGINT, SIGTERM };


struct SignalMask::Impl
{
    sigset_t mask = {0};
};


SignalMask::SignalMask() : m_impl(std::make_unique<Impl>()) {}

SignalMask::SignalMask(std::initializer_list<Sig::Sig> signals)
    : SignalMask(Sig::none)
{
    for (const auto& s : signals)
    {
        auto idx = static_cast<std::underlying_type<Sig::Sig>::type>(s);
        // TODO: Throw ImplError on -1.
        int err = sigaddset(&m_impl->mask, sig_mapping[idx]);
        assert(err == 0 && "Invalid signum!");
        (void)err;
    }
}

SignalMask::SignalMask(Sig::detail::AllSignalsTag)
    : SignalMask()
{
    // TODO: Throw ImplError on -1.
    int err = sigfillset(&m_impl->mask);
    assert(err == 0 && "Invalid signum!");
    (void)err;
}

SignalMask::SignalMask(Sig::detail::NoSignalsTag)
    : SignalMask()
{
    // TODO: Throw ImplError on -1.
    int err = sigemptyset(&m_impl->mask);
    assert(err == 0 && "Invalid system signal!");
    (void)err;
}

SignalMask::~SignalMask() = default;

void SignalMask::lock() noexcept
{
    sigset_t tmp = m_impl->mask;
    int err = pthread_sigmask(SIG_BLOCK, &tmp, &m_impl->mask);
    assert(err == 0 && "Invalid arguments for sigmask!");
    (void)err;
}

void SignalMask::unlock() noexcept
{
    int err = pthread_sigmask(SIG_UNBLOCK, &m_impl->mask, nullptr);
    assert(err == 0 && "Invalid arguments for sigmask!");
    (void)err;
}
