#include <iostream>

class Node {
	public:
		int value;
		Node* next;
};


int main() {

	Node* head = nullptr;
	Node* one = nullptr;
	Node* two = nullptr;
	Node* three = nullptr;

  // Allocate three nodes
	one = new Node();
	two = new Node();
	three = new Node();

	one->value = 33;
	two->value = 66;
	three->value = 99;


	one->next = two;
	two->next = three;
	three->next = nullptr;
	
	head = one;
	while( head != nullptr) {
	
		std::cout << head->value << std::endl;
		head = head->next;
	}

	return 0;
}