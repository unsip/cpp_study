#include <iostream>
#include <ranges>
#include <numeric>
#include <vector>
#include <iomanip>
#include <type_traits>


struct order
{
    double avg_price;
    std::size_t quantity;

    static order mempty() { return {0, 0}; }
    bool operator ==(const order& rhv) const { return rhv.avg_price == avg_price && rhv.quantity == quantity;}
};


order avg(order lhv, order rhv)
{
    std::cout << "LHV price: " << lhv.avg_price << " quantity: " << lhv.quantity << std::endl;
    std::cout << "RHV price: " << rhv.avg_price << " quantity: " << rhv.quantity << std::endl;
    if (rhv == order::mempty())
        return lhv;

    if (lhv == order::mempty())
        return rhv;

    std::size_t total = lhv.quantity + rhv.quantity;
    static_assert(std::is_same_v<std::common_type_t<double, std::size_t>, double>);
    static_assert(std::is_same_v<double, std::common_type_t<double, std::size_t>>);
    //static_assert(std::is_same_v<decltype(lhv.quantity / total), double>);
    static_assert(std::is_same_v<decltype(total * lhv.quantity), double>);
    double avg = lhv.quantity / total * lhv.avg_price + rhv.quantity / total * rhv.avg_price;
    std::cout << "AVG: " << avg << std::endl;
    std::cout << std::endl;
    return {avg, total};
}

int main()
{
    auto cast = [](double a) -> order
    {
        return {a, 1};
    };
    std::vector<double> lst = {1, 2, 3, 4, 5, 6, 42, 11, 13};
    auto rng = lst | std::ranges::views::transform(cast);
    auto res = std::accumulate(begin(rng), end(rng), order::mempty(), avg);

    std::cout << "Result: " << std::setprecision(10) << res.avg_price << " " << res.quantity << std::endl;
}
