#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include "ConsoleUI.c"

int main(int argc, char const *argv[])
{
    ATMCard card;
    if (!loginUI(&card))
        return 0;
    char *mainMenu[] = {"Checking account balance", "Withdrawal", "Transfers", "Change PIN", "End of transaction"};
    long long money;
    int result;
    int choose, choose1;
    do
    {
        choose = menu("ATM Machine", mainMenu, 5);
        switch (choose)
        {
        case 1: // Checking account balance
            checkBalanceUI(card);
            break;
        case 2: // Withdrawal
            withdrawalUI(card);
            break;
        case 3: // Transfers
            transfersUI(card);
            break;
        case 4: // Change PIN
            changePinUI(card);
            break;
        }
        getCard(card.AccountNo, &card);
    } while (choose != 5);

    return 0;
}
