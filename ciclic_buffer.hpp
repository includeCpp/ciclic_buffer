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
	//
	void clear() noexcept;

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
	~list_node() noexcept;
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
	//clears all elements without affecting capacity
	void clear() noexcept;

private:
	internal::list_node<T>* first_; 
	internal::list_node<T>* last_; 
	size_type storage_size_;
	size_type size_;
	//
	void clear_and_free() noexcept;
	void buffer_allocation(size_type storage_size);
	void constructor_call(const ciclic_buffer<value_type>& other);
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
list_impl::internal::list_node<T>::~list_node() noexcept{
	std::free(value_ptr_);
};

//TODO add exception handling
template<typename T> //++
list_impl::ciclic_buffer<T>::ciclic_buffer(size_type storage_size){
	buffer_allocation(storage_size);
}

//TODO add exception handling 
template<typename T> //++
list_impl::ciclic_buffer<T>::ciclic_buffer(const ciclic_buffer<value_type>& other) : ciclic_buffer(other.storage_size_) {
	constructor_call(other);
}

template<typename T>
list_impl::ciclic_buffer<T>::ciclic_buffer(ciclic_buffer<value_type>&& other) noexcept : first_(other.first_), last_(other.last_), 
storage_size_(other.storage_size_), size_(other.size_) {
	other.first_ = nullptr;
	other.last_ = nullptr;
	other.storage_size_ = 0;
	other.size_ = 0;
}

template<typename T>
list_impl::ciclic_buffer<T>& list_impl::ciclic_buffer<T>::operator=(const ciclic_buffer<T>& other) {
	clear_and_free();
	buffer_allocation(other.storage_size_);
	constructor_call(other);
}

template<typename T>
list_impl::ciclic_buffer<T>& list_impl::ciclic_buffer<T>::operator=(ciclic_buffer<T>&& other) noexcept{
	clear_and_free();
	first_ = other.first_;
	last_ = other.last_;
	storage_size_ = other.storage_size_;
	size_ = other.size_;
	other.first_ = nullptr;
	other.last_ = nullptr;
	other.storage_size_ = 0;
	other.size_ = 0;
}

template<typename T>
list_impl::ciclic_buffer<T>::~ciclic_buffer(){
	clear_and_free();
}

template<typename T>
void list_impl::ciclic_buffer<T>::buffer_allocation(size_type storage_size){
	internal::list_node<T>* node_ptr = nullptr;
	size_ = 0;
	storage_size_ = storage_size;
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

template<typename T>
void list_impl::ciclic_buffer<T>::clear() noexcept{
	for(size_type i = 0; i < size_; i++){
		first_ -> value_ptr_ -> ~T();
		first_ = first_ -> next_;
	}
	size_ = 0;
}

template<typename T>
void list_impl::ciclic_buffer<T>::clear_and_free() noexcept{
	clear();
	last_ = first_;
	for(size_type i = 0; i < storage_size_; i++){
		last_ = last_ -> next_;
		delete first_;
		first_ = last_;
	}
	first_ = nullptr;
	last_ = nullptr;
	storage_size_ = 0;
}

template<typename T>
void list_impl::ciclic_buffer<T>::constructor_call(const ciclic_buffer<T>& other){
	for(size_type i = 0; i < other.size_; i++){
		new(last_) T{other[i]};
		last_ = last_ -> next_;
	}
	size_ = other.size_;
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

template<typename T>
void array_impl::ciclic_buffer<T>::push_back(const_reference value){
	if(size_ == storage_size_){
		throw std::logic_error{"Buffer overflow!"};
	}
	new(data_ + (size_ + indent_) % storage_size_) value_type{value};
	size_++;
}

template<typename T>
void array_impl::ciclic_buffer<T>::push_front(const_reference value){
	if(size_ == storage_size_){
		throw std::logic_error{"Buffer overflow!"};
	}
	indent_ = ((indent_ + storage_size_) - 1) % storage_size_;
	new(data_ + indent_) value_type{value};
	size_++;
}

template<typename T>
void array_impl::ciclic_buffer<T>::pop_back(){
	if(size_ == 0){
		throw std::logic_error{"Buffer is empty"};
	}
	(data_ + (size_ - 1 + indent_) % storage_size_) -> ~T();
	size_--;
}

template<typename T>
void array_impl::ciclic_buffer<T>::pop_front(){
	if(size_ == 0){
		throw std::logic_error{"Buffer is empty"};
	}
	(data_ + indent_) -> ~T();
	indent_ = (indent_ + 1) % storage_size_;
	size_--;
}

template<typename T>
typename array_impl::ciclic_buffer<T>::reference array_impl::ciclic_buffer<T>::operator[](size_type i) noexcept{
	if(size_ < i){
		throw std::out_of_range{"Argument is out of range."};
	}
	return data_[(indent_ + i) % storage_size_];
}

template<typename T>
typename array_impl::ciclic_buffer<T>::const_reference array_impl::ciclic_buffer<T>::operator[](size_type i) const noexcept{
	if(size_ < i){
		throw std::out_of_range{"Argument is out of range."};
	}
	return data_[(indent_ + i) % storage_size_];
}

template<typename T>
void array_impl::ciclic_buffer<T>::clear() noexcept{
	for(size_type i = 0; i < size_; i++){
		(data_ + (i + indent_) % storage_size_) -> ~T();
	}
}

template<typename T>
typename array_impl::ciclic_buffer<T>::reference array_impl::ciclic_buffer<T>::at(size_type i){
	if(i > size_){
		throw std::out_of_range{"Invalid argument"};
	} else{
		return this -> operator[](i);
	}
}

template<typename T>
typename array_impl::ciclic_buffer<T>::const_reference array_impl::ciclic_buffer<T>::at(size_type i) const{
	if(i > size_){
		throw std::out_of_range{"Invalid argument"};
	} else{
		return this -> operator[](i);
	}
}

#endif //__CICLIC_BUFFER_HPP__





