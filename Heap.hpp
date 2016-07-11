#pragma once

template<class T>
struct Less
{
	bool operator()(const T& l, const T& r)
	{
		return l < r;
	}
};

template<class T>
struct Big
{
	bool operator()(const T& l, const T& r)
	{
		return l > r;
	}
};

template<class T>
struct Less<T*>
{
	bool operator()(const T*Nodel, const T*Noder)
	{
		return Nodel->_wight < Noder->_wight;
	}
};

template<class T, class Compare = Less<T>>//默认为小堆
class Heap
{

public:
	Heap()
	{}

	Heap(vector<T> a)
	{
		_a.swap(a);

		// 建堆
		for (int i = (_a.size() - 2) / 2; i >= 0; --i)
		{
			_AdjustDown(i);
		}
	}

	Heap(const T* a, size_t size, const T& invalid)
	{
		_a.reserve(size);

		for (size_t i = 0; i < size; ++i)
		{
			if (a[i] != invalid)
			{
				_a.push_back(a[i]);
			}
		}

		//建堆
		for (int i = (_a.size() - 2) / 2; i >= 0; i--)
			//从第一个非叶子结点开始下调，叶子结点可以看作是一个大堆或小堆
		{

			_AdjustDown(i);
		}
	}

	Heap(T* a, size_t size)
	{
		_a.reserve(size);

		for (size_t i = 0; i < size; ++i)
		{
			_a.push_back(a[i]);
		}
		//建堆

		for (int i = (_a.size() - 2) / 2; i >= 0; --i)
		{
			_AdjustDown(i);
		}
		//Disp(_a, size);
	}

	void Pop()
	{
		size_t _size = _a.size();
		assert(_size > 0);
		swap(_a[0], _a[_size - 1]);
		_a.pop_back();
		_size = _a.size();
		_AdjustDown(0);
		//Disp(_a, _size);
	}

	void Push(const T& x)
	{
		_a.push_back(x);
		size_t _size = _a.size();
		_AdjustUp(_size - 1);
		//Disp(_a, _size);
	}

	T& Top()
	{
		assert(!_a.empty());
		return _a[0];
	}

	bool empty()
	{
		return _a.size() == 0;
	}

	size_t Size()
	{
		return _a.size();
	}

protected:
	vector<T> _a;

protected:
	//找最后一个非叶子结点
	//如果当前结点的孩子结点左孩子大于右孩子，就让child指向最大孩子结点（在此必须满足存在右孩子）
	//如果当前结点小于孩子结点，就交换，下调，将孩子给父亲，孩子结点下移
	//不满足  就break；
	void _AdjustDown(size_t parent)    //大堆     下调
	{
		size_t child = parent * 2 + 1;

		while (child < _a.size())
		{
			//compare<T> _com;
			if (child + 1 < _a.size() && (_a[child + 1]<_a[child]))
			{
				++child;
			}
			if ((_a[child]< _a[parent]))
			{
				swap(_a[child], _a[parent]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}
	//上调，传当前结点，令当前节点为孩子结点，上一结点为父结点，
	//在这里不用考虑左右结点谁大谁小
	//如果孩子结点大于父亲结点，交换，上移
	//不满足  就break；
	void _AdjustUp(size_t child)   //上调
	{
		//compare<T> _com;
		size_t parent = (child - 1) / 2;

		while (child > 0)
		{
			if ((_a[child]<_a[parent]))
			{
				swap(_a[child], _a[parent]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
			{
				break;
			}
		}
	}
};


