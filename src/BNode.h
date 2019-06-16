#pragma once
#include<iostream>
#include<vector>
#include<cstring>

template <typename T>
using link=std::pair<T, int>;

#define INF 0x7FFFFFFF

template <typename T>
class BNode
{
public:
	BNode();
	BNode(BNode* L);
	~BNode();
	int getSize();
	bool clear();
	void insert(link<T>);
	void insert(BNode*);
	void setParent(BNode*);
	void setLeaf();
	bool isLeaf();
	BNode* parent();
	BNode* nearNode();
	link<T> midKey(BNode *);
	void print();
	bool contain(link<T>);
	// Node size

	// If node is leaf
	bool leaf;

	BNode* fa;
	// Node keys
	std::vector<link<T>> keys;
	// Node pointers
	std::vector<BNode*>ptrs;
	BNode* nex;
private:
};


template <typename T>
BNode<T>::BNode()
{
	keys.clear();
	ptrs.clear();
	leaf = 0;
}

template <typename T>
BNode<T>::BNode(BNode* L)
{
	leaf = L->leaf;
	keys.clear();
	ptrs.clear();
	for (int i = 0; i < L->keys.size(); i++)keys.push_back(L->keys[i]);
	for (int i = 0; i < L->ptrs.size(); i++)ptrs.push_back(L->ptrs[i]);
}

template <typename T>
BNode<T>::~BNode()
{

}

template <typename T>
BNode<T>* BNode<T>::parent() 
{
	return fa;
}

template <typename T>
void BNode<T>::insert(link<T> key)
{
	keys.push_back(key);
}

template <typename T>
void BNode<T>::setParent(BNode* node)
{
	fa = node;
}

template <typename T>
void BNode<T>::setLeaf()
{
	leaf = 1;
	nex = NULL;
}

template <typename T>
bool BNode<T>::isLeaf()
{
	return leaf;
}

template <typename T>
void BNode<T>::insert(BNode* ntr)
{
	ptrs.push_back(ntr);
}

template <typename T>
bool BNode<T>::contain(link<T> key)
{
	int tag = 0, SIZE = getSize();
	for (int i = 0; i < SIZE; i++)
		if (keys[i].first == key.first || keys[i].second == key.second)tag = 1;
	return tag;
}

template <typename T>
int BNode<T>::getSize()
{
	return keys.size();
}

template <typename T>
bool BNode<T>::clear()
{
	keys.clear();
	ptrs.clear();
	return true;
}

template <typename T>
BNode<T>* BNode<T>::nearNode()
{
	int SIZE = fa->getSize(), pos = -1;
	BNode* p;
	for (int i = 0; i <= SIZE; i++)
		if (fa->ptrs[i] == this) { pos = i; break; }
	if (pos < SIZE)p = fa->ptrs[pos + 1];
	else p = fa->ptrs[pos - 1];
	return p;
}

template <typename T>
link<T> BNode<T>::midKey(BNode* G)
{
	int SIZE = fa->getSize(), pos = -1;
	for (int i = 0; i < SIZE; i++)
		if (fa->ptrs[i] == G || fa->ptrs[i] == this) { pos = i; break; }
	link<T> value = fa->keys[pos];
	return value;
}

template <typename T>
void BNode<T>::print()
{
	if (this == NULL)
	{
		printf("NULL point\n");
		return;
	}
	printf("Node:\n");
	int SIZE = getSize();
	for (int i = 0; i < SIZE; i++)printf("%d ", keys[i].first);
	printf("\n\n");
}