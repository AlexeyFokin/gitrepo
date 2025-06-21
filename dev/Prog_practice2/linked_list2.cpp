#include <iostream>

class Node
{
	public:
		int data;
		Node* next;
};

class LinkedList
{
	public:
		LinkedList() {
			head = NULL;
		}

		// inserts a node in front of all other nodes in LinkedList
		void insertFront(int data);

		// deletes first node in a LinedList
		void deleteFront();

		// prints all nodes in LinkedList starting form node
		void display();

	private:
		Node* head;
};

void LinkedList::insertFront(int data) {
	Node* newNode = new Node();

	newNode->data = data;
	newNode->next = head;
	head = newNode;
}

void LinkedList::deleteFront() {
	Node* headToDel = head;
	if(head == NULL) {
		std::cout << "LinkedList is empty, nothing to delete" << std::endl;
		return;
	}
	head = head->next;
}

void LinkedList::display() {
	// if(head == NULL) {
	// 	return;
	// }
	Node* currentNode = head;
	
	while (currentNode != NULL) {
		std::cout << "Node ref " << currentNode << " has data " << currentNode->data << std::endl;
		currentNode = currentNode->next;
	}
}

int main()
{
	LinkedList* linked_list = new LinkedList();

	linked_list->insertFront(25);
	linked_list->insertFront(2);
	linked_list->insertFront(325);
	linked_list->insertFront(23335);
	linked_list->insertFront(2235);

	linked_list->display();
	linked_list->deleteFront();

	linked_list->display();

}
