#ifndef LINEARVECTOR_H_
#define LINEARVECTOR_H_

#include <cstdlib>
#include <cassert>
#include <iostream>
#include <vector>

#include "vector_operations.h"

struct Linear_vector { 
	Linear_vector(const std::vector<float>& v)
		: vector(v) 
	{ 
	} 
 
	Linear_vector(size_t s, float a)
		: vector(s, a) 
	{
	}	

	inline size_t size() const { 
		return vector.size(); 	
	} 

	inline std::vector<float> get_vector() const { 
		return vector; 
	} 

	inline void set_element(int i, float a) { 
		assert(i < vector.size()); 
		vector[i] = a; 
	} 

	inline float get_element(int i) const { 
		if (i < vector.size()) { 
			return vector[i];
		} 
		return 0; 
	} 

	inline void add_vector(const Linear_vector& a) { 
		assert(a.vector.size() == vector.size()); 
		add(&(vector[0]), &(a.vector[0]), vector.size());
	} 

	inline void sub_vector(const Linear_vector& a) { 
		assert(a.vector.size() == vector.size()); 
		sub(&(vector[0]), &(a.vector[0]), vector.size()); 
	} 

	inline void print() const { 
		for(std::vector<float>::const_iterator it = vector.begin(); it < vector.end(); ++it) 
			std::cout << *it << " "; 
		std::cout << std::endl; 
	} 
private: 
	std::vector<float> vector; 
}; 
#endif
