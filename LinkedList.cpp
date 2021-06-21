#include "LinkedList.h"
#include <iostream>

LinkedList* linkedListInit()
{
	LinkedList *ans = new LinkedList();
	ans->pHead = nullptr;
	ans->pTail = nullptr;

	return ans;
}

bool isEmpty(LinkedList* ls)
{
	if (!ls->pHead && !ls->pTail)
		return true;

	return false;
}

Node* findPos(LinkedList* ls, int pos)
{
	if (isEmpty(ls))
		return nullptr;

	int cnt = 0;
	for (Node* iter = ls->pHead; iter; iter = iter->nxt, cnt++) {
		if (cnt == pos)
			return iter;
	}

	return nullptr;
}

Node* findValue(LinkedList* ls, int value)
{
	if (isEmpty(ls))
		return nullptr;

	for (Node* iter = ls->pHead; iter; iter = iter->nxt) {
		if (iter->value == value)
			return iter;
	}

	return nullptr;
}

Node* findTail(LinkedList* ls)
{
	if (isEmpty(ls))
		return nullptr;

	return ls->pTail;
}

Node* nodeInit(int value)
{
	Node* ans = new Node();
	ans->pre = ans->nxt = nullptr;
	ans->value = value;

	return ans;
}

void linkNode(Node* a, Node* b)
{
	a->nxt = b;
	b->pre = a;
}

void insertFront(LinkedList* ls, int value)
{
	Node* newHead = nodeInit(value);

	if (isEmpty(ls)) {
		ls->pHead = ls->pTail = newHead;
		return;
	}

	linkNode(newHead, ls->pHead);
	ls->pHead = newHead;
}

void insertBack(LinkedList* ls, int value)
{
	if (isEmpty(ls)) {
		insertFront(ls, value);
		return;
	}

	Node* newTail = nodeInit(value);
	linkNode(ls->pTail, newTail);
	ls->pTail = newTail;
}

void insertPos(LinkedList* ls, int pos, int value)
{
	if (isEmpty(ls))
		return;

	Node* foo = findPos(ls, pos);
	if (!foo)
		return;

	Node* newNode = nodeInit(value);
	Node* bar = foo->nxt;

	linkNode(foo, newNode);

	if (bar)
		linkNode(newNode, bar);
	else
		ls->pTail = newNode;
}

void insertKeepOrder(LinkedList* ls, int value)
{
	if (isEmpty(ls)) {
		insertFront(ls, value);
		return;
	}

	if (ls->pTail->value <= value) {
		insertBack(ls, value);
		return;
	}

	if (value <= ls->pHead->value) {
		insertFront(ls, value);
		return;
	}

	for (Node* iter = ls->pHead; iter; iter = iter->nxt) {
		if (value < iter->value) {
			Node* pre = iter->pre;
			Node* newNode = nodeInit(value);
			linkNode(pre, newNode);
			linkNode(newNode, iter);
			break;
		}
	}
}

void eraseFront(LinkedList* ls)
{
	if (isEmpty(ls))
		return;

	if (!ls->pHead->nxt) {
		delete[] ls->pHead;
		ls->pHead = ls->pTail = nullptr;
		return;
	}

	Node* after = ls->pHead->nxt;

	delete[] ls->pHead;
	after->pre = nullptr;
	ls->pHead = after;
}

void eraseBack(LinkedList* ls) {
	if (isEmpty(ls)) {
		return;
	}

	if (!ls->pHead->nxt) {
		eraseFront(ls);
		return;
	}

	Node* pre = ls->pTail->pre;
	delete[] ls->pTail;
	pre->nxt = nullptr;
	ls->pTail = pre;
}

void erasePos(LinkedList* ls, int pos)
{
	Node* foo = findPos(ls, pos + 1);
	
	if (!foo)
		return;

	if (foo == ls->pHead) {
		eraseFront(ls);
		return;
	}

	if (foo == ls->pTail) {
		eraseBack(ls);
		return;
	}

	Node* pre = foo->pre;
	Node* after = foo->nxt;

	linkNode(pre, after);
	delete[] foo;
}

void eraseValue(LinkedList* ls, int value)
{
	Node* foo = findValue(ls, value);
	
	if (!foo)
		return;

	if (foo == ls->pHead) {
		eraseFront(ls);
		return;
	}

	if (foo == ls->pTail) {
		eraseBack(ls);
		return;
	}

	Node* pre = foo->pre;
	Node* after = foo->nxt;

	linkNode(pre, after);
	delete[] foo;
}

void eraseAll(LinkedList* ls)
{
	while (!isEmpty(ls))
		eraseFront(ls);
}

void eraseLinkedList(LinkedList* ls)
{
	eraseAll(ls);
	delete[] ls;
}

void printFoward(LinkedList* ls)
{
	std::cout << "[";
	for (Node* iter = ls->pHead; iter; iter = iter->nxt) {
		std::cout << iter->value;
		if (iter->nxt)
			std::cout << ' ';
	}
	std::cout << "]" << std::endl;
}

void printBackward(LinkedList* ls)
{
	std::cout << "[";
	for (Node* iter = ls->pTail; iter; iter = iter->pre) {
		std::cout << iter->value;
		if (iter->pre)
			std::cout << ' ';
	}
	std::cout << "]" << std::endl;
}