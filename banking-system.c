#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define	LINECONSTR	148
#define NAME_LENGTH 20
#define SURNAME_LENGTH 30
#define ADDRESS_LENGTH 50
#define PESEL_LENGTH 11

struct database
{
	int number;
	char name[NAME_LENGTH];
	char surname[SURNAME_LENGTH];
	char address[ADDRESS_LENGTH];
	unsigned long long int pesel;
	double balance;
};

struct database account;
void displayMainMenu();
void inputStringToLowerCase();
void createNewAccount();
bool isCorrect();
void displayAllExistingAccounts();
void searchAccountByPhrase();
void whichOperation();
void displayAccount(char *accountNumber);
void transferMoneyBetweenAccounts();
void displaySearchChoice(int choice);
int findAccountNumber(char *accountNumber);
void makeMoneyTranfer(char *accountNumber, double amount, bool isWithdrawal);
void makeDeposit();
void makeWithdrawal();
void getAccountNumber();
void getAccountName();
void getAccountSurname();
void getAccountAddress();
void getAccountPesel();
void getAccountBalance();
int checkIfNameContainsDigits();
int checkIfSurnameContainsDigits();


char choice_buffer[NAME_LENGTH];

int main()
{
	while(true)
	{
		system("clear");
		displayMainMenu();
		inputStringToLowerCase();
		if(strcmp(choice_buffer, "0") == 0)
		{
			exit(1);
		}
		else if(strcmp(choice_buffer, "1") == 0)
		{
			displayAllExistingAccounts();
		}
		else if(strcmp(choice_buffer, "2") == 0)
		{
			searchAccountByPhrase();
		}
		else if(strcmp(choice_buffer, "3") == 0)
		{
			createNewAccount();
		}
		else if(strcmp(choice_buffer, "4") == 0)
		{
			whichOperation();
		}


	}
}

void displayMainMenu()
{
	printf("Milosz Kadziela [2017 - IFE] Banking System\n\n");
	printf("[1]\tList all existing accounts\n");
	printf("[2]\tSearch through existing accounts\n");
	printf("[3]\tAdd a new account\n");
	printf("[4]\tMake a money transfer\n");
	printf("[0]\tQuit the application\n\n");
	printf("Your input: ");
}

void inputStringToLowerCase()
{
	int iterator;
	
	scanf("%[^\n]%*c", choice_buffer);

	for(iterator = 0;iterator < strlen(choice_buffer); iterator++)
	{
		if(isupper(choice_buffer[iterator]))
		{
			choice_buffer[iterator] = tolower(choice_buffer[iterator]);
		}
	}
}

void getAccountNumber()
{
	bool isUnique;
	char temp[NAME_LENGTH];
	
	do
	{
		isUnique=true;
		do
		{
			printf("\n1. Enter account's number :\n");
			printf("Your input: ");
			scanf("%10s", temp);
			getchar();
		} while(sscanf(temp, "%d", &account.number)!=1);
		if(findAccountNumber(temp)!=-1)
		{
			printf("Account with given Account number already exists!\n");
			isUnique=false;
		}
	} while(!isUnique);
}

int checkIfNameContainsDigits()
{
	int x;
	for(x = 0; x < NAME_LENGTH; x++)
	{
		if(isdigit((account.name)[x]))
		{
			return 0;
		}
	}
	return 1;
}

int checkIfSurnameContainsDigits()
{
	int x;
	for(x = 0; x < NAME_LENGTH; x++)
	{
		if(isdigit((account.name)[x]))
		{
			return 0;
		}
	}
	return 1;
}
	
void getAccountName()
{
	int correctness;
	
	do
	{
		correctness = 1;
		printf("2. Enter holder's name (<=%d letters):\n", NAME_LENGTH);
		printf("Your input: ");
		scanf("%21s", account.name);
		getchar();
		correctness = checkIfNameContainsDigits();
	} while(strlen(account.name)<1||strlen(account.name)>NAME_LENGTH||correctness != 1);
}

void getAccountSurname()
{
	int correctness;
	do
	{
		correctness = 1;
		printf("3. Enter holder's surname (<=%d letters):\n", SURNAME_LENGTH);
		printf("Your input: ");
		scanf("%31s", account.surname);
		getchar();
		correctness = checkIfSurnameContainsDigits();
	} while(strlen(account.surname)<1||strlen(account.surname)>SURNAME_LENGTH||atof(account.surname)!=0||correctness != 1);
}

