#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <set>
#include <chrono>
#include <format>
#include <cstdint>
#include <array>
#include <iomanip>
struct Node
{
    Node* parent;
    std::array<uint8_t, 16> value;
    uint8_t depth;
};

struct Statistics {
    std::array<uint8_t, 16> start;
    int operations;
    std::chrono::duration<double> time;
    bool status;
};

Statistics DFS_runtime;
Statistics IDS_runtime;

enum directions
{
    right = 1,
    down = 2,
    left = 3,
    up = 4
};

std::array<uint8_t, 16> parse(std::string input) {

    std::array<uint8_t, 16> nums;
    int i = 0;
    for (char num : input) {
        if (num >= '0' && num <= '9') {
            nums[i] = (num - '0');
        }
        else {
            nums[i] = (num - '0' - 7);
        }
        i += 1;
    }
    return nums;
}

bool isSolvable(std::array<uint8_t, 16> nums) {
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

std::array<uint8_t, 16> move(std::array<uint8_t, 16> nums ,directions direction,int pos) {
    
    switch (direction)
    {
    case right:
        if (pos % 4 != 0)
            std::swap(nums[pos - 1], nums[pos]);
            // std::iter_swap(element_iter, element_iter + 1);
        break;
    case down:
        if (pos < 13)
            std::swap(nums[pos - 1], nums[pos + 3]);
            //std::iter_swap(element_iter, element_iter + 4);

        break;
    case left:
        if ((pos + 3) % 4 != 0)
            std::swap(nums[pos - 1], nums[pos - 2]);
            //std::iter_swap(element_iter, element_iter - 1);
        break;
    case up:
        if (pos > 4)
            std::swap(nums[pos - 1], nums[pos - 5]);
            //std::iter_swap(element_iter, element_iter - 4);
        break;
    }
    return nums;
}

void prettyPrint(std::array<int,16> nums) {
    for (int i = 0; i < 16; i += 4) {
        std::cout << nums[i] << ((nums[i] > 9) ? " " : "  ");
        std::cout << nums[i + 1] << ((nums[i  + 1] > 9) ? " " : "  ");
        std::cout << nums[i + 2] << ((nums[i + 2] > 9) ? " " : "  ");
        std::cout << nums[i + 3] << std::endl;
    }
    std::cout << std::endl;
}

Statistics BFS(std::array<uint8_t, 16> start, std::array<uint8_t, 16>  end) {
    int operations = 0;
    bool done = false;
    std::chrono::time_point<std::chrono::system_clock> timeStart, timeEnd;
    timeStart = std::chrono::system_clock::now();
    std::deque<Node*> front;
    Node* startNode = new Node();
    startNode->parent = NULL;
    startNode->value = start;
    startNode->depth = 0;
    front.push_back(startNode);
    while (!front.empty()) {
        
        Node* variant = front.front();
        front.pop_front();
        
        if (variant->depth == 20) {
            
            break;
        }

        if (variant->value == end) {
            timeEnd = std::chrono::system_clock::now();
            operations = variant->depth;
            done = true;
            break;
        }
        else {
            auto element_iter = std::find(variant->value.begin(), variant->value.end(), 0);
            auto pos = std::distance(variant->value.begin(), element_iter) + 1;

            auto move_right = move(variant->value, right, pos);
            auto move_left = move(variant->value, left,  pos);
            auto move_up = move(variant->value, up, pos);
            auto move_down = move(variant->value, down, pos);

            if (variant->parent == nullptr || variant->value != move_right && variant->parent->value != move_right) {
                Node* one = new Node();
                one->parent = variant;
                one->value = move_right;
                one->depth = one->parent->depth + 1;
                front.push_back(one);
        
            }

            if (variant->parent == nullptr ||  variant->value != move_left && variant->parent->value != move_left ) {
                Node* two = new Node();
                two->parent = variant;
                two->value = move_left;
                two->depth = two->parent->depth + 1;
                front.push_back(two);
                
            }
            if (variant->parent == nullptr || variant->value != move_up && variant->parent->value != move_up ) {
                Node* three = new Node();
                three->parent = variant;
                three->value = move_up;
                three->depth = three->parent->depth + 1;
                front.push_back(three);
                
            }
            if (variant->parent == nullptr || variant->value != move_down && variant->parent->value != move_down) {
                Node* four = new Node();
                four->parent = variant;
                four->value = move_down;
                four->depth = four->parent->depth + 1;
                front.push_back(four);
                
            }
        }
    }

    Statistics runtime_stats;
    runtime_stats.start = start;
    runtime_stats.operations = operations;
    runtime_stats.time = timeEnd - timeStart;
    runtime_stats.status = done;
    return runtime_stats;
}

bool DFS(Node* node, std::array<uint8_t, 16>  end, uint8_t depth_control = 30) {
    if (node->depth >= depth_control) {
        return false;
    }
    if (node->value == end) {
        DFS_runtime.operations = node->depth;
        return true;
    }

    auto element_iter = std::find(node->value.begin(), node->value.end(), 0);
    auto pos = std::distance(node->value.begin(), element_iter) + 1;

    auto move_right = move(node->value, right, pos);
    auto move_left = move(node->value, left, pos);
    auto move_up = move(node->value, up, pos);
    auto move_down = move(node->value, down, pos);
    if (node->parent == nullptr || node->value != move_left && node->parent->value != move_left) {
        Node left;
        left.parent = node;
        left.value = move_left;
        left.depth = node->depth + 1;
        if (DFS(&left, end, depth_control)) {
            return true;
        }
    }
    
    if (node->parent == nullptr || node->value != move_right && node->parent->value != move_right) {
        Node right;
        right.parent = node;
        right.value = move_right;
        right.depth = node->depth + 1;
        if (DFS(&right, end, depth_control)) {
            return true;
        }
    }
    
    

    if (node->parent == nullptr || node->value != move_up && node->parent->value != move_up) {
        Node up;
        up.parent = node;
        up.value = move_up;
        up.depth = node->depth + 1;
        if (DFS(&up, end, depth_control)) {
            return true;
        }
    }


    if (node->parent == nullptr || node->value != move_down && node->parent->value != move_down) {
        Node down;
        down.parent = node;
        down.value = move_down;
        down.depth = node->depth + 1;
        if (DFS(&down, end, depth_control)) {
            return true;
        }
    }
    return false;
}

Statistics DFS_wrap(std::array<uint8_t, 16> start, std::array<uint8_t, 16>  end, uint8_t depth_control = 30) {
    Node* startNode = new Node();
    startNode->parent = NULL;
    startNode->value = start;
    startNode->depth = 0;

    DFS_runtime.start = start;
    std::chrono::time_point<std::chrono::system_clock> timeStart, timeEnd;
    timeStart = std::chrono::system_clock::now();
    if (DFS(startNode, end, depth_control)) {
        DFS_runtime.status = true;
    }
    else {
        DFS_runtime.status = false;
    }
    timeEnd = std::chrono::system_clock::now();
    DFS_runtime.time = timeEnd - timeStart;

    return DFS_runtime;
}

Statistics IDS_wrap(std::array<uint8_t, 16> start, std::array<uint8_t, 16>  end, uint8_t depth_control = 30) {
    Node* startNode = new Node();
    startNode->parent = NULL;
    startNode->value = start;
    startNode->depth = 0;
    DFS_runtime.start = start;
    std::chrono::time_point<std::chrono::system_clock> timeStart, timeEnd;
    timeStart = std::chrono::system_clock::now();
    for (int i = 0; i <= depth_control; i++) {
        if (DFS(startNode, end, i)) {
            timeEnd = std::chrono::system_clock::now();
            DFS_runtime.time = timeEnd - timeStart;
            DFS_runtime.status = true;
            return DFS_runtime;
        }
    }
    DFS_runtime.status = false;
    return DFS_runtime;
}

void print_BFS(std::string input) {
    Statistics stats;
    auto nums = parse(input);
    if (isSolvable(nums)) {
        auto dest = parse("123456789ABCDEF0");
        stats = BFS(nums, dest);
        std::cout
            << input
            << " "
            << (stats.status ? "+" : "-")
            << "    "
            << stats.operations
            << ((stats.operations >= 10) ? "         " : "          ")
            << stats.time.count()
            << std::endl;
    }
    else {
        std::cout
            << input
            << " "
            << "Error"
            << std::endl;
    }
}

void print_DFS(std::string input, uint8_t depth_control = 30) {
    auto nums = parse(input);
    if (isSolvable(nums)) {
        auto dest = parse("123456789ABCDEF0");
        DFS_wrap(nums, dest, depth_control);
        std::cout
            << input
            << " "
            << (DFS_runtime.status ? "+" : "-")
            << "    "
            << DFS_runtime.operations
            << ((DFS_runtime.operations >= 10) ? "         " : "          ")
            << DFS_runtime.time.count()
            << std::endl;
    }
    else {
        std::cout
            << input
            << " "
            << "Error"
            << std::endl;
    }
}

int main(){
    std::setprecision(4);
    /*
    std::cout << "================BFS==================" << std::endl;
    std::cout << "Numbers          Done Operations Time" << std::endl;
    print_BFS("123456789AFB0EDC");
    print_BFS("F2345678A0BE91CD");
    print_BFS("123456789ABCDEF0");
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
    std::cout << "================DFS==================" << std::endl;
    std::cout << "Numbers          Done Operations Time" << std::endl;
    print_DFS("123456789AFB0EDC", 1);
    print_DFS("F2345678A0BE91CD", 1);
    print_DFS("123456789ABCDEF0", 1);
    print_DFS("1234067859ACDEBF", 6);
    print_DFS("5134207896ACDEBF", 9);
    print_DFS("16245A3709C8DEBF", 11);
    print_DFS("1723068459ACDEBF", 14);
    print_DFS("12345678A0BE9FCD", 20);
    print_DFS("51247308A6BE9FCD", 28);
    print_DFS("F2345678A0BE91DC", 1);
    print_DFS("75AB2C416D389F0E", 1);
    print_DFS("04582E1DF79BCA36", 1);
    print_DFS("FE169B4C0A73D852", 1);
    print_DFS("D79F2E8A45106C3B", 1);
    print_DFS("DBE87A2C91F65034", 1);
    print_DFS("BAC0F478E19623D5", 1);*/
    std::cout << "================IDS==================" << std::endl;
    std::cout << "Numbers          Done Operations Time" << std::endl;
    print_DFS("123456789AFB0EDC", 1);
    print_DFS("F2345678A0BE91CD", 1);
    print_DFS("123456789ABCDEF0", 1);
    print_DFS("1234067859ACDEBF", 6);
    print_DFS("5134207896ACDEBF", 9);
    print_DFS("16245A3709C8DEBF", 11);
    print_DFS("1723068459ACDEBF", 14);
    print_DFS("12345678A0BE9FCD", 20);
    print_DFS("51247308A6BE9FCD", 28);
    print_DFS("F2345678A0BE91DC", 1);
    print_DFS("75AB2C416D389F0E", 1);
    print_DFS("04582E1DF79BCA36", 1);
    print_DFS("FE169B4C0A73D852", 1);
    print_DFS("D79F2E8A45106C3B", 1);
    print_DFS("DBE87A2C91F65034", 1);
    print_DFS("BAC0F478E19623D5", 1);

 
}