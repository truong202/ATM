#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "Model.c"
#include <conio.h>
#include "Function.c"

char *BANK_NAME = "VTC Academy Bank";
const int LENGTH_LINE = 40;
ATMCard inputCard();
int menu(char *title, char *menuItems[], int menuItemCount);
int loginUI(ATMCard *card);
void withdrawalUI(ATMCard card);
void checkBalanceUI(ATMCard card);
void printWithdrawalReceipt(Receipt receipt);
void printTransfersReceipt(Transaction trans);
void transfersUI(ATMCard card);
void changePinUI(ATMCard card);
void writeTextCenter(char *text, int lengthLine);
void getName(char *str);
void getString(char *str, int length);
void getNumberString(char *str, int length, char *msg);
char getYN();
void getPinCode(char *str);
long long getMoney(char *msg);
void printTitle(char *title);
void printLine(char c, int lengthLine);

int loginUI(ATMCard *card)
{
    int result = 0;
    char exit;
    char accountNo[18];
    char pinCode[8];
    do
    {
        printTitle("ATM MACHINE");
        printf(" Account No: ");
        getNumberString(accountNo, 14, "account No");
        printf(" Pin Code: ");
        getPinCode(pinCode);
        result = login(accountNo, pinCode, card);
        if (!result)
        {
            printf(" Account No or Pin Code incorrect!\n");
            printf(" Do you want to try again? [Y/N]: ");
            exit = getYN();
        }
        else
        {
            printLine('-', LENGTH_LINE);
            printf(" Account Name: %s\n", card->AccountName);
            return result;
        }
    } while (exit == 'y' || exit == 'Y');
    return result;
}

void checkBalanceUI(ATMCard card)
{
    char balance[50];
    priceFormat(card.Balance, balance);
    printf(" Balance in the account: %s VND\n", balance);
    printf(" Press any key to continue...");
    _getch();
    printf("\n");
}

void withdrawalUI(ATMCard card)
{
    char *withdrawalMenu[] = {"100.000 VND", "200.000 VND", "500.000 VND", "1.000.000 VND", "2.000.000 VND", "Orther number"};
    long long money;
    int result;
    char isPrint;
    int choose = menu("ATM Machine - Withdrawal", withdrawalMenu, 6);
    switch (choose)
    {
    case 1:
        money = 100000;
        break;
    case 2:
        money = 200000;
        break;
    case 3:
        money = 500000;
        break;
    case 4:
        money = 1000000;
        break;
    case 5:
        money = 2000000;
        break;
    case 6:
        printf(" Input money: ");
        money = getMoney("Input money");
        break;
    }
    if (card.Balance < money)
    {
        printf(" Insufficient balance!\n");
    }
    else
    {
        Receipt receipt;
        time_t now;
        time(&now);
        receipt.Date = localtime(&now);
        receipt.Date->tm_year += 1900;
        receipt.Money = money;
        receipt.Card = card;
        receipt.ReceiptNo = 1;
        result = withdrawal(receipt);
        printf(" withdrawal %s\n", result == 1 ? "completed" : "not complete");
        if (result)
        {
            printf(" Do you want print? [Y/N]: ");
            isPrint = getYN();
            if (isPrint == 'y' || isPrint == 'Y')
            {
                printWithdrawalReceipt(receipt);
            }
        }
    }
    printf(" Press any key to continue...");
    _getch();
    printf("\n");
}

void printWithdrawalReceipt(Receipt receipt)
{
    char *accountNo;
    char price[50];
    long long fees = receipt.Money * FEES;
    long long vat = receipt.Money * VAT;
    getCard(receipt.Card.AccountNo, &receipt.Card);
    strcpy(accountNo, receipt.Card.AccountNo);
    for (int i = 4; i < 10; i++)
    {
        accountNo[i] = 'x';
    }
    printTitle("BIEN LAI RUT TIEN TAI ATM");
    printf(" Ngay: %02d/%02d/%d            Gio: %02d:%02d\n", receipt.Date->tm_mday, receipt.Date->tm_mon + 1,
           receipt.Date->tm_year, receipt.Date->tm_hour, receipt.Date->tm_min);
    printf(" So the: %30s\n", accountNo);
    printf(" So giao dich: %24d\n", receipt.ReceiptNo);
    priceFormat(receipt.Money, price);
    strcat(price, " VND");
    printf(" So tien: %29s\n", price);
    printf(" Noi dung: %28s\n", "Rut tien tai ATM");
    printLine('-', LENGTH_LINE);
    priceFormat(receipt.Card.Balance, price);
    strcat(price, " VND");
    printf(" So du: %31s\n", price);
    priceFormat(fees, price);
    strcat(price, " VND");
    printf(" Le phi: %30s\n", price);
    priceFormat(vat, price);
    strcat(price, " VND");
    printf(" VAT: %33s\n", price);
    printLine('-', LENGTH_LINE);
    writeTextCenter("Cam on quy khach da su dung", LENGTH_LINE);
    writeTextCenter("dich vu cua chung toi!", LENGTH_LINE);
    printLine('-', LENGTH_LINE);
}

void transfersUI(ATMCard card)
{
    int result;
    char isPrint;
    ATMCard recipient;
    char recipientAccountNo[20];
    printf(" Transfers to Account: ");
    getNumberString(recipientAccountNo, 14, "Account No");
    result = getCard(recipientAccountNo, &recipient);
    if (!result)
    {
        printf(" Account does not exist!\n");
    }
    else if (strcmp(recipientAccountNo, card.AccountNo) == 0)
    {
        printf(" Can't transfer money to my own account!\n");
    }
    else
    {

        Transaction trans;
        printf(" Money to transfer: ");
        trans.Money = getMoney("Money to transfer");
        if (trans.Money > card.Balance)
        {
            printf(" Insufficient balance!\n");
        }
        else
        {
            Receipt receipt;
            time_t now;
            time(&now);
            trans.Date = localtime(&now);
            trans.Date->tm_year += 1900;
            trans.TransactionNo = 1;
            trans.Sender = card;
            trans.Recipient = recipient;
            result = transfers(trans);
            printf(" Transfers %s!\n", result == 1 ? "completed" : "not complete");
            if (result)
            {
                printf(" Do you want print? [Y/N]: ");
                isPrint = getYN();
                if (isPrint == 'y' || isPrint == 'Y')
                {
                    printTransfersReceipt(trans);
                }
            }
        }
    }
    printf(" Press any key to exit...");
    _getch();
    printf("\n");
}

void printTransfersReceipt(Transaction trans)
{
    char price[50];
    long long fees = trans.Money * FEES;
    long long vat = trans.Money * VAT;
    getCard(trans.Sender.AccountNo, &trans.Sender);
    printTitle("BIEN LAI CHUYEN TIEN TAI ATM");
    printf(" Ngay: %02d/%02d/%d            Gio: %02d:%02d\n", trans.Date->tm_mday, trans.Date->tm_mon + 1,
           trans.Date->tm_year, trans.Date->tm_hour, trans.Date->tm_min);
    printf(" So giao dich: %24d\n", trans.TransactionNo);
    printf(" Nguoi gui: %27s\n", trans.Sender.AccountName);
    printf(" Nguoi nhan: %26s\n", trans.Recipient.AccountName);
    priceFormat(trans.Money, price);
    strcat(price, " VND");
    printf(" So tien: %29s\n", price);
    printf(" Noi dung: %28s\n", "Chuyen tien tai ATM");
    printLine('-', LENGTH_LINE);
    priceFormat(fees, price);
    strcat(price, " VND");
    printf(" Le phi: %30s\n", price);
    priceFormat(vat, price);
    strcat(price, " VND");
    printf(" VAT: %33s\n", price);
    printLine('-', LENGTH_LINE);
    writeTextCenter("Cam on quy khach da su dung", LENGTH_LINE);
    writeTextCenter("dich vu cua chung toi!", LENGTH_LINE);
    printLine('-', LENGTH_LINE);
}

void changePinUI(ATMCard card)
{
    int result;
    char pin[10], newPin[10], confirmPin[10];
    printf(" Your Pin Code: ");
    getPinCode(pin);
    if (strcmp(pin, card.PinCode) != 0)
    {
        printf(" Pin Code incorrect!\n");
    }
    else
    {
        while (1)
        {
            printf(" New Pin Code: ");
            getPinCode(newPin);
            printf(" Confirm Pin Code: ");
            getPinCode(confirmPin);
            if (strcmp(newPin, confirmPin) == 0)
            {
                result = changePin(newPin, card.AccountNo);
                printf(" Change Pin Code %s!\n", result ? "completed" : "not complete");
                break;
            }
            printf(" Pin Code incorrect!\n\n");
        }
    }
    printf(" Press any key to exit...");
    _getch();
    printf("\n");
}

