#include <stdio.h>
#include <string.h>
#include "ConsoleUI.c"
#include "Function.c"
#include "Model.c"

int main(int argc, char const *argv[])
{
    char isContinue = 'Y';
    do
    {
        ATMCard card = inputCard();
        int result = createATMCard(card);
        printf(" Create ATM %s!\n", result == 1 ? "completed" : "not complete");
        printf(" Do you want to create ATM Card? (Y/N): ");
        isContinue = getYN();
    } while (isContinue == 'Y' || isContinue == 'y');

    return 0;
}
