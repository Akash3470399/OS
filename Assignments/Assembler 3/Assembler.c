#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "dataStructure.h"
#include "globel_arrs.h"
#include "utilityFunc.h"
#include "ic_code.h"

#define VAL_CONC_LENGTH 300

// UTILITY FUNCTIONS
int pass1(int argc, char *argv[]);
void createds(char *[], int);

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


//char *numbers[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", NULL};
//char *punctuations[] = {"!", "#", "$", "%", "&", "(", ")", "*", "+", ",", "-", ".", "/", ":", ";", "<", "=", ">", "?", "@", "[", "]", "^", "_", "{", "|", "~", "}", NULL};

int main(int argc, char *argv[])
{
	pass1(argc, argv);
	printSymTab();
	printLitTab();
	printSymTabError();
	return 0;
}

int pass1(int argc, char *argv[])
{
	FILE *fp = NULL, *ic_v_1_fp=NULL, *ic_v_2_fp = NULL;
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
	fgets(line, 80, fp);
	tokenCount = tokenizer(line, tokens);
	if (strcmp(tokens[0], "START") == 0)
	{
		if (tokenCount == 2)
		{
			tokens[1][strcspn(tokens[1], "\n\r")] = 0;
			if (isValidAddress(tokens[1]))
				LC = atoi(tokens[1]);
			else
			{
				printf("Please enter a valid memory address.\n");
				return 0;
			}
		}
	}
	else
	{
		printf("First statement should be START.");
		return 0;
	}

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
		case 1:
			if (valOneToken(tokens, valConc) == false)
			{
				printf("\n%s:%d %s\n\t>'%s'\n", argv[1], lineNo, valConc[0], line);
				errors++;
			}
			else
			{
				if (strcmp(tokens[0], "LTORG") == 0 || strcmp(tokens[0], "END") == 0)
				{
					giveAddToLits();
				}
			}

			clearStrArr(valConc);
			break;
		case 2:
			if (valTwoTokens(tokens, valConc) == false)
			{
				tokenCount = 0; // reusing tokenCount as temp variable
				while (valConc[tokenCount] != NULL)
				{
					printf("\n%s:%d %s\n\t>'%s'\n", argv[1], lineNo, valConc[tokenCount], line);
					tokenCount++;
					errors++;
				}
			}
			else
			{
				createds(tokens, 1);
				if (strcmp(tokens[1], "LTORG") == 0 || strcmp(tokens[1], "END") == 0)
					giveAddToLits();
			}
			clearStrArr(valConc);

			break;
		case 3:

			if (valThreeTokens(tokens, valConc) == false)
			{
				tokenCount = 0; // reusing tokenCount as temp variable
				while (valConc[tokenCount] != NULL)
				{
					printf("\n%s:%d %s\n\t>'%s'\n", argv[1], lineNo, valConc[tokenCount], line);
					tokenCount++;
					errors++;
				}
			}
			else
			{
				createds(tokens, 2);

			}
			clearStrArr(valConc);
			break;
		case 4:
			if (valFourTokens(tokens, valConc) == false)
			{
				tokenCount = 0; // reusing tokenCount as temp variable
				while (valConc[tokenCount] != NULL)
				{
					printf("\n%s:%d %s\n\t>'%s'\n", argv[1], lineNo, valConc[tokenCount], line);
					tokenCount++;
					errors++;
				}
			}
			else
			{
				createds(tokens, 3);
			}

			clearStrArr(valConc);
			break;
		default:
			printf("%s ", tokens[0]);
			printf("Invalid statement ");
			errors++;
		}

		lineNo++;
		LC++;
	}

	if (errors == 0)
	{
		rewind(fp);
		ic_v_1_fp = fopen("IC_Code_var1.txt", "w+"); 
		ic_v_2_fp = fopen("IC_Code_var2.txt", "w+"); 
		fgets(line, 80, fp);
		line[strcspn(line, "\n\r")] = 0;
		tokenCount = tokenizer(line, tokens);
		(tokenCount == 2) ? fprintf(ic_v_1_fp, "<AD ,1> <C, %s>\n", tokens[1]) : fprintf(ic_v_1_fp, "<AD ,1>\n");
		(tokenCount == 2) ? fprintf(ic_v_2_fp, "<AD ,1> <C, %s>\n", tokens[1]) : fprintf(ic_v_2_fp, "<AD ,1>\n");
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
			
			case 1:
				line = createIcCodeVar1(tokens[0], NULL, NULL);
				break;

			case 2:
				if(isValidLabel(tokens[0]))
					line = createIcCodeVar1(tokens[1], NULL, NULL);
				else
					line = createIcCodeVar1(tokens[0], tokens[1], NULL);
				break;
			
			case 3:
				if (isStrInArr(tokens[1], ds) != -1)
					line = createIcCodeVar1(tokens[1], tokens[0], NULL);
				else if (isValidLabel(tokens[0]) )
					line = createIcCodeVar1(tokens[1], tokens[2], NULL);
				else
					line = createIcCodeVar1(tokens[0], tokens[1], tokens[2]);
				break;
			case 4:
				line = createIcCodeVar1(tokens[1], tokens[2], tokens[3]);
			break;
			default:
				break;
			}
			fprintf(ic_v_1_fp, "%s\n", line);

			switch (tokenCount)
			{
			
			case 1:
				line = createIcCodeVar2(tokens[0], NULL, NULL);
				break;

			case 2:
				if(isValidLabel(tokens[0]))
					line = createIcCodeVar2(tokens[1], NULL, NULL);
				else
					line = createIcCodeVar2(tokens[0], tokens[1], NULL);
				break;
			
			case 3:
				if (isStrInArr(tokens[1], ds) != -1)
					line = createIcCodeVar2(tokens[1], tokens[0], NULL);
				else if (isValidLabel(tokens[0]) )
					line = createIcCodeVar2(tokens[1], tokens[2], NULL);
				else
					line = createIcCodeVar2(tokens[0], tokens[1], tokens[2]);
				break;
			case 4:
				line = createIcCodeVar2(tokens[1], tokens[2], tokens[3]);
			break;
			default:
				break;
			}

			fprintf(ic_v_2_fp, "%s\n", line);
		}
	
	}
	fclose(fp);
	fclose(ic_v_1_fp);
	fclose(ic_v_2_fp);
	return 0;
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

	if (isStrInArr(tokens[0], allowedMnem) != -1)
	{
		valTwoOperandMnem(tokens, valConc);
	}
	else
	{
		// validation for label
		valLabel(tokens, valConc);

		// <label> DS/DC <constant>
		if (strcmp(tokens[1], "DS") == 0 || strcmp(tokens[1], "DC") == 0)
		{
			if (strstr(tokens[2], "\""))
				strcpy(tokens[2], removeChar(tokens[2], "\""));
			else if (strstr(tokens[2], "'"))
				strcpy(tokens[2], removeChar(tokens[2], "'"));

			if (!isValidAddress(tokens[2]))
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
	if (isStrInArr(tokens[0], arr) != -1)
		valOneOperandMnem(tokens, valConc);

	else
	{
		valLabel(tokens, valConc);

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
	tokens[1] = (char *)malloc(sizeof(char) * 5);
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
	if (isStrInArr(tokens[0], tempArr) == -1)
		mindex += 1;

	// checking for mnemonic validation
	if (isStrInArr(tokens[mindex], tempArr) == -1)
	{
		strcpy(valConc[temp], "error: Invalid Mnemonic instruction, '");
		strcat(valConc[temp], tokens[mindex]);
		strcat(valConc[temp], "'\nnote: check if you have entered correct opcode.");
		valConc[++temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
	}

	// checking for operand 1
	if ((strcmp(tokens[mindex], "BC") == 0) && ((isStrInArr(tokens[mindex + 1], cc) == -1) || !(isValidLabel(tokens[mindex + 2]))))
	{
		strcpy(valConc[temp], "error: Invalid operands of BC statements, ");
		strcat(valConc[temp], "\nnote: check you have enter correct condition code & memory label.");
		valConc[++temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
	}
	else if (isStrInArr(tokens[mindex + 1], registers) == -1)
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
		if (isStrInArr(tokens[mindex + 2], arr[i]) != -1)
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
		if (isStrInArr(tokens[0], arrs[i]) != -1)
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
	if (isStrInArr(tokens[0], arr) == -1)
		mindex += 1;
	// validity of mnemonics
	if (isStrInArr(tokens[mindex], arr) == -1)
	{
		strcpy(valConc[temp], "error: Invalid Mnemonic instruction, '");
		strcat(valConc[temp], tokens[mindex]);
		strcat(valConc[temp], "'\nnote: check if you have entered correct opcode Or check label field.");
		valConc[++temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
	}

	// validity of oprand 1
	tokens[mindex + 1][strcspn(tokens[mindex + 1], "\n")] = 0;
	char **arr1[] = {mnem, ad, ds, registers, cc};

	if (strcmp(tokens[mindex], "START") == 0)
	{
		if (!isValidAddress(tokens[mindex + 1]))
		{
			strcpy(tempStr, "\nnote: check address field in START.");
			flag = 1;
		}
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			if (isStrInArr(tokens[mindex + 1], arr1[i]) != -1)
			{
				flag = 1;
				strcat(tempStr, "\b'\nnote: Check operand 1, it can't be reserve word.");
				break;
			}
		}
	}

	if (flag)
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
	if (tokens[1] == NULL)
		mindex = 0;
	if (isStrInArr(tokens[mindex], arr) == -1)
	{
		strcpy(valConc[temp], "error: Invalid Mnemonic Instruction, '");
		strcat(valConc[temp], tokens[mindex]);
		strcat(valConc[temp], "'\nnote: Check operation mnemonic once.");
		valConc[++temp] = (char *)malloc(sizeof(char) * VAL_CONC_LENGTH);
	}
}


// function to create appropicate data structuer
void createds(char *tokens[], int last)
{
	if (isValidLabel(tokens[0]))
	{
		temp_ptr = getSymbolAdd(tokens[0]);
		if (temp_ptr == NULL)
			addToSymTab(tokens[0], LC, 1, 0);
		else if (temp_ptr->used == 1)
			temp_ptr->address = LC;
		else
			temp_ptr->defined += 1;
	}

	if (isValidLabel(tokens[last]))
	{

		if (isValidLiteral(tokens[last]))
		{
			if (!isPresentInPool(tokens[last]))
				addToLitTab(tokens[last]);
		}
		else if (last == 2 && isStrInArr(tokens[1], ds) != -1)
		{
			temp_ptr = getSymbolAdd(tokens[0]);
			temp_ptr->value = atoi(tokens[2]);
			temp_ptr->defined = 1;
		}
		else
		{
			temp_ptr = getSymbolAdd(tokens[last]);
			if (temp_ptr == NULL)
				addToSymTab(tokens[last], -1, 0, 1);
			else
				temp_ptr->used = 1;
		}
	}
}
