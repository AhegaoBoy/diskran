#include <iostream>
#include <vector>
#include <algorithm>

class dynamic_solution
{
    enum operation_type
    {
        minus_one = 1,
        division_by_two,
        division_by_three
    };

    std::vector<int> dp;
    std::vector<operation_type> operation;

    int n;

public:
    dynamic_solution(int n) : n(n), dp(std::vector<int>(n + 1)), operation(std::vector<operation_type>(n+1)) {};
    std::pair<int, std::vector<std::string>> min_cost_to_convert();
};

std::pair<int, std::vector<std::string>> dynamic_solution::min_cost_to_convert()
{

    dp[1] = 0;

    for (int i = 2; i <= n; ++i) {

        dp[i] = dp[i - 1] + i;
        operation[i] = minus_one;

        if (i % 2 == 0 && dp[i] > dp[i / 2] + i)
        {
            dp[i] = dp[i / 2] + i;
            operation[i] = division_by_two;
        }

        if (i % 3 == 0 && dp[i] > dp[i / 3] + i)
        {
            dp[i] = dp[i / 3] + i;
            operation[i] = division_by_three;
        }
    }


    std::vector<std::string> steps;
    int current = n;
    while (current != 1)
    {
        switch (operation[current])
        {
            case minus_one:
                steps.push_back("-1");
                current -= 1;
                break;
            case division_by_two:
                steps.push_back("/2");
                current /= 2;
                break;
            case division_by_three:
                steps.push_back("/3");
                current /= 3;
                break;
            default:
                throw std::logic_error("Unexpected error, can't identify command!");
        }
    }

    return {dp[n], steps};
}
int main() {
    int n;

    std::cin >> n;

    dynamic_solution solution(n);

    auto result = solution.min_cost_to_convert();

    std::cout << result.first << std::endl;

    for (const std::string& op : result.second) {
        std::cout << op << " ";
    }


    return 0;
}