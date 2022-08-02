#ifndef __CICLIC_BUFFER_HPP__
#define __CICLIC_BUFFER_HPP__

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
	ciclic_buffer(size_type storage_size);
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
	list_node(const T& value, T* next = nullptr, T* prev = nullptr);
	T value_;
	T* next_;
	T* prev_;
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
	ciclic_buffer(size_type storage_size);
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
	//checks if the container is empty
	bool is_empty() const noexcept;

private:
	internal::list_node<T>* first_; 
	size_type storage_size_;
	size_type size_;
};

} //namespace list_impl

//---------------------------------------------------------------------------------------------------------------
//										Implimentation part starts here
//---------------------------------------------------------------------------------------------------------------

bool list_impl::ciclic_buffer::is_empty(){
	return nullptr == first_;
}

void list_impl::ciclic_buffer::push_back(const_reference value){
	list_impl::internal::list_node<value_type>* tmp_ptr = new list_node<value_type>(value);
	if(is_empty()){
		first_ = tmp_ptr;
		first -> next_ = first_;
		first_ -> prev_ = first_;
		return;
	}
	list_impl::internal::list_node<value_type>* checker = first_;
	while(checker -> next_ != first_){
		checker = checker -> next_;
	}
	checker -> next_ = tmp_ptr;
	tmp_ptr -> next_ = first_;
	tmp_ptr -> prev_ = checker;
}

void list_impl::ciclic_buffer::push_front(const_reference value){
	list_impl::internal::list_node<value_type>* tmp_ptr = new list_impl::internal::list_node<value_type>(value);
	if(is_empty()){
		first_ = tmp_ptr;
		first_ -> next_ = first_;
		first_ -> prev_ = first_;
	}
	list_impl::internal::list_node<value_type>* checker = first_ -> prev_;
	tmp_ptr -> next_ = first_;
	tmp_ptr -> prev_ = checker;
	checker -> next_ = tmp_ptr;
	first_ -> prev_ = tmp_ptr;
	first_ = tmp_ptr;
	tmp_ptr = nullptr;
	tmp_ptr -> next_ = nullptr;
	tmp_ptr -> prev_ = nullptr;
}

void list_impl::ciclic_buffer::pop_back(){
	if(is_empty()){
		std::cout << "Error. The container is empty." << std::endl;
		return;
	}
	list_impl::internal::list_node<value_type>* checker = first_ -> prev_;
	list_impl::internal::list_node<value_type>* tmp_ptr = checker -> prev_;
	tmp_ptr -> next_ = first_;
	first_ -> prev_ = tmp_ptr;
	std::free(checker);
	checker = nullptr;
	checker -> prev_ = nullptr;
	checker -> next_ = nullptr;
}

void list_impl::ciclic_buffer::pop_front(){
	if(is_empty()){
		std::cout << "Error. The container is empty." << std::endl;
		return;	
	}
	list_impl::internal::list_node<value_type>* checker = first_ -> next_;
	list_impl::internal::list_node<value_type>* tmp_ptr = first_ -> prev_;
	tmp_ptr -> next_ = checker;
	checker -> prev_ = tmp_ptr;
	first_ -> next_ = nullptr;
	first_ -> prev_ = nullptr;
	std::free(first_);
	first_ = checker;
	checker = nullptr;
	checker -> next_ = nullptr;
	checker -> prev_ = nullptr;
}


#endif //__CICLIC_BUFFER_HPP__





