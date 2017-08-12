#include<iostream>
using namespace std;
#include<gtest/gtest.h>

template<class K,class V,int M>
struct BTreeNode
{
	//多给一个方便分裂
	pair<K, V> _kvs[M];
	BTreeNode<K, V, M> *_subs[M+1]; //M指的是孩子的数量
	BTreeNode<K, V, M> *_parent;
	size_t _size;  //实际存储的结点个数
	BTreeNode()
		:_parent(NULL)
		, _size(0)
	{
		for (int i = 0; i < M + 1; ++i)
		{
			_kvs[i] = make_pair(K(),V()); //_kvs的初始化的值是空
			_subs[i] = NULL;
		}
	}
};

template<class K,class V,int M>
class BTree
{
	typedef BTreeNode<K, V, M> Node;
public:
	BTree()
		:_root(NULL)
	{}
	void InOrder()
	{
		_InOrder(_root);
		cout << endl;
	}
	Node *GetRoot()
	{
		return _root;
	}
	pair<Node *, int> FindNode(const K& key)
	{
		Node *cur = _root;
		Node *parent = NULL;
		while (cur)
		{
			size_t i = 0;
			while (i < cur->_size)
			{
				if (cur->_kvs[i].first == key)
					return make_pair(cur, i);
				else if (cur->_kvs[i].first > key)
					break;
				else
					i++;
			}
			//此时退出有两种情况
			//1、当前结点cur遍历到尾结点了
			//2、找到了正确的插入位置
			parent = cur;
			cur = cur->_subs[i];
		}
		return make_pair(parent,-1); //没找到则返回其父结点
	}
	void InsertKV(Node *cur, pair<K,V> newKV, Node *sub)  
	{
		int end = cur->_size - 1;
		while (end >= 0)
		{
			if (cur->_kvs[end].first < newKV.first)
				break;
			else    //cur->_kvs[end].first > kv.first
			{
				cur->_kvs[end + 1] = cur->_kvs[end];
				cur->_subs[end + 2] = cur->_subs[end+1];  //挪动的是右孩子
				--end;
			}
		}
		cur->_kvs[end + 1] = newKV;
		//更新孩子和父亲
		cur->_subs[end + 2] = sub;
		if (sub)
		{
			sub->_parent = cur;
		}
		cur->_size++;
	}
	bool Insert(pair<K, V> kv)
	{
		if (_root == NULL)   //空树
		{
			_root = new Node;
			_root->_kvs[0] = kv;
			_root->_size = 1;
			return true;
		}
		pair<Node *, int> res = FindNode(kv.first);
		//原来的树中存在该结点，直接错误返回
		if (res.second != -1)   
		{
			return false;
		}
		Node *cur = res.first;
		Node *sub = NULL;
		pair<K,V> newKV = kv;
		while (1)
		{
			InsertKV(cur, newKV, sub);
			if (cur->_size < M) //如果没满则插入成功
			{
				return true;
			}
			//满了则需要分裂
			Node *brother = new Node;
			int mid = cur->_size / 2;
			int j = 0;
			for (size_t i = mid + 1; i < cur->_size; ++i)
			{
				brother->_kvs[j] = cur->_kvs[i];
				if (cur->_subs[i])
				{
					brother->_subs[j] = cur->_subs[i];  //拷贝左孩子
					brother->_subs[j + 1] = sub;   //拷贝右孩子
					cur->_subs[i] = NULL;
					//更新左右孩子的父节点
					brother->_subs[j]->_parent = brother;
					brother->_subs[j + 1]->_parent = brother;
				}
				brother->_size++;
				cur->_kvs[i] = make_pair(K(),V());
				cur->_size--;
				j++;
			}
			//分裂到了根结点
			if (cur->_parent == NULL)
			{
				_root = new Node;
				//将mid的位置拷贝到上层，更新父亲的左右孩子
				_root->_kvs[0] = cur->_kvs[mid];
				_root->_subs[0] = cur;
				_root->_subs[1] = brother;
				_root->_size++;

				cur->_kvs[mid] = make_pair(K(),V());
				cur->_size--;
				//更新cur、brother的父结点
				cur->_parent = _root;
				brother->_parent = _root;
				return true;
			}
			//尚未更新到父结点
			newKV = cur->_kvs[mid];
			cur->_kvs[mid] = make_pair(K(),V());
			cur->_size--;
			sub = brother;
			cur = cur->_parent;
		}
		return true;
	}
private:
	void _InOrder(Node *root) //B树的中序遍历
	{
		if (root == NULL)
			return;
		size_t i = 0;
		for (; i < root->_size; i++)
		{
			_InOrder(root->_subs[i]);
			cout << root->_kvs[i].first << " ";
		}
		_InOrder(root->_subs[i]);
	}
protected:
	Node *_root;
};
