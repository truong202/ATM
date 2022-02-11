//#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Model.c"

const char *FILE_NAME = "ATMCards.dat";

int createATMCard(ATMCard card);
int login(char *accountNo, char *pinCode, ATMCard *card);
int getCard(char *accountNo, ATMCard *card);
int findIndex(char *accountNo, ATMCard *cards, int count);
int withdrawal(Receipt receipt);
int changePin(char *pinCode, char *accountNo);
int isValidStringNumber(char *str);
int isValidName(char *str);
void trim(char *str);
void standardizeName(char *value);
void priceFormat(long long n, char *str);
int writeToFile(ATMCard *ATMCards, int count, const char *fileName);
ATMCard *readFromFile(int *count, const char *fileName);

int createATMCard(ATMCard card)
{
    int result = 0;
    int count = 0;
    ATMCard *cards;
    cards = readFromFile(&count, FILE_NAME);
    if (cards == NULL || count == 0)
    {
        cards = (ATMCard *)calloc(0, sizeof(ATMCard));
    }
    cards = (ATMCard *)realloc(cards, (count + 1) * sizeof(ATMCard));
    cards[count] = card;
    count++;
    if (writeToFile(cards, count, FILE_NAME))
    {
        result = 1;
    }
    free(cards);
    return result;
}

int login(char accountNo[], char pinCode[], ATMCard *card)
{
    int count = 0;
    ATMCard *cards;
    cards = readFromFile(&count, FILE_NAME);
    if (cards == NULL)
    {
        return 0;
    }
    for (int i = 0; i < count; i++)
    {

        if ((strcmp(accountNo, cards[i].AccountNo) == 0) && (strcmp(pinCode, cards[i].PinCode) == 0))
        {
            *card = cards[i];
            return 1;
        }
    }
    return 0;
}

int withdrawal(Receipt receipt)
{
    int count = 0;
    ATMCard *cards;
    cards = readFromFile(&count, FILE_NAME);
    if (cards == NULL)
    {
        return 0;
    }
    int index = findIndex(receipt.Card.AccountNo, cards, count);
    if (index == -1)
        return 0;
    if (cards[index].Balance < receipt.Money)
        return 0;
    cards[index].Balance -= receipt.Money;
    return writeToFile(cards, count, FILE_NAME);
    return 0;
}

int transfers(Transaction trans)
{
    if (trans.Money <= 0)
        return 0;
    int count = 0;
    ATMCard *cards;
    cards = readFromFile(&count, FILE_NAME);
    if (cards == NULL)
        return 0;
    int index;
    index = findIndex(trans.Sender.AccountNo, cards, count);
    if (index == -1)
        return 0;
    if (trans.Money > cards[index].Balance)
        return 0;
    cards[index].Balance -= trans.Money;
    index = findIndex(trans.Recipient.AccountNo, cards, count);
    if (index == -1)
        return 0;
    cards[index].Balance += trans.Money;
    return writeToFile(cards, count, FILE_NAME);
}

int changePin(char *pinCode, char *accountNo)
{
    int count = 0;
    ATMCard *cards;
    cards = readFromFile(&count, FILE_NAME);
    if (cards == NULL)
        return 0;
    int index = findIndex(accountNo, cards, count);
    if (index == -1)
        return 0;
    strcpy(cards[index].PinCode, pinCode);
    return writeToFile(cards, count, FILE_NAME);
}

int getCard(char *accountNo, ATMCard *card)
{
    int count = 0;
    ATMCard *cards;
    cards = readFromFile(&count, FILE_NAME);
    for (int i = 0; i < count; i++)
    {
        if (strcmp(cards[i].AccountNo, accountNo) == 0)
        {
            *card = cards[i];
            return 1;
        }
    }
    return 0;
}
int findIndex(char *accountNo, ATMCard *cards, int count)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(cards[i].AccountNo, accountNo) == 0)
        {
            return i;
        }
    }
    return -1;
}
int isValidStringNumber(char *str)
{
    int length = strlen(str);
    for (int i = 0; i < length; i++)
    {
        if (str[i] < '0' || str[i] > '9')
            return 0;
    }
    return 1;
}

int isValidName(char *str)
{
    int length = strlen(str);
    for (int i = 0; i < length; i++)
    {
        if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || str[i] == 32)
        {
            continue;
        }
        return 0;
    }
    return 1;
}

void trim(char *str)
{
    int length = strlen(str);
    while ((str[length - 1]) == ' ')
    {
        strcpy(&str[length - 1], "\0");
        length--;
    }
    while (str[0] == ' ')
    {
        strcpy(&str[0], &str[1]);
    }
    length = strlen(str);
    for (int i = 0; i < length; i++)
    {
        while (str[i] == ' ' && str[i + 1] == ' ')
        {
            strcpy(&str[i], &str[i + 1]);
        }
    }
}

void standardizeName(char *value)
{
    trim(value);
    int length = strlen(value);
    for (int i = 1; i < length; i++)
    {
        if (value[i - 1] == ' ' && (value[i] >= 97 && value[i] <= 122))
            value[i] -= 32;
        else if (value[i - 1] != ' ' && (value[i] >= 65 && value[i] <= 90))
            value[i] += 32;
    }
    if (value[0] >= 97 && value[0] <= 122)
        value[0] -= 32;
}

void priceFormat(long long n, char *str)
{
    int isNegative = 0;
    char numString[100];
    strcpy(str, "");
    if (n < 0)
    {
        isNegative = 1;
        n *= -1;
    }
    if (n < 1000)
    {
        if (isNegative)
        {
            strcpy(str, "-");
        }
        sprintf(numString, "%lld", n);
        strcat(str, numString);
        return;
    }
    while (n > 1000)
    {
        sprintf(numString, "%03lld", n % 1000);
        strcat(str, strrev(numString));
        strcat(str, ".");
        n = n / 1000;
    }
    sprintf(numString, "%lld", n);
    strcat(str, strrev(numString));
    if (isNegative)
        strcat(str, "-");
    strrev(str);
}

int writeToFile(ATMCard *ATMCards, int count, const char *fileName)
{
    FILE *f;
    int result = 0;
    f = fopen(fileName, "w");
    if (f != NULL)
    {
        for (int i = 0; i < count; i++)
        {
            fprintf(f, "%s\n%s\n%s\n%lld\n", ATMCards[i].AccountName, ATMCards[i].AccountNo, ATMCards[i].PinCode, ATMCards[i].Balance);
        }
        fclose(f);
        result = 1;
    }
    return result;
}

ATMCard *readFromFile(int *count, const char *fileName)
{
    FILE *f;
    int result = 0;
    f = fopen(fileName, "r");
    *count = 0;
    if (f != NULL)
    {
        ATMCard *cards = (ATMCard *)calloc(0, sizeof(ATMCard));
        fseek(f, 0L, SEEK_END);
        int a = ftell(f);
        if (a)
        {
            fclose(f);
            f = fopen(fileName, "r");
            while (1)
            {
                ATMCard card;
                fscanf(f, "%[^\n]%*c%s%s%lld\n", card.AccountName, card.AccountNo, card.PinCode, &card.Balance);
                cards = (ATMCard *)realloc(cards, ((*count) + 1) * sizeof(ATMCard));
                cards[*count] = card;
                (*count)++;
                if (feof(f))
                    break;
            }
            fclose(f);
        }
        return cards;
    }
    return NULL;
}
