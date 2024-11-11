#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <climits>

class grid_search final
{
    struct node
    {
        friend class grid_search;
        int x, y, g, f;
        node(int x, int y, int g, int f) : x(x), y(y), g(g), f(f){};
        bool operator<(node const& other) const
        {
            return f > other.f;
        }
    };
    static int heuristic(int x1, int y1, int x2, int y2)
    {
        return abs(x1 - x2) + abs(y1 - y2);
    }
public:

    static int a_star(std::vector<std::vector<unsigned char>> const& grid, std::pair<int, int> start, std::pair<int, int> end)
    {
        int n = grid.size();
        int m = grid[0].size();

        std::priority_queue<node> open_list;
        std::vector<std::vector<int>> g_score(n, std::vector<int>(m, INT_MAX));

        int start_x = start.first, start_y = start.second;
        int end_x = end.first, end_y = end.second;

        open_list.push(node(start_x, start_y, 0, heuristic(start_x, start_y, end_x, end_y)));

        g_score[start_x][start_y] = 0;

        while(!open_list.empty())
        {
            node current = open_list.top();
            open_list.pop();

            if(current.x == end_x && current.y == end_y)
                return current.g;

            int directions[4][2] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};
            for(auto move : directions)
            {
                int x_move = current.x + move[0];
                int y_move = current.y + move[1];

                if(x_move >= 0 && x_move < n && y_move >= 0 && y_move < m && grid[x_move][y_move] == '.')
                {
                    int tentative_g_score = current.g + 1;
                    if(tentative_g_score < g_score[x_move][y_move])
                    {
                        g_score[x_move][y_move] = tentative_g_score;
                        int f_score = tentative_g_score + heuristic(x_move, y_move, end_x, end_y);
                        open_list.push(node(x_move, y_move, tentative_g_score, f_score));
                    }
                }
            }
        }
        return -1;
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);


    int n, m;
    std::cin>>n>>m;
    std::vector<std::vector<unsigned char>> grid;
    for(int i = 0; i < n; ++i)
    {
        std::vector<unsigned char> line(m);
        for(int j = 0 ; j < m; ++j)
            std::cin>>line[j];
        grid.push_back(std::move(line));
    }

    int q;
    std::cin >> q;

    while(q--)
    {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;

        std::cout<<grid_search::a_star(grid, {--x1, --y1}, {--x2, --y2})<<'\n';
    }
    return 0;
}