ATMCard inputCard()
{
    ATMCard card;
    printTitle("Create ATM Cards");
    printf(" Input Account Name: ");
    getName(card.AccountName);
    standardizeName(card.AccountName);
    printf(" Input Account No:   ");
    getNumberString(card.AccountNo, 14, "Account No");
    printf(" Input Pin Code:     ");
    getPinCode(card.PinCode);
    printf(" Input balance:      ");
    card.Balance = getMoney("Input balance");
    while (card.Balance < 50000)
    {
        printf(" Account balance greater than or equal 50.000 VND!\n");
        printf(" Re-enter balance: ");
        card.Balance = getMoney("Input Money");
    }
    printLine('-', LENGTH_LINE);
    return card;
}

int menu(char *title, char *menuItems[], int menuItemCount)
{
    int choose = 0;
    if (title != NULL)
        printTitle(title);
    for (int i = 0; i < menuItemCount; i++)
    {
        printf(" %d. %s\n", i + 1, menuItems[i]);
    }
    printLine('-', LENGTH_LINE);
    printf(" #Your choice: ");
    fflush(stdin);
    scanf("%d", &choose);
    while (choose <= 0 || choose > menuItemCount)
    {
        printf(" ENTERED INCORRECTLY, RE-ENTER: ");
        fflush(stdin);
        scanf("%d", &choose);
    }
    return choose;
}

void getString(char *str, int length)
{
    fseek(stdin, 0, SEEK_END);
    fflush(stdin);
    gets(str);
    str[length - 1] = '\0';
    fseek(stdin, 0, SEEK_END);
    fflush(stdin);
}

void getName(char *str)
{
    while (1)
    {
        getString(str, 50);
        trim(str);
        if (isValidName(str) && strlen(str))
            return;
        printf(" Only letters (a-Z) and spaces are allowed!\n");
        printf(" Re-enter Name: ");
    }
}

long long getMoney(char *msg)
{
    char moneyString[50];
    char moneyFormat[50];
    long long money;
    int count = 0;
    int key;
    while (1)
    {
        key = _getch();
        if (key == 13)
        {
            if (count == 0)
            {
                printf(" Invalid money!\n");
                printf(" %s:", msg);
                continue;
            }
            printf("\n");
            return money;
        }
        else if (key == 8 && count > 0)
        {
            count--;
            moneyString[count] = '\0';
            money = atoll(moneyString);
            priceFormat(money, moneyFormat);
            printf("\r                                               ");
            printf("\r %s: ", msg);
            if (count > 0)
            {
                printf("%s", moneyFormat);
            }
        }
        else if (key >= '0' && key <= '9' && count < 50)
        {
            moneyString[count] = (char)key;
            count++;
            moneyString[count] = '\0';
            money = atoll(moneyString);
            priceFormat(money, moneyFormat);
            printf("\r %s: %s", msg, moneyFormat);
        }
    }
}

void getNumberString(char *str, int length, char *msg)
{
    while (1)
    {
        getString(str, length + 2);
        int lengths = strlen(str);
        if (lengths == length)
        {
            if (isValidStringNumber(str))
                return;
            printf(" Only numbers (0-9) are allowed!\n");
        }
        else
            printf(" %s must have %d digits\n", msg, length);

        printf(" Re-enter %s: ", msg);
    }
}

void getPinCode(char *str)
{
    int key;
    int count = 0;
    while (1)
    {
        key = _getch();
        if (key == 13)
        {
            str[count] = '\0';
            if (count < 6)
            {
                printf("\n Pin Code have must 6 digits!\n");
                printf(" Re-enter Pin Code: ");
                count = 0;
                continue;
            }
            printf("\n");
            return;
        }
        else if (key == 8 && count > 0)
        {
            count--;
            printf("\b \b");
        }
        else if (key >= '0' && key <= '9' && count < 6)
        {
            printf("*");
            str[count] = key;
            count++;
        }
    }
}

char getYN()
{
    char c;
    while (true)
    {
        fflush(stdin);
        c = getchar();
        if (c == 'y' || c == 'Y' || c == 'n' || c == 'N')
            return c;
        printf(" ENTERED INCORRECTLY, RE-ENTER: ");
    }
}

void printTitle(char *title)
{
    printLine('=', LENGTH_LINE);
    writeTextCenter(BANK_NAME, LENGTH_LINE);
    printLine('=', LENGTH_LINE);
    writeTextCenter(title, LENGTH_LINE);
    printLine('-', LENGTH_LINE);
}

void printLine(char c, int length)
{
    for (int i = 0; i < length; i++)
    {
        putchar(c);
    }
    printf("\n");
}

void writeTextCenter(char *text, int lengthLine)
{
    int length = strlen(text);
    int position = lengthLine / 2 - length / 2;
    for (int i = 0; i < position; i++)
    {
        printf(" ");
    }
    printf("%s\n", text);
}