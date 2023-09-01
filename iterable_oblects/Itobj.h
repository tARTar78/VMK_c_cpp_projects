#include <iostream>
#include <cstring>
#include <typeinfo>

struct Data{
	

	int* v_int = nullptr;
	double* v_L = nullptr;
	char* str = nullptr;



	Data(int n){
		v_int = new int;
		*v_int = n;
	}
	Data(double d){
		v_L = new double;
		*v_L = d;
	}
	Data(const char* s){
		str = new char[strlen(s)+1];
		strcpy(str,s);
	}
	friend bool operator==(const Data& a,const  Data& b){
		if((a.str != NULL)&&(b.str != NULL))
			return (a.v_int == b.v_int)&&(a.v_L == b.v_L)&&(!(strcmp(a.str,b.str)));
		return (a.v_int == b.v_int)&&(a.v_L == b.v_L);
	}
	~Data(){
		if(str != nullptr)
			delete[] str;
		if(v_int != nullptr)
                        delete v_int;
		if(v_L != nullptr)
                        delete v_L;

	}



};

//template< typename T>
class IterableObject{

protected:
	int length;
	//virtual int contains(const char* a) = 0;
	//virtual int contains(const int a) = 0;
        //virtual int contains(const double a) = 0;
        
public:
	IterableObject(int n = 0){
		length = n;
	}
	virtual int contains(const Data b) = 0;
	virtual ~IterableObject() {}
};

//String////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class String : public IterableObject{
	char*  pointer;
public:
         //virtual int contains(const int a);
         //virtual int contains(const double a);
	 virtual int contains(const Data b){
		const char* a = b.str;
		char* cmp1 = new char[strlen(a)+1];
		int c = 0;
		while(c != length - strlen(a) + 1){
			//strcpy(cmp1,dop);
			for(int i = c; i < strlen(a)+c;++i){
				cmp1[i] = pointer[i];
			}
			cmp1[strlen(a)] = '\0';
			//std::cout <<"{"<< cmp1 << "}" << std::endl;
			if(!strcmp(cmp1,a)){
				delete[] cmp1;
				return c;
			}
			c++;
		}
		delete[] cmp1;
		return -1;
	 };
	 String() : pointer(NULL){};
	 String(const char* a){
		length = strlen(a);
                pointer = new char[length + 1];
                strcpy(pointer,a);	 
	 };
	operator Data() const{
		return Data(pointer);
	}
	operator char*() const{
		return pointer;
	};

	String(const String& s){
		length = s.length;
                pointer = new char[length + 1];
                strcpy(pointer,s.pointer);
	
	};
	
	friend bool operator ==(const String& a,const String& b);
	friend String operator+(const String& a,const String& b);
	String& operator=(const String& a){
		if(this == &a)
                	return *this;
        	delete []pointer;
        	pointer = new char [(length = a.length) + 1];
		strcpy(pointer,a.pointer);
        	return *this;
	};
	int len(){
		return length;	
	};
	char& operator[](int n){
		return (this->pointer)[n];	
	};
	~String(){
		delete[] pointer;
	}



};

/*char* String::operator char*() const{
	return pointer;
}

int String::length(){
	return length;
}
char& String::operator[](int n){
	return (this->pointer)[n];
}

*/

bool operator ==(const String& a,const String& b){

	return (!strcmp(a.pointer,b.pointer));

}
String operator+(const String& a,const String& b){
	char* dop = new char[a.length + b.length +1];
	strcpy(dop,a.pointer);
	strcat(dop,b.pointer);
	String s(dop);
	delete []dop;
	return s;
}
/*

String& String::operator=(const String& a){
	if(this == &a)
		return *this;
	delete []pointstruct node{
                T val = 0;
                node* next;
        };
er;
	pointer = new char [(length = a.length) + 1];
	strcpy(pointer,a.pointer);
	return *this;
}

String::String(const char* a = ""){
                length = strlen(a);
                pointer = new char[length + 1];
                strcpy(pointer,a);
}

String::String(const String& s){
                length = s.length;
                pointer = new char[length + 1];
                strcpy(pointer,s.pointer);
}


String::~String(){
                delete[] pointer;
}
*/
//Vector////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
class Vector : public IterableObject{
protected:
	struct node{
		T val;
		node* next;
	};
	node* first;
	node* last;
public:
	virtual int contains(const Data b){

                	node* dop = last;
			int c = length-1;
                        while(dop != NULL){
                                if(b == dop->val){

                                        return c;
                                }
                                c--;
                                dop = dop->next;
				
                        }
                        return -1;
	};
	Vector() : first(NULL), last(NULL){};
	void push_back(T a){
		if (last == NULL){
			last = new node;
			last->val = a;
			last->next = NULL;
			first = last;
		}else{
			node* tmp = new node;
			tmp->val = a;
			tmp->next = last;
			last = tmp;
		}
		length++;
	};
	T pop_back(){
		node* tmp = last;
		last = last->next;
		delete tmp;
		length--;	
	};
        Vector(const Vector& v){
		
		first = NULL;
		last = NULL;
		node* tmp = v.last;
		while(tmp != NULL){
			(*this).push_back(tmp->val);
			tmp = tmp->next;
		}
		
	};
        Vector& operator=(const Vector& a){
		Vector  res(a);	
		return res;
	};
        int len(){
		return length;	
	};
        T& operator[](int n){
		node* tmp = last;
		int c = length-n-1;
                while(c != 0){
                        tmp= last->next;
                }
		return tmp->val;
	
	};
        ~Vector(){
		node* tmp;
		first = NULL;
		while(last != NULL){
                        tmp = last;
			last= last->next;
                        delete tmp;
                }
	
	}
	void print(){
		node* tmp = last;
                while(tmp != NULL){
                	std::cout << "[" << tmp->val << "]" << std::endl;   
		    	tmp = tmp->next;	
                }
	}
};


class StringList :public Vector<String> {


	virtual int contains(const Data b){
		node* dop = last;
		const char* a = b.str;
                int c = this->len()-1;
                while(dop != NULL){
			//Data p(a);
                	if(dop->val.contains(a) != -1 ){

                        	return c;
                        }
                        c--;
                        dop = dop->next;
                }
               	return -1;


	}


};




