#include<iostream>
#include"ciclic_buffer.hpp"

template<typename T, typename Q>
bool check_equal(const T& buff_1, const Q& buff_2);

int main(){
	array_impl::ciclic_buffer<int> buff_1(100);
	list_impl::ciclic_buffer<int> buff_2(100);

	for(int i = 0; i < 50; i++){
		buff_1.push_back(i + (i * 2));
		buff_2.push_back(i + (i * 2));

		buff_1.push_front(i - (i * 2));
		buff_2.push_front(i - (i * 2));
	}	if(check_equal(buff_1, buff_2)){
		std::cout << "Both buffers are equal." << std::endl;
	} else {
		std::cout << "Both buffers aren't equal." << std::endl;
		return 0;
	}

	for(int i = 0; i < buff_2.size(); i++){
		std::cout << buff_2[i] << std::endl;
	}

	for(int i = 0; i < 25; i++){
		buff_1.pop_back();
		buff_2.pop_back();

		buff_1.pop_front();
		buff_2.pop_front();
	}

	

	if(check_equal(buff_1, buff_2)){
		std::cout << "Both buffers are equal." << std::endl;
	} else {
		std::cout << "Both buffers aren't equal." << std::endl;
		return 0;
	}

	return 0;
} //main

template<typename T, typename Q>
bool check_equal(const T& buff_1, const Q& buff_2){
	if(buff_1.size() == buff_2.size()){
		for(std::size_t i = 0; i < buff_1.size(); i++){
			if(buff_1[i] == buff_2[i]){
				std::cout << "buff_1 " << buff_1[i] << "    buff_2 " << buff_2[i] << std::endl;
				continue;
			} else{
				std::cout << i << std::endl;
				std::cout << "buff_1 = " << buff_1[i] << "    " << "buff_2 = "<< buff_2[i] << std::endl;
				return false;
			}
		}
		std::cout << "Everything is cool." << std::endl;
		return true;
	} else{
		std::cout << "false" << std::endl;
		return false;
	}
}





