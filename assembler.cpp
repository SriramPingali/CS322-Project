#include <bits/stdc++.h>

using namespace std;

// Define structure for symbols
struct symbol
{
    string name;
    int address;
    bool set;
    int set_value;
};

// Define structure for literals
struct literal
{
	string literal;
	int address;
};

// Symbol table
vector<symbol> sym_table;

// Literal table
vector<literal> lit_table;

// Utility function to check of a string is a number
bool is_number(const std::string& s)
{
    int i = 0;
    string temp;
    temp = s;
    if (s.front() == '-' or s.front() == '+') 
    {
        temp = s.substr(1, s.length());
    }
    std::string::const_iterator it = temp.begin();
    while (it != temp.end() && std::isdigit(*it)) ++it;
    return !temp.empty() && it == temp.end();
}

// Utility function to check for elements in symbol table
bool contains(string name)
{
   auto iter = find_if(sym_table.begin(), sym_table.end(), 
               [&](const symbol& ts){return ts.name == name;});
   return iter != sym_table.end();
}

// Utility function to convert int to hex
string int_to_hex(int i)
{
  stringstream stream;
  stream << setfill ('0') << setw(8) 
         << hex << i;
  return stream.str();
}

map<string, string> mot;

// MOT table
void mot_init()
{
	mot["ldc"] = string("00");
	mot["adc"] = string("01");
	mot["ldl"] = string("02");
	mot["stl"] = string("03");
	mot["ldnl"] = string("04");
	mot["stnl"] = string("05");
	mot["add"] = string("06");
	mot["sub"] = string("07");
	mot["shl"] = string("08");
	mot["shr"] = string("09");
	mot["adj"] = string("0a");
	mot["a2sp"] = string("0b");
	mot["sp2a"] = string("0c");
	mot["call"] = string("0d");
	mot["return"] = string("0e");
	mot["brz"] = string("0f");
	mot["brlz"] = string("10");
	mot["br"] = string("11");
	mot["HALT"] = string("12");
	mot["data"] = string("13");
	mot["SET"] = string("14");
}

// Utility function to deal with white spaces
static inline string &trim(string &s) 
{
    s.erase(find_if(s.rbegin(), s.rend(),
            not1(ptr_fun<int, int>(isspace))).base(), s.end());
    s.erase(s.begin(), find_if(s.begin(), s.end(),
            not1(ptr_fun<int, int>(isspace))));
    return s;
}

// Reading instructions and add into literal and symbol table
void inst_to_table(string instr, int* loc_ptr, int line)
{
    int loc = *loc_ptr;

    // Identify label and variables
    if(instr.find(':') != string::npos)
    {
        int colon = instr.find(":", 0);

        if(contains(instr.substr(0, colon)))
        {
        	cout << "ERROR: Duplicate Label at line " << line << endl;
        }

        // Instruction could be present after the colon
        if(colon != instr.length() - 1)
        {
        	string subs = instr.substr(colon + 1, instr.length());
        	subs = trim(subs);
        	inst_to_table(subs, &loc, line);
        	int space = subs.find(" ", 0);
        	string sub_op = subs.substr(0, space);
        	string sub_val = subs.substr(space + 1, subs.length());
        	sub_op = trim(sub_op);
        	sub_val = trim(sub_val);

        	// Dealing with set instructions
        	if(sub_op == "SET")
        	{
        		sym_table.push_back({instr.substr(0, colon), loc, 1, stoi(sub_val)});
        	}
        	else
        	{
        		sym_table.push_back({instr.substr(0, colon), loc, 0, -1});
        	}
        }
        else
        {
        	*loc_ptr = *loc_ptr - 1;
        	sym_table.push_back({instr.substr(0, colon), loc, 0, -1});
        }
    }

    // Identify literals and constants
    else
    {
        // Considering that second part of instruction is operand
        int space = instr.find(" ", 0);
        string subs = instr.substr(space + 1, instr.length());
        subs = trim(subs);

        // Checking for numeral 
        if(is_number(subs))
        {
        	lit_table.push_back({subs, -1});
        }
    }
}

int analyse(string file)
{
    // Temp str variable for get line
    string line;
    int loc = 0;
    int line_count = 0;

    // Reading the input file
    ifstream MyFile(file);

    // Read individual lines
    while (getline (MyFile, line)) 
    {
        // Pre-process the line, trim extra spaces
        string instr;
        instr = line.substr(0, line.find(";", 0));
        instr = trim(instr);

        // Skip empty lines
        if(instr == "")
        {
            continue;
        }

        inst_to_table(instr, &loc, line_count++);
        loc++;
    }

    // LTORG loop for literals
    for(int i=0; i < lit_table.size(); i++)
    {
       	if(lit_table[i].address == -1)
       	{
       		lit_table[i].address = loc++;
       	}
    }

    	// For verification
 //    for(int i=0; i < sym_table.size(); i++)
 //        cout << sym_table[i].name << " " << sym_table[i].address << " " << sym_table[i].set << endl;

	// for(int i=0; i < lit_table.size(); i++)
 //        cout << lit_table[i].literal << " " << lit_table[i].address << endl;   


    // Close the file after EOF
    MyFile.close();
}

