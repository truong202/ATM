#pragma once
#include <time.h>

#define VAT 0.0001
#define FEES 0.001;

typedef struct ATMCard
{
    char AccountNo[20];
    char AccountName[51];
    char PinCode[10];
    long long Balance;
} ATMCard;

typedef struct Transaction
{
    long TransactionNo;
    ATMCard Sender;
    ATMCard Recipient;
    long long Money;
    struct tm *Date;
    char Node[100];
} Transaction;

typedef struct Receipt
{
    long ReceiptNo;
    ATMCard Card;
    long long Money;
    struct tm *Date;
} Receipt;