// Token type
// 'struct' as this implies the data type is public
struct Token {
	char kind;
	double value;
	string name;
	Token(char ch) :kind(ch) { }
	Token(char ch, double val) :kind(ch), value(val) { }
	Token(char ch, string n) :kind{ch}, name{n} { }
};

// Token_stream type
class Token_stream {
public:
	Token_stream() :full(0), buffer(0) { }

	Token get();			// get a token

	void unget(Token t) 		// put a token back
	{ 
		buffer=t; full=true;
	}

	void ignore(char c);		// discards characters up to and including a c

private:
	bool full {false};			// is there a token in the buffer
	Token buffer;			// here is where we keep a Token put back using unget()
};

// read characters from cin and compose a token
Token Token_stream::get()
{
	if (full) {	// check if we already have a Token ready
		full=false;
		return buffer;
	}

	char ch;
	cin.get(ch);

	// Keep on getting input until we get something that isn't a space
	while (ch== ' ') cin.get(ch);

	switch (ch) {
	// Handle newline and the print command as the same thing
	// case newline1: case newline2: 
	// 	cout << "\nnewline detected.";
	// 	return Token(print);
	case print:
		return Token(print);
	case newline:
		return Token(newline);
	case '(': case '{':
	case ')': case '}':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case '=':
	case '!':
	case ',':
			return Token(ch); 	// let each character represent itself
	case '.':					// a floating point literal can start with a dot
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':	// numeric literal
		{	cin.unget();		// put digit back into input stream
			double val;
			cin >> val;		// read a floating-point number
			return Token(number,val);
		}
	case '#':
	 		return Token(let);
	case '~':
			return Token(change);
	default:
		if (isalpha(ch) || ch=='_') {
			string s;			// Set up a string variable
			s += ch;		// Append latest alpha charcter onto string variable
			while(cin.get(ch) && (isalpha(ch) || isdigit(ch)||ch=='_'))	// keep on reading while we have either digits, alphas or underscores
				s+=ch;
			cin.unget();		// Put the last character back into input stream. i.e. '+'
			// if (s == declkey) return Token(let);	
			if (s == quitkey) return Token(quit);
			if (s == chngkey) return Token(change);
			if (s == sqrtkey) return Token(squarert);
			if (s == powkey) return Token(power);
			if (s == constkey) return Token(constant);
			if (s == helpkey) return Token(help);
			return Token(name,s);
		}
		error("Bad token");
	}
}

void Token_stream::ignore(char c)
	// c represents the kind of token
{
	// first look in buffer:
	if (full && c==buffer.kind ) {
		full = false;
		return;
	}
	full = false;

	// now search input:
	char ch;
	while (cin >>ch)
		if (ch==c) return;

	return;
}