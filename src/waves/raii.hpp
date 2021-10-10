#pragma once

#include <optional>
#include <functional>
#include <concepts>
#include <type_traits>

template <typename Resource, typename UninitF> class raii;

template <
    typename Resource
  , std::regular_invocable<Resource> UninitF
>
class raii<Resource, UninitF>
{
public:
    template <
        std::regular_invocable IF
      , typename UF
    >
    raii(IF&& init, UF&& uninit)
    requires(std::is_invocable_r_v<Resource, IF>)
      : m_uninit(std::forward<UF>(uninit))
      , m_resource{std::invoke(std::forward<IF>(init))}
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
    std::optional<Resource> m_resource;
};

template <std::regular_invocable UninitF>
class raii<void, UninitF>
{
public:
    template <
        std::regular_invocable IF
      , typename UF
    >
    raii(IF&& init, UF&& uninit)
      : m_uninit{(
            std::invoke(std::forward<IF>(init))
          , std::forward<UF>(uninit)
        )}
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
  
    ~raii() { if (m_uninit) std::invoke(*m_uninit); }
  
private:
    std::optional<UninitF> m_uninit;
};

template <typename IF, typename UF>
raii(IF&& init, UF&& uninit) -> raii<
    std::decay_t<std::invoke_result_t<IF>>
  , UF
>;

