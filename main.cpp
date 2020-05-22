#include <iostream>
#include <cstring> 
#include <cmath>
#include <fstream>

using namespace std;

struct Node {
	int data;
	int redBlack;
	Node* left = NULL;
	Node* right = NULL;
	Node* parent;
};

int convertToInt(char* num);
int inputToInt(char* input, int* array);
int fileToInt(char* fileName, int* array);
Node* read(Node* root, int* array, int size);
void display(Node* currRoot);
void print(Node* currRoot, int tabNum);
Node* getParent(Node* node);
Node* getGrandparent(Node* node);
Node* getSibling(Node* node);
Node* getUncle(Node* node);
void rotateLeft(Node* node);
void rotateRight(Node* node);
Node* insert(Node* node, Node* root);
void insertRecurse(Node* node, Node* root);
void insertRepairTree(Node* node);
void insertCase1(Node* node);
void insertCase2(Node* node);
void insertCase3(Node* node);
void insertCase4(Node* node);
void insertCase4Step2(Node* node);
bool search(Node* currRoot, int num);
void replaceNode(Node* node, Node* child);
void deleteOneChild(Node* node);
void deleteCase1(Node* node); 
void deleteCase2(Node* node);
void deleteCase3(Node* node);
void deleteCase4(Node* node);
void deleteCase5(Node* node);
void deleteCase6(Node* node);
Node* findNode(Node* currRoot, int num);
void deleteNode(Node* &currRoot, int num);
void deleteCases(Node* node, Node* &root);

const int BLACK = 0;
const int RED = 1;

int main() {
	char fileName[50];
	char input[20];
	int* array = new int[100];
	int size;
	Node* root = NULL;
	bool running = true;
	
	cout << "Please enter a file name: ";
	cin.get(fileName, 50);
	cin.ignore(50, '\n');
	size = fileToInt(fileName, array);	

	root = read(root, array, size);

	while (running) {
		cout << "Please enter whether you would like to search, remove, insert, print, or quit: ";
		cin.get(input, 20);
		cin.ignore(20, '\n');
		
		if (strcmp(input, "INSERT") == 0) {
			int num;
			cout << "Please enter a number: ";
			cin >> num;
			cin.ignore(20, '\n');
			Node* node = new Node();
			node -> data = num;
			root = insert(node, root);
		}
		else if (strcmp(input, "SEARCH") == 0) {
			bool in;
			int num;
			cout << "Please enter a number: ";
			cin >> num;
			cout << endl;
			cin.ignore(20, '\n');
			in = search(root, num);
			if (in) {
				cout << "The number is in the tree." << endl;
			}
			else {
				cout << "The number is not in the tree." << endl;
			}	
		}
		else if (strcmp(input, "REMOVE") == 0) {
			bool in;
			int num;
			cout << "Please enter a number: ";
			cin >> num;
			cout << endl;
			cin.ignore(20, '\n');
			in = search(root, num);
			if (in) {
				Node* node = findNode(root, num);
				deleteCases(node, root);
				cout << "Deleted." << endl;
			}
			else {
				cout << "The number is not in the tree." << endl;
			}
		}	
		else if (strcmp(input, "PRINT") == 0) {
			print(root, 0);
			cout << "Inorder traversal: ";
			display(root);
			cout << endl;
		}
		else if (strcmp(input, "QUIT") == 0) {
			running = false;
		}	
	}
	

	return 0;
}

int convertToInt(char* num) { // Using a char array, make an int.
	int fin = 0;
	int power = strlen(num) - 1;
	for (int i = 0; i < strlen(num); i++) {
		int digit = num[i] - '0';
		fin += digit * pow(10, power-i);
	}
	return fin;
}

int inputToInt(char* input, int* array) { // Take the input and convert it into an int array.
	int arrayIndex = 0;
	for (int i = 0; i < strlen(input); i++) {
		if (!isspace(input[i]) && isdigit(input[i])) {
			int index = 0;
			char* num = new char[3];
			num[index] = input[i];
			index++;
			while (!isspace(input[i+1])) {
				i++;
				num[index] = input[i];
			        index++;
			}
			num[index] = '\0';
			array[arrayIndex] = convertToInt(num);
		        arrayIndex++;
		}
	}
	return arrayIndex;
}

int fileToInt(char* fileName, int* array) { // File input to int array, utilizes input to int.
	char input[200];
	int size;
	ifstream file(fileName);
	if (file.is_open()) {
		file.getline(input, 200);
		file.close();
		size = inputToInt(input, array);	
		return size;
	}
	else {
		cout << "Unable to open file." << endl;
		return -1;
	}
}	

void print(Node* currRoot, int tabNum) { // Print the tree.
	if (currRoot -> right) {
		print(currRoot -> right, tabNum+1);
	}
	int i = 0;
	while (i < tabNum) {
		cout << '\t';
		i++;
	}
	cout << currRoot -> data << endl;
	if (currRoot -> left) {
		print(currRoot -> left, tabNum+1);
	}
}

void display(Node* currRoot) { // An inorder traversal of the tree.
	if (currRoot) {
		display(currRoot -> left);
		cout << currRoot -> data << " - ";
		if (currRoot -> redBlack == RED) {
			cout << "Red, ";
		}
		else {
			cout << "Black, ";
		}
		display(currRoot -> right);
	}
}


Node* read(Node* root, int* array, int size) {
	for (int i = 0; i < size; i++) {
		Node* temp = new Node();
		temp -> data = array[i];
		root = insert(temp, root);
	}
	return root;
}

Node* getParent(Node* node) {
	return node -> parent;
}

Node* getGrandparent(Node* node) {
	return getParent(getParent(node));
}

Node* getSibling(Node* node) {
	Node* parent = getParent(node);

	if (!parent) {
		return NULL;
	}

	if (node == parent -> left) {
		return parent -> right;
	}
	else {
		return parent -> left;
	}
}

Node* getUncle(Node* node) {
	Node* parent = getParent(node);
	
	return getSibling(parent);
}

void rotateLeft(Node* node) {
	Node* newNode = node -> right;
	Node* parent = getParent(node);

	node -> right = newNode -> left;
	newNode -> left = node;
	node -> parent = newNode;

	if (node -> right) {
		node -> right -> parent = node;
	}

	if (parent) {
		if (node == parent -> left) {
			parent -> left = newNode;
		}
		else if (node == parent -> right) {
			parent -> right = newNode;
		}
	}
	
	newNode -> parent = parent;
}

void rotateRight(Node* node) {
	Node* newNode = node -> left;
	Node* parent = getParent(node);

	node -> left = newNode -> right;
	newNode -> right = node;
	node -> parent = newNode;

	if (node -> left) {
		node -> left -> parent = node;
	}

	if (parent) {
		if (node == parent -> left) {
			parent -> left = newNode;
		}
		else if (node == parent -> right) {
			parent -> right = newNode;
		}
	}
	
	newNode -> parent = parent;
}

Node* insert(Node* node, Node* root) {
	if (root) {
		insertRecurse(node, root);
		insertRepairTree(node);
	}
	else {
		root = node;
	}

	while (getParent(root)) {
		root = getParent(root);
	}
	return root;
}

void insertRecurse(Node* node, Node* root) {
	if(root) {
		if(node -> data < root -> data) {
			if(root -> left) {
				insertRecurse(node, root -> left);
				return;
			}
			else {
				root -> left = node;
			}
		}
		else {
			if(root -> right) {
				insertRecurse(node, root -> right);
				return;
			}
			else {
				root -> right = node;
			}
		}
	}

	node -> parent = root;
	node -> left = NULL;
	node -> right = NULL;
	node -> redBlack = RED;
}

void insertRepairTree(Node* node) {
	if(!getParent(node)) {
		insertCase1(node);
	}
	else if (getParent(node) -> redBlack == BLACK) {
		insertCase2(node);
	}
	else if (getUncle(node) && getUncle(node) -> redBlack == RED) {
		insertCase3(node);
	}
	else {
		insertCase4(node);
	}
}

void insertCase1(Node* node) {	
	node -> redBlack = BLACK;
}

void insertCase2(Node* node) {	
	return;
}

void insertCase3(Node* node) {	
	getParent(node) -> redBlack = BLACK;
	getUncle(node) -> redBlack = BLACK;
	getGrandparent(node) -> redBlack = RED;
	insertRepairTree(getGrandparent(node));
}

void insertCase4(Node* node) {
	Node* parent = getParent(node);
	Node* grandparent = getGrandparent(node);

	if (node == parent -> right && parent == grandparent -> left) {
		rotateLeft(parent);
		node = node -> left;
	}
	else if (node == parent -> left && parent == grandparent -> right) {
		rotateRight(parent);
		node = node -> right;
	}

	insertCase4Step2(node);
}

void insertCase4Step2(Node* node) {
	Node* parent = getParent(node);
	Node* grandparent = getGrandparent(node);

	if (node == parent -> left) {
		rotateRight(grandparent);
	}
	else {
		rotateLeft(grandparent);
	}

	parent -> redBlack = BLACK;
	grandparent -> redBlack = RED;
}

