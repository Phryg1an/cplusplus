
/*
	Simple calculator

	Revision history

		Revised by Eleanor Gidman May 2015
		Revised by Bjarne Stroustrup November 2013
		Revised by Bjarne Stroustrup May 2007
		Revised by Bjarne Stroustrup August 2006
		Revised by Bjarne Stroustrup August 2004
		Origionally written by Bjarne Stroustrup
			(bs@cs.tamu.edu) Spring 2004.

	This program implements a basic expression calculator.
	Input from cin; output to cout.
	The grammar for input is:

	Calculation:
		Statement
		Print
		Quit
		Calculation Statement

	Print:
		;

	Quit:
		q

	Statement:
		Declaration
		Expression
	
	Declaration:
		"let" Name "=" Expression

	Expression:
		Term
		Expression + Term
		Expression - Term

	Term:
		Primary
		Term * Primary
		Term / Primary
		Term % Primary

	Primary:
		Number
		( Expression )
		- Primary
		+ Primary
		! Primary
		"sqrt" ( Primary )

	Number:
		floating-point-literal


	Input comes from cin through the Token_stream called ts.
*/

// Include Bjarne's facilities
#include "std_lib_facilities.h"
#include "help.cpp"
#include "constants.h"
#include "token_stream.cpp"
#include "variable.cpp"


// Declare a Token_stream
Token_stream ts;
Symbol_table st;

// Function to deal with factorial
long factorial(int x)
{
    long fact = 1;

    if (x>=0)
    {
        if (x==0) return fact;

        for (int i = 2; i < x+1; ++i) {
            fact *=i;
            if (fact<=0) error("Product of factorial too large");
        }

        return fact;
    }

    error("Factorial cannot be a negative number");    
}

// Declare expression() early
double expression(Token_stream& ts);

double primary(Token_stream& ts)
{
	double d;
	Token t = ts.get();

	switch (t.kind) {
	case '(': case '{':
	{	d = expression(ts);
		t = ts.get();
		if (t.kind != ')' && t.kind != '}'){
			cin.unget();
			error("')' or '}' expected");
		}
		return d;
	}
	case squarert:
		d = expression(ts);
		if (d < 0) error("Cannot take square root of negative number");
		return sqrt(d);
	case power:
	{	t = ts.get(); 
		if (t.kind=='(' | t.kind == '{')
			d = expression(ts);
		t = ts.get();
		if (t.kind != ',')
			error("',' expected");
		int d2 = narrow_cast<int>(expression(ts));
		t = ts.get(); 
		if (t.kind != ')' && t.kind != '}') error("')' or '}' expected");
		return pow(d,d2);
	}
	case '!':
		return double(factorial(int(primary(ts)))); 
	case '-':
		return - primary(ts);
	case '+':
		return primary(ts);
	case number:
		return t.value;
	case name:
		return st.get_value(t.name);
	default:
		error("primary expected");
	}
}

double term(Token_stream& ts)
{
	double left = primary(ts);

	while(true) {
		Token t = ts.get();
		switch(t.kind) {
		case '*':
			left *= primary(ts);
			break;
		case '/':
		{	double d = primary(ts);
			if (d == 0) error("divide by zero");
			left /= d;
			break;
		}
		case '%':
		{
            		double m = primary(ts);
            		left = fmod(left,m);
            		break;
            	}
		default:
			ts.unget(t);
			return left;
		}
	}
}

double expression(Token_stream& ts)
{
	double left = term(ts);
	while(true) {
		Token t = ts.get();
		switch(t.kind) {
		case '+':
			left += term(ts);
			break;
		case '-':
			left -= term(ts);
			break;
		default:
			ts.unget(t);
			return left;
		}
	}
}

double declaration(char type)
	// assume we have seen "let"
	// handle: name = expression
	// decalre a variable called "name" with the initial value "expression"
{
	Token t = ts.get();
	if (t.kind != name) error ("name expected in declaration");
	string var_name = t.name;

	Token t2 = ts.get();
	if (t2.kind != '=') error("= missing in declaration of " ,var_name);

	double d = expression(ts);

	// Handling based on creating new variable or overwriting existing
	if(type==constant) {
		st.define_name(var_name,d,constant);
	}
	if(type==let) {
		st.define_name(var_name,d);
	}	
	else if (type==change) {
		st.set_value(var_name,d);
	}
	return d;
}

double statement()
{
	Token t = ts.get();
	switch(t.kind) {
	case let: case change: case constant:
		return declaration(t.kind);
	default:
		ts.unget(t);
		return expression(ts);
	}
}

void clean_up_mess()
{
	ts.ignore(print);
}

const string prompt = "> ";
const string result = "= ";

void calculate()
{
	cout << prompt;
	while(true) try {
		Token t = ts.get();
		while (t.kind == print | t.kind == help | t.kind == newline)
		{ 
			if (t.kind==help) helptext();
			if (t.kind==newline) cout << prompt; 
			t=ts.get();
		}
		if (t.kind == quit) return;
		ts.unget(t);

		cout << result << statement() << endl;
	}
	catch(runtime_error& e) {
		cerr << e.what() << endl;
		clean_up_mess();
	}
}

int main()

	try {
		// Predefine names
		st.define_name("pi",3.1415926535,'c');
		st.define_name("e",2.7182818284,'c');
		st.define_name("k", 1000);

		calculate();

		return 0;
	}
	catch (exception& e) {
		cerr << "exception: " << e.what() << endl;
		char c;
		while (cin >>c && c!=';') ;
		return 1;
	}
	catch (...) {
		cerr << "exception\n";
		char c;
		while (cin>>c && c!=';');
		return 2;
	}
