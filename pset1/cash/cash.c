#include <stdio.h>
#include <cs50.h>
#include <math.h>

int get_min_coins(float change);

int main(void)
{
    // get input
    float change;
    do
    {
        change = get_float("Change owed: ");
    } while (change < 0);
    printf("%d\n", get_min_coins(change));
}

// available coins are 0.25, 0.1, 0.05, 0.01
int get_min_coins(float change)
{
    // must use round to avoid 4.19xxxxx dollar evaluate to 419 cents
    int bal = round(change*100);
    int coins[] = {25, 10, 5, 1};
    int coin_index = 0;
    int coin_used = 0;
    while (bal > 0) {
        if (bal >= coins[coin_index]) {
            coin_used += bal / coins[coin_index];
            bal = bal % coins[coin_index];

        } else {
            coin_index++;
        }
    }
    return coin_used;
}