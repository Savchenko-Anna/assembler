#include "Matrix.h"

Matrix::Matrix(size_t cr, size_t cc)
	: count_rows(cr)
	, count_columns(cc)
	, matrix(count_rows * count_columns, 0) 
{ 
} 

Matrix::Matrix(const std::vector<float>& input, size_t cr, size_t cc)
	: count_rows(cr)
	, count_columns(cc)
	, matrix(input)
{ 
}  

Matrix::Matrix(const std::vector<float>& diag) 
	: count_rows(diag.size())
	, count_columns(diag.size())
	, matrix(count_rows * count_columns, 0) 
{
	for(size_t i = 0, j = 0; j < diag.size(); i += count_columns + 1, ++j) { 
		matrix[i] = diag[j];
	}  
} 

std::vector<float> Matrix::get_column(int column) const {	
	std::vector<float> output(count_rows); 
	
	for(size_t i = 0; i < count_rows; ++i) { 
		output[i] = matrix[i * count_columns + column];
	} 

	return output; 	
}


std::vector<float> Matrix::get_diag() const { 
	std::vector<float> output(std::min(count_rows, count_columns)); 
	
	for(size_t i = 0, j = 0; j < std::min(count_rows, count_columns); i += count_columns + 1, ++j) { 
		output[j] = matrix[i];
	}  

	return output; 		
} 

std::vector<float> Matrix::get_row(int row) const { 
	std::vector<float> output(count_columns); 	
	output.assign(matrix.begin() + count_columns * row, matrix.begin() + count_columns * (row + 1));
	return output;
} 

void Matrix::add_matrix(const Matrix& b) { 
	assert((count_rows == b.count_rows) && (count_columns == b.count_columns)); 
 	add(&(matrix[0]), &(b.matrix[0]), matrix.size()); 
} 
 
void Matrix::sub_matrix(const Matrix& b) { 
	assert((count_rows == b.count_rows) && (count_columns == b.count_columns)); 
 	sub(&(matrix[0]), &(b.matrix[0]), matrix.size()); 
} 

Matrix Matrix::mul_matrix(const Matrix& b) {
	Matrix result(count_rows, b.count_columns);

	if (count_columns == b.count_rows) { 
		std::vector<float> current_column;
		std::vector<float> temp;
		for(int i = 0; i < b.count_columns; ++i) { 
			current_column = b.get_column(i);
			current_column = mul_vector(current_column);  
			for(int j = i, k = 0; k < current_column.size(); j += count_columns, ++k) { 	
				result.matrix[j] = current_column[k];
			}
		}  
	}  

	return result; 
} 		

void Matrix::mul_const(float b) {
	for(std::vector<float>::iterator it = matrix.begin(); it < matrix.end(); ++it) { 
		*it *= b;
	} 
} 

Linear_vector Matrix::right_mul_vector(const Linear_vector& b) {
	std::vector<float> am = b.get_vector(); 
	am  = mul_vector(am); 
	return 	Linear_vector(am);
} 

void Matrix::back_matrix_diag() {
	for(int i = 0; i < std::min(count_rows, count_columns); ++i) { 	
		if (fabs(get_element(i, i)- 0.0) > EPS) { 
			set_element(i, i, 1 / get_element(i, i));
		} 
	} 
}			

void Matrix::print() const { 
	for(size_t i = 0; i < count_rows; ++i) { 
		for(size_t j = 0;  j < count_columns; ++j)
			std::cout << get_element(i, j) << " ";
		std::cout << std::endl; 	
	} 
	std::cout << std::endl;	
} 		

std::vector<float> Matrix::mul_vector(std::vector<float>& b) { 
	std::vector<float> result(count_rows, 0);
	if (count_columns == b.size()) { 
		size_t version = what_sse(); 
		std::vector<float> current_row; 
		for(int i = 0; i < count_rows; ++i) {
			current_row = get_row(i);

			int boundary1 = 4 - current_row.size() % 4; 
			for(int j = 0; j < boundary1; ++j)
				current_row.push_back(0);				

			int boundary2 = 4 - b.size() % 4;
			for(int j = 0; j < boundary2; ++j)
				b.push_back(0);				
							
			if (version == SSE3_SUPPORTED) {
				result[i] = mul_SSE3(&(current_row[0]), &(b[0]), b.size());  
			} else if (version == SSE41_SUPPORTED) { 
				result[i] = mul_SSE41(&(current_row[0]), &(b[0]), b.size());  
			} else {  
				std::cerr << "SSE not supported" << std::endl;
			} 
			
			for(int j = 0; j < boundary1; ++j)
				current_row.pop_back();				

			for(int j = 0; j < boundary2; ++j)
				b.pop_back();							
		}		
	}  
	
	return result;
} 
