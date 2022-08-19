# ciclic_buffer
Test task for Lesta Studio

Advantages of list_impl realisation is that we are able to have buffer of elements every of each able to have big objects in memory 
because every element is placed in a different section of the available free memory.


Advantage of array_impl is that our array takes one large piece of space in available memory what makes the process of access to elements work faster 
and have difficulty O(1). Also it gives possibility for processor work faster by "taking" a piece of memory to the cache and work with it without 
going through the array in memory again and again.



  List methods:

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
  
	//clears all elements without affecting capacity
	void clear() noexcept;
  
	//returns size of the container in int type
	size_type size() const noexcept;
  
  

  Array methods:
  
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
  
	//clears data of the container 
	void clear() noexcept;
  
	//return size of the container in int type
	size_type size() const noexcept;



