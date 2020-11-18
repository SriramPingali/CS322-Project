#include <bits/stdc++.h>

using namespace std;

// Utility function to convert int to hex
string int_to_hex(int i)
{
  stringstream stream;
  stream << setfill ('0') << setw(8) 
         << hex << i;
  return stream.str();
}

void trace(uint32_t instr_code)
{
	string instr, operation;
	int A, B, pc, sp, operand;
	vector<int> memory[100];
	instr = int_to_hex(instr_code).substr(0, 6);
	operation = instr.substr(6, 8);
	if(instr.front() == 'f')
	{
		operand = stoul("ff" + instr, nullptr, 16);
	}
	else
	{
		operand = stoul("00" + instr, nullptr, 16);
	}
	cout << operand << endl;

	if(operation == "00")
	{
		B = A;
		A = operand;
	}
	else if(operation == "01")
	{
		A += operand;
	}
	else if(operation == "02")
	{
		B = A;
		A = memory[SP + operand];
	}
	else if(operation == "03")
	{
		memory[SP + operand] = A;
		A = B;
	}
	else if(operation == "04")
	{
		A = memory[A + operand];
	}
	else if(operation == "05")
	{
		memory[A + operand] = B;
	}
	else if(operation == "06")
	{
		A += B;
	}
	else if(operation == "07")
	{
		A = B - A;
	}
	else if(operation == "08")
	{
		A = B * 256;
	}
	else if(operation == "09")
	{
		A = B / 256;
	}
	else if(operation == "0a")
	{
		SP += operand;
	}
	else if(operation == "0b")
	{
		SP = A;
		A = B;
	}
	else if(operation == "0c")
	{
		B = A;
		A = SP;
	}
	else if(operation == "0d")
	{
		B = A;
		A = PC;
		PC = PC + operand;
	}
	// else if(operation == "0e")
	// {
	// 	cout << instr << endl;
	// }
	// else if(operation == "0f")
	// {
	// 	cout << instr << endl;
	// }
	// else if(operation == "10")
	// {
	// 	cout << instr << endl;
	// }
	// else if(operation == "11")
	// {
	// 	cout << instr << endl;
	// }
	// else if(operation == "12")
	// {
	// 	cout << instr << endl;
	// }
	// else if(operation == "13")
	// {
	// 	cout << instr << endl;
	// }
	// else if(operation == "14")
	// {
	// 	cout << instr << endl;
	// }
}

int main(int argc, char* argv[])
{
	// Argument for input file
	string in_file = argv[1];

	// Input object file
	ifstream infile; 
	infile.open(in_file, ios::in | ios::out | ios::binary);

	// 
	uint32_t instr_code;
	uint8_t bytes[4];

	while(!infile.eof())
	{
		infile.read((char*)bytes, 4);
		instr_code = bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
		trace(instr_code);
	}
	infile.close();
}