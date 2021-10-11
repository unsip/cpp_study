#include "NcurseOneLineRenderer.hpp"
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

} // anonymous namespace

struct NcurseOneLineRenderer::Impl
{
    // ncurses main screen
    decltype(create_screen()) m_screen = create_screen();
    // terminal screen dimenstions
    int m_width{COLS};
    int m_height{LINES};
    // ncurses pads to implement double bufferring
    decltype(create_pad(m_height, m_width)) m_prev = create_pad(m_height, m_width);
    decltype(create_pad(m_height, m_width)) m_cur = create_pad(m_height, m_width);
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
    wclear(*m_impl->m_cur);
    copywin(*m_impl->m_prev, *m_impl->m_cur, 0, 0, 0, 1, m_impl->m_height - 1, m_impl->m_width - 2, false/*not override*/);

    // signal lvl in terms of graph bar size
    int bar_sz = lvl < m_min_level
        ? 0
        : m_max_level < lvl
            ? m_impl->m_height
            : std::abs((lvl - m_min_level) / m_sample_rate);

    // invert graph if ratio is negative
    if (bar_sz < 0)
        bar_sz = m_impl->m_height + bar_sz; 

    assert(bar_sz >= 0);
    assert(bar_sz <= m_impl->m_height);

    // graph line color depends on signal level
    constexpr auto COLORS = 4;
    const int color = static_cast<int>(float(bar_sz) * COLORS / m_impl->m_height) + 1;
    const auto rng = float(m_impl->m_height) / COLORS;
    assert(color != 1 || float(bar_sz) < rng);
    assert(color != 2 || float(bar_sz) < 2 * rng);
    assert(color != 3 || float(bar_sz) < 3 * rng);
    assert(color != 4 || float(bar_sz) < 4 * rng);

    // a separate color range
    const auto cur_color_rng = bar_sz - rng * static_cast<int>(bar_sz / rng);
    constexpr auto ATTRS = 3;
    // color depth attribute
    const int attr = static_cast<int>(float(cur_color_rng) * ATTRS / rng);
    assert(attr >= 0);
    assert(attr <= ATTRS);
    assert(attr != 0 || cur_color_rng < rng / 2);
    assert(attr != 2 || cur_color_rng > rng / 2);

    assert(attr != 0 || cur_color_rng < rng / ATTRS);
    assert(attr != 1 || cur_color_rng < 2 * rng / ATTRS);
    assert(attr != 2 || cur_color_rng < 3 * rng / ATTRS);

    // graph bar's top Y coordinate
    const auto pos = m_impl->m_height - bar_sz;
    wmove(*m_impl->m_cur, pos, 0);
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
    const auto gauge = std::min(MAX_GAUGE, bar_sz);
    assert(gauge >= 0);
    assert(pos + gauge <= m_impl->m_height);

    wvline_set(*m_impl->m_cur, WACS_BLOCK, gauge);

    using namespace std::chrono;
    auto now = steady_clock::now();
    constexpr auto FRAME_RATE = 25;
    using namespace std::literals::chrono_literals;
    if (now >= m_prev_tp + 1000ms / FRAME_RATE)
    {
        prefresh(*m_impl->m_cur, 0, 0, 0, 0, m_impl->m_height - 1, m_impl->m_width - 1);
        m_prev_tp = now;
    }

    std::swap(m_impl->m_prev, m_impl->m_cur);
}
