#include <iostream>
#include <cstring>
#include <typeinfo>

struct Data{

	int v_int = 0;
	double v_L = 0;
	char* str = NULL;

	Data(int n){
		v_int = n;
	}
	Data(double d){
		v_L = d;
	}
	Data(const char* s){
		str = new char[strlen(s)+1];
		strcpy(str,s);
	}
	~Data(){
		delete[] str;
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
	virtual int contains(const Data* b);
	virtual ~IterableObject() {}
};

//String////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class String : IterableObject{
	char*  pointer;
public:
         //virtual int contains(const int a);
         //virtual int contains(const double a);
	 virtual int contains(const Data* b){
		const char* a = b->str;
		char* cmp1 = new char[strlen(a)+1];
		char* dop = pointer;
		int c = 0;
		while(dop != NULL){
			strcpy(cmp1,dop);
			if(!strcmp(cmp1,a)){
				delete[] cmp1;
				return c;
			}
			c++;
			dop++;
		}
		delete[] cmp1;
		return -1;
	 };
	 String(const char* a = ""){
		length = strlen(a);
                pointer = new char[length + 1];
                strcpy(pointer,a);	 
	 };
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
	operator char*(){
		return pointer;
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
	dop = a.pointer;
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
class Vector : IterableObject{
protected:
	struct node{
		T val = 0;
		node* next;
	};
	node* first;
	node* last;
public:
	virtual int contains(const Data* b){

                node* dop = last;
		if(typeid(T) == typeid(b->v_int)){
			int a = b->v_int;
			int c = length-1;
                	while(dop != NULL){
                        	if(a == dop->val){

                                	return c;
                        	}
                        	c--;
                        	dop = last->next;;
                	}
                	return -1;

		}else if(typeid(T) == typeid(String)){
			
			String a = String(b->str);
                        int c = length-1;
                        while(dop != NULL){
                                if(a == dop->val){

                                        return c;
                                }
                                c--;
                                dop = last->next;;
                        }
                        return -1;

			
		}else{
			double a = b->v_L;
			int c = length-1;
                	while(dop != NULL){
                        	if(a == dop->val){
                
                                	return c;
                        	}
                        	c--;
                        	dop = last->next;;
                	}
                	return -1;
         	}
	};
	Vector() : first(NULL), last(NULL){};
	void push_back(T a){
		if (first == NULL){
			
			last = new node;
			last->val = a;
			last->next = NULL;
			first = last;
		}else{
			node* tmp = new node;
			tmp->val = a;
			tmp->next = last;
			last = &tmp;
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
};


class StringList :public Vector<String> {

	virtual int contains(const Data* b){
		node* dop = last;
		const char* a = b->str;
                int c = this->len()-1;
                while(dop != NULL){
			Data p(a);
                	if(dop->val.contains(&p) != -1 ){

                        	return c;
                        }
                        c--;
                        dop = last->next;;
                }
               	return -1;


	}


};




