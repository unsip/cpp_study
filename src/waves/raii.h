#pragma once

#include <boost/optional.hpp>
#include <type_traits>

/// @todo support C++20 (deduction guide, avoid make-helper)
template <typename Resource, typename UninitF>
class raii
{
public:
  template <typename IF, typename UF>
  raii(IF&& init, UF&& uninit)
    : m_uninit(std::forward<UF>(uninit))
    , m_resource{std::forward<IF>(init)()}
  {}

  Resource& operator*() { return *m_resource; }
  const Resource& operator*() const { return *m_resource; }

  raii(const raii&) = delete;
  raii& operator=(const raii&) = delete;

  raii(raii&& rhv)
    : m_uninit(std::move(rhv.m_uninit))
    , m_resource(std::move(rhv.m_resource))
  {
    rhv.m_resource.reset();
  }

  raii& operator=(raii&& rhv)
  {
    m_uninit = std::move(rhv.m_uninit);
    m_resource = std::move(rhv.m_resource);
    rhv.m_resource.reset();
    return *this;
  }

  ~raii() { if (m_resource) m_uninit(*m_resource); }

private:
  UninitF m_uninit;
  boost::optional<Resource> m_resource;
};

template <typename UninitF>
class raii<void, UninitF>
{
public:
  template <typename IF, typename UF>
  raii(IF&& init, UF&& uninit)
    : m_uninit{(std::forward<IF>(init)(), std::forward<UF>(uninit))}
  {}

  raii(const raii&) = delete;
  raii& operator=(const raii&) = delete;

  raii(raii&& rhv)
    : m_uninit(std::move(rhv.m_uninit))
  {
    rhv.m_uninit.reset();
  }

  raii& operator=(raii&& rhv)
  {
    m_uninit = std::move(rhv.m_uninit);
    rhv.m_uninit.reset();
    return *this;
  }

  ~raii() { if (m_uninit) (*m_uninit)(); }

private:
  boost::optional<UninitF> m_uninit;
};

template <typename InitF, typename UninitF>
inline raii<
    typename std::decay<decltype(std::declval<InitF&&>()())>::type
  , UninitF
  >
make_raii(InitF&& init, UninitF&& uninit)
{
  using Resource = typename std::decay<decltype(std::declval<InitF&&>()())>::type;
  return raii<Resource, UninitF>{std::forward<InitF>(init), std::forward<UninitF>(uninit)};
}