void getAccountAddress()
{
	
	do
	{
		printf("4. Enter holder's address (<=%d letters):\n", ADDRESS_LENGTH);
		printf("Your input: ");
		fgets(account.address, 52, stdin);
	} while(strlen(account.address)<1||strlen(account.address)> (ADDRESS_LENGTH + 1));
	account.address[strlen(account.address)-1]='\0';
}

void getAccountPesel()
{
	char temp[NAME_LENGTH + 1];
	
	do
	{
		printf("5. Enter holder's PESEL (%d digits):\n", PESEL_LENGTH);
		printf("Your input: ");
		scanf("%12s", temp);
		getchar();
	} while(sscanf(temp, "%llu", &account.pesel)!=1||strlen(temp)!=PESEL_LENGTH);
}

void getAccountBalance()
{
	char temp[NAME_LENGTH + 1];
	
	do
	{
		printf("6. Enter account's balance:\n");
		printf("Your input: ");
		scanf("%21s", temp);
		getchar();
	} while(sscanf(temp, "%lf", &account.balance)!=1);
}

void createNewAccount()
{
	bool addNext;
	/*char temp[NAME_LENGTH + 1];*/
	FILE *fp;
	
	if((fp=fopen("database.txt", "a"))==NULL)
	{
		printf ("Error! Can't open database.txt!\n");
		exit(1);
	}
	do
	{
		addNext=false;
		do
		{
			getAccountNumber();
			getAccountName();
			getAccountSurname();
			getAccountAddress();
			getAccountPesel();
			getAccountBalance();
			
			printf("\nnumber: %u;\nname: %s;\nsurname: %s;\naddress: %s;\npesel: %llu;\nbalance: %.2lf;\n", account.number, account.name, account.surname, account.address, account.pesel, account.balance);
		} while(!isCorrect());
		fseek(fp , 0 , SEEK_END);
		fprintf (fp, "%10u; %20s; %30s; %50s; %11llu; %15.2lf;\n", account.number, account.name, account.surname, account.address, account.pesel, account.balance);
		printf("Add another account?\n[y] Yes\n[n] No\n");
		printf("Your input: ");
		if(getchar()=='y')
		{
			addNext=true;
		}
		getchar();
	} while(addNext);
	fclose (fp);
}

bool isCorrect()
{
	int choice;
	while(true)
	{
		printf("Is the data correct?\n[y] Yes\n[n] No\n");
		printf("Your input: ");
		choice=getchar();
		getchar();
		if((char) choice=='y')
		{
			return true;
		}
		else if((char) choice=='n')
		{
			return false;
		}
		else
		{
			continue;
		}
	}
}

void displayAllExistingAccounts()
{
	system("clear");
	printf("Banking System v1.0\n\n");
	FILE *fp;
	int c;
		
	do
	{
		if((fp=fopen("database.txt", "r"))==NULL)
		{
		printf ("Error! Can't open database.txt!\n");
		exit(1);
		}
		system("clear");
		printf("All existing accounts in the database: \n\n");
		while ((c=getc(fp)) != EOF)
		{
			putchar(c);
				
		}
		fclose(fp);
		printf("\n[return] Return to previous menu\n");
		printf("Your input: ");
		inputStringToLowerCase();
	}
	while(strcmp(choice_buffer, "return") != 0);
	
}

void displaySearchChoice(int choice)
{
	switch((char) choice)
	{
		case 0:
			break;
		case 1:
			printf("\nYou have chosen to search by Account Number\n");
			break;
		case 2:
			printf("\nYou have chosen to search by Name\n");
			break;
		case 3:
			printf("\nYou have chosen to search by Surname\n");
			break;
		case 4:
			printf("\nYou have chosen to search by Owner Address\n");
			break;
		case 5:
			printf("\nYou have chosen to search by PESEL\n");
			break;
		case 6:
			printf("\nYou have chosen to search by Balance\n");
			break;
		default:
			break;
	}
}