string inst_to_code(string instr, int* loc_ptr, int line)
{
	// Program Counter
	int loc = *loc_ptr;

	// Hex Code of Program COunter
	string encoding = int_to_hex(loc) + " ";

    // Identify label and variables
    if(instr.find(':') != string::npos)
    {
        int colon = instr.find(":", 0);

        // Instruction could be present after the colon
        if(colon != instr.length() - 1)
        {
        	string subs = instr.substr(colon + 1, instr.length());
        	subs = trim(subs);
        	string temp = inst_to_code(subs, &loc, line);
        	temp = temp.substr(9, 9);
        	encoding += temp;
        }
        // If no instruction after colon, PC shouldn't change
        else
        {
        	encoding += "         ";
        	*loc_ptr = *loc_ptr - 1;
        }
        encoding += instr + "\n";
    }

    // Identify literals and constants
    else
    {
        // Considering that second part of instruction is operand
        int space = instr.find(" ", 0);

        // Temp variable for operand encoding
        string hex_string;

        // String containing operation
        string sub_operation = instr.substr(0, space);

        // String containing operand
        string sub_operand = instr.substr(space + 1, instr.length());

        sub_operand = trim(sub_operand);
        sub_operation = trim(sub_operation);

        // Checking for No operand instructions
        if(sub_operation == "add" || sub_operation == "sub"
        		|| sub_operation == "shl"|| sub_operation == "shr"
        		|| sub_operation == "a2sp"|| sub_operation == "sp2a"
        		|| sub_operation == "return"|| sub_operation == "HALT")
        {
        	encoding += "000000" + mot[sub_operation] + " "; 
        	if(sub_operation.length() != instr.length())
			{
				cout << "ERROR: Operand not expected at line " << line << endl;
			}
        }
        // Checking for numeral operand to encode directly
        else if(is_number(sub_operand))
        {
        	hex_string = int_to_hex(stoi(sub_operand));
        	encoding += hex_string.substr(hex_string.length() - 6, hex_string.length()) + mot[sub_operation] + " "; 
        }
        // Checking for variable operand to encode address
        else
        {
        	int defined = 0;
        	for(int i=0; i < sym_table.size(); i++)
        	{
        		if(sym_table[i].name.compare(sub_operand) == 0)
        		{
        			defined = 1;
        			// SET variables considered numeral
        			if(sym_table[i].set)
        			{
        				hex_string = int_to_hex(sym_table[i].set_value);
        			}
        			// Operands which need offset from PC
        			else if(sub_operation == "call" || sub_operation == "brz"
        				|| sub_operation == "brlz"|| sub_operation == "br")
        			{
        				hex_string = int_to_hex(sym_table[i].address - loc - 1);
        			}
        			// Normal case, encode address
        			else
        			{
        				hex_string = int_to_hex(sym_table[i].address);
        			}
        			encoding += hex_string.substr(hex_string.length() - 6, hex_string.length()) + mot[sub_operation] + " "; 
        			break;
        		}
        	}
        	if(sub_operation.length() == instr.length())
        	{
        		cout << "ERROR: Operand expected at line " << line << endl;
        	}
        	else if(!defined)
        	{
        		cout << "ERROR: Unknown Symbol as operand at line " << line << endl;
        	}
        }
        encoding += instr + "\n";
    }
    return(encoding);
}

int synthesize(string file, string out_file)
{
    // Temp str variable for get line
    string line;

    // Program Counter
    int loc = 0;
    int line_count = 1;

    // Reading the input file
    ifstream MyFile(file);

    // Defining output listing file
    ofstream outfile(out_file);

    // Read individual lines
    while (getline (MyFile, line)) 
    {
        // Pre-process the line, trim extra spaces
        string instr;
        instr = line.substr(0, line.find(";", 0));
        instr = trim(instr);

        // Skip empty lines
        if(instr == "")
        {
            line_count++;
            // cout << "WARNING: EMPTY LINE AT LINE " << line_count << endl;
            continue;
        }

        // Write Encoded instruction into listing file
        outfile << inst_to_code(instr, &loc, line_count++);
        loc++;
    } 
    outfile.close();
}

int main(int argc, char* argv[]) 
{
	// Initalize Machine operation table
	mot_init();

	// Argument for input file
	string in_file = argv[1];

	// Argument for output file
	string out_file = in_file.substr(0, in_file.find(".", 0)) + ".L";

    // Pass-1 of assembly, analysis phase
    analyse(in_file);

    // Pass-2 of assembly, synthesis phase
    synthesize(in_file, out_file);
}