#ifndef MATRIX_H_
#define MATRIX_H_

#include <cmath>	
#include <algorithm>

#include "vector_operations.h"
#include "Linear_vector.h"

#define EPS 0.00001

struct Matrix { 
	Matrix(size_t cr, size_t cc);
	Matrix(const std::vector<float>& input, size_t cr, size_t cc); 
	Matrix(const std::vector<float>& diag); 
			
	inline double get_element(size_t i, size_t j) const { 
		if (i < count_rows && j < count_columns) { 
			return matrix[i * count_columns + j];
		} else {
			return 0.0; 
		} 
	} 
 
	inline void set_element(size_t i, size_t j, double value) { 
		assert(i < count_rows && j < count_columns); 
		matrix[i * count_columns + j] = value;
	} 

	std::vector<float> get_column(int column) const; 
	std::vector<float> get_row(int row) const; 
	std::vector<float> get_diag() const;

	inline size_t get_count_columns() const { 
		return count_columns;
	} 

	inline size_t get_count_rows() const { 
		return count_rows;
	} 

	void add_matrix(const Matrix& b); 
	void sub_matrix(const Matrix& b);
	Matrix mul_matrix(const Matrix& b);		
	void mul_const(float b); 
	Linear_vector right_mul_vector(const Linear_vector& b);
	void back_matrix_diag(); 			
			
	void print() const; 
private: 
	std::vector<float> mul_vector(std::vector<float>& b);	
private: 
	size_t count_rows;		
	size_t count_columns;
	std::vector<float> matrix; 
}; 

#endif
