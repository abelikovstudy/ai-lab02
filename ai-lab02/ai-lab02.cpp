#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <set>
#include <chrono>
#include <format>
#include <cstdint>
struct Node
{
    Node* parent;
    std::vector<uint8_t> value;
};

struct Statistics {
    std::vector<uint8_t> start;
    int operations;
    std::chrono::duration<double> time;
};



enum directions
{
    right = 1,
    down = 2,
    left = 3,
    up = 4
};

std::vector<uint8_t> parse(std::string input) {
    std::vector<uint8_t> nums;
    for (char num : input) {
        if (num >= '0' && num <= '9') {
            nums.push_back(num - '0');
        }
        else {
            nums.push_back(num - '0' - 7);
        }
    }
    return nums;
}

bool isSolvable(std::vector<uint8_t> nums) {
    int N = 0, e = 0;
    for (int i = 0; i < nums.size(); i++) {
        if (nums[i] == 0) {
            e = (i + 4) / 4;
            continue;
        }
        for (int j = i+1; j < nums.size(); j++) {
            if (nums[i] > nums[j] && nums[j] != 0) {
                N += 1;
            }
        }
    }
    N += e;
    return (N % 2 == 0) ? true : false;
}

std::vector<uint8_t> move(std::vector<uint8_t> nums ,directions direction) {
    auto element_iter = std::find(nums.begin(), nums.end(), 0);
    int pos = std::distance(nums.begin(), element_iter) + 1;
    switch (direction)
    {
    case right:
        if (pos % 4 != 0)
            std::iter_swap(element_iter, element_iter + 1);
        break;
    case down:
        if (pos < 13)
            std::iter_swap(element_iter, element_iter + 4);

        break;
    case left:
        if (pos != 1 && pos % 5 != 0)
            std::iter_swap(element_iter, element_iter - 1);
        break;
    case up:
        if (pos > 4)
            std::iter_swap(element_iter, element_iter - 4);
        break;
    }
    return nums;
}

bool move_out(std::vector<int> &nums, directions direction) {
    auto element_iter = std::find(nums.begin(), nums.end(), 0);
    int pos = std::distance(nums.begin(), element_iter) + 1;
    switch (direction)
    {
    case right:
        if (pos % 4 != 0)
            std::iter_swap(element_iter, element_iter + 1);
        else
            return false;
        break;
    case down:
        if (pos < 13)
            std::iter_swap(element_iter, element_iter + 4);
        else
            return false;
        break;
    case left:
        if (pos != 1 && pos % 5 != 0)
            std::iter_swap(element_iter, element_iter - 1);
        else
            return false;
        break;
    case up:
        if (pos > 4)
            std::iter_swap(element_iter, element_iter - 4);
        else
            return false;
        break;
    default:
        break;
    }
    return true;
}


void prettyPrint(std::vector<int> nums) {
    for (int i = 0; i < nums.size(); i += 4) {
        std::cout << nums[i] << ((nums[i] > 9) ? " " : "  ");
        std::cout << nums[i + 1] << ((nums[i  + 1] > 9) ? " " : "  ");
        std::cout << nums[i + 2] << ((nums[i + 2] > 9) ? " " : "  ");
        std::cout << nums[i + 3] << std::endl;
    }
}

int solve(std::string input) {
    for (char num : input) {

    }
    return 0;
}

Statistics BFS(std::vector<uint8_t> start, std::vector<uint8_t>  end) {
    int operations = 0;
    std::chrono::time_point<std::chrono::system_clock> timeStart, timeEnd;
    timeStart = std::chrono::system_clock::now();
    std::deque<Node*> front;
    Node* startNode = new Node();
    startNode->parent = NULL;
    startNode->value = start;
    front.push_back(startNode);
    std::set<std::vector<uint8_t>> visited;
    while (!front.empty()) {
        Node* variant = front.front();
        front.pop_front();;
        if (visited.find(variant->value) != visited.end()) {
            continue;
        }
        else {
            visited.insert(variant->value);
            if (variant->value == end) {
                timeEnd = std::chrono::system_clock::now();
                while (variant->parent != NULL) {
                    variant = variant->parent;
                    operations += 1;
                }
                break;
            }
            else {
                if (variant->value != move(variant->value, right)) {
                    Node* one = new Node();
                    one->parent = variant;
                    one->value = move(variant->value, right);
                    front.push_back(one);
                }

                if (variant->value != move(variant->value, left)) {
                    Node* two = new Node();
                    two->parent = variant;
                    two->value = move(variant->value, left);
                    front.push_back(two);
                }
                if (variant->value != move(variant->value, up)) {
                    Node* three = new Node();
                    three->parent = variant;
                    three->value = move(variant->value, up);
                    front.push_back(three);
                }
                if (variant->value != move(variant->value, down)) {
                    Node* four = new Node();
                    four->parent = variant;
                    four->value = move(variant->value, down);
                    front.push_back(four);
                }
            }
        }
        
    }
    Statistics runtime_stats;
    runtime_stats.start = start;
    runtime_stats.operations = operations;
    runtime_stats.time = timeEnd - timeStart;
    return runtime_stats;
}

void print_BFS(std::string input) {
    Statistics stats;
    auto nums = parse(input);
    if (isSolvable(nums)) {
        auto dest = parse("123456789ABCDEF0");
        stats = BFS(nums, dest);
        std::cout << input << " " << stats.time.count() << " " << stats.operations << std::endl;
    }
    else {
        std::cout << "Error";
    }
} 

int main()
{
    
    std::cout << "Numbers          Time          Operations" << std::endl;
    print_BFS("1234067859ACDEBF");
    print_BFS("5134207896ACDEBF");
    print_BFS("16245A3709C8DEBF");
    print_BFS("1723068459ACDEBF");
    print_BFS("12345678A0BE9FCD");
    print_BFS("51247308A6BE9FCD");
    print_BFS("F2345678A0BE91DC");
    print_BFS("75123804A6BE9FCD");
    print_BFS("75AB2C416D389F0E");
    print_BFS("04582E1DF79BCA36");
    print_BFS("FE169B4C0A73D852");
    print_BFS("D79F2E8A45106C3B");
    print_BFS("DBE87A2C91F65034");
    print_BFS("BAC0F478E19623D5");
}