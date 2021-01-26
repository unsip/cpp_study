#include <string>
#include <cassert>

/*
 * Compute the sum of two big numbers.
 */
std::string sum(const std::string& a, const std::string& b)
{
    std::string res;
    auto it1 = a.rbegin();
    auto it2 = b.rbegin();
    for (int rem = 0; it1 != a.rend() || it2 != b.rend() || rem != 0;)
    {
        int sum = rem;
        if (it1 != a.rend())
        {
            sum += *it1 - '0';
            ++it1;
        }
        if (it2 != b.rend())
        {
            sum += *it2 - '0';
            ++it2;
        }
        rem = sum / 10;
        res = std::to_string(sum % 10) + res;
    }

    return res;
}

int main()
{
    assert(sum("0", "1") == "1");
    assert(sum("2", "3") == "5");
    assert(sum("30", "30") == "60");
    assert(sum("33", "30") == "63");
    assert(sum("120", "100") == "220");
    assert(sum("120", "10") == "130");
    assert(sum("55", "100") == "155");
    assert(sum("3145", "5") == "3150");
    assert(sum("999999999", "1") == "1000000000");
}
