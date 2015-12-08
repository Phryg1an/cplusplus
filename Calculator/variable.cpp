
// Variable type to hold constant vars, i.e. pi
struct Variable {
	string name;
	double value;
	char type;
	Variable(string var, double val) :name(var), value(val) { }
	Variable(string var, double val, char type) :name(var), value(val), type(type) { }
};

// Symbol_table type
class Symbol_table {
public:
	vector<Variable> var_table;

	double get_value(string var);
	void set_value(string var, double val);
	bool is_declared(string var);
	double define_name(string var, double val, char type);

};



// Array declaration for constant vars
// vector<Variable> var_table;	

// Gets a constant variable from the var_table
double Symbol_table::get_value(string var)
{
	for (const Variable& v : var_table)		// Loop through var_table
		if (v.name == var) return v.value;	// Did we find it?
	error("get: undefined variable ",var);		// No we didn't.
}

// Give a value to an already declared constant variable to the var_table
void Symbol_table::set_value(string var, double val)
{
	for (Variable& v : var_table)
		if (v.name == var) {
			if (v.type==constant) error("set: cannot change the constant variable ", var);
			v.value = val;
			return;
		}
	error("set: undefined variable ",var);
}

// Check if a constant variable exists in the var_table
bool Symbol_table::is_declared(string var)
{
	for (const Variable& v : var_table)
		if (v.name == var) return true;
	return false;
}

// add (var, val) to var_table
double Symbol_table::define_name(string var, double val, char type='0')
{
	if(is_declared(var)) error(var," declared twice");

	if (type!='0')
		var_table.push_back(Variable(var,val,type));
	else
		var_table.push_back(Variable(var,val));
	return val;
}