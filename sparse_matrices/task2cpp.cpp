#include "matrix.h"


int main(){
	//std::cout << "Hello!" < std::endl;
        Matrix A;
	Rational_number x;
        A.set_cols(1000);
        A.set_rows(2000);
        A[1][30] = 3;
        A[5][250] = 8;
	Rational_number a(3,8);
	A[70][90] = a;
        char s[50] = "art.txt";
        M_to_file(A,s);
	A>>x;
	std::cout << R_N_to_str(x) <<  std::endl;



}
