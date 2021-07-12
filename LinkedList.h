#pragma once
struct Node {
	int value;
	int freq;
	Node* pre;
	Node* nxt;
};

struct LinkedList {
	Node* pHead;
	Node* pTail;
};

LinkedList *linkedListInit();
bool isEmpty(LinkedList* ls);

Node* findPos(LinkedList* ls, int pos);
Node* findValue(LinkedList* ls, int value);
Node* findTail(LinkedList* ls);

Node* nodeInit(int value = 0, int freq = 0);
void linkNode(Node* a, Node* b);

void insertFront(LinkedList* ls, int value, int freq = 0);
void insertBack(LinkedList* ls, int value, int freq = 0);
void insertPos(LinkedList* ls, int pos, int value);
void insertKeepOrder(LinkedList* ls, int value);

void eraseFront(LinkedList* ls);
void eraseBack(LinkedList* ls);
void erasePos(LinkedList* ls, int pos);
bool eraseValue(LinkedList* ls, int value);
void eraseAll(LinkedList* ls);

void eraseLinkedList(LinkedList* ls);

void printFoward(LinkedList* ls);
void printBackward(LinkedList* ls);