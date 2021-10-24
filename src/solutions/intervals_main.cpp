#include "intervals.hpp"
#include <iostream>
#include <fstream>
#include <limits>
#include <cstdlib>

int main()
{
    using namespace std;
    ofstream fout(getenv("OUTPUT_PATH"));

    int s;
    cin >> s;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int s_itr = 0; s_itr < s; s_itr++)
    {
        int n;
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        vector<range> intervals;
        intervals.reserve(n);
        for (int intervals_row_itr = 0; intervals_row_itr < n; intervals_row_itr++)
        {
            range rng;
            cin >> get<0>(rng);
            cin >> get<1>(rng);
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            intervals.emplace_back(rng);
        }

        int result = interval_selection(intervals);

        fout << result << "\n";
    }

    fout.close();

    return 0;
}
