#pragma once

#include <tuple>
#include <vector>

using range = std::tuple<int, int>;

range get_overlap(range lhv, range rhv);
bool is_overlap(range lhv, range rhv);
int interval_selection(const std::vector<range>& intervals);

