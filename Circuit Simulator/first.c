#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_GATES 1024
#define MAX_VARS  512

////////////////////////////////////////////////////////////////////////////////
// Type Definition

enum ID_TYPE {
	INPUTVAR,OUTPUTVAR,NOT,AND,OR,NAND,NOR,XOR,UNKNOWN
};

struct DIRECTIVE {
	enum ID_TYPE type;
	int input[2];
	int output;
};

struct VARIABLE {
	char name[21];
	int value;
};

////////////////////////////////////////////////////////////////////////////////
// Global Variables

struct DIRECTIVE arrGates[MAX_GATES]; //the array of logic gate directives
struct VARIABLE arrVars[MAX_VARS] = { {"0",0},{"1",1},}; //the array of variables
int numGates = 0; //the number of building blocks
int numInputs = 0; //the number of input variables
int numOutputs = 0; //the number of output variables 
int numTempVars = 0; //the number of temporary variables

////////////////////////////////////////////////////////////////////////////////
// Function Declarations

int id_to_index(char * szIdentifier);
enum ID_TYPE id_to_type(char * szIdentifier);
void read_gates(FILE * stream);
void calculate_output();
void calculate_gate(int i);

////////////////////////////////////////////////////////////////////////////////
// Function Implementations

int main(int argc,char *argv[])
{
	FILE * stream;
	int i;
	char szIdentifier[21];

	if(argc < 2)
		return 1;
	
	stream = fopen(argv[1],"r");
	if(stream == NULL)
		return 1;
	
	// read the INPUTVAR directive
	if(fscanf(stream,"%s%d",szIdentifier,&numInputs)!=2 || id_to_type(szIdentifier)!=INPUTVAR || numInputs<1)
	{
		printf("INPUTVAR Error!\n");
		fclose(stream);
		return 1;
	}
	for(i=0;i<numInputs;i++)
	{
		fscanf(stream,"%20s",szIdentifier);
		strcpy(arrVars[2+i].name,szIdentifier);
	}
	
	// read the OUTPUTVAR directive
	if(fscanf(stream,"%s%d",szIdentifier,&numOutputs)!=2 || id_to_type(szIdentifier)!=OUTPUTVAR || numOutputs<1)
	{
		printf("OUTPUTVAR Error!\n");
		fclose(stream);
		return 1;
	}
	for(i=0;i<numOutputs;i++)
	{
		fscanf(stream,"%20s",szIdentifier);
		strcpy(arrVars[2+numInputs+i].name,szIdentifier);
	}
	
	read_gates(stream);//Read the following building blocks: NOT,AND,OR,NAND,NOR,and XOR.
	
	fclose(stream);
	
	calculate_output();//Calculate the circuit and output the truthtable
	
	return 0;
}

int id_to_index(char * szIdentifier)
{
	int i = 0;
	for(;i<2+numInputs+numOutputs+numTempVars;i++)
		if(!strcmp(arrVars[i].name,szIdentifier))
			return i;
	numTempVars++;
	strcpy(arrVars[i].name,szIdentifier);
	return i;
}

enum ID_TYPE id_to_type(char * szIdentifier)
{
	if(!strcmp(szIdentifier,"INPUTVAR"))
		return INPUTVAR;
	else if(!strcmp(szIdentifier,"OUTPUTVAR"))
		return OUTPUTVAR;
	else if(!strcmp(szIdentifier,"NOT"))
		return NOT;
	else if(!strcmp(szIdentifier,"AND"))
		return AND;
	else if(!strcmp(szIdentifier,"OR"))
		return OR;
	else if(!strcmp(szIdentifier,"NAND"))
		return NAND;
	else if(!strcmp(szIdentifier,"NOR"))
		return NOR;
	else if(!strcmp(szIdentifier,"XOR"))
		return XOR;
	else
		return UNKNOWN;
}

void read_gates(FILE * stream)
{
	enum ID_TYPE type;
	char szIdentifier[21];
	
	while(fscanf(stream,"%20s",szIdentifier)==1)
	{
		if(numGates >= MAX_GATES)
		{
			printf("Too many building blocks!\n");
			break;
		}
		type = id_to_type(szIdentifier);
		switch(type)
		{
			case NOT:
				arrGates[numGates].type = type;
				fscanf(stream,"%20s",szIdentifier);
				arrGates[numGates].input[0] = id_to_index(szIdentifier);
				fscanf(stream,"%20s",szIdentifier);
				arrGates[numGates].output = id_to_index(szIdentifier);
				numGates++;
				break;
			case AND:
			case OR:
			case NAND:
			case NOR:
			case XOR:
				arrGates[numGates].type = type;
				fscanf(stream,"%20s",szIdentifier);
				arrGates[numGates].input[0] = id_to_index(szIdentifier);
				fscanf(stream,"%20s",szIdentifier);
				arrGates[numGates].input[1] = id_to_index(szIdentifier);
				fscanf(stream,"%20s",szIdentifier);
				arrGates[numGates].output = id_to_index(szIdentifier);
				numGates++;
				break;
			default:
				break;
		}
	}
}

void calculate_gate(int i)
{
	if(i>=numGates)
		return;
	switch(arrGates[i].type)
	{
		case NOT:
			arrVars[arrGates[i].output].value = !arrVars[arrGates[i].input[0]].value;
			break;
		case AND:
			arrVars[arrGates[i].output].value = arrVars[arrGates[i].input[0]].value & arrVars[arrGates[i].input[1]].value;
			break;
		case OR:
			arrVars[arrGates[i].output].value = arrVars[arrGates[i].input[0]].value | arrVars[arrGates[i].input[1]].value;
			break;
		case NAND:
			arrVars[arrGates[i].output].value = !(arrVars[arrGates[i].input[0]].value & arrVars[arrGates[i].input[1]].value);
			break;
		case NOR:
			arrVars[arrGates[i].output].value = !(arrVars[arrGates[i].input[0]].value | arrVars[arrGates[i].input[1]].value);
			break;
		case XOR:
			arrVars[arrGates[i].output].value = arrVars[arrGates[i].input[0]].value ^ arrVars[arrGates[i].input[1]].value;
			break;
		default:
			printf("Error: unknown directive!\n");
			break;
	}
}

void calculate_output()
{
	int i,j;
	for(j=0;j<(1<<numInputs);j++)
	{
		for(i=0;i<numInputs;i++) //Initialize the input variables.
			arrVars[numInputs+1-i].value = (j>>i)&0x1;
		for(i=2+numInputs;i<2+numInputs+numOutputs+numTempVars;i++)
			arrVars[i].value = 0;//Initialize the output variables and temporary variables.
		for(i=0;i<numGates;i++)
			calculate_gate(i);//Calculate the logical gate
		for(i=0;i<numInputs+numOutputs;i++)//Output the truthtable
		{
			printf("%d",arrVars[2+i].value);
			if(i<numInputs+numOutputs-1)
				printf(" ");
			else
				printf("\n");
		}
	}
}