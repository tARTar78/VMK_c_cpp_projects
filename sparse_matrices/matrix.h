#include <climits>
#include <string>
#include <iostream>
#include <fstream>
#define MAX_COORD 100

//Rational numbers
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
long int evkl(long int a, long int b){


	if(a != 0 &&  b != 0){
		if(a > b){
			a = a % b;
			return evkl(a,b);
		}else{
			b = b % a;
			return evkl(a,b);
		}			
		
	}else{
		return a+b;
	}


}

class Rational_number{
        long int up = 0, down = 1;

public:

	void set_up(long int a){
		up = a;
	}
	void set_down(long int b){
		if(b == 0){
                        throw "Impossible\n";
                }
		down = b;
	}
	Rational_number(long int a = 0, long int b = 1){
                up = a;
		if(b == 0){
			throw "Impossible\n";
		}
		down = b;
        }
	Rational_number(const char* a,const char* b = "1"){
                up = atoi(a);
                down = atoi(b);
                
        }
        Rational_number(int a , int b = 1){
                up = a;
                down = b;
        }
        Rational_number(short a, short b = 1){
                up = a;
                down = b;
        }
        Rational_number(unsigned int a, unsigned int b = 1){
                up = a;
                down = b;
        }
	operator int() const {
		long int res = up/down;
		if(up%down != 0){
			throw "Impossible conversion\n";
		}else if(res < INT_MIN || res > INT_MAX ){
			throw "Overflow\n";

		}else{
			return res;
		}

	}
	operator short() const {
                long int res = up/down;
                if(up%down != 0){
                        throw "Impossible conversion\n";
                }else if(res < SHRT_MIN || res > SHRT_MAX ){
                        throw "Overflow\n";

                }else{
                        return res;
                }

        }
	operator long int() const {
                long int res = up/down;
                if(up%down != 0){
                        throw "Impossible conversion\n";
                }else{
                        return res;
                }

        }
	operator unsigned int() const {
                long int res = up/down;
                if(up%down != 0){
                        throw "Impossible conversion\n";
                }else if(res < 0 || res > UINT_MAX ){
                        throw "Overflow\n";

                }else{
                        return res;
                }

        }
        friend Rational_number operator+ (const Rational_number &a, const Rational_number &b);
        friend Rational_number operator- (const Rational_number &a, const Rational_number &b);
        friend Rational_number operator*(const Rational_number &a, const Rational_number &b);
        friend Rational_number operator/(const Rational_number &a, const Rational_number &b);
        friend bool operator>(const Rational_number &a, const Rational_number &b);
        friend bool operator<(const Rational_number &a, const Rational_number &b);
        friend bool operator>=(const Rational_number &a, const Rational_number &b);
        friend bool operator<=(const Rational_number &a, const Rational_number &b);
	friend bool operator==(const Rational_number &a, const Rational_number &b);
        friend bool operator!=(const Rational_number &a, const Rational_number &b);

	Rational_number & operator++(){
                up = up + down;
                return *this;
        }
        Rational_number operator++(int){
                Rational_number res = *this;
                up = up + down;
                return res;
        }
	Rational_number & operator--(){
                up = up - down;
                return *this;
        }
        Rational_number operator--(int){
                Rational_number res = *this;
                up = up - down;
                return res;
        }
	friend Rational_number& operator+=(Rational_number &a, const Rational_number &b);
        friend Rational_number& operator-=(Rational_number &a, const Rational_number &b);
        friend Rational_number& operator*=(Rational_number &a, const Rational_number &b);
        friend Rational_number& operator/=(Rational_number &a, const Rational_number &b);
	
	friend Rational_number make_canonical(Rational_number &a);
	friend Rational_number floor(Rational_number &a);
	friend Rational_number round(Rational_number &a);
	friend std::string R_N_to_str(Rational_number a);

};

Rational_number make_canonical(Rational_number &a){
	if(a.up == 0){
		a.down = 1;
	}else{
		long int ev = evkl(a.up,a.down);
		a.up /= ev;
		a.down /= ev;
	}
	return a;
}

Rational_number floor(Rational_number &a){

	a.up /= a.down;
	a.down = 1;
	return a;

}

Rational_number round(Rational_number &a){
	a.up = a.up*2+a.down;
	a.down *= 2;
	return floor(a);

}

Rational_number operator+(const Rational_number &a, const Rational_number &b){

	Rational_number res(a.up*b.down+b.up*a.down, a.down*b.down);
	return res;
}

Rational_number operator-(const Rational_number &a,const Rational_number &b){

        Rational_number res(a.up*b.down-b.up*a.down, a.down*b.down);
        return res;
}

Rational_number operator*(const Rational_number &a,const Rational_number &b){

        Rational_number res(a.up*b.up, a.down*b.down);
        return res;
}

Rational_number operator/(const Rational_number &a,const Rational_number &b){

	if(b.up != 0){
        	Rational_number res(a.up*b.down, a.down*b.up);
        	return res;
	}else{
		throw "Division by zero!\n";
	}
}

bool operator>(const Rational_number &a,const Rational_number &b){

	return(a.up*b.down > b.up*a.down);
}

bool operator<(const Rational_number &a, const Rational_number &b){

        return(a.up*b.down < b.up*a.down);
}

bool operator>=(const Rational_number &a, const Rational_number &b){

        return(a.up*b.down >= b.up*a.down);
}

bool operator<=(const Rational_number &a, const Rational_number &b){

        return(a.up*b.down <= b.up*a.down);
}

bool operator==(const Rational_number &a, const Rational_number &b){
	
	return(a.up*b.down == b.up*a.down);

}

bool operator!=(const Rational_number &a, const Rational_number &b){

        return(a.up*b.down != b.up*a.down);

}

Rational_number& operator+=(Rational_number &a, const Rational_number &b){
	a.up = a.up*b.down + b.up*a.down;
	a.down *= b.down;
	return a;
}

Rational_number& operator-=(Rational_number &a, const Rational_number &b){
	a.up = a.up*b.down - b.up*a.down;
        a.down *= b.down;
        return a;

}

Rational_number& operator*=(Rational_number &a, const Rational_number &b){
	a.up *= b.up;
        a.down *= b.down;
        return a;

}
Rational_number& operator/=(Rational_number &a, const Rational_number &b){
	if(b.up != 0){
                a.up *= b.down;
		a.down *= b.up;
                return a;
        }else{
		throw "Division by zero!\n";
	}

}


//Vectors
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct  DV{
                 int coord = -1;
                 Rational_number val = 0;
                 DV *next = 0;
};

class Vector{
	int all = 0;
	int size = 100;
	/*struct  DV{
		int coord = -1;
		Rational_number val = 0;
		DV *next = 0;	
	};*/
	DV *first;
public:
	Vector() : first(NULL){}
	~Vector();
	void set_size(int n){
                this->size = n;
        }
	Vector(const Vector& a);
        Vector operator=(const Vector& a);
	friend Vector operator+ (const Vector &a,const Vector &b);
        friend Vector operator- (const Vector &a,const Vector &b);
        friend Vector operator*(const Vector &a, const Rational_number &b);
        friend Vector operator/(const Vector &a, const Rational_number &b);
	friend int V_to_file(Vector& a, char* file);
	Vector operator-();
	class DOP{
		friend class Vector;
		Vector *v_ptr;
		int coord;
		DOP(Vector *a_ptr, int crd) : v_ptr(a_ptr), coord(crd){} 
		Rational_number& Provide();
		void Remove();
	public:
		operator Rational_number();
		Rational_number operator=(Rational_number x);
		Rational_number operator+=(Rational_number x);
		Rational_number operator-=(Rational_number x);
		Rational_number operator*=(Rational_number x);
		Rational_number operator/=(Rational_number x);
		Rational_number operator++();
                Rational_number operator++(int);
		Rational_number operator--();
		Rational_number operator--(int);

	};
	friend class DOP;

	DOP operator[](int crd){
		if(crd > size){
                	throw "Wrong coordinate";
        	}
		return DOP(this,crd);
	}
/*private:
	Vector(const Vector&){}
	void operator=(const Vector&){}*/
};

Vector::Vector(const Vector& a){

        DV* tmp = a.first;
        while(tmp){
                (*this)[tmp->coord] += tmp->val;
                tmp = tmp->next;
        }
	

}

Vector Vector::operator=(const Vector& a){

        DV* tmp = a.first;
        while(tmp){
                (*this)[tmp->coord] += tmp->val;
                tmp = tmp->next;
        }
        return *this;


}


Vector::~Vector(){
	while(first){
		DV *tmp = first;
		first = first->next;
		delete tmp;
	}

}

Rational_number Vector::DOP::operator=(Rational_number a){
	if(a == Rational_number(0)){
		Remove();
	}else
		Provide() = a;
	return a;
}

Rational_number Vector::DOP::operator+=(Rational_number a){
	Rational_number& loc = Provide();
	loc += a;
	Rational_number res = loc;
	if(res == Rational_number(0))
		Remove();
	return res;

}

Rational_number Vector::DOP::operator-=(Rational_number a){
        Rational_number& loc = Provide();
        loc -= a;
        Rational_number res = loc;
        if(res == Rational_number(0))
                Remove();
        return res;

}

Rational_number Vector::DOP::operator*=(Rational_number a){
        Rational_number& loc = Provide();
        loc *= a;
        Rational_number res = loc;
        if(res == Rational_number(0))
                Remove();
        return res;

}

Rational_number Vector::DOP::operator/=(Rational_number a){
        Rational_number& loc = Provide();
        loc /= a;
        Rational_number res = loc;
        if(res == Rational_number(0))
                Remove();
        return res;

}


Rational_number Vector::DOP::operator++(){
        Rational_number& loc = Provide();
        Rational_number res = ++loc;
        if(loc == Rational_number(0))
                Remove();
        return res;

}

Rational_number Vector::DOP::operator++(int){
        Rational_number& loc = Provide();
        Rational_number res = loc++;
        if(res == Rational_number(0))
                Remove();
        return res;

}


Rational_number Vector::DOP::operator--(){
        Rational_number& loc = Provide();
        Rational_number res = --loc;
        if(loc == Rational_number(0))
                Remove();
        return res;

}

Rational_number Vector::DOP::operator--(int){
        Rational_number& loc = Provide();
        Rational_number res = loc--;
        if(res == Rational_number(0))
                Remove();
        return res;

}


Vector::DOP::operator Rational_number(){
       	DV* tmp;
	for(tmp = v_ptr->first; tmp; tmp = tmp->next){
		if(tmp->coord == coord){
			return tmp->val;
		}
	}
	return 0;

}

Rational_number& Vector::DOP::Provide(){
	DV* tmp;
	for(tmp = v_ptr->first; tmp; tmp = tmp->next){
		if(tmp->coord == coord){
			return tmp->val;
		}

	}
	tmp = new DV;
	tmp->coord = coord;
	tmp->next =v_ptr->first;
	v_ptr->first = tmp;
	return tmp->val;



}

void Vector::DOP::Remove(){
        DV** tmp;
        for(tmp = &(v_ptr->first); *tmp; tmp = &(*tmp)->next){
                if((*tmp)->coord == coord){
         		DV* dp = (*tmp)->next;
			*tmp = (*tmp)->next;
	 		delete dp;
			return;	
                }

        }
}


Vector operator+(const Vector &a,const Vector &b){
        Vector res;
        DV* tmp = a.first;
        while(tmp){
                res[tmp->coord] += tmp->val;
		tmp = tmp->next;
        }
        tmp = b.first;
        while(tmp){
                res[tmp->coord] += tmp->val;
		tmp = tmp->next;
        }
        return res;
}

Vector operator-(const Vector &a,const Vector &b){
        Vector res;
        DV* tmp = a.first;
        while(tmp){
                res[tmp->coord] += tmp->val;
		tmp = tmp->next;
        }
        tmp = b.first;
        while(tmp){
                res[tmp->coord] -= tmp->val;
		tmp = tmp->next;
        }
        return res;
}

Vector operator*(const Vector &a,const Rational_number &b){
	Vector res;
        DV* tmp = a.first;
        while(tmp){
                res[tmp->coord] += tmp->val;
		tmp = tmp->next;
        }
        tmp = res.first;
        while(tmp){
                res[tmp->coord] *= b;
		tmp = tmp->next;
        }
        return res;
}

Vector operator/(const Vector &a,const Rational_number &b){
        Vector res;
        DV* tmp = a.first;
        while(tmp){
                res[tmp->coord] += tmp->val;
		tmp = tmp->next;
        }
        tmp = res.first;
        while(tmp){
                res[tmp->coord] /= b;
		tmp = tmp->next;
        }
        return res;
}

Vector Vector::operator-(){
        Vector res;
        DV* tmp = first;
        while(tmp){
                res[tmp->coord] -= tmp->val;
		tmp = tmp->next;
        }
        return res;
}

//Matrices
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct  DV_V{
                 int x = -1;
		 int y= -1;
                 Rational_number val = 0;
                 DV_V *next = 0;
};

class Matrix{
        int all = 0;
	int rows = 100;
	int cols = 100;
        /*struct  DV{
                int coord = -1;
                Rational_number val = 0;
                DV *next = 0;   
        };*/
        DV_V *first;
public:
        Matrix() : first(NULL){}
        ~Matrix();
	Matrix(const Matrix& a);
        Matrix operator=(const Matrix& a);
	void set_rows(int n){
		this->rows = n;
	}
	void set_cols(int n){
                this->cols = n;
        }
        friend Matrix operator+ (const Matrix &a,const Matrix &b);
        friend Matrix operator- (const Matrix &a,const Matrix &b);
        friend Matrix operator*(const Matrix &a, const Matrix &b);
	friend int M_to_file(Matrix& a, char* file);
	void operator>>(Rational_number &x){
		Rational_number max(INT_MIN);
		DV_V* tmp = first;
		while(tmp){
                	
                	if(tmp->val > max){
				max = tmp->val;
			}
			tmp = tmp->next;
        	}
		x = max;

	}

        Matrix operator-();
        class DOP_M{
                friend class Matrix;
                Matrix *v_ptr;
                int x;
                DOP_M(Matrix *a_ptr, int crd1) : v_ptr(a_ptr), x(crd1){}
                /*Vector& Provide();
                void Remove();*/
        public:
		class DOP_M2{
			friend class DOP_M;
			int x;
			int y;
			Matrix *v_ptr;
			DOP_M2(Matrix *a_ptr, int crd1, int crd2) : v_ptr(a_ptr), x(crd1), y(crd2){}
			Rational_number& Provide();
                	void Remove();
		public:
			operator Rational_number();
                	Rational_number operator=(Rational_number x);
                	Rational_number operator+=(Rational_number x);
                	Rational_number operator-=(Rational_number x);
                	Rational_number operator*=(Rational_number x);
                	Rational_number operator/=(Rational_number x);
                	Rational_number operator++();
                	Rational_number operator++(int);
                	Rational_number operator--();
                	Rational_number operator--(int);

		};
               /* operator Rational_number();
                Rational_number operator=(Rational_number x);
                Rational_number operator+=(Rational_number x);
                Rational_number operator-=(Rational_number x);
                Rational_number operator*=(Rational_number x);
                Rational_number operator/=(Rational_number x);
                Rational_number operator++();
                Rational_number operator++(int);
                Rational_number operator--();
                Rational_number operator--(int);*/
		friend class DOP_M2;
		DOP_M2 operator[](int y){
			if(y > this->v_ptr->cols){
				throw "Wrong index of col\n";
			}
                	return DOP_M2(this->v_ptr,x,y);
        	}

        };
        friend class DOP_M;

        DOP_M operator[](int x){
		if(x > rows){
			throw "Wrong index of row\n";
		}
                return DOP_M(this,x);
        }

        /*Matrix(const Matrix&){}
        void operator=(const Matrix&){}*/
};


Matrix::~Matrix(){
        while(first){
                DV_V *tmp = first;
                first = first->next;
                delete tmp;
        }

}

Matrix::Matrix(const Matrix& a){

        DV_V* tmp = a.first;
        while(tmp){
                (*this)[tmp->x][tmp->y] += tmp->val;
                tmp = tmp->next;
        }


}

Matrix Matrix::operator=(const Matrix& a){

        DV_V* tmp = a.first;
        while(tmp){
                (*this)[tmp->x][tmp->y] += tmp->val;
                tmp = tmp->next;
        }
        return *this;


}


Rational_number Matrix::DOP_M::DOP_M2::operator=(Rational_number a){
        if(a == Rational_number(0)){
                Remove();
        }else
                Provide() = a;
        return a;
}

Rational_number Matrix::DOP_M::DOP_M2::operator+=(Rational_number a){
        Rational_number& loc = Provide();
        loc += a;
        Rational_number res = loc;
        if(res == Rational_number(0))
                Remove();
        return res;
}

Rational_number Matrix::DOP_M::DOP_M2::operator-=(Rational_number a){
        Rational_number& loc = Provide();
        loc -= a;
        Rational_number res = loc;
        if(res == Rational_number(0))
                Remove();
        return res;

}

Rational_number Matrix::DOP_M::DOP_M2::operator*=(Rational_number a){
        Rational_number& loc = Provide();
        loc *= a;
        Rational_number res = loc;
        if(res == Rational_number(0))
                Remove();
        return res;

}

Rational_number Matrix::DOP_M::DOP_M2::operator/=(Rational_number a){
        Rational_number& loc = Provide();
        loc /= a;
        Rational_number res = loc;
        if(res == Rational_number(0))
                Remove();
        return res;

}


Rational_number Matrix::DOP_M::DOP_M2::operator++(){
        Rational_number& loc = Provide();
        Rational_number res = ++loc;
        if(loc == Rational_number(0))
                Remove();
        return res;

}

Rational_number Matrix::DOP_M::DOP_M2::operator++(int){
        Rational_number& loc = Provide();
        Rational_number res = loc++;
        if(res == Rational_number(0))
                Remove();
        return res;

}

Rational_number Matrix::DOP_M::DOP_M2::operator--(){
        Rational_number& loc = Provide();
        Rational_number res = --loc;
        if(loc == Rational_number(0))
                Remove();
        return res;

}

Rational_number Matrix::DOP_M::DOP_M2::operator--(int){
        Rational_number& loc = Provide();
        Rational_number res = loc--;
        if(res == Rational_number(0))
                Remove();
        return res;

}


Matrix::DOP_M::DOP_M2::operator Rational_number(){
        DV_V* tmp;
        for(tmp = v_ptr->first; tmp; tmp = tmp->next){
                if((tmp->x == x) && (tmp->y = y)) {
                        return tmp->val;
                }
        }
        return 0;

}

Rational_number& Matrix::DOP_M::DOP_M2::Provide(){
        DV_V* tmp;
        for(tmp = v_ptr->first; tmp; tmp = tmp->next){
                if((tmp->x == x) && (tmp->y == y)){
                        return tmp->val;
                }

        }
        tmp = new DV_V;
	tmp->x = x;
	tmp->y = y;
        tmp->next =v_ptr->first;
        v_ptr->first = tmp;
        return tmp->val;
}

void Matrix::DOP_M::DOP_M2::Remove(){
        DV_V** tmp;
        for(tmp = &(v_ptr->first); *tmp; tmp = &(*tmp)->next){
                if(((*tmp)->x == x) && ((*tmp)->y == y)){
                        DV_V* dp = (*tmp)->next;
                        *tmp = (*tmp)->next;
                        delete dp;
                        return;
                }

        }
}


Matrix operator+(const Matrix &a,const Matrix &b){
        Matrix res;
        DV_V* tmp = a.first;
        while(tmp){
                res[tmp->x][tmp->y] += tmp->val;
		tmp = tmp->next;
        }
        tmp = b.first;
        while(tmp){
                res[tmp->x][tmp->y] += tmp->val;
		tmp = tmp->next;
        }
        return res;
}

Matrix operator-(const Matrix &a,const Matrix &b){
        Matrix res;
        DV_V* tmp = a.first;
        while(tmp){
                res[tmp->x][tmp->y] += tmp->val;
		tmp = tmp->next;
        }
        tmp = b.first;
        while(tmp){
                res[tmp->x][tmp->y] -= tmp->val;
		tmp = tmp->next;
        }
        return res;
}
Matrix operator*(const Matrix &a,const Matrix &b){
        Matrix res;
        DV_V* tmp = a.first;
        while(tmp){
                res[tmp->x][tmp->y] += tmp->val;
		tmp = tmp->next;
        }
        tmp = res.first;
        while(tmp){
		Matrix prom;
		for(DV_V* dop = b.first; dop; dop = dop->next){
			if(tmp->y == dop->x){
				res[tmp->x][dop->y] += res[tmp->x][tmp->y] * dop->val;
			}
		}

		tmp = tmp->next;
        }
        return res;
}

// for files//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string R_N_to_str(Rational_number a){
	std::string str;
	if(a.up == 0){
		str = "0";
		return str;	
	}
	if(a.down == 1){
		str = std::to_string(a.up);
		return str;
	}
	str = std::to_string(a.up) + " / " + std::to_string(a.down);
	return str;
}

int V_to_file(Vector& a, char* file){
	std::ofstream out;
	out.open(file);
	if(out.is_open()){
		out << "#\n#This file describes\n#sparse vector\n#\nvector " << a.size << std::endl;
	
		DV* tmp = a.first;
		while(tmp){
			out << tmp->coord << " " << R_N_to_str(tmp->val) << std::endl;
			tmp = tmp->next;
		}
	}else{
		return -1;
	}
	out.close();
	return 0;

}

int M_to_file(Matrix& a, char* file){
	std::ofstream out;
        out.open(file);
        if(out.is_open()){
                out << "#\n#This file describes\n#sparse matrix\n#\nmatrix " << a.rows <<" " << a.cols << std::endl;
        
        	DV_V* tmp = a.first;
        	while(tmp){
                	out << tmp->x << " " << tmp->y << " " << R_N_to_str(tmp->val) << std::endl;
                	tmp = tmp->next;
        	}
	}else{
		return -1;
	}
        out.close();
	return 0;

}



