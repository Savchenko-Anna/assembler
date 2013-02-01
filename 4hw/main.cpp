#include <fstream>
#include <vector>
#include <cstdlib>

#include "Matrix.h"
#include "Linear_vector.h"

void read_input(std::vector<float>& matrix, std::vector<float>& vector, size_t& count_rows, size_t& count_columns) { 
	std::ifstream input("input.txt");
	float temp = 0.0; 

	input >> count_rows >> count_columns;

	for(size_t i = 0; i < count_rows * count_columns; ++i) { 
		input >> temp;  
		matrix.push_back(temp); 
	} 
	
	for(size_t i = 0; i < count_rows; ++i) { 
		input >> temp;  
		vector.push_back(temp); 
	}
}  

std::vector<float> jacob_method(const Matrix& matr, const Linear_vector& b) {
	Matrix D(matr.get_diag());
	D.back_matrix_diag();

	Linear_vector g = D.right_mul_vector(b);
	
	std::vector<float> e(D.get_count_rows(), 1);

	Matrix B(e);
	B.sub_matrix(D.mul_matrix(matr));

	float norm = 0.0; 
	Linear_vector tempX(matr.get_count_columns(), 0); 
	Linear_vector x(matr.get_count_columns(), 1) ;	
	int count = 0;

	do { 
		++count;
		tempX = B.right_mul_vector(x);	
		tempX.add_vector(g); 
		
		Linear_vector norm_vector(x);
		norm_vector.sub_vector(tempX);	
		norm = fabs(norm_vector.get_element(0));
		
		for(int i = 0; i <  norm_vector.size(); ++i) { 
			if (fabs(norm_vector.get_element(i)) > norm) 
				norm = fabs(norm_vector.get_element(i));
		}  

		x = tempX;
		if (count > 1000000)
			break;
	} while (norm > EPS);
	
	return x.get_vector();
}

void print_answer(const std::vector<float>& result) { 
	std::ofstream output("output.txt");
	output << "answer" << std::endl; 

	for(std::vector<float>::const_iterator it = result.begin(); it < result.end(); ++it) { 
		output << *it << std::endl; 
	} 
} 

int main() { 
	size_t count_rows = 0;
	size_t count_columns = 0; 
	std::vector<float> matrix; 
	std::vector<float> vector; 

	read_input(matrix, vector, count_rows, count_columns);  	
 
	Matrix a(matrix, count_rows, count_columns);
	Linear_vector b(vector);

	std::vector<float> answer = jacob_method(a, b);

	print_answer(answer);
} 
