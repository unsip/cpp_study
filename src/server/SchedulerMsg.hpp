#pragma once
#include <chrono>
#include <string>

struct add_msg
{
    std::chrono::system_clock::timepoint tp;
    std::string payload;

    std::unique_ptr<char []> serialize()
    {
        auto s_tp = tp.time_since_epoch().count();
        std::size_t sz = payload.size() + sizeof(s_tp);
        std::unique_ptr<char []> data{new char [sz]};
        std::copy(s);
    }
};

struct add_response
{
    std::size_t id;
};

struct del_msg
{
    std::size_t id;
};
