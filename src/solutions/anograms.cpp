#include <algorithm>
#include <functional>
#include <vector>
#include <string>
#include <cassert>


class AnogramsDict
{
    std::vector<std::string> m_storage;
public:
    bool is_anagram(const std::string& lhv, std::string rhv) const
    {
        std::sort(rhv.begin(), rhv.end());
        return lhv == rhv;
    }

    void insert(std::string word)
    {
        m_storage.push_back(std::move(word));
    }

    std::vector<std::string> get_entries(std::string word) const
    {
        std::vector<std::string> res;
        std::sort(word.begin(), word.end());
        auto func = std::bind(&AnogramsDict::is_anagram, this, std::cref(word), std::placeholders::_1);

        //for (
        //    auto last = m_storage.end(), it = std::find_if(m_storage.begin(), last, func)
        //  ; it != last
        //  ; it = std::find_if(++it, last, func)
        //)
        //    res.push_back(*it);

        std::copy_if(m_storage.begin(), m_storage.end(), std::back_inserter(res), func);

        return res;
    }
};


int main()
{
    AnogramsDict dict;
    dict.insert("rwod");
    {
    auto res = dict.get_entries("word");
    assert(res.size() == 1 && res[0] == "rwod");
    }

    dict.insert("wodr");
    {
        auto res = dict.get_entries("word");
        assert(res.size() == 2 && res[1] == "wodr");
    }

    // Member-function type.
    using mem_fun = bool (AnogramsDict::*)(const std::string&, std::string) const;
    mem_fun f = &AnogramsDict::is_anagram;

    // Member-function call.
    (dict.*f)("", "");
}
