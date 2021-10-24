#include "OneLineRenderer.hpp"
#include <iostream>
#include <string>
#include <algorithm>

OneLineRenderer::OneLineRenderer(std::size_t line_len, double min_level, double max_level)
    : m_line_len(line_len)
{
    m_sample_rate = (max_level - min_level) / line_len;
    m_min_level = std::min(min_level, max_level);
    m_max_level = std::max(min_level, max_level);
}

void OneLineRenderer::render(double point)
{
    constexpr char BLOCK = '@';
    std::string line;

    if (m_max_level < point)
        line = std::string (m_line_len, BLOCK);
    else if (m_min_level < point)
    {
        int pos = (point - m_min_level) / m_sample_rate;
        if (pos > 0)
            line = std::string (pos, BLOCK);
        else
            line = std::string (m_line_len + pos, ' ') + std::string (-pos, BLOCK);
    }

    std::cout << line << std::endl;
}
