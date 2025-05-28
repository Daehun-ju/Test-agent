#include <iostream>
#include <memory>

class MemoryLeak {
public:
    MemoryLeak() {
        data = new int[100];
    }

    ~MemoryLeak() {
        // delete[] data;
    }

    void doSomething() {
        for (int i = 0; i < 100; ++i) {
            data[i] = i;
        }
    }

private:
    int* data;
};

class Node {
public:
    Node(int value) : value(value) {
        std::cout << "Node created: " << value << std::endl;
    }
    ~Node() {
        std::cout << "Node destroyed: " << value << std::endl;
    }

    void setNext(std::shared_ptr<Node> nextNode) {
        next = nextNode;
    }

private:
    int value;
    std::shared_ptr<Node> next;
};

int main() {
    MemoryLeak* leak = new MemoryLeak();
    leak->doSomething();

	std::shared_ptr<Node> node1 = std::make_shared<Node>(1);
	std::shared_ptr<Node> node2 = std::make_shared<Node>(2);

	node1->setNext(node2);
	node2->setNext(node1);

    return 0;
}
