#pragma once

#include<queue>

template<class T>
struct HuffmanNode
{
	HuffmanNode<T>* _left;
	HuffmanNode<T>* _right;
	T _weight;  //权值
	HuffmanNode(const T& wight)
		:_left(NULL)
		, _right(NULL)
		,_weight(wight)
	{}
};

template<class T>
class HuffmanTree
{
	typedef HuffmanNode<T> Node;
public:
	HuffmanTree()
		:_root(NULL)
	{}

	~HuffmanTree()
	{
		if (_root)
		{
			_Destroy(_root);
		}
	}

	HuffmanTree(const T*a, size_t size, const T&invalid)
	{
		_root = CreateTree(a, size, invalid);
	}

	Node* CreateTree(const T*a, size_t size, const T&invalid)
	{
		Heap<Node*, Less<Node*>> minHeap;

		for (size_t i = 0; i < size; ++i)
		{
			if (a[i] != invalid)
			{
				Node* tmp = new Node(a[i]);
				minHeap.Push(tmp);
			}
		}

		while (!minHeap.empty())
		{
			Node* left = minHeap.Top();
			minHeap.Pop();
			Node* right = NULL;

			if (!minHeap.empty())
			{
				right = minHeap.Top();
				minHeap.Pop();
			}

			Node* parent = NULL;

			if (right)
			{
				parent = new Node(left->_weight + right->_weight);
			}
			else
			{
				parent = new Node(left->_weight);
			}

			parent->_left = left;
			parent->_right = right;

			if (minHeap.empty())
			{
				return parent;
			}

			minHeap.Push(parent);
		}

		return NULL;
	}

	Node* GetRootNode()
	{
		return _root;
	}

	void LeveLorder()    //层序
	{
		queue<Node*> tmp;

		if (_root == NULL)
			return;
		else
		{
			tmp.push(_root);
			while (!tmp.empty())
			{
				Node* Front = tmp.front();
				cout << Front->_data << " ";
				tmp.pop();

				if (Front->_left)
				{
					tmp.push(Front->_left);
				}

				if (Front->_right)
				{
					tmp.push(Front->_right);
				}
			}
		}
	}

protected:
	void _Destroy(Node* root)
	{
		if (root->_left == NULL && root->_right == NULL)
		{
			delete root;
			root = NULL;
		}
		else
		{
			_Destroy(root->_left);
			_Destroy(root->_right);
		}
	}

protected:
		Node* _root;
};
