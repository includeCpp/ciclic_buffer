#include<iostream>
#include"ciclic_buffer.hpp"

template<typename T>
bool check_equal(const T& buff_1, const T& buff_2);

int main(){
	array_impl::ciclic_buffer<int> buff_1;
	list_impl::ciclic_buffer<int> buff_2;

	for(int i = 0; i < 50; i++){
		buff_1.push_back(i + (i * 2));
		buff_2.push_back(i + (i * 2));

		buff_1.push_front(i - (i * 2));
		buff_2.push_front(i - (i * 2));
	}

	if(check_equal(buff_1, buff_2)){
		std::cout << "Both buffers are equal." << std::endl;
	} else {
		std::cout << "Both buffers aren't equal." << std::endl;
		return 0;
	}

	for(int i = 0; i < 25; i++){
		buff_1.pop_back(i + (i * 2));
		buff_2.pop_back(i + (i * 2));

		buff_1.pop_front(i - (i * 2));
		buff_2.pop_front(i - (i * 2));
	}

	if(check_equal(buff_1, buff_2)){
		std::cout << "Both buffers are equal." << std::endl;
	} else {
		std::cout << "Both buffers aren't equal." << std::endl;
		return 0;
	}



	return 0;
} //main
