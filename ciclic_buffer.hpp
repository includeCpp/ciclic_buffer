#ifndef __CICLIC_BUFFER_HPP__
#define __CICLIC_BUFFER_HPP__

#define _DEFAULT_CICLIC_BUFFER_STORAGE_SIZE_ = 100

namespace array_impl{
//This structre implements ciclic buffer with fixated storage size
template<typename T>
struct ciclic_buffer{
	using size_type = std::size_t;
	using value_type = T;
	using reference = T&;
	using const_reference = const T&;

public:
	//creates empty buffer with given storage size
	ciclic_buffer(size_type storage_size = _DEFAULT_CICLIC_BUFFER_STORAGE_SIZE_);
	ciclic_buffer(const ciclic_buffer<value_type>& other);
	ciclic_buffer(ciclic_buffer<value_type>&& other);
	ciclic_buffer<value_type>& operator=(const ciclic_buffer<value_type>& other);
	ciclic_buffer<value_type>& operator=(ciclic_buffer<value_type>&& other);
	~ciclic_buffer();

//public methods 
	//adds element to the end of container, throws std::out_of_range if there is no memory for new element
	void push_back(const_reference value);
	//adds element to the beginning of container, throws std::out_of_range if there is no memory for new element
	void push_front(const_reference value);
	//removes element from the back of the container 
	void pop_back() noexcept;
	//removes element from the beginning of the container 
	void pop_front() noexcept;
	//returns i-th element of the container
	reference operator[](size_type i) noexcept;
	//returns i-th element of the container
	const_reference operator[](size_type i) noexcept const;
	//returns i-th element of the container with bounds checking
	reference at(size_type i);
	//returns i-th element of the container with bounds checking
	const_reference at(size_type i) const;

private:
	T* data_;
	T* begin_;
	size_type storage_size_;
	size_type size_;

};

} //namespace array_impl

namespace list_impl{
namespace internal{
template<typename T>
struct list_node{
public:
	list_node(const T& value, T* next = nullptr, T* prev = nullptr);
	T value_;
	T* next_;
	T* prev_;
};
} //namespace internal

//This structre implements ciclic buffer with fixated storage size
template<typename T>
struct ciclic_buffer{
	using size_type = std::size_t;
	using value_type = T;
	using reference = T&;
	using const_reference = const T&;

public:
	//creates empty buffer with given storage size
	ciclic_buffer(size_type storage_size = _DEFAULT_CICLIC_BUFFER_STORAGE_SIZE_);
	ciclic_buffer(const ciclic_buffer<value_type>& other);
	ciclic_buffer(ciclic_buffer<value_type>&& other);
	ciclic_buffer<value_type>& operator=(const ciclic_buffer<value_type>& other);
	ciclic_buffer<value_type>& operator=(ciclic_buffer<value_type>&& other);
	~ciclic_buffer();

//public methods 
	//adds element to the end of container, throws std::out_of_range if there is no memory for new element
	void push_back(const_reference value);
	//adds element to the beginning of container, throws std::out_of_range if there is no memory for new element
	void push_front(const_reference value);
	//removes element from the back of the container 
	void pop_back() noexcept;
	//removes element from the beginning of the container 
	void pop_front() noexcept;
	//returns i-th element of the container
	reference operator[](size_type i) noexcept;
	//returns i-th element of the container
	const_reference operator[](size_type i) noexcept const;
	//returns i-th element of the container with bounds checking
	reference at(size_type i);
	//returns i-th element of the container with bounds checking
	const_reference at(size_type i) const;

private:
	internal::list_node<T>* node; 
	size_type storage_size_;
	size_type size_;
};

} //namespace list_impl

#endif //__CICLIC_BUFFER_HPP__
