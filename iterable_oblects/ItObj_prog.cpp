#include "Itobj.h"

int main(){
	String a = "Art";
	std::cout << a << std::endl;
	a[0] = 'a';
	std::cout << a << std::endl;
	std::cout << a.len() << std::endl;
	String b = a;
	std::cout << b << std::endl;
	String c = a+b;
	std::cout << c << std::endl;
	String* p_a = &a;
	IterableObject* p = p_a;
	std::cout << p->contains("ar") << std::endl;


	Vector<int> v1;
	v1.push_back(1);
        std::cout << "ok" << std::endl;
	v1.push_back(2);
	std::cout << "ok" << std::endl;
	v1.push_back(3);
	std::cout << "ok" << std::endl;
	v1.print();
	Vector<int>* p_v1 = &v1;
        IterableObject* pv = p_v1;
        std::cout << pv->contains(1) << std::endl;
	//std::cout << "ok" << std::endl;
	std::cout << pv->contains(2) << std::endl;
	std::cout << pv->contains(3) << std::endl;
	

	StringList v2;
	std::cout <<"ok" << std::endl;
        v2.push_back("art");
        std::cout << "ok" << std::endl;
        v2.push_back("aaa");
        std::cout << "ok" << std::endl;
        v2.push_back("bbb");
        std::cout << "ok" << std::endl;
        v2.print();
        StringList* p_v2 = &v2;
        IterableObject* pvs = p_v2;
	std::cout <<"error" << std::endl;
        std::cout << pvs->contains("ar") << std::endl;
        //std::cout << "ok" << std::endl;
        std::cout << pvs->contains("aa") << std::endl;
        std::cout << pvs->contains("bb") << std::endl;
	std::cout <<pvs->contains("2215") << std::endl;

}
