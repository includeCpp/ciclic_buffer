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
	size_type indent_;
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
	size_type storage_size_;
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
//TODO add exception handling
template<typename T> //++
list_impl::ciclic_buffer<T>::ciclic_buffer(size_type storage_size) : storage_size_(storage_size), size_(0) {
	internal::list_node<T>* node_ptr = nullptr;
	if(storage_size_ == 1){
		node_ptr = new internal::list_node<T>();
		node_ptr -> next_ = node_ptr;
		node_ptr -> prev_ = node_ptr;
		first_ = node_ptr;
	} else{
		for(size_type i = 0; i < storage_size_; i++){
			if(i == 0){
				node_ptr = new internal::list_node<T>();
				first_ = node_ptr;
				last_ = node_ptr;
			} 
			else if((i > 0) && (i < (storage_size_ - 1))) {
				node_ptr = new internal::list_node<T>(nullptr, last_);
				last_ -> next_ = node_ptr;
				last_ = node_ptr;
			} else {
				node_ptr = new internal::list_node<T>(first_, last_);
				first_ -> prev_ = node_ptr;
			}
		}
	}
	last_ = first_;
}

template<typename T> //++
list_impl::ciclic_buffer<T>::ciclic_buffer(const ciclic_buffer<value_type>& other) : storage_size_(other.storage_size_), size_(other.size_)
{
	internal::list_node<T>* node_ptr = nullptr;
	internal::list_node<T>* node_tmp_ptr = nullptr;
	internal::list_node<T>* first_el = nullptr;
	if(storage_size_ == 1){
		node_ptr = new internal::list_node<T>();
		node_ptr -> value_ptr_ = &other[0];
		node_ptr -> next_ = node_ptr;
		node_ptr -> prev_ = node_ptr;
		return;
	}
	for(size_type i = 0; i < storage_size_; i++){
		if(i == 0){
			node_ptr = new internal::list_node<T>();
			node_ptr -> value_ptr_ = &other[i];
			node_tmp_ptr = node_ptr;
			first_el = node_ptr;
		}
		else if(i > 0 && i < storage_size_ - 2) {
			node_ptr = new internal::list_node<T>();
			node_ptr -> value_ptr_ = &other[i];
			node_tmp_ptr -> next_ = node_ptr;
			node_ptr -> prev_ = node_tmp_ptr;
			node_tmp_ptr = node_ptr;
		} else {
			node_ptr = new internal::list_node<T>();
			node_ptr -> value_ptr_ = &other[i];
			node_tmp_ptr -> next_ = node_ptr;
			node_ptr -> prev_ = node_tmp_ptr;
			node_tmp_ptr = node_ptr;
			node_tmp_ptr -> next_ = first_el;
			first_el -> prev_ = node_tmp_ptr;
		}
	}
}

template<typename T>
list_impl::ciclic_buffer<T>::ciclic_buffer(ciclic_buffer<value_type>&& other) noexcept : storage_size_(other.storage_size_), size_(other.size_) 
{
	//clear
	internal::list_node<T>* node_ptr = nullptr;
	internal::list_node<T>* node_tmp_ptr = nullptr;
	internal::list_node<T>* first_el = nullptr;
	if(storage_size_ == 1){

	}
	for(size_type i = 0; i < storage_size_; i++){
		if(i == 0){
			node_ptr = new internal::list_node<T>();
			node_ptr -> value_ptr_ = &other[i];
			node_tmp_ptr = node_ptr;
			first_el = node_ptr;
			other[i] -> value_ptr_ = nullptr;
		}
		else if(i > 0 && i < storage_size_ - 2) {
			node_ptr = new internal::list_node<T>();
			node_ptr -> value_ptr_ = &other[i];
			node_tmp_ptr -> next_ = node_ptr;
			node_ptr -> prev_ = node_tmp_ptr;
			node_tmp_ptr = node_ptr;
			other[i] -> value_ptr_ = nullptr;
		} else {
			node_ptr = new internal::list_node<T>();
			node_ptr -> value_ptr_ = &other[i];
			node_tmp_ptr -> next_ = node_ptr;
			node_ptr -> prev_ = node_tmp_ptr;
			node_tmp_ptr = node_ptr;
			node_tmp_ptr -> next_ = first_el;
			first_el -> prev_ = node_tmp_ptr;
			other[i] -> value_ptr_ = nullptr;
		}
	}
	other.storage_size_ = 0;
	other.size_ = 0;
}

template<typename T>
list_impl::ciclic_buffer<T>& list_impl::ciclic_buffer<T>::operator=(const ciclic_buffer<T>& other) {
	
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
		return *(p -> value_ptr_);
	} else {
		list_impl::internal::list_node<T>* p = last_;
		for(size_type j = size_; j > i; j--){
			p = p -> prev_;
		}
		return *(p -> value_ptr_);
	}
}

template<typename T>
typename list_impl::ciclic_buffer<T>::const_reference list_impl::ciclic_buffer<T>::operator[](size_type i) const noexcept{
	if((size_ / 2) < i){
		list_impl::internal::list_node<T>* p = first_;
		for(size_type j = 0; j < i; j++){
			p = p -> next_;
		}
		return *(p -> value_ptr_);
	} else {
		list_impl::internal::list_node<T>* p = last_;
		for(size_type j = size_; j > i; j--){
			p = p -> prev_;
		}
		return *(p -> value_ptr_);
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
array_impl::ciclic_buffer<T>::ciclic_buffer(size_type storage_size) : indent_(0), storage_size_(storage_size), size_(0){
	data_ = reinterpret_cast<T*>(std::malloc(sizeof(T) * storage_size_));
	if(data_ == nullptr){
		throw std::bad_alloc{};
	}
}

template<typename T>
array_impl::ciclic_buffer<T>::ciclic_buffer(const ciclic_buffer<value_type>& other) : indent_(other.indent_), storage_size_(other.storage_size_), size_(other.size_) {
	data_ = reinterpret_cast<T*>(std::malloc(sizeof(T) * other.storage_size_));
	if(data_ == nullptr){
		throw std::bad_alloc{};
	}
	for(size_type i = 0; i < size_; i++){
		new(data_ + (i + indent_) % storage_size_) T{other.data_ + (i + indent_) % storage_size_};
	}
}

template<typename T>
array_impl::ciclic_buffer<T>::ciclic_buffer(ciclic_buffer<value_type>&& other) noexcept : data_(other.data_), indent_(other.indent_), 
storage_size_(other.storage_size_), size_(other.size_) {
	other.data_ = nullptr;
	other.indent_ = 0;
	other.storage_size_ = 0;
	other.size_ = 0;
}

//optimization note try to avoid multiple memory reallocation
//TODO change logic to calling throwing methods first than changing methods without exceptions 
template<typename T>
array_impl::ciclic_buffer<T>& array_impl::ciclic_buffer<T>::operator=(const ciclic_buffer<T>& other){
	for(size_type i = 0; i < size_; i++){
		(data_ + (i + indent_) % storage_size_) -> ~T();
	}
	std::free(data_); 
	storage_size_ = other.storage_size_;
	size_ = other.size_;
	data_ = reinterpret_cast<T*>(std::malloc(sizeof(T) * storage_size_));
	if(data_ == nullptr){
		throw std::bad_alloc{};
	}
	for(size_type i = 0; i < size_; i++){
		new(data_ + (i + indent_) % storage_size_) T{other.data_ + (i + indent_) % storage_size_};
	}
}

template<typename T>
array_impl::ciclic_buffer<T>& array_impl::ciclic_buffer<T>::operator=(ciclic_buffer<T>&& other) noexcept {
	for(size_type i = 0; i < size_; i++){
		(data_ + (i + indent_) % storage_size_) -> ~T();
	}
	std::free(data_);
	data_ = other.data_;
	indent_ = other.indent_;
	storage_size_ = other.storage_size_;
	size_ = other.size_;
	other.data_ = nullptr;
	other.indent_ = 0;
	other.storage_size_ = 0;
	other.size_ = 0;
}

template<typename T>
array_impl::ciclic_buffer<T>::~ciclic_buffer(){
	for(size_type i = 0; i < size_; i++){
		(data_ + (i + indent_) % storage_size_) -> ~T();
	}
	std::free(data_);
};

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