void searchAccountByPhrase()
{
	char line[150], search[50], temp[100];
	int currentField, i=0, j=0, choice=0;
	FILE *fp;
	if((fp=fopen("database.txt", "r"))==NULL)
	{
		printf ("Error! Can't open database.txt!\n");
		exit(1);
	}
	printf("\nWhich field you'd like to search in?:\n[1] Account Number\n[2] Name\n[3] Surname\n[4] Owner Address\n[5] PESEL\n[6] Balance\n[0] Exit\n\n");
	printf("Your input: ");
	choice=getchar()-48;
	displaySearchChoice(choice);
	getchar();
	printf("Enter the data of field [%d] that you're looking for: ", choice);
	scanf("%[^\n]", search);
	printf("You are searching by: %s\n\n", search);
	getchar();
	while(fgets(line, 150, fp))
	{
		i=0, j=0;
		currentField=1;
		if(strstr(line, search)!=NULL)
		{
			while(line[i]!='\n')
			{
				if(choice==1)
				{
					break;
				}
				if(line[i]==';')
				{
					currentField++;
					if(currentField==choice)
					{
						i++;
						break;
					}
				}
				i++;
			}
			while(line[i]!=';')
			{
				temp[j]=line[i];
				i++;
				j++;
			}
			temp[j]='\0';
			if(strstr(temp, search)!=NULL)
			{
				printf("%s\n",line);
			}
		}
	}
	fclose(fp);
	printf("\n[return] Return to previous menu\n\n");
	do
	{
		inputStringToLowerCase();
	}
	while(strcmp(choice_buffer, "return") != 0);
}

int findAccountNumber(char *accountNumber)
{
	char line[149], temp [10];
	bool wasFound=false;
	int i=0, j=0, c=0;
	FILE *fp;
	if((fp=fopen("database.txt", "rb"))==NULL)
	{
		printf ("Error! Can't open database.txt!\n");
		exit(1);
	}
	fseek(fp , 0 , SEEK_SET);
	while(fgets(line, 149, fp))
	{
		if(strstr(line, accountNumber)!=NULL)
		{
			while(line[i]!=';')
			{
				temp[i]=line[i];
				i++;
			}
			temp[i]='\0';
			while(temp[0]==' ')
			{
				for(c=0; c<11; c++)
				{
					temp[c]=temp[c+1];
					if(temp[c+1]=='\0')
					{
						break;
					}
				}
			}
			if(strcmp(temp, accountNumber)==0)
			{
				wasFound=true;
				break;
			}
		}
		i=0;
		j++;
	}
	fclose(fp);
	if(wasFound)
	{
		return j;
	}
	else
	{
		return -1;
	}
}

void whichOperation()
{
	int choice;
	printf("Which operation you'd like to perform?\n[1] Transfer between accounts\n[2] Make a deposit\n[3] Make a withdrawal\n[0] Back\n\n");
	choice=getchar();
	getchar();
	switch((char) choice)
	{
		case '0':
			break;
		case '1':
			transferMoneyBetweenAccounts();
			break;
		case '2':
			makeDeposit();
			break;
		case '3':
			makeWithdrawal();
			break;
		default:
			break;
	}
}

void makeMoneyTranfer(char *accountNumber, double amount, bool isWithdrawal)
{
	double balance;
	char temp[18];
	int i=0, j=findAccountNumber(accountNumber);
	FILE *fp;
	if((fp=fopen("database.txt", "r+"))==NULL)
	{
		printf ("Error! Can't open database.txt!\n");
		exit(1);
	}
	fseek(fp, (j+1)*148-18, SEEK_SET);
	for(int m=0; m<16; m++)
	{
		temp[i]=(char) getc(fp);
		i++;
	}
	temp[i]='\0';
	balance=atof(temp);
	fseek(fp, (j+1)*148-18, SEEK_SET);
	if(isWithdrawal)
	{
		fprintf(fp, "%16.2lf", balance-amount);
	}
	else
	{
		fprintf(fp, "%16.2lf", balance+amount);
	}
	fclose(fp);
}

void displayAccount(char *accountNumber)
{
	char line[148];
	
	FILE *fp;
	if((fp=fopen("database.txt", "r"))==NULL)
	{
		printf ("Error! Can't open database.txt!\n");
		exit(1);
	}
	int j=findAccountNumber(accountNumber);
	if(j!=-1)
	{
		fseek(fp, (j)*LINECONSTR, SEEK_SET);
		fgets(line, sizeof(line), fp);
		printf("%s\n", line);
	}
	else
	{
		printf("\nAccount with number %s doesn't exist!\n", accountNumber);
	}
	fclose(fp);
	
}

