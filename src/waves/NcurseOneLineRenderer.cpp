#include "NcurseOneLineRenderer.hpp"
#include <algorithm>
#include <iostream>
#include <cassert>


NcurseOneLineRenderer::NcurseOneLineRenderer(double min_level, double max_level)
  : m_screen{initscr()}
  , m_width{COLS}
  , m_height{LINES}
  , m_sample_rate{(max_level - min_level) / m_height}
  , m_min_level{std::min(min_level, max_level)}
  , m_max_level{std::max(min_level, max_level)}
  , m_prev{newpad(m_height, m_width)} 
  , m_cur{newpad(m_height, m_width)} 
{
    /// @todo exception safety
    if (!m_screen || !m_prev || !m_cur)
        throw std::runtime_error{"Unable to initialize ncurse"};
    if (m_sample_rate == 0.0)
        throw std::runtime_error{"Invalid signal range"};

    assert(m_min_level < m_max_level);

    noecho();
    refresh();
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
}

NcurseOneLineRenderer::~NcurseOneLineRenderer()
{
    delwin(m_cur);
    delwin(m_prev);
    endwin();
}

void NcurseOneLineRenderer::render(double point)
{
    constexpr char BLOCK = '@';

    wclear(m_cur);
    copywin(m_prev, m_cur, 0, 0, 0, 1, m_height - 1, m_width - 2, false/*not override*/);

    int color = 1;
    int pos = point < m_min_level
        ? m_height - 1
        : m_max_level < point
            ? 0
            : std::abs((point - m_min_level) / m_sample_rate);

    if (pos < 0)
        pos = m_height - 1 + pos; 

    assert(pos >= 0);
    assert(pos < m_height);

    // graph line color depends on signal level
    color = pos * 3 / m_height + 1;
    wmove(m_cur, pos, 0);
    wattrset(m_cur, COLOR_PAIR(color) | A_BOLD);

    // graph line size
    constexpr auto MAX_GAUGE = 5;
    auto gauge = std::min(MAX_GAUGE, m_height - pos);
    assert(gauge >= 1);
    assert(pos + gauge <= m_height);
    wvline(m_cur, BLOCK, gauge);

    prefresh(m_cur, 0, 0, 0, 0, m_height - 1, m_width - 1);
    std::swap(m_prev, m_cur);
}
