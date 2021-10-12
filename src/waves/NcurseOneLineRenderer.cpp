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

constexpr auto MAX_GAUGE = 5;

int gauge_by_bar(float bar_sz)
{
    assert(bar_sz >= 0);
    const auto gauge = std::min<decltype(MAX_GAUGE)>(MAX_GAUGE, bar_sz);
    assert(gauge >= 0);
    return gauge;
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
    decltype(create_pad(0, 0)) m_prev_pad = create_pad(m_height, m_width * BUFFER_MULT);
    decltype(m_prev_pad) m_cur_pad = create_pad(m_height, m_width * BUFFER_MULT);
    int m_cur_x_pos = m_width * (BUFFER_MULT - 1);

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

float NcurseOneLineRenderer::level_to_bar(float lvl) const
{
    float bar_sz = lvl < m_min_level
        ? 0
        : m_max_level < lvl
            ? m_impl->m_height
            : std::abs((lvl - m_min_level) / m_sample_rate);

    // invert graph if ratio is negative
    if (bar_sz < 0)
        bar_sz = m_impl->m_height + bar_sz;

    return bar_sz;
}

void NcurseOneLineRenderer::render(double lvl)
{
    constexpr auto COLORS = 4;
    constexpr auto ATTRS = 3;
    constexpr auto SHADES_NUM = COLORS * ATTRS;

    assert(m_impl->m_height >= 0);
    Zones zones {static_cast<std::size_t>(m_impl->m_height), SHADES_NUM};

    auto bar_sz = level_to_bar(lvl);

    assert(bar_sz >= zones.min());
    assert(bar_sz <= zones.max());

    /// @todo bar may be equal to zones.max() and leads to assert
    // graph line color depends on signal level
    const auto color_by_bar = [&zones] (float bar) { /// @todo bar_lvl
        return COLOR_PAIR(zones.idx_by_lvl(bar) / ATTRS + 1);
    };

    // color depth attribute
    const auto attr_by_bar = [&zones] (float bar) {
        switch (zones.idx_by_lvl(bar) % ATTRS)
        {
          default:
            assert(false);
          case 1: return 0u;
          case 0: return A_DIM;
          case 2: return A_BOLD;
        }
    };

    // note for bar=0 result is out of end coordinate
    const auto bar_to_y = [this](int bar) {
        assert(bar >= 0);
        assert(bar <= m_impl->m_height);
        return m_impl->m_height - bar;
    };

    const auto y_to_bar = [this](int y) {
        assert(y >= 0);
        assert(y < m_impl->m_height);
        return m_impl->m_height - y;
    };

    auto draw_segment = [this, color_by_bar, attr_by_bar, y_to_bar](auto y, auto symbol) {
        wmove(*m_impl->m_cur_pad, y, m_impl->m_cur_x_pos);
        wattrset(*m_impl->m_cur_pad, color_by_bar(y_to_bar(y)) | attr_by_bar(y_to_bar(y)));
        const auto gauge = gauge_by_bar(y_to_bar(y));
        assert(y + gauge <= m_impl->m_height);
        wvline_set(*m_impl->m_cur_pad, symbol, gauge);
        return y + gauge;
    };

    // graph bar's top Y coordinate
    // if falling too fast
    auto cur_y = bar_to_y(bar_sz);
    if (m_prev_lvl)
        if (
            auto prev_bar = level_to_bar(*m_prev_lvl)
          ; static_cast<std::size_t>(prev_bar) > static_cast<std::size_t>(bar_sz + MAX_GAUGE)
        )
        {
            for (
                auto y = bar_to_y(prev_bar) + gauge_by_bar(prev_bar)
              ; y < cur_y
              ; y = draw_segment(y, WACS_DARROW)
            ) ;
        }

    if (cur_y < m_impl->m_height)
        cur_y = draw_segment(cur_y, WACS_BLOCK);

    // if raising too fast
    if (m_prev_lvl)
        if (
            auto prev_bar = level_to_bar(*m_prev_lvl)
            /// @todo It's necessry to distinguish bar_lvl (float) and bar (int)
          ; static_cast<std::size_t>(bar_sz - gauge_by_bar(bar_sz)) > static_cast<std::size_t>(prev_bar)
        )
        {
            assert(cur_y < m_impl->m_height);
            for (
                auto y = cur_y
              ; y < bar_to_y(prev_bar)
              ; y = draw_segment(y, WACS_UARROW)
            ) ;
        }

    using namespace std::chrono;
    auto now = steady_clock::now();
    constexpr auto FRAME_RATE = 7;
    using namespace std::literals::chrono_literals;
    // draw new frame according frame rate setting
    if (now >= m_prev_tp + 1000ms / FRAME_RATE)
    {
        prefresh(
            *m_impl->m_cur_pad                              // pad
          , 0                                               // pminrow
          , m_impl->m_cur_x_pos                             // pmincol
          , 0                                               // sminrow
          , 0                                               // smincol
          , m_impl->m_height - 1                            // smaxrow
          , m_impl->m_width - 1                             // smaxcol
        );
        m_prev_tp = now;
    }

    // copy visible history data and swap pads
    if (0 == m_impl->m_cur_x_pos--)
    {
        m_impl->m_cur_x_pos = m_impl->m_width * (BUFFER_MULT - 1);
        std::swap(m_impl->m_prev_pad, m_impl->m_cur_pad);
        wclear(*m_impl->m_cur_pad);
        auto dmincol = m_impl->m_cur_x_pos  + 1;
        auto dmaxcol = m_impl->m_width * BUFFER_MULT - 1;
        copywin(
            *m_impl->m_prev_pad                             // src
          , *m_impl->m_cur_pad                              // dst
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

    m_prev_lvl = lvl;
}