void transferMoneyBetweenAccounts()
{
	bool isWithdrawal=true, isWrongAccount;
	char tempAccount[12], tempAccount2[12], sum[16];
	do
	{
		isWrongAccount=false;
		do
		{
			printf("Enter the account number to withdraw money:\n");
			printf("Your input: ");
			scanf("%s", tempAccount);
			getchar();
		} while(sscanf(tempAccount, "%d", &account.number)!=1||strlen(tempAccount)>10);
		displayAccount(tempAccount);
		if(findAccountNumber(tempAccount)==-1)
		{
			isWrongAccount=true;
		}
	} while(isWrongAccount||!isCorrect());
	do
	{
		do
		{
			printf("Enter the sum you want to transfer:\n");
			printf("Your input: ");
			scanf("%s", sum);
			getchar();
		} while(sscanf(sum, "%lf", &account.balance)!=1||strlen(sum)>16);
		printf("Sum to transfer:%.2lf\n", account.balance);
	} while(!isCorrect());
	makeMoneyTranfer(tempAccount, account.balance, isWithdrawal);
	do
	{
		isWrongAccount=false;
		do
		{
			printf("Enter the account number to transfer funds:\n");
			printf("Your input: ");
			scanf("%s", tempAccount2);
			getchar();
		} while(sscanf(tempAccount2, "%d", &account.number)!=1||strlen(tempAccount2)>10);
		displayAccount(tempAccount2);
		if(findAccountNumber(tempAccount2)==-1)
			isWrongAccount=true;
	} while(isWrongAccount||!isCorrect());
	makeMoneyTranfer(tempAccount2, account.balance, !isWithdrawal);
	
	
	do
	{
		printf("Operation completed successfully. Current data:\n");
		displayAccount(tempAccount);
		displayAccount(tempAccount2);
		printf("\n[return] Return to previous menu\n\n");
		inputStringToLowerCase();
	}
	while(strcmp(choice_buffer, "return") != 0);
}

void makeWithdrawal()
{
	bool isWithdrawal=true, isWrongAccount;
	char tempAccount[12], sum[15];
	do
	{
		isWrongAccount=false;
		do
		{
			printf("Enter the account number to withdraw money:\n");
			printf("Your input: ");
			scanf("%s", tempAccount);
			getchar();
		} while(sscanf(tempAccount, "%d", &account.number)!=1||strlen(tempAccount)>10);
		displayAccount(tempAccount);
		if(findAccountNumber(tempAccount)==-1)
		{
			isWrongAccount=true;
		}
	} while(isWrongAccount||!isCorrect());
	do
	{
		do
		{
			printf("Enter the sum you want to withdraw:\n");
			printf("Your input: ");
			scanf("%s", sum);
			getchar();
		} while(sscanf(sum, "%lf", &account.balance)!=1||strlen(sum)>15);
		printf("\nSum to withdraw:%.2lf\n", account.balance);
	} while(!isCorrect());
	makeMoneyTranfer(tempAccount, account.balance, isWithdrawal);
	
	
	do
	{
		printf("Operation completed successfully. Current data:\n");
		displayAccount(tempAccount);
		printf("\n[return] Return to previous menu\n\n");
		inputStringToLowerCase();
	}
	while(strcmp(choice_buffer, "return") != 0);
}

void makeDeposit()
{
	bool isWithdrawal=true, isWrongAccount;
	char tempAccount[12], sum[15];
	do
	{
		isWrongAccount=false;
		do
		{
			printf("Enter the account number to deposit money:\n");
			printf("Your input: ");
			scanf("%s", tempAccount);
			getchar();
		} while(sscanf(tempAccount, "%d", &account.number)!=1||strlen(tempAccount)>10);
		displayAccount(tempAccount);
		if(findAccountNumber(tempAccount)==-1)
		{
			isWrongAccount=true;
		}
	} while(isWrongAccount||!isCorrect());
	do
	{
		do
		{
			printf("Enter the sum you want to deposit:\n");
			printf("Your input: ");
			scanf("%s", sum);
			getchar();
		} while(sscanf(sum, "%lf", &account.balance)!=1||strlen(sum)>15);
		printf("Sum to deposit:%.2lf\n", account.balance);
	} while(!isCorrect());
	makeMoneyTranfer(tempAccount, account.balance, !isWithdrawal);
	
	do
	{
		printf("Operation completed successfully. Current data:\n");
		displayAccount(tempAccount);
		printf("\n[return] Return to previous menu\n\n");
		inputStringToLowerCase();
	}
	while(strcmp(choice_buffer, "return") != 0);
}