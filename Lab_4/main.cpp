#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

int calculate_summ(std::vector<int>& array, int idx)
{
    int sum = 0;
    for (int i = 0; i < idx; i++) {
        sum += array[i];
    }
    return sum;
}

std::vector<unsigned long long> copy(std::vector<unsigned long long>& text, int idx)
{
    std::vector<unsigned long long> newText;
    for (int i = idx; i < text.size(); i++) {
        newText.push_back(text[i]);
    }
    return newText;
}

std::vector<int> KMP(const std::vector<unsigned long long>& pattern, std::vector<unsigned long long>& text)
{
    std::vector<int> PI(pattern.size());

    PI[0] = 0;
    int j = 0, i = 1;
    while (i < static_cast<int>(pattern.size()))
    {
        if (pattern[i] != pattern[j])
        {
            if (j == 0)
            {
                PI[i] = 0;
                i++;
            }
            else
                j = PI[j - 1];

        }
        else
        {
            PI[i++] = ++j;
//            i++;
//            j++;
        }
    }

    i = 0;
    j = 0;

    std::vector<int> result;
    while (i < static_cast<int>(text.size())) {
        if (text[i] == pattern[j]) {
            i++;
            j++;
            if (j == static_cast<int>(pattern.size())) {
                result.push_back(i - j + 1);
                j = PI[j - 1];
            }
        }
        else {
            if (j > 0) {
                j = PI[j - 1];
            }
            else {
                i++;
            }
        }
    }
    return result;
}

int main()
{
    std::vector<unsigned long long> pattern;
    unsigned long long num;

    while (std::cin >> num)
    {
        pattern.push_back(num);
        char nextchar = std::cin.get();

        while (nextchar == ' ')
            nextchar = std::cin.get();


        if (nextchar == '\n')
            break;

        else
            std::cin.unget();

    }

    std::vector<unsigned long long> text;

    std::vector<int> not_empty_lines_numbers;
    std::vector<int> each_line_numbers_summ;
    std::vector<int> line_numbers_quantity;
    std::vector <int> result;

    int numbers_of_line = 0;
    int quantity_of_lines = 0;
    int summ_numbers_of_line = 0;

    std::vector <int> FinalResult;
    std::string line;

    bool not_empty = false;
    int summ = 0;


    while(std::getline(std::cin, line))
    {
        if (line.empty())
        {
            quantity_of_lines++;
            continue;
        }

        std::stringstream ss(line);
        while (ss >> num)
        {
            not_empty = true;
            summ_numbers_of_line++;
            numbers_of_line++;
            text.push_back(num);
            if (text.size() >= pattern.size() * 2)
            {
                result = KMP(pattern, text);

                for (int res = 0; res < result.size(); res++)
                    result[res] += summ;

                FinalResult.insert(FinalResult.end(), result.begin(), result.end());

                summ += text.size() - pattern.size() + 1;
                text = copy(text, text.size() - pattern.size() + 1);

            }
        }

        quantity_of_lines++;
        not_empty_lines_numbers.push_back(quantity_of_lines);
        each_line_numbers_summ.push_back(summ_numbers_of_line);
        line_numbers_quantity.push_back(numbers_of_line);
        numbers_of_line = 0;
    }

    if (pattern.empty() || !(not_empty))
        return 0;


    result = KMP(pattern, text);
    for (int i = 0; i < result.size(); i++)
        result[i] += summ;


    int max = not_empty_lines_numbers.back();

    std::vector<int> nums_quantity_of_every_lines(max, 0);
    std::vector<int> numbers_sum_for_all_lines(max, 0);

    for (int i = 0; i < not_empty_lines_numbers.size(); i++)
    {
        nums_quantity_of_every_lines[not_empty_lines_numbers[i] - 1] = line_numbers_quantity[i];
        numbers_sum_for_all_lines[not_empty_lines_numbers[i] - 1] = each_line_numbers_summ[i];
    }

    FinalResult.insert(FinalResult.end(), result.begin(), result.end());
    for (int k = 0; k < FinalResult.size(); k++)
    {
        for (int j = 0; j < numbers_sum_for_all_lines.size(); j++)
        {
            if (FinalResult[k] <= numbers_sum_for_all_lines[j])
            {
                std::cout << j + 1 << ", " << FinalResult[k] - calculate_summ(nums_quantity_of_every_lines, j) << std::endl;
                break;
            }
        }
    }

    return 0;
}