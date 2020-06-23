# Circuit-Simulator
Introduction
You have to write a circuit simulator. One of the inputs to your program will be a circuit description file that will describe a circuit using various directives. Your program will print the output of the circuit for all possible input values.
1 Circuit Description Directives
The input variables used in the circuit are provided using the INPUTVAR directive. The INPUTVAR directive is followed by the number of input variables and the names of the input variables. All the input variables will be named with capitalized identifiers. An identifier consists of at least one character (A-Z) followed by a series of zero or many characters (A-Z) or digits (0-9). For example, some identifiers are IN1, IN2, and IN3. An example specification of the inputs for a circuit with three input variables: IN1, IN2, IN3 is as follows:
INPUTVAR 3 IN1 IN2 IN3
The outputs produced by the circuit is specified using the OUTPUTVAR directive. The OUTPUTVAR directive is
followed by the number of outputs and the names of the outputs.
An example specification of the circuit with output OUT1 is as follows:
OUTPUTVAR 1 OUT1
The circuits used in this assignment will be built using the following building blocks: NOT, AND, OR, NAND,
NOR, and XOR.
The building blocks can produce temporary variables as outputs, and can use either the input variables or
temporary variables as input. Output variables will never be used as inputs in a building block.
All the temporary variables will also be named with lower case identifiers (i.e., temp1, temp2, temp3, ...). The specification of each building block is as follows:
• NOT: for example, NOT IN1 OUT1
• AND: for example,
     AND IN1 IN2 OUT1
• OR: for example,
     OR IN1 IN2 OUT1
• NAND: for example,
1
2
•
•
NAND IN1 IN2 OUT1 NOR: for example, NOR IN1 IN2 OUT1 XOR: for example, XOR IN1 IN2 OUT1
Describing Circuits using the Directives
It is possible to describe any combinational circuit using the above set of directives. For example, the circuit OUT1 = IN1.IN2 + IN1.IN3 can be described as follows:
INPUTVAR 3 IN1 IN2 IN3
OUTPUTVAR 1 OUT1
AND IN1 IN2 temp1
AND IN1 IN3 temp2
OR temp1 temp2 OUT1
Note that OUT1 is the output variable. IN1, IN2, and IN3 are input variables. temp1 and temp2 are temporary
variables.
A circuit description is a sequence of directives. You can assume that every temporary variable occurs as a output variable in the sequence before occurring as an input variable.
Note: A temporary variable can occur as an output variable in at most one directive.
3 Format of the Input Files
Your program will be given one file as input, containing the description of a circuit using the directives described above.
For example:
INPUTVAR 3 IN1 IN2 IN3
OUTPUTVAR 1 OUT1
AND IN1 IN2 temp1
AND IN1 IN3 temp2
OR temp1 temp2 OUT1
4 The problem
You have to write a program called first as described above. You are guaranteed that the circuit descriptions given as input to your program will be sorted. Let’s look at an example we have encountered before.
Example Execution
Supposeacircuitdescriptionfilenamedcircuit.txthasthedescriptionforthecircuitOUT1 = IN1.IN2 + IN1.IN3
2

INPUTVAR 3 IN1 IN2 IN3
OUTPUTVAR 1 OUT1
AND IN1 IN2 temp1
AND IN1 IN3 temp2
OR temp1 temp2 OUT1
Then, on executing the program with the above circuit description file, your program should produce the following output:
./first circuit.txt 0000
0010
0100
0110 1000 1011 1101 1111
The output of the first three columns are INPUTVAR IN1, IN2, and IN3 respectively. And the last column denotes the OUTPUTVAR OUT1.
Note: the values of the input and output variables should be space separated and be in the same order as the output variables in the INPUTVAR and OUTPUTVAR directive, e.g., if the circuit description file has the directive INPUTVAR 3 IN1 IN2 IN3, and OUTPUTVAR 3 OUT1 OUT2 OUT3, then the first values should be those of the input variables IN1, IN2, and IN3, and output variable OUT1, followed by that of OUT2, and then that of OUT3.
