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
#include <queue>

struct Node
{
	Node* parent;
	std::array<uint8_t, 16> value;
	uint8_t depth;
};

struct NodeOpt
{
	NodeOpt* parent;
	std::array<uint8_t, 16> value;
	uint8_t depth;
	uint8_t zeroPos;
};

struct Statistics {
	std::array<uint8_t, 16> start;
	int operations;
	std::chrono::duration<double> time;
	bool status;
};

Statistics DFS_runtime;
Statistics IDS_runtime;
Statistics IDA_runtime;

enum directions
{
	right = 1,
	down = 2,
	left = 3,
	up = 4
};

uint8_t Lc(std::array<uint8_t, 16> input) {
	uint8_t sum = 0;
	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			for (int x1 = x + 1; x1 < 4; ++x1)
			{
				if (input[y * 4 + x] && input[y * 4 + x1])
					if ((input[y * 4 + x] - 1) / 4 == y && (input[y * 4 + x1] - 1) / 4 == y && input[y * 4 + x] > input[y * 4 + x1])
					{
						sum += 2;
					}
			}
			for (int y1 = y + 1; y1 < 4; ++y1)
			{
				if (input[y * 4 + x] && input[y1 * 4 + x])
					if ((input[y * 4 + x] - 1) % 4 == x && (input[y1 * 4 + x] - 1) % 4 == x && input[y * 4 + x] > input[y1 * 4 + x])
					{
						sum += 2;
					}
			}
		}
	}

	return sum;
}
uint8_t Mn(std::array<uint8_t, 16> input) {
	int sum = 0;
	for (int i = 0; i < 16; ++i)
	{
		if (input[i] == 0)
			continue;
		sum += abs(int( (input[i] - 1) % 4) - (i % 4)) + abs(int( (input[i] - 1) / 4) - (i / 4));
	}


	return sum;
}



class Compare {
public:
	
	bool operator()(NodeOpt* first, NodeOpt* second)
	{
		return Mn(first->value) + Lc(first->value) + first->depth > Mn(second->value) + Lc(second->value) + second->depth;
	}
	bool operator()(Node* first, Node* second)
	{
		return Mn(first->value) + Lc(first->value) + first->depth > Mn(second->value) + Lc(second->value) + second->depth;
	}
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
		for (int j = i + 1; j < nums.size(); j++) {
			if (nums[i] > nums[j] && nums[j] != 0) {
				N += 1;
			}
		}
	}
	N += e;
	return (N % 2 == 0) ? true : false;
}

