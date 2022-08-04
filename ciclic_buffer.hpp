#ifndef __CICLIC_BUFFER_HPP__
#define __CICLIC_BUFFER_HPP__

#include<exception>

#define _DEFAULT_STORAGE_SIZE_ 100

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
	ciclic_buffer(size_type storage_size = _DEFAULT_STORAGE_SIZE_);
	ciclic_buffer(const ciclic_buffer<value_type>& other);
	ciclic_buffer(ciclic_buffer<value_type>&& other) noexcept;
	ciclic_buffer<value_type>& operator=(const ciclic_buffer<value_type>& other);
	ciclic_buffer<value_type>& operator=(ciclic_buffer<value_type>&& other) noexcept;
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
	const_reference operator[](size_type i) const noexcept;
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
	list_node(list_node<T>* next = nullptr, list_node<T>* prev = nullptr);
	T* value_ptr_;
	list_node<T>* next_;
	list_node<T>* prev_;
};
} //namespace internal

//This structure implements ciclic buffer with fixated storage size
template<typename T>
struct ciclic_buffer{
	using size_type = std::size_t;
	using value_type = T;
	using reference = T&;
	using const_reference = const T&;

public:
	//creates empty buffer with given storage size
	ciclic_buffer(size_type storage_size = _DEFAULT_STORAGE_SIZE_);
	ciclic_buffer(const ciclic_buffer<value_type>& other);
	ciclic_buffer(ciclic_buffer<value_type>&& other) noexcept;
	ciclic_buffer<value_type>& operator=(const ciclic_buffer<value_type>& other);
	ciclic_buffer<value_type>& operator=(ciclic_buffer<value_type>&& other) noexcept;
	~ciclic_buffer();

//public methods 
	//adds element to the end of container, throws std::out_of_range if there is no memory for new element
	void push_back(const_reference value);  //++
	//adds element to the beginning of container, throws std::out_of_range if there is no memory for new element
	void push_front(const_reference value);
	//removes element from the back of the container 
	void pop_back();
	//removes element from the beginning of the container 
	void pop_front();
	//returns i-th element of the container
	reference operator[](size_type i) noexcept;
	//returns i-th element of the container
	const_reference operator[](size_type i) const noexcept;
	//returns i-th element of the container with bounds checking
	reference at(size_type i);
	//returns i-th element of the container with bounds checking
	const_reference at(size_type i) const;

private:
	internal::list_node<T>* first_; 
	internal::list_node<T>* last_; 
	const size_type storage_size_;
	size_type size_;
};

} //namespace list_impl

//---------------------------------------------------------------------------------------------------------------
//										Implimentation part starts here
//---------------------------------------------------------------------------------------------------------------

template<typename T>
list_impl::internal::list_node<T>::list_node(list_node<T>* next, list_node<T>* prev) : next_(next), prev_(prev){
	value_ptr_ = reinterpret_cast<T*>(std::malloc(sizeof(T)));
	if(nullptr == value_ptr_){
		throw std::bad_alloc{};
	}
}

template<typename T>
void list_impl::ciclic_buffer<T>::push_back(const_reference value){
	if(size_ == storage_size_){
		throw std::logic_error{"Buffer overflow!"};
	} else {
		last_ = last_ -> next_;
		new(last_ -> value_ptr_) T{value};
		size_++;
	}
}

template<typename T>
void list_impl::ciclic_buffer<T>::push_front(const_reference value){
	if(size_ == storage_size_){
		throw std::logic_error{"Buffer overflow!"};
	} else {
		first_ = first_ -> prev_;
		new(first_ -> value_ptr_) T{value};
		size_++;
	}
}

template<typename T>
void list_impl::ciclic_buffer<T>::pop_back(){
	if(0 == size_){
		throw std::logic_error{"Buffer is empty"};
	} else {
		last_ -> value_ptr_ -> ~T();
		last_ = last_ -> prev_;
		size_--;
	}
}

template<typename T>
void list_impl::ciclic_buffer<T>::pop_front(){
	if(0 == size_){
		throw std::logic_error{"Buffer is empty"};
	} else {
		first_ -> value_ptr_ -> ~T();
		first_ = first_ -> next_;
		size_--;
	}
}