bool search(Node* currRoot, int num) { 
	if (!currRoot) {
		return false;
	}
	else if (currRoot -> data == num) {
		return true;
	}
	else if (currRoot -> data > num) {
		return search(currRoot -> left, num);
	}
	else if (currRoot -> data < num) {
		return search(currRoot -> right, num);
	}
}

Node* findNode(Node* currRoot, int num) {
	if (currRoot -> data == num) {
		return currRoot;
	}
	else if (currRoot -> data > num) {
		return findNode(currRoot -> left, num);
	}
	else if (currRoot -> data < num) {
		return findNode(currRoot -> right, num);
	}
}

void deleteNode(Node* &currRoot, int num) {
	if (currRoot -> data == num) {
		delete currRoot;
		currRoot = NULL;
	}
	else if (currRoot -> data > num) {
		deleteNode(currRoot -> left, num);
	}
	else if (currRoot -> data < num) {
		deleteNode(currRoot -> right, num);
	}
}

void replaceNode(Node* node, Node* child) {
	Node* otherChild = NULL;
	int leftRight;
	if (child == node -> left && node -> right) {
		otherChild = node -> right;
		leftRight = 1;
	}
	else if (child == node -> right && node -> left) {
		otherChild = node -> left;
		leftRight = 0;
	}

	child -> parent = node -> parent;
	if (node -> parent) {
		if(node == node -> parent -> left) {
			node -> parent -> left = child;
		}
		else {
			node -> parent -> right = child;
		}
	}

	if (otherChild) {
		if (leftRight == 0) {
			child -> left = otherChild;
		}
		else {
			child -> right = otherChild;
		}
	}	
}

void deleteOneChild(Node* node) {
	int nodeData = node -> data;
	Node* child = new Node();
	if (!(node -> right)) {
		child = node -> left;
	}
	else {
		child = node -> right;
	}

	replaceNode(node, child);
	if (node -> redBlack == BLACK) {
		if (child -> redBlack == RED) {
			child -> redBlack = BLACK;
		}
		else {
			deleteCase1(child);
		}
	}
	delete node;
}
			
void deleteCase1(Node* node) {
	if (node -> parent) {
		deleteCase2(node);
	}
}	

void deleteCase2(Node* node) {
	Node* sibling = getSibling(node);

	if(sibling -> redBlack == RED) {
		node -> parent -> redBlack = RED;
		sibling -> redBlack = BLACK;
		if (node == node -> parent -> left) {
			rotateLeft(node -> parent);
		}
		else {
			rotateRight(node -> parent);
		}
	}
	deleteCase3(node);
}

void deleteCase3(Node* node) {
	Node* sibling = getSibling(node);

	if (node -> parent -> redBlack == BLACK && sibling -> redBlack == BLACK && sibling -> left -> redBlack == BLACK && node -> right -> redBlack == BLACK) {
		sibling -> redBlack = RED;
		deleteCase1(node -> parent);
	}
	else {
		deleteCase4(node);
	}
}

void deleteCase4(Node* node) {
	Node* sibling = getSibling(node);

	if (node -> parent -> redBlack == RED && sibling -> redBlack == BLACK && sibling -> left -> redBlack == BLACK && node -> right -> redBlack == BLACK) {
		sibling -> redBlack = RED;
		node -> parent -> redBlack = BLACK;
	}
	else {
		deleteCase5(node);
	}
}	

void deleteCase5(Node* node) {
	Node* sibling = getSibling(node);
	
	if (sibling -> redBlack == BLACK) {
		if (node == node -> parent -> left && sibling -> right -> redBlack == BLACK && sibling -> left -> redBlack == RED) {
			sibling -> redBlack = RED;
			sibling -> left -> redBlack = BLACK;
			rotateRight(sibling);
		}
		else if (node == node -> parent -> right && sibling -> left -> redBlack == BLACK && sibling -> right -> redBlack == RED) {
			sibling -> redBlack = RED;
			sibling -> right -> redBlack = BLACK;
			rotateLeft(sibling);
		}
		deleteCase6(node);
	}
}

void deleteCase6(Node* node) {
	Node* sibling = getSibling(node);
	sibling -> redBlack = node -> parent -> redBlack;
	node -> parent -> redBlack = BLACK;

	if (node == node -> parent -> left) {
		sibling -> right -> redBlack = BLACK;
		rotateLeft(node -> parent);
	}
	else {
		sibling -> left -> redBlack = BLACK;
		rotateRight(node -> parent);
	}
}

void deleteCases(Node* node, Node* &root) {
	if (!(node -> left) && !(node -> right)) {
		if (node -> redBlack == BLACK) {
			node -> parent -> redBlack = BLACK;
		}
		deleteNode(root, node -> data);
	}
	else {
		deleteOneChild(node);
	}
}
