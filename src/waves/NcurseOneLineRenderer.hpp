#pragma once

#include "Renderer.hpp"
#include <ncurses.h>

class NcurseOneLineRenderer : public Renderer
{
    // ncurses main screen
    WINDOW* m_screen;
    // terminal screen dimenstions
    int m_width;
    int m_height;
    // income signal level to represent one vertical point
    // can be negative if original max lvl less than min
    double m_sample_rate;
    double m_min_level;
    double m_max_level;
    // ncurses pads to implement double bufferring
    WINDOW* m_prev;
    WINDOW* m_cur;

public:
    NcurseOneLineRenderer(double min_level, double max_level);
    ~NcurseOneLineRenderer();

    NcurseOneLineRenderer(const NcurseOneLineRenderer&) = delete;
    NcurseOneLineRenderer& operator=(const NcurseOneLineRenderer&) = delete;

    void render(double point) override;
};

