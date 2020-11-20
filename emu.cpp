/*****************************************************************************
TITLE: Claims																																
AUTHOR: P. V. Sriram
ROLL NO.: 1801CS37
Declaration of Authorship
This txt file, claims.txt, is part of the assignment of CS321 at the 
department of Computer Science and Engg, IIT Patna . 
*****************************************************************************/

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

int32_t A, B, PC, SP;
int32_t memory[10000];

tuple<int32_t, int32_t> inst_to_operand_opcode(int32_t instr)
{
	int32_t opcode, operand;
	opcode = instr & 0xff;
	operand = instr & 0xffffff00;
	operand >>= 8;
	return(make_tuple(operand, opcode));
}

// MOT table
map<int, string> mot;
void mot_init()
{
	mot[0] = string("ldc");
	mot[1] = string("adc");
	mot[2] = string("ldl");
	mot[3] = string("stl");
	mot[4] = string("ldnl");
	mot[5] = string("stnl");
	mot[6] = string("add");
	mot[7] = string("sub");
	mot[8] = string("shl");
	mot[9] = string("shr");
	mot[10] = string("adj");
	mot[11] = string("a2sp");
	mot[12] = string("sp2a");
	mot[13] = string("call");
	mot[14] = string("return");
	mot[15] = string("brz");
	mot[16] = string("brlz");
	mot[17] = string("br");
	mot[18] = string("HALT");
	mot[19] = string("data");
	mot[20] = string("SET");
}

int trace(int PC, int poc)
{
	int count = 0;
	// while(count <= 100)
	while(true)
	{
		int old_pc = PC;
		int32_t operand, operation;
		int32_t instr = memory[PC];
		tie(operand, operation) = inst_to_operand_opcode(instr);

		cout << "PC: " << int_to_hex(old_pc) << " SP: " << int_to_hex(SP) << " A: " 
		<< int_to_hex(A) << " B: " << int_to_hex(B) << " " << mot[operation] 
		<< " " <<operand << endl << endl;

		switch(operation) {
            case 0:
                B = A;
                A = operand;
                break;
            case 1:
                A = A + operand;
                break;
            case 2:
                B = A;
                A = memory[SP+operand];
                break;
            case 3:
                memory[SP+operand] = A;
                A = B;
                break;
            case 4:
                A = memory[A+operand];
                break;
            case 5:
                memory[A+operand] = B;
                break;
            case 6:
                A = B + A;
                break;
            case 7:
                A = B - A;
                break;
            case 8:
                A = B << A;
                break;
            case 9:
                A = B >> A;
                break;
            case 10:
                SP = SP + operand;
                break;
            case 11:
                // memory[SP] = A;
                SP = A;
                A = B;
                break;
            case 12:
                B = A;
                A = SP;//memory[SP];
                break;
            case 13:
                B = A;
                A = PC;
                PC = PC + operand;
                break;
            case 14:
                PC = A;
                A = B;
                // cout << PC << "\t" << A << "\t" << B << "\n";
                break;
            case 15:
                if(A == 0) PC = PC + operand;
                break;
            case 16:
                if(A < 0) PC = PC + operand;
                break;
            case 17:
                PC = PC + operand;
                break;
            case 18:
            	cout << count << " number of instructions executed" << endl << endl;
                // cout << "\n" << dec << ++noIns << " instructions executed\n\n";
                return 0;
            // default:
                // cout << "\n" << dec << ++noIns << " instructions executed\n\n";
                // cout << "Invalid Opcode Found. Stopping Emulation\n";
                // return 1;
        }
        PC++;

		// if(operation == 0)
		// {
		// 	B = A;
		// 	A = operand;
		// 	PC++;
		// }
		// else if(operation == 1)
		// {
		// 	A += operand;
		// 	PC++;
		// }
		// else if(operation == 2)
		// {
		// 	B = A;
		// 	A = memory[SP + operand];
		// 	PC++;
		// }
		// else if(operation == 3)
		// {
		// 	memory[SP + operand] = A;
		// 	A = B;
		// 	PC++;
		// }
		// else if(operation == 4)
		// {
		// 	A = memory[A + operand];
		// 	PC++;
		// }
		// else if(operation == 5)
		// {
		// 	memory[A + operand] = B;
		// 	PC++;
		// }
		// else if(operation == 6)
		// {
		// 	A += B;
		// 	PC++;
		// }
		// else if(operation == 7)
		// {
		// 	A = B - A;
		// 	PC++;
		// }
		// else if(operation == 8)
		// {
		// 	A = B << A;
		// 	PC++;
		// }
		// else if(operation == 9)
		// {
		// 	A = B >> A;
		// 	PC++;
		// }
		// else if(operation == 10)
		// {
		// 	SP += operand;
		// 	PC++;
		// }
		// else if(operation == 11)
		// {
		// 	SP = A;
		// 	A = B;
		// 	PC++;
		// }
		// else if(operation == 12)
		// {
		// 	B = A;
		// 	A = SP;
		// 	PC++;
		// }
		// else if(operation == 13)
		// {
		// 	B = A;
		// 	A = PC;
		// 	PC = PC + operand + 1;
		// }
		// else if(operation == 14)
		// {
		// 	PC = A;
		// 	A = B;
		// 	PC++;
		// }
		// else if(operation == 15)
		// {
		// 	if(A == 0)
		// 		PC += operand + 1;
		// 	else
		// 		PC++;
		// }
		// else if(operation == 16)
		// {
		// 	if(A < 0)
		// 		PC += operand + 1;
		// 	else
		// 		PC++;
		// }
		// else if(operation == 17)
		// {
		// 	PC += operand + 1;
		// }
		// else if(operation == 18)
		// {
		// 	break;
		// 	// return(0);
		// }
		// else if(operation == 19)
		// {
		// 	PC++;
		// }
		// else if(operation == 20)
		// {
		// 	PC++;
		// }


		if(old_pc == PC)
		{
			cout << "Infinite loop detected" << endl;
			break;
		}
		count++;
	}
	cout << count << " number of instructions executed" << endl << endl;
}

void inst_to_table(int32_t instr_code, int poc)
{
	// int operand, operation;
	// tie(operand, operation) = inst_to_operand_opcode(instr_code);
	memory[poc] = instr_code;
}

int main(int argc, char* argv[])
{
	mot_init();

	// Argument for input file
	string in_file = argv[1];

	// Input object file
	ifstream infile; 
	infile.open(in_file, ios::out | ios::binary);

	// 
	int32_t instr_code;
	int8_t bytes[4];
	int poc = 0;

	while (infile.read((char*) &instr_code, sizeof(instr_code)))
	{
		int32_t operand, operation;
		tie(operand, operation) = inst_to_operand_opcode(instr_code);
		if(operation == 19 || operation == 20)
		{
			memory[poc++] = operand;
		}
		else if(operation >= 0 && operation < 20)
		{
			memory[poc++] = instr_code;
		}
		else
		{
			cout << "Invalid instruction\n";
			return(0);
		}
		// inst_to_table(instr_code, poc++);
	}

	A = 0;
	B = 0;
	PC = 0;
	SP = sizeof(memory)/sizeof(memory[0])-1;
	// SP = 5000;

	// cout << poc << endl;
	trace(0, poc);
	for (int i = 0; i < poc; ++i)
	{
		cout << int_to_hex(i) << " " << memory[i] << endl;
	}
	// execute();
	infile.close();
}