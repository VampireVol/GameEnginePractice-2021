#pragma once
#include <memory.h>

template <class T>
class MTQueue
{
private:
	T* m_pData;
	size_t m_nSize;
	size_t m_nCapacity;
public:
	MTQueue();
	~MTQueue();
	T* allocate();
	T* deallocate();
	T* Resize(size_t nNewSize);
	// Очистить массив
	void Clear();
	// Получить размер файла
	size_t capacity();
	// Лучше другой оператор, например *, чтобы работать было ещё красивее
	T* operator()();
};

template<class T>
inline MTQueue<T>::MTQueue()
	: m_pData(nullptr), m_nSize(0), m_nCapacity(0)
{
}

template<class T>
MTQueue<T>::~MTQueue()
{
	if (m_pData)
		delete m_pData;
}

template<class T>
T* MTQueue<T>::allocate()
{
	return new T[m_nCapacity];
}

template<class T>
T* MTQueue<T>::deallocate()
{
	if (m_pData)
		delete m_pData;
	return nullptr;
}

template<class T>
T* MTQueue<T>::Resize(size_t nNewSize)
{
	m_nSize = m_nCapacity;
	m_nCapacity = nNewSize;
	auto newData = allocate();
	if (m_pData)
		memcpy(newData, m_pData, m_nCapacity);
	m_pData = deallocate();
	m_pData = newData;
	return m_pData + m_nSize;
}

template<class T>
void MTQueue<T>::Clear()
{
	m_pData = deallocate();
	m_nSize = 0;
	m_nCapacity = 0;
}

template<class T>
size_t MTQueue<T>::capacity()
{
	return m_nCapacity;
}

template<class T>
T* MTQueue<T>::operator()()
{
	return m_pData;
}

