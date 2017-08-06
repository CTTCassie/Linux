#include<iostream>
using namespace std;
#include<cassert>
#include<gtest/gtest.h>

template<class T>
class MyVector
{
public:
	MyVector()
		:_pData(NULL)
		,_capacity(0)
		,_size(0)
	{}
	MyVector(size_t n,const T& data)
		:_pData(new T[n])
		,_capacity(n)
		,_size(n)
	{
		for(int i=0;i<_size;i++)
		{
			_pData[i]=data;
		}
	}
	//MyVector<int> V2(v1);
	MyVector(const MyVector<T>& v)  //拷贝构造函数
		:_pData(new T[v._size])
		,_capacity(v._size)
		,_size(v._size)
	{
		for(int i=0;i<v._size;i++)
		{
			_pData[i]=v._pData[i];
		}
		//memcpy(_pData,v._pData,sizeof(T)*_size);
	}
	MyVector& operator=(const MyVector<T>& v) //赋值运算符的重载
	{
		if(this != &v)
		{
			MyVector<T> tmp(v);
			Swap(tmp);
		}
		return *this;
	}
	~MyVector()  //析构函数
	{
		if(_pData != NULL)
		{
			delete[]_pData;
			_pData=NULL;
		}
		_capacity=0;
		_size=0;
	}
public:
	void Swap(MyVector<T> v)
	{
		std::swap(_pData,v._pData);
		std::swap(_capacity,v._capacity);
		std::swap(_size,v._size);
	}
	void PushBack(const T&data)
	{
		_CheckCapacity();
		_pData[_size++]=data;
	}
	void PopBack()
	{
		assert(_size > 0);
		_size--;
	}
	void Insert(const int& pos,const T& data)
	{
		assert(pos >=0 && pos < _size); //所给的位置下标是否正确
		_CheckCapacity();   //检查容量是否已满
		if(_size-1 == pos){ //尾插
			PushBack(data);
		}
		else{   //不是尾插
			for(int i=_size-1;i>=pos;i--){
				_pData[i+1]=_pData[i];
			}
			_pData[pos]=data;
			_size++;
		}
	}
	void Erase(const int& pos)
	{
		assert(pos >= 0 && pos < _size);
		for(int i=pos+1;i<_size;i++){
			_pData[i-1]=_pData[i];
		}
		_size--;
	}
	void Clear()
	{
		_size=0;
	}
	T& operator[](int index)
	{
		assert(index >= 0 && index < _size);
		return _pData[index];
	}
	const T& Front()const
	{
		assert(_size > 0);
		return _pData[0];
	}
	const T& Back()const
	{
		assert(_size > 0);
		return _pData[_size-1];
	}
	size_t Size()
	{
		return _size;
	}
	size_t Capacity()
	{
		return _capacity;
	}
	void Resize(size_t nums,T c=T())  //修改Vector的大小
	{
		if(nums > _size){ //新扩充的元素个数比当前的大
			T *pData=new T[nums];
			for(int i=0;i<_size;i++){
				pData[i]=_pData[i];
			}
			for(int i=_size;i<nums;i++){
				pData[i]=c;
			}
			delete[]_pData;
			_pData=pData;
			_size=nums;
		}
		else{  //新扩充元素的个数比当前的元素个数小
			for(int i=nums;i<_size;i++){
				_pData[i]=0;
			}
			_size -= nums;
		}
	}
	void Reverse(size_t nums) //修改容量
	{
		if(nums > _capacity){
			T *pData=new int[nums];
			for(int i=0;i<_size;i++){
				pData[i]=_pData[i];
			}
			delete[]_pData;
			_pData=pData;
			_capacity=nums;
		}
	}
	bool Empty()
	{
		return _size == 0;
	}
private:
	void _CheckCapacity()
	{
		if(_size == _capacity)
		{
			int newCapacity=2*_capacity+3;
			T *pData=new T[newCapacity];

			memcpy(pData,_pData,sizeof(T)*_size);   //如果T是string的话容易出问题

			delete[]_pData;
			_pData=pData;
			_capacity=newCapacity;
			_size=_size;
		}
	}
protected:
	T *_pData;
	size_t _capacity;//容量
	size_t _size;    //实际存储的数据个数
};
