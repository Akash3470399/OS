#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define VAL_CONC_LENGTH 300

// UTILITY FUNCTIONS
int tokenizer(char *, char *[]);
char *removeChar(char *, char *);
bool isStrInArr(char *, char *[]);
void clearStrArr(char *[]);
bool isNumber(char *);
bool isValidAddress(char *);
bool isValidLabel(char *);

// VALIDITY FUNCTION
bool valFourTokens(char *[], char *[]);
bool valThreeTokens(char *[], char *[]);
bool valTwoTokens(char *[], char *[]);
bool valOneToken(char *[], char *[]);
void valTwoOperandMnem(char *[], char *[]);
void valLabel(char *[], char *[]);
void valOneOperandMnem(char *[], char *[]);
void valNoOperandMnem(char *[], char *[]);

// GLOBAL VARAIBLES
int temp = 0, errors = 0;

// GLOBAL KAYWORDS ARRAYS
char *mnem[] = {"STOP", "ADD", "SUB", "MULT", "MOVER", "MOVEM", "COMP", "BC", "DIV", "READ", "PRINT", NULL};
char *registers[] = {"AREG", "BREG", "CREG", "DREG", NULL};
char *cc[] = {"LT", "LE", "EQ", "GT", "GE", "ANY", NULL};
char *ad[] = {"START", "END", "ORIGIN", "EQU", "LTORG", NULL};
char *ds[] = {"DS", "DC", NULL};
//char *numbers[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", NULL};
//char *punctuations[] = {"!", "#", "$", "%", "&", "(", ")", "*", "+", ",", "-", ".", "/", ":", ";", "<", "=", ">", "?", "@", "[", "]", "^", "_", "{", "|", "~", "}", NULL};

int main(int argc, char *argv[])
{
	FILE *fp = NULL;
	char *line, *tokens[5], *valConc[10];
	int tokenCount, lineNo = 1;

	if (argc != 2)
	{
		printf("Please enter source program filename.");
		return 0;
	}

	fp = fopen(argv[1], "r+");

	if (fp == NULL)
	{
		printf("Failed to open '%s' file.", argv[1]);
		return 0;
	}

	line = (char *)malloc(sizeof(char) * 80);
//		printf("\033[0;36m");			

	while (fgets(line, 80, fp) != NULL)
	{
		line[strcspn(line, "\n\r")] = 0;

		if (strstr(line, ",") != NULL)
		{
			line = removeChar(line, ",");
			tokenCount = tokenizer(line, tokens);
		}
		tokenCount = tokenizer(line, tokens);

		switch (tokenCount)
		{
		case 1: if(valOneToken(tokens, valConc) == false)
					{
						printf("\n%s:%d %s\n\t>'%s'\n", argv[1], lineNo, valConc[0], line);
						errors++;
					}	
					
					break;
		case 2:
			if (valTwoTokens(tokens, valConc) == false)
			{
				tokenCount = 0; // reusing tokenCount as temp variable
				while (valConc[tokenCount] != NULL)
				{
					printf("\n%s:%d %s\n\t>'%s'\n", argv[1], lineNo, valConc[tokenCount], line);
					tokenCount++;errors++;
				}
			}
			else
			{
				if(isValidLabel(tokens[0]))
				{
						 
				}
			} 
			break;
		case 3:

			if (valThreeTokens(tokens, valConc) == false)
			{
				tokenCount = 0; // reusing tokenCount as temp variable
				while (valConc[tokenCount] != NULL)
				{
					printf("\n%s:%d %s\n\t>'%s'\n", argv[1], lineNo, valConc[tokenCount], line);
					tokenCount++;errors++;
				}
			}
			break;
		case 4:
			if (valFourTokens(tokens, valConc) == false)
			{
				tokenCount = 0; // reusing tokenCount as temp variable
				while (valConc[tokenCount] != NULL)
				{
					printf("\n%s:%d %s\n\t>'%s'\n", argv[1], lineNo, valConc[tokenCount], line);
					tokenCount++;errors++;
				}
			}

			clearStrArr(valConc);
			break;
		default:
			printf("%s ", tokens[0]);
			printf("Invalid statement ");
			errors++;
		}

		lineNo++;
	}

	if(errors == 0)
	{
		printf("\033[0;32m");
		printf("\n\t*****");
		printf("\033[0;32m");
		printf(" CODE VALIDATED ");
		printf("\033[0;32m");
		printf("*****\n\n\n");

	}
}

// function to tokenize a source program line
// returns tokens count in a line & tokens in tokens array
int tokenizer(char *line, char *tokens[5])
{
	char str[80], *token;
	int tokenCount = 0;
	strcpy(str, line);
	token = strtok(str, " ");

	while (token != NULL)
	{
		tokens[tokenCount] = (char *)malloc(sizeof(char) * 80);
		strcpy(tokens[tokenCount], token);
		token = strtok(NULL, " ");
		tokenCount++;
	}
	return tokenCount;
}

// function to remove all ouccurance of char in a string
// parameters :
//     line : String
//     ch   : single char string to remove
char *removeChar(char *line, char *ch)
{
	char *token, str[80];
	strcpy(str, line);
	token = strtok(str, ch);
	strcpy(line, "");

	while (token != NULL)
	{
		line = strcat(line, token);
		token = strtok(NULL, ch);
	}
	return line;
}

// function to check if a string is present in array of strings
// parameters:
//     str: string to search for
//     arr: array of strings in which to search for str string
// returns 1 if found else 0
bool isStrInArr(char *str, char *arr[])
{
	for (int i = 0; arr[i] != NULL; i++)
	{
		if (strcmp(str, arr[i]) == 0)
			return true;
	}

	return false;
}

// function to free a block of memory allocated for a string array
// ****NOTE: Array's last entry should be NULL, (to indicate end of array)
// parameter:
//    *ptr : pointer to array of string which is to be free

void clearStrArr(char *ptr[])
{
	int i = 0;
	while (ptr[i] != NULL)
	{
		free(ptr[i]);
		i++;
	}
}

// function to create a  string is a number
bool isNumber(char *str)
{
	int i;
	for(i = 0; str[i] != '\0'; i++)
	{
		if(!isdigit(str[i]))
			return false;
	}
	return true;
}

// function to check if address is valid or not
bool isValidAddress(char *add)
{
	int i = 0;
	while(add[i] != '\0')
	{	
		if(!isdigit(add[i]))
			return false;
		i++;
	}		

	if(strlen(add) > 3)
		return false;

	return true;
}
// function to check if a str is valid label
bool isValidLabel(char *str)
{
	char **arrs[5] = {mnem, registers, ad, ds, cc};
	// checking for label validity
	for (int i = 0; i < 5; i++)
	{
		if (isStrInArr(str, arrs[i]) || !isalpha(str[0]))
			return false;
	}

	return true;
}

// function to check for validity of line with 4 tokens
// parameters:
//     tokens : array of strings(tokens)
//     valConc : char pointer to return validation conclusion
// returns false if validation fails else return true
//
bool valFourTokens(char *tokens[], char *valConc[10])
{
	temp = 0;
	valLabel(tokens, valConc); // to validate label

	valTwoOperandMnem(tokens, valConc); // to validate mnemonics with 2 operands

	valConc[temp] = NULL;
	if (temp > 0)
		return false;
	else
		return true;
}

// function for validation line code with 3 tokens
// parameters:
//     tokens : array of tokens
//     valConc : validation conclusion array of string
bool valThreeTokens(char *tokens[], char *valConc[])
{
	int flag = 0;
	char *allowedMnem[] = {"ADD", "SUB", "MULT", "MOVER", "MOVEM", "COMP", "BC", "DIV", NULL}, tempStr[300];
	temp = 0; // global variable
	valConc[temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);

	if (isStrInArr(tokens[0], allowedMnem))
	{
		valTwoOperandMnem(tokens, valConc);
	}
	else 
	{
		// validation for label
		valLabel(tokens, valConc);

		// <label> DS/DC <constant>
		if(strcmp(tokens[1], "DS" ) == 0 || strcmp(tokens[1], "DC") == 0)
		{
			if(strstr(tokens[2], "\""))
				strcpy(tokens[2],removeChar(tokens[2], "\""));
			else if(strstr(tokens[2], "'"))
				strcpy(tokens[2], removeChar(tokens[2], "'"));

			if(!isValidAddress(tokens[2]))
			{
				strcpy(valConc[temp], "error: Invalid memory size value, '");
				strcat(valConc[temp], tokens[2]);
				strcat(valConc[temp], "'\nnote: check if you have entered correct memory blocks value.");
				valConc[++temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
			}	
		}
		else
			valOneOperandMnem(tokens, valConc); // for  <label> <mnemonic instruction> <memory operand>
	}
	valConc[temp] = NULL;
	if (temp > 0)
		return false;
	else
		return true;
}

// function for validation line code with 2 tokens
// parameters:
//     tokens : array of tokens
//     valConc : validation conclusion array of string
bool valTwoTokens(char *tokens[], char *valConc[])
{
	char *arr[] = {"START", "READ", "PRINT", NULL}, tempStr[300];
	temp = 0;
	valConc[temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
	if(isStrInArr(tokens[0], arr))
		valOneOperandMnem(tokens, valConc);
	
	else
	{ 	valLabel(tokens, valConc);

		valNoOperandMnem(tokens, valConc);
	}

	valConc[temp] = NULL;
	if (temp > 0)
		return false;
	else
		return true;
}

// function for validation line code with 1 token
// parameters:
//     tokens : array of tokens
//     valConc : validation conclusion array of string
bool valOneToken(char *tokens[], char *valConc[])
{
	temp = 0;
	valConc[temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
 	tokens[1] = (char *)malloc(sizeof(char)*5);
	tokens[1] = NULL;
	valNoOperandMnem(tokens, valConc);
	valConc[temp] = NULL;
	if (temp > 0)
		return false;
	else
		return true;

}
// function to validate operation with 2 operands
void valTwoOperandMnem(char *tokens[], char *valConc[])
{
	char *tempArr[] = {"ADD", "SUB", "MULT", "MOVER", "MOVEM", "COMP", "BC", "DIV", NULL};
	int mindex = 0;
	if (!isStrInArr(tokens[0], tempArr))
		mindex += 1;

	// checking for mnemonic validation
	if (!isStrInArr(tokens[mindex], tempArr))
	{
		strcpy(valConc[temp], "error: Invalid Mnemonic instruction, '");
		strcat(valConc[temp], tokens[mindex]);
		strcat(valConc[temp], "'\nnote: check if you have entered correct opcode.");
		valConc[++temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
	}

	
	// checking for operand 1
	if ((strcmp(tokens[mindex], "BC") == 0) && (!(isStrInArr(tokens[mindex + 1], cc)) || !(isValidLabel(tokens[mindex+2]))))
	{
		strcpy(valConc[temp], "error: Invalid operands of BC statements, ");
		strcat(valConc[temp], "\nnote: check you have enter correct condition code & memory label.");
		valConc[++temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
	}
	else if (!isStrInArr(tokens[mindex + 1], registers) && strcmp(tokens[mindex+1], "BC") == 0)
	{
		strcpy(valConc[temp], "error: Invalid register operand, '");
		strcat(valConc[temp], tokens[mindex + 1]);
		strcat(valConc[temp], "'\nnote: Operand 1 can only be a register or condition code in case of BC.");
		valConc[++temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
	}

	// checking for operand 2
	tokens[mindex + 2][strcspn(tokens[mindex + 2], "\n")] = 0;
	char **arr[] = {mnem, ad, ds, registers};

	for (int i = 0; i < 4; i++)
	{

		if (isStrInArr(tokens[mindex + 2], arr[i]))
		{
			strcpy(valConc[temp], "error: Invalid symbolic name, '");
			strcat(valConc[temp], tokens[mindex + 2]);
			strcat(valConc[temp], "'\nnote: Check operand 2");
			valConc[++temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
			break;
		}
	}
}

void valLabel(char *tokens[], char *valConc[])
{
	char **arrs[5] = {mnem, registers, ad, ds, cc};
	// checking for label validity
	valConc[temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
	for (int i = 0; i < 5; i++)
	{
		if (isStrInArr(tokens[0], arrs[i]) || !isalpha(tokens[0][0]))
		{
			strcpy(valConc[temp], "error: Invalid symbolic name, '");
			strcat(valConc[temp], tokens[0]);
			strcat(valConc[temp], "'\nnote: Reserved word, punctuations or numbers can't be use as a label");
			valConc[++temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
			break;
		}
	}
}
// functin to validate operations with one operands
void valOneOperandMnem(char *tokens[], char *valConc[])
{
	char *arr[] = {"START", "READ", "PRINT", NULL}, tempStr[300];
	int mindex = 0, flag = 0;
	if (!isStrInArr(tokens[0], arr))
		mindex += 1;
	// validity of mnemonics
	if (!isStrInArr(tokens[mindex], arr))
	{
		strcpy(valConc[temp], "error: Invalid Mnemonic instruction, '");
		strcat(valConc[temp], tokens[mindex]);
		strcat(valConc[temp], "'\nnote: check if you have entered correct opcode Or check label field.");
		valConc[++temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
	}

	// validity of oprand 1
	tokens[mindex + 1][strcspn(tokens[mindex + 1], "\n")] = 0;
	char **arr1[] = {mnem, ad, ds, registers, cc};
	
	if(strcmp(tokens[mindex], "START") == 0)
	{
		if(!isValidAddress(tokens[mindex+1]))
		{
			strcpy(tempStr, "\nnote: check address field in START.");
			flag = 1;
		}
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
	
			if (isStrInArr(tokens[mindex + 1], arr1[i]))
			{
				flag = 1;
				strcat(tempStr, "\b'\nnote: Check operand 1, it can't be reserve word.");
				break;
			}
		}	
	}

	if(flag)
	{ 
		strcpy(valConc[temp], "error: Invalid symbolic name, '");
		strcat(valConc[temp], tokens[mindex + 1]);
		strcat(valConc[temp], tempStr);
		valConc[++temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);

	}
}

// function to validate operations with no operands
void valNoOperandMnem(char *tokens[], char *valConc[])
{
	char *arr[] = {"STOP", "END", "LTORG", NULL};	
	int mindex = 1;
	if(tokens[1] == NULL)
		mindex = 0;
	if(!isStrInArr(tokens[mindex], arr))
	{
		strcpy(valConc[temp], "error: Invalid Mnemonic Instruction, '"); 
		strcat(valConc[temp], tokens[mindex]);
		strcat(valConc[temp], "'\nnote: Check operation mnemonic once.");
		valConc[++temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
	}
}
