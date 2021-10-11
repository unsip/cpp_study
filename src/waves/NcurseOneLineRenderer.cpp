#include "NcurseOneLineRenderer.hpp"
#include "Zones.h"
#include "raii.hpp"
#include <algorithm>
#include <iostream>
#include <cassert>
#include <ncurses.h>

namespace {

auto create_pad(int height, int width)
{
    return raii{
        [height, width] {
            auto rslt = newpad(height, width);
            if (!rslt)
                throw std::runtime_error{"Ncurses error: unable to initialize pad"};
            return rslt;
        }
      , [] (auto pad) { delwin(pad); }
    };
}

auto create_screen()
{
    return raii{
        []{
            if (!initscr())
                throw std::runtime_error{"Ncurses error: unable to initialize main screen"};
        }
      , []{ endwin(); }
    };
}

/// @todo External parameter to change buffer size depending on a sampler rate
constexpr int BUFFER_MULT = 2;

} // anonymous namespace

struct NcurseOneLineRenderer::Impl
{
    // ncurses main screen
    decltype(create_screen()) m_screen = create_screen();
    // terminal screen dimenstions
    int m_width{COLS};
    int m_height{LINES};
    // ncurses pads to implement double bufferring
    decltype(create_pad(0, 0)) m_prev = create_pad(m_height, m_width * BUFFER_MULT);
    decltype(m_prev) m_cur = create_pad(m_height, m_width * BUFFER_MULT);
    int m_cur_pos = m_width * (BUFFER_MULT - 1);
};

NcurseOneLineRenderer::NcurseOneLineRenderer(double min_level, double max_level)
  : m_impl{std::make_unique<Impl>()}
  , m_sample_rate{ [](auto min_level, auto max_level, auto height) -> double {
      auto rslt = (max_level - min_level) / height;
      if (rslt == 0.0)
          throw std::runtime_error{"Invalid signal range"};
      return rslt;
  }(min_level, max_level, m_impl->m_height) }
  , m_min_level{std::min(min_level, max_level)}
  , m_max_level{std::max(min_level, max_level)}
{
    assert(m_min_level < m_max_level);

    noecho();
    refresh();
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);
}

NcurseOneLineRenderer::~NcurseOneLineRenderer() = default;

void NcurseOneLineRenderer::render(double lvl)
{
    constexpr auto COLORS = 4;
    constexpr auto ATTRS = 3;
    constexpr auto SHADES_NUM = COLORS * ATTRS;

    assert(m_impl->m_height >= 0);
    Zones zones{static_cast<std::size_t>(m_impl->m_height), SHADES_NUM};

    // signal lvl in terms of graph bar size
    float bar_sz = lvl < m_min_level
        ? zones.min()
        : m_max_level < lvl
            ? zones.max()
            : std::abs((lvl - m_min_level) / m_sample_rate);

    // invert graph if ratio is negative
    if (bar_sz < 0)
        bar_sz = m_impl->m_height + bar_sz; 

    assert(bar_sz >= zones.min());
    assert(bar_sz <= zones.max());

    // graph line color depends on signal level
    const int color = zones.idx_by_lvl(bar_sz) / ATTRS + 1;
    // color depth attribute
    const int attr = zones.idx_by_lvl(bar_sz) % ATTRS;

    // graph bar's top Y coordinate
    const auto pos = m_impl->m_height - bar_sz;
    wmove(*m_impl->m_cur, pos, m_impl->m_cur_pos);
    wattrset(*m_impl->m_cur, COLOR_PAIR(color) | [attr] {
        switch (attr) {
          default:
            assert(false);
          case 1: return 0u;
          case 0: return A_DIM;
          case 2: return A_BOLD;
        }
    }());

    // graph line size
    constexpr auto MAX_GAUGE = 5;
    const auto gauge = std::min<decltype(MAX_GAUGE)>(MAX_GAUGE, bar_sz);
    assert(gauge >= 0);
    assert(pos + gauge <= m_impl->m_height);

    wvline_set(*m_impl->m_cur, WACS_BLOCK, gauge);

    using namespace std::chrono;
    auto now = steady_clock::now();
    constexpr auto FRAME_RATE = 7;
    using namespace std::literals::chrono_literals;
    // draw new frame according frame rate setting
    if (now >= m_prev_tp + 1000ms / FRAME_RATE)
    {
        prefresh(
            *m_impl->m_cur                                  // pad
          , 0                                               // pminrow
          , m_impl->m_cur_pos                               // pmincol
          , 0                                               // sminrow
          , 0                                               // smincol
          , m_impl->m_height - 1                            // smaxrow
          , m_impl->m_width - 1                             // smaxcol
        );
        m_prev_tp = now;
    }

    // copy visible history data and swap pads
    if (0 == m_impl->m_cur_pos--)
    {
        m_impl->m_cur_pos = m_impl->m_width * (BUFFER_MULT - 1);
        std::swap(m_impl->m_prev, m_impl->m_cur);
        wclear(*m_impl->m_cur);
        auto dmincol = m_impl->m_cur_pos  + 1;
        auto dmaxcol = m_impl->m_width * BUFFER_MULT - 1;
        copywin(
            *m_impl->m_prev                                 // src
          , *m_impl->m_cur                                  // dst
          , 0                                               // sminrow, y
          , 0                                               // smincol, x
          , 0                                               // dminrow, y
          , dmincol                                         // dmincol, x
          , m_impl->m_height - 1                            // dmaxrow, y
          , dmaxcol                                         // dmaxcol, x
          , false                                           // don't override
        );
        assert(dmincol < dmaxcol);
        assert(m_impl->m_width - 2 == dmaxcol - dmincol);
    }
}