std::array<uint8_t, 16> move(std::array<uint8_t, 16> nums, directions direction, int pos) {

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

void prettyPrint(std::array<uint8_t, 16> nums) {
	for (int i = 0; i < 16; i += 4) {
		std::cout << unsigned(nums[i]) << ((unsigned(nums[i]) > 9) ? " " : "  ");
		std::cout << unsigned(nums[i + 1]) << ((unsigned(nums[i + 1]) > 9) ? " " : "  ");
		std::cout << unsigned(nums[i + 2]) << ((unsigned(nums[i + 2]) > 9) ? " " : "  ");
		std::cout << unsigned(nums[i + 3]) << std::endl;
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
			auto move_left = move(variant->value, left, pos);
			auto move_up = move(variant->value, up, pos);
			auto move_down = move(variant->value, down, pos);

			if (variant->parent == nullptr || variant->value != move_right && variant->parent->value != move_right) {
				Node* one = new Node();
				one->parent = variant;
				one->value = move_right;
				one->depth = one->parent->depth + 1;
				front.push_back(one);

			}

			if (variant->parent == nullptr || variant->value != move_left && variant->parent->value != move_left) {
				Node* two = new Node();
				two->parent = variant;
				two->value = move_left;
				two->depth = two->parent->depth + 1;
				front.push_back(two);

			}
			if (variant->parent == nullptr || variant->value != move_up && variant->parent->value != move_up) {
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

Statistics A_Star(std::array<uint8_t, 16> start, std::array<uint8_t, 16>  end) {
	Statistics runtime_stats;
	std::set<std::array<uint8_t, 16>> visited;
	std::priority_queue<NodeOpt*, std::vector<NodeOpt*>, Compare> front;
	NodeOpt* startNode = new NodeOpt();
	startNode->parent = NULL;
	startNode->value = start;
	startNode->depth = 0;
	auto element_iter = std::find(startNode->value.begin(), startNode->value.end(), 0);
	auto pos = std::distance(startNode->value.begin(), element_iter);
	startNode->zeroPos = pos;
	front.push(startNode);
	std::chrono::time_point<std::chrono::system_clock> timeStart, timeEnd;
	timeStart = std::chrono::system_clock::now();
	while (!front.empty()) {
		NodeOpt* node = front.top();
		front.pop();
		if (!visited.contains(node->value)) {
			visited.insert(node->value);
			if ((node->zeroPos + 1) % 4 != 0) {
				NodeOpt* right = new NodeOpt();
				right->value = node->value;
				right->zeroPos = node->zeroPos + 1;
				right->parent = node;
				right->depth = node->depth + 1;
				std::swap(right->value[node->zeroPos], right->value[right->zeroPos]);
				if (right->value == end) {
					timeEnd = std::chrono::system_clock::now();
					runtime_stats.time = timeEnd - timeStart;
					runtime_stats.status = true;
					runtime_stats.operations = node->depth + 1;
					runtime_stats.start = start;
					break;
				}
				if (right->parent == nullptr || right->value != right->parent->value) {
					front.push(right);
				}
			}

			if ((node->zeroPos + 4) % 4 != 0) {
				NodeOpt* left = new NodeOpt();
				left->value = node->value;
				left->zeroPos = node->zeroPos - 1;
				left->parent = node;
				left->depth = node->depth + 1;
				std::swap(left->value[node->zeroPos], left->value[left->zeroPos]);
				if (left->value == end) {
					timeEnd = std::chrono::system_clock::now();
					runtime_stats.time = timeEnd - timeStart;
					runtime_stats.status = true;
					runtime_stats.operations = node->depth + 1;
					runtime_stats.start = start;
					break;
				}
				if (left->parent == nullptr || left->value != left->parent->value) {
					front.push(left);
				}
			}
			if (node->zeroPos < 12) {

				NodeOpt* down = new NodeOpt();
				down->value = node->value;
				down->zeroPos = node->zeroPos + 4;
				down->parent = node;
				down->depth = node->depth + 1;
				std::swap(down->value[node->zeroPos], down->value[down->zeroPos]);
				if (down->value == end) {
					
					timeEnd = std::chrono::system_clock::now();
					runtime_stats.time = timeEnd - timeStart;
					runtime_stats.status = true;
					runtime_stats.operations = node->depth + 1;
					runtime_stats.start = start;
					break;
				}
				if (down->parent == nullptr || down->value != down->parent->value) {
					front.push(down);
				}
			}
			if (node->zeroPos > 3) {
				NodeOpt* up = new NodeOpt();
				up->value = node->value;
				up->zeroPos = node->zeroPos - 4;
				up->parent = node;
				up->depth = node->depth + 1;
				std::swap(up->value[node->zeroPos], up->value[up->zeroPos]);
				if (up->value == end) {

					timeEnd = std::chrono::system_clock::now();
					runtime_stats.time = timeEnd - timeStart;
					runtime_stats.status = true;
					runtime_stats.operations = node->depth + 1;
					runtime_stats.start = start;
					break;
				}
				if (up->parent == nullptr || up->value != up->parent->value) {
					front.push(up);
				}
			}
		}
		
	}
	return runtime_stats;
}

int IDA_Star_Search(std::vector<Node *> &nodes, int bound, std::array<uint8_t, 16>  end) {
	
	int min = INT_MAX;
	Node* node = nodes.back();
	std::vector<Node*> successors;
	int f = node->depth + Mn(node->value) + Lc(node->value);
	if (f > bound)
		return f;
	if (node->value == end) {
		IDA_runtime.operations = node->depth;
		IDA_runtime.status = true;

		return -1;
	}
	auto element_iter = std::find(node->value.begin(), node->value.end(), 0);
	auto pos = std::distance(node->value.begin(), element_iter) + 1;

	auto move_right = move(node->value, right, pos);
	auto move_left = move(node->value, left, pos);
	auto move_up = move(node->value, up, pos);
	auto move_down = move(node->value, down, pos);
	if (node->parent == nullptr || node->value != move_left && node->parent->value != move_left) {
		Node * left = new Node();
		left->parent = node;
		left->value = move_left;
		left->depth = node->depth + 1;
		successors.push_back(left);
	}

	if (node->parent == nullptr || node->value != move_right && node->parent->value != move_right) {
		Node * right = new Node();
		right->parent = node;
		right->value = move_right;
		right->depth = node->depth + 1;
		successors.push_back(right);
	}

	if (node->parent == nullptr || node->value != move_up && node->parent->value != move_up) {
		Node * up = new Node();
		up->parent = node;
		up->value = move_up;
		up->depth = node->depth + 1;
		successors.push_back(up);
	}


	if (node->parent == nullptr || node->value != move_down && node->parent->value != move_down) {
		Node * down = new Node();
		down->parent = node;
		down->value = move_down;
		down->depth = node->depth + 1;
		successors.push_back(down);
	}

	std::sort(successors.begin(), successors.end(),Compare());
	std::reverse(successors.begin(), successors.end());
	for (auto temp1 : successors)
	{
		//std::cout << unsigned(temp1->depth);
		bool fl = true;
		for (auto& succ : successors)
			if (succ->value == temp1->value) {
				fl = false;
			}
		if (!fl) {
			nodes.push_back(temp1);
			int t = IDA_Star_Search(nodes, bound, end);

			if (t == -1)
				return -1;
			if (t < min)
				min = t;
			nodes.pop_back();
		}

	}
	return min;
}

Statistics IDA_Star(std::array<uint8_t, 16> start, std::array<uint8_t, 16>  end) {
	IDA_runtime.start = start;
	std::chrono::time_point<std::chrono::system_clock> timeStart, timeEnd;
	timeStart = std::chrono::system_clock::now();
	Node* startNode = new Node();
	startNode->parent = NULL;
	startNode->value = start;
	startNode->depth = 0;
	int bound = Mn(startNode->value) + Lc(startNode->value);
	std::vector<Node*> vec;
	vec.push_back(startNode);
	while (true) {
		int t = IDA_Star_Search(vec, bound, end);
		if (t == -1) {
			timeEnd = std::chrono::system_clock::now();

			break;
		}
		bound = t;
	}
	IDA_runtime.time = timeEnd - timeStart;
	return IDA_runtime;
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

void print_IDS(std::string input, uint8_t depth_control = 30) {
	auto nums = parse(input);
	if (isSolvable(nums)) {
		auto dest = parse("123456789ABCDEF0");
		IDS_wrap(nums, dest, depth_control);
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

void print_Astar(std::string input) {
	auto nums = parse(input);
	if (isSolvable(nums)) {
		auto dest = parse("123456789ABCDEF0");
		Statistics stats = A_Star(nums, dest);
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

void print_IDAStar(std::string input) {
	auto nums = parse(input);
	if (isSolvable(nums)) {
		auto dest = parse("123456789ABCDEF0");
		Statistics stats = IDA_Star(nums, dest);
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

int main() {
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
	print_DFS("BAC0F478E19623D5", 1);
	std::cout << "================IDS==================" << std::endl;
	std::cout << "Numbers          Done Operations Time" << std::endl;
	print_IDS("123456789AFB0EDC", 1);
	print_IDS("F2345678A0BE91CD", 1);
	print_IDS("123456789ABCDEF0", 1);
	print_IDS("1234067859ACDEBF", 6);
	print_IDS("5134207896ACDEBF", 9);
	print_IDS("16245A3709C8DEBF", 11);
	print_IDS("1723068459ACDEBF", 14);
	print_IDS("12345678A0BE9FCD", 20);
	print_IDS("51247308A6BE9FCD", 28);
	print_IDS("F2345678A0BE91DC", 1);
	print_IDS("75AB2C416D389F0E", 1);
	print_IDS("04582E1DF79BCA36", 1);
	print_IDS("FE169B4C0A73D852", 1);
	print_IDS("D79F2E8A45106C3B", 1);
	print_IDS("BAC0F478E19623D5", 1);
	print_IDS("DBE87A2C91F65034", 1);
	std::cout << "=================A*==================" << std::endl;
	std::cout << "Numbers          Done Operations Time" << std::endl;
	print_Astar("123456789AFB0EDC");
	print_Astar("F2345678A0BE91CD");
	print_Astar("123456789ABCDEF0");
	print_Astar("1234067859ACDEBF");
	print_Astar("5134207896ACDEBF");
	print_Astar("16245A3709C8DEBF");
	print_Astar("1723068459ACDEBF");
	print_Astar("12345678A0BE9FCD");
	print_Astar("51247308A6BE9FCD");
	print_Astar("F2345678A0BE91DC");
	print_Astar("75123804A6BE9FCD");
	print_Astar("75AB2C416D389F0E");
	print_Astar("04582E1DF79BCA36");
	print_Astar("FE169B4C0A73D852");
	print_Astar("D79F2E8A45106C3B");
	print_Astar("DBE87A2C91F65034");
	print_Astar("BAC0F478E19623D5");
	*/
	print_IDAStar("123456789AFB0EDC");
	print_IDAStar("F2345678A0BE91CD");
	print_IDAStar("123456789ABCDEF0");
	print_IDAStar("1234067859ACDEBF");
	print_IDAStar("5134207896ACDEBF");
	print_IDAStar("16245A3709C8DEBF");
	print_IDAStar("1723068459ACDEBF");
	print_IDAStar("12345678A0BE9FCD");
	print_IDAStar("51247308A6BE9FCD");
	print_IDAStar("F2345678A0BE91DC");
	print_IDAStar("75123804A6BE9FCD");
	print_IDAStar("75AB2C416D389F0E");
	print_IDAStar("04582E1DF79BCA36");
	print_IDAStar("FE169B4C0A73D852");
	print_IDAStar("D79F2E8A45106C3B");
	print_IDAStar("DBE87A2C91F65034");
	print_IDAStar("BAC0F478E19623D5");


}