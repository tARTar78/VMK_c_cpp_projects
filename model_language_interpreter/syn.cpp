#include <iostream>
#include <string>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
using namespace std;
int pr_id = 0;
enum type_of_lex {
    LEX_NULL,                                                                                   /* 0*/
    LEX_AND, LEX_BEGIN, LEX_STRING, LEX_DO, LEX_ELSE, LEX_END, LEX_IF, LEX_FALSE, LEX_INT,        /* 9*/
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_TRUE, LEX_BOOL, LEX_WHILE, LEX_WRITE,  /*18*/
    LEX_FIN,                                                                                    /*19*/
    LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS,   /*27*/
    LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ,              /*35*/
    LEX_NUM,LEX_STR,    										/*36-37*/
    LEX_ID,                                                                                     /*38*/
    POLIZ_LABEL,                                                                                /*39*/
    POLIZ_ADDRESS,                                                                              /*40*/
    POLIZ_GO,                                                                                   /*41*/
    POLIZ_FGO                                                                                   /*42*/
};

int mas[43];

class Lexem{
	type_of_lex l_t;
	int l_v;
	string l_s;
public:
	Lexem(type_of_lex a = LEX_NULL, int b = 0, string s = ""){

		l_t = a;
		l_v = b;
	}
	type_of_lex get_type() const {
		return l_t;
	}
	int get_val() const {
		return l_v;
	}
	string get_value_s(){
		return l_s;
	}
	friend ostream& operator <<(ostream & s, Lexem a);
};

class Ident {
    string      name;
    bool        declare;
    type_of_lex type;
    bool        assign;
    int          value;
    string value_s;
public:
    Ident() {
        declare = false;
        assign  = false;
    }
    bool operator== ( const string& s ) const {
        return name == s;
    }
    Ident (const string n){
        name    = n;
        declare = false;
	assign  = false;
	
    }
    string get_name () const {
      return name;
    }
    bool get_declare () const {
      return declare;
    }
    void set_declare () {
      declare   = true;
    }
    type_of_lex get_type () const {
      return type;
    }
    void set_type ( type_of_lex t ) {
      type      = t;
    }
    bool get_assign () const {
      return assign;
    }
    void set_assign () {
      assign    = true;
    }
    int  get_value () const {
      return value;
    }
    void set_value ( int v ) {
      value     = v;
    }
    void set_value_s(string s){
		value_s = s;
    }
};

vector<Ident> TID;

int put ( const string & buf) {
    vector<Ident>::iterator k;

    if ( ( k = find ( TID.begin (), TID.end (), buf ) ) != TID.end () )
        return k - TID.begin();
    TID.push_back ( Ident(buf) );
    return TID.size () - 1;
}

class Scanner{

	FILE *fp;
	char c;
	int look(const string& buf, const char** lst){
		int k = 0;
		while(lst[k]){
			if (buf == lst[k])
				return k;
			++k;
		}
		return 0;
	}
	void gc() { c = fgetc(fp);}
public:
	static const char * TW[], * TD[];
	Scanner (const char* prog){
		fp = fopen(prog,"r");
		if(fp == nullptr)
			throw "error opening file";
	}
	~Scanner(){
		fclose(fp);
	}
	Lexem get_lex();

};


const char * Scanner::TW[] = { "", "and", "{", "string", "do", "else", "}", "if", "false", "int", "not", "or", "program",
                      "read", "then", "true", "bool", "while", "write", NULL };
 
const char * Scanner::TD[] = { "@", ";", ",", ":", "=", "(", ")", "==", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", NULL };



Lexem Scanner::get_lex() {
    enum    state { H, IDENT, NUMB, STR, COM, ALE, NEQ };
    int     d, j;
    string s1;
    string  buf;
    state   CS = H;
    do {
        gc();
        switch ( CS ) {
            case H:
                if ( c==' ' || c == '\n' || c== '\r' || c == '\t' );
		    else if(c == '{'){
			return Lexem(LEX_BEGIN);
		    }
		    else if(c == '}'){
			return Lexem(LEX_END);
		    }
		    else if(c == '\"'){
			s1.push_back(c);
			CS = STR;
		    }
                    else if ( isalpha (c) ) {
                        buf.push_back (c);
                        CS  = IDENT;
                    }
                    else if ( isdigit (c) ) {
                        d   = c - '0';
                        CS  = NUMB;
                    }
                    /*else if ( c== '{' ) {
                        CS  = COM;
                    }*/
                    else if ( c == '=' || c == '<' || c == '>' ) {
                        buf.push_back (c);
			//cout <<"assign" << endl;
                        CS  = ALE;
                    }
                    else if (c == EOF)
                        return Lexem ( LEX_FIN );
                    else if (c == '!') {
                        buf.push_back (c);
                        CS  = NEQ;
                    }
                    else {
                        buf.push_back (c);
                        if ( ( j = look ( buf, TD) ) ){
                            return Lexem ( (type_of_lex)( j + (int) LEX_FIN ), j );
                        }
                        else
                            throw c;
                    }
                    break;
            case IDENT:
                if ( isalpha (c) || isdigit (c) ) {
                    buf.push_back (c);
                }
                else {
                    ungetc ( c, fp );
                    if ( (j = look ( buf, TW) ) ) {
                        return Lexem ( (type_of_lex) j, j );
                        }
                        else {
                        j   = put ( buf );
			pr_id = j;
                        return Lexem ( LEX_ID, j );
                    }
                }
                break;
            case NUMB:
                if ( isdigit (c) ) {
                    d = d * 10 + ( c - '0' );
                }
                else {
                    ungetc ( c, fp );
                    return Lexem ( LEX_NUM, d );
                }
                break;
	    case STR:
		s1.push_back(c);
		if(c == '\"')
			return Lexem(LEX_STR,100,s1);
		break;
            /*work*/case COM:
                if ( c == '}' ) {
                    CS  = H;
                }
                else if ( c == '@' || c == '{' )
                    throw c;
                break;//work
            case ALE:
		//cout <<"assign" << endl;
                if ( c == '=' ) {
                    buf.push_back ( c );
                    j   = look ( buf, TD );
                    return Lexem ( (type_of_lex) ( j + (int) LEX_FIN ), j );
                }
                else {
                    ungetc ( c, fp );
		    //cout << "!!!!" << endl;
                    j   = look ( buf, TD );
                    return Lexem ( (type_of_lex) ( j + (int) LEX_FIN ), j );
                }
                break;
            case NEQ:
                if ( c == '=' ) {
                    buf.push_back(c);
                    j   = look ( buf, TD );
                    return Lexem ( LEX_NEQ, j );
                }
                else
                    throw '!';
                break;
    } //end switch
  } while (true);
}



ostream & operator<< ( ostream &s, Lexem l ) {
    string t;
    if ( l.l_t <= LEX_WRITE )
        t = Scanner::TW[l.l_t];
    else if ( l.l_t >= LEX_FIN && l.l_t <= LEX_GEQ )
        t = Scanner::TD[ l.l_t - LEX_FIN ];
    else if ( l.l_t == LEX_NUM )
        t = "NUMB";
    else if(l.l_t == LEX_STR)
	    t = "STR";
    else if ( l.l_t == LEX_ID )
        t = TID[l.l_v].get_name ();
    else if ( l.l_t == POLIZ_LABEL )
        t = "Label";
    else if ( l.l_t == POLIZ_ADDRESS )
        t = "Addr";
    else if ( l.l_t == POLIZ_GO )
        t = "!";
    else if ( l.l_t == POLIZ_FGO )
        t = "!F";
    else
        throw l;
    s << '(' << t << ',' << l.l_v << ");" << endl;
    return s;
}

template <class T, class T_EL>
void from_st ( T & st, T_EL & i ) {
    i = st.top(); st.pop();
}

class Parser {
    char prev_type;
    Lexem          curr_lex;
    type_of_lex  c_type;
    int          c_val;
    string 	 c_val_s;
    Scanner      scan;
    stack < int >           st_int;
    stack < type_of_lex >   st_lex;
    void  P();
    void  D1();
    void  D();
    void A();
    void B();
    void T();
    void V();
    void  S();
    void  E();
    void  E1();
    void S1();
    void  K();
    void  F();
    void  dec ( type_of_lex type);
    void  check_id ();
    void  check_op ();
    void  check_not ();
    void  eq_type ();
    void  eq_bool ();
    void  check_id_in_read ();
    void  gl () {
        curr_lex  = scan.get_lex ();
	cout << "OK" << endl;
	cout << curr_lex << endl;
	//mas[(int)c_type]++;
        c_type    = curr_lex.get_type ();
	mas[(int)c_type]++;
        c_val     = curr_lex.get_val ();
	c_val_s = curr_lex.get_value_s();
    }
public:
    vector <Lexem> poliz;
    Parser ( const char *program ) : scan (program) { }
    void  analyze();
};

void Parser::analyze () {
    gl ();
    P ();
    //cout << "aaa" << endl;
    if (c_type != LEX_FIN)
        throw curr_lex;
    //for_each( poliz.begin(), poliz.end(), [](Lex l){ cout << l; });
    for ( Lexem l : poliz )
        cout << l;
    cout << endl << "Yes!!!" << endl;
}

void Parser::P () {
    if ( c_type == LEX_PROGRAM ) {
        gl ();
    }
    else
        throw curr_lex;
    D1 ();
    /*if ( c_type == LEX_SEMICOLON )
        gl ();
    else
        throw curr_lex;
    B ();*/
}

void Parser::D1 () {
    if ( c_type == LEX_BEGIN) {
        gl();
        D();
	if(c_type != LEX_END)
		throw curr_lex;
	gl();
    }else
        throw curr_lex;
}

/*void Parser::D () {
    if ( c_type != LEX_ID )
        throw curr_lex;
    else {
        st_int.push ( c_val );
        gl ();
        while ( c_type == LEX_COMMA ) {
            gl ();
            if ( c_type != LEX_ID )
                throw curr_lex;
            else {
                st_int.push ( c_val );
                gl ();
            }
        }
        if ( c_type != LEX_COLON )
            throw curr_lex;
        else {
            gl ();
            if ( c_type == LEX_INT ) {
                dec ( LEX_INT );
                gl ();
            }
            else
                if ( c_type == LEX_BOOL ) {
                    dec ( LEX_BOOL );
                    gl ();
                }
                else
                    throw curr_lex;
        }
    }
}*/

void Parser::D() {

	//cout << curr_lex << endl;	
	while(c_type == LEX_INT || c_type == LEX_STRING){
		A();
		//cout << curr_lex << endl;
		if(c_type != LEX_SEMICOLON)
			throw curr_lex;
		gl();
	}
	S1();

}

void Parser::A() {
	T();
	V();
	//cout << "A" << curr_lex << endl;
	while(c_type == LEX_COMMA){
		gl();
		V();
	}


}

void Parser::T(){
	if(c_type == LEX_INT){
		gl();
		//dec ( LEX_INT );
		//cout << "2!!!!" << endl;
		prev_type = 'i';
               
	}else if(c_type == LEX_STRING){
		gl();
		//dec ( LEX_STRING );
		prev_type ='s';
               
	}else{
		throw curr_lex;
	}
}

void Parser::V(){

	if(c_type != LEX_ID){
		throw curr_lex;
	}else{
		st_int.push ( c_val );
		if(prev_type == 'i')
			dec(LEX_INT);
		else
			dec(LEX_STRING);
        	gl ();
	}
	if(c_type == LEX_ASSIGN){
		gl();
		//cout << curr_lex << endl;
		if(c_type == LEX_NUM){
			if(prev_type != 'i'){
				throw "wrong type";
			}
			TID[pr_id].set_value(c_val);
			TID[pr_id].set_assign();
		}else if(c_type == LEX_STR){
                        if(prev_type != 's'){
                                throw "wrong type";
                        }
			TID[pr_id].set_value_s(c_val_s);
			TID[pr_id].set_assign();
                }else{
				throw curr_lex;
		}
		gl();

	}

}


/*void Parser::B () {
    if ( c_type == LEX_BEGIN ) {
        gl ();
        S ();
        while ( c_type == LEX_SEMICOLON ) {
            gl ();
            S ();
        }
        if ( c_type == LEX_END ) {
            gl ();
        }
        else {
            throw curr_lex;
        }
    }
    else
        throw curr_lex;
}*/

/*void Parser::S1(){

	if(c_type != LEX_END){
		if(c_type == LEX_BEGIN){
			gl();
			while(c_type == LEX_IF ||c_type == LEX_READ || c_type == LEX_WRITE || c_type == LEX_WHILE || c_type == LEX_ID){
				//cout << "S1" << curr_lex << endl;
				S();
			
			}
		
		if(c_type != LEX_END)
			throw curr_lex;
	}else{
		while(c_type == LEX_IF ||c_type == LEX_READ || c_type == LEX_WRITE || c_type == LEX_WHILE || c_type == LEX_ID){
                                //cout << "S1" << curr_lex << endl;
                                S();

                        }
		}
	}

}*/


void Parser::S1(){
	bool chg = false;
        if(c_type != LEX_END){
                if(c_type == LEX_BEGIN){
			chg = true;
                        gl();
		}
                        while(c_type == LEX_IF ||c_type == LEX_READ || c_type == LEX_WRITE || c_type == LEX_WHILE || c_type == LEX_ID){
                                cout << "S1" << curr_lex << endl;
                                S();


                        }
			cout << chg << endl;
                if(c_type != LEX_END && chg == true)
                       throw curr_lex;
		if(chg == true)
			gl();	
                /*while(c_type == LEX_IF ||c_type == LEX_READ || c_type == LEX_WRITE || c_type == LEX_WHILE || c_type == LEX_ID){
                                cout << "S1" << curr_lex << endl;
                                S();
                        }*/
                
        }

}


void Parser::S () {
    int pl0, pl1, pl2, pl3;

    if ( c_type == LEX_IF ) {
        gl ();
        E ();
        eq_bool ();
        pl2 = poliz.size();
        poliz.push_back ( Lexem() );
        poliz.push_back ( Lexem(POLIZ_FGO) );
        if ( c_type == LEX_THEN ) {
            gl ();
            S ();
            pl3 = poliz.size ();
            poliz.push_back ( Lexem() );

            poliz.push_back ( Lexem ( POLIZ_GO ) );
            poliz[pl2] = Lexem ( POLIZ_LABEL, poliz.size() );

            if ( c_type == LEX_ELSE ) {
                gl ();
                S ();
                poliz[pl3] = Lexem ( POLIZ_LABEL, poliz.size() );
            }
            else
                throw curr_lex;
        }
        else
            throw curr_lex;
    }//end if
    else if ( c_type == LEX_WHILE ) {
        pl0 = poliz.size ();
        gl ();
        E ();
        eq_bool ();
        pl1 = poliz.size ();
        poliz.push_back ( Lexem () );
        poliz.push_back ( Lexem (POLIZ_FGO) );
        if ( c_type == LEX_DO ) {
            gl();
	    cout << curr_lex << endl;
            S1();
            poliz.push_back ( Lexem ( POLIZ_LABEL, pl0 ) );
            poliz.push_back ( Lexem ( POLIZ_GO) );
            poliz[pl1] = Lexem ( POLIZ_LABEL, poliz.size() );
        }
        else
            throw curr_lex;
        }//end while
        else if ( c_type == LEX_READ ) {
            gl ();
            if ( c_type == LEX_LPAREN ) {
                gl ();
                if ( c_type == LEX_ID ) {
			//cout <<"S" << curr_lex << endl;
                    check_id_in_read ();
                    poliz.push_back ( Lexem( POLIZ_ADDRESS, c_val) );
                    gl();
                }
                else
                    throw curr_lex;
                if ( c_type == LEX_RPAREN ) {
                    gl ();
                    poliz.push_back ( Lexem(LEX_READ) );
                }
                else
                    throw curr_lex;
            }
            else
                throw curr_lex;
	    if(c_type == LEX_SEMICOLON){
			gl();
	    }else{
		throw curr_lex;
	    }
        }//end read
        else if ( c_type == LEX_WRITE ) {
            gl ();
            if ( c_type == LEX_LPAREN ) {
                gl ();
                E ();
                if ( c_type == LEX_RPAREN ) {
                    gl ();
                    poliz.push_back ( Lexem ( LEX_WRITE ) );
                }
                else
                    throw curr_lex;
            }
            else
                throw curr_lex;
	    if(c_type == LEX_SEMICOLON){
                        gl();
            }else{
                throw curr_lex;
            }
        }//end write
        else if ( c_type == LEX_ID ) {
            check_id ();
            poliz.push_back (Lexem ( POLIZ_ADDRESS, c_val ) );
            gl();
            if ( c_type == LEX_ASSIGN ) {
		//cout <<"S"<< curr_lex << endl;
                gl ();
                E ();
                eq_type ();
                poliz.push_back ( Lexem ( LEX_ASSIGN ) );
            }
            else
                throw curr_lex;
	    if(c_type == LEX_SEMICOLON){
                        gl();
            }else{
                throw curr_lex;
            }

        }//assign-end
        else
            B();
}

void Parser::B(){
	if(c_type != LEX_BEGIN){
		throw curr_lex;
	}
	gl();
	S();
	if(c_type != LEX_END){
		throw curr_lex;
	}

}

void Parser::E () {
    E1 ();
    //cout << "E" << curr_lex << endl;
    if ( c_type == LEX_EQ  || c_type == LEX_LSS || c_type == LEX_GTR ||
         c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ ) {
        st_lex.push ( c_type );
        gl ();
        E1 ();
        check_op ();
    }
}

void Parser::E1 () {
    K();
    while ( c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR) {
        st_lex.push ( c_type );
        gl ();
        K();
        check_op ();
    }
}

void Parser::K () {
    F ();
    while ( c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND) {
        st_lex.push ( c_type );
        gl ();
        F ();
        check_op ();
    }
}

void Parser::F () {
	//cout << "FF" << curr_lex << endl;
    if ( c_type == LEX_ID ) {
	//cout <<"F" << curr_lex << endl;
        check_id ();
        poliz.push_back ( Lexem ( LEX_ID, c_val ) );
        gl ();
    }
    else if ( c_type == LEX_NUM ) {
	//cout << "F" << curr_lex << endl;
        st_lex.push ( LEX_INT );
        poliz.push_back ( curr_lex );
        gl ();
    }
    else if ( c_type == LEX_TRUE ) {
        st_lex.push ( LEX_BOOL );
        poliz.push_back ( Lexem (LEX_TRUE, 1) );
        gl ();
    }
    else if ( c_type == LEX_FALSE) {
        st_lex.push ( LEX_BOOL );
        poliz.push_back ( Lexem (LEX_FALSE, 0) );
        gl ();
    }
    else if ( c_type == LEX_NOT ) {
        gl ();
        F ();
        check_not ();
    }
    else if ( c_type == LEX_LPAREN ) {
        gl ();
        E ();
        if ( c_type == LEX_RPAREN)
            gl ();
        else
            throw curr_lex;
    }
    else
        throw curr_lex;
}


void Parser::dec ( type_of_lex type ) {
    int i;
    while ( !st_int.empty () ) {
        from_st ( st_int, i );
        if ( TID[i].get_declare () )
            throw "twice";
        else {
            TID[i].set_declare ();
            TID[i].set_type ( type );
        }
    }
}

void Parser::check_id () {
    if ( TID[c_val].get_declare() ){
	    cout <<"type"<< TID[c_val].get_type() << endl;
        st_lex.push ( TID[c_val].get_type () );
    }
    else
        throw "not declared";
}

void Parser::check_op () {
    type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOL;

    from_st ( st_lex, t2 );
    from_st ( st_lex, op );
    from_st ( st_lex, t1 );

	cout << "cp" << Lexem(t1) << endl;
	cout << "cp" << Lexem(t2) << endl;
	cout << "cp" << Lexem(op) << endl;
    	

    if(t1 == LEX_INT){  	
    if ( op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH )
        r = LEX_INT;
    }else if(t1 == LEX_STR){
	if(op == LEX_PLUS)
		r = LEX_STR;
	else
		throw "wrong operation for string";
    }
    if ( op == LEX_OR || op == LEX_AND )
        t = LEX_BOOL;
    if ( t1 == t2  &&  t1 == t )
        st_lex.push (r);
    else
        throw "wrong types are in operation";
    poliz.push_back (Lexem(op) );
}

void Parser::check_not () {
    if (st_lex.top() != LEX_BOOL)
        throw "wrong type is in not";
    else
        poliz.push_back ( Lexem (LEX_NOT) );
}

void Parser::eq_type () {
    type_of_lex t;
    from_st ( st_lex, t );
    if ( t != st_lex.top () )
        throw "wrong types are in :=";
    st_lex.pop();
}

void Parser::eq_bool () {
    if ( st_lex.top () != LEX_BOOL )
        throw "expression is not boolean";
    st_lex.pop ();
  }

void Parser::check_id_in_read () {
    if ( !TID [c_val].get_declare() )
        throw "not declared";
}

class Executer {
public:
    void execute ( vector<Lexem> & poliz );
};
 
void Executer::execute ( vector<Lexem> & poliz ) {
    Lexem pc_el;
    stack < int > args;
    int i, j, index = 0, size = poliz.size();
    while ( index < size ) {
        pc_el = poliz [ index ];
        switch ( pc_el.get_type () ) {
            case LEX_TRUE: case LEX_FALSE: case LEX_NUM: case POLIZ_ADDRESS: case POLIZ_LABEL:
                args.push ( pc_el.get_val() );
                break;
 
            case LEX_ID:
                i = pc_el.get_val();
                if ( TID[i].get_assign () ) {
                  args.push ( TID[i].get_value() );
                  break;
                }
                else
                  throw "POLIZ: indefinite identifier";
 
            case LEX_NOT:
                from_st ( args, i );
                args.push( !i );
                break;
 
            case LEX_OR:
                from_st ( args, i ); 
                from_st ( args, j );
                args.push ( j || i );
                break;
 
            case LEX_AND:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j && i );
                break;
 
            case POLIZ_GO:
                from_st ( args, i );
                index = i - 1;
                break;
 
            case POLIZ_FGO:
                from_st ( args, i );
                from_st ( args, j );
                if ( !j ) index = i - 1;
                break;
 
            case LEX_WRITE:
                from_st ( args, j );
                cout << j << endl;
                break;
 
            case LEX_READ:
                int k;
                from_st ( args, i );
                if ( TID[i].get_type () == LEX_INT ) {
                    cout << "Input int value for" << TID[i].get_name () << endl;
                    cin >> k;
                }
                else {
                    string j;
                    while (1) {
                        cout << "Input boolean value (true or false) for" << TID[i].get_name() << endl;
                        cin >> j;
                        if ( j != "true" && j != "false" ) {
                            cout << "Error in input:true/false" << endl;
                            continue;
                        }
                        k = ( j == "true" ) ? 1 : 0;
                        break;
                    }
                }
                TID[i].set_value (k);
                TID[i].set_assign ();
                break;
 
            case LEX_PLUS:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( i + j );
                break;
 
            case LEX_TIMES:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( i * j );
                break;
 
            case LEX_MINUS:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j - i );
                break;
 
            case LEX_SLASH:
                from_st ( args, i );
                from_st ( args, j );
                if (i) {
                    args.push ( j / i );
                    break;
                }
                else
                    throw "POLIZ:divide by zero";
 
            case LEX_EQ:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( i == j );
                break;
 
            case LEX_LSS:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j < i );
                break;
 
            case LEX_GTR:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j > i );
                break;
 
            case LEX_LEQ:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j <= i );
                break;
 
            case LEX_GEQ:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j >= i );
                break;
 
            case LEX_NEQ:
                from_st ( args, i );
                from_st ( args, j );
                args.push ( j != i );
                break;
 
            case LEX_ASSIGN:
                from_st ( args, i );
                from_st ( args, j );
                TID[j].set_value (i);
                TID[j].set_assign (); 
                break;
 
            default:
                throw "POLIZ: unexpected elem";
        }//end of switch
        ++index;
    };//end of while
    cout << "Finish of executing!!!" << endl;
}
 
class Interpretator {
    Parser   pars;
    Executer E;
public:
    Interpretator ( const char* program ): pars (program) {}
    void     interpretation ();
};
 
void Interpretator::interpretation () {
    pars.analyze ();
    E.execute ( pars.poliz );
}
 
int main () {
    try {
        Interpretator I ("sum.txt");
        I.interpretation ();
        return 0;
    }
    catch ( char c ) {
        cout << "unexpected symbol " << c << endl;
        return 1;
    }
    catch ( Lexem l ) {
        cout << "unexpected lexeme" << l << endl;
        return 1;
    }
    catch ( const char *source ) {
        cout << source << endl;
        return 1;
    }
}
 

int main(){
	try{
		Parser art("prog_art.txt");
		art.analyze();
		//return 0;
	} catch ( char c ) {
        cout << "unexpected symbol " << c << endl;
        //return 1;
    }
    catch ( Lexem l ) {
        cout << "unexpected lexeme" << l << endl;
        //return 1;
    }
    catch ( const char *source ) {
        cout << source << endl;
        //return 1;
    }

    int max = 0, mx_i = 0;
    for(int i = 0; i < 43; i++){
	    cout << i << ":" << mas[i] << endl;
		if( mas[i] > max){
			max = mas[i];
			mx_i = i;
		}
    }
    if(mx_i == 38){
	cout << "LEX_ID" << endl;
    }else
    	cout << Lexem((type_of_lex)mx_i) << endl;

}