template<typename T>
typename list_impl::ciclic_buffer<T>::reference list_impl::ciclic_buffer<T>::operator[](size_type i) noexcept{
	if((size_ / 2) < i){
		list_impl::internal::list_node<T>* p = first_;
		for(size_type j = 0; j < i; j++){
			p = p -> next_;
		}
		return *(p -> value_);
	} else {
		list_impl::internal::list_node<T>* p = last_;
		for(size_type j = size_; j > i; j--){
			p = p -> prev_;
		}
		return *(p -> value_);
	}
}

template<typename T>
typename list_impl::ciclic_buffer<T>::const_reference list_impl::ciclic_buffer<T>::operator[](size_type i) const noexcept{
	if((size_ / 2) < i){
		list_impl::internal::list_node<T>* p = first_;
		for(size_type j = 0; j < i; j++){
			p = p -> next_;
		}
		return *(p -> value_);
	} else {
		list_impl::internal::list_node<T>* p = last_;
		for(size_type j = size_; j > i; j--){
			p = p -> prev_;
		}
		return *(p -> value_);
	}
}

template<typename T>
typename list_impl::ciclic_buffer<T>::reference list_impl::ciclic_buffer<T>::at(size_type i){
	if(i > size_){
		throw std::out_of_range("Invalid index");
	} else {
		return this -> operator[](i);
	}
} 

template<typename T>
typename list_impl::ciclic_buffer<T>::const_reference list_impl::ciclic_buffer<T>::at(size_type i) const{
	if(i > size_){
		throw std::out_of_range("Invalid index");
	}
	else {
		return this -> operator[](i);
	}
} 

//---------------------------------------------------------------------------------------------------------------
//													  	Array													
//---------------------------------------------------------------------------------------------------------------

template<typename T>
array_impl::ciclic_buffer<T>::ciclic_buffer(size_type storage_size) : storage_size_(storage_size){
	data_ = reinterpret_cast<T*>(std::malloc(sizeof(T) * storage_size_));
	begin_ = data_;
	for(int i = 0; i < storage_size_; i++){
		new(data_ + i) T{};
	}
	size_ = 0;
}

template<typename T>
array_impl::ciclic_buffer<T>::ciclic_buffer(const ciclic_buffer<value_type>& other) : storage_size_(other.storage_size_), size_(other.size_) {
	data_ = reinterpret_cast<T*>(std::malloc(sizeof(T) * other.storage_size_));
	for(int i = 0; i < size_; i++){
		new(data_ + i) T{other.data_ + i};
	}
}

template<typename T>
array_impl::ciclic_buffer<T>::ciclic_buffer(ciclic_buffer<value_type>&& other) noexcept : data_(other.data_), begin_(other.begin_), 
storage_size_(other.storage_size_), size_(other.size_) {
	other.data_ = nullptr;
	other.begin_ = nullptr;
	other.storage_size_ = 0;
	other.size_ = 0;
}

template<typename T>
typename array_impl::ciclic_buffer<T>::ciclic_buffer<T>& operator=(const ciclic_buffer<T>& other){  //почему не можем писать <value_type>
	for(int i = 0; i < storage_size_; i++){
		(data_ + i) -> ~T();
	}
	storage_size_ = other.storage_size_;
	size_ = other.size_;
	data_ = reinterpret_cast<T*>(std::malloc(sizeof(T) * storage_size_));
	for(int i = 0; i < storage_size_; i++){
		new(data_ + i) T{other.data_};
	}
}

template<typename T>
typename array_impl::ciclic_buffer<T>::ciclic_buffer<T>& operator=(ciclic_buffer<T>&& other) noexcept {
	for(int i = 0; i < storage_size_; i++){
		(data_ + i) -> ~T();
	}
	data_ = other.data_;
	begin_ = other.begin;
	storage_size_ = other.storage_size_;
	size_ = other.size_;
	other.data_ = nullptr;
	other.begin_ = nullptr;
	other.storage_size_ = 0;
	other.size_ = 0;
}

/*
void array_impl::ciclic_buffer::push_back(const_reference value){
	if(size_ == storage_size_){
		storage_size_ *= 2;
	}
	new(data_ + size_) value_type{value};
}

void array_impl::ciclic_buffer::push_front(const_reference value){
	
}

void array_impl::ciclic_buffer::pop_back(){
	~value_type(data_ + (size_ - 1));
	std::free(data + (size_ - 1));
	data_++;
	begin_ = data_;
	size_--;
}

void array_impl::ciclic_buffer::pop_front(){
	data_++;
	~value_type(begin_);
	std::free(begin_);
	begin_ = data_;
	size_--;
}
*/
#endif //__CICLIC_BUFFER_HPP__





