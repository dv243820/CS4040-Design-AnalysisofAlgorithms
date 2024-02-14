/**
 *        @file: hw4.cc
 *      @author: Drew VanAtta
 *        @date: November 02, 2023
 *       @brief: HW 4 - Dynamic Programming
 */

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <chrono>
#include <vector>
using namespace std;

/// function prototypes
// MAX FUNCTION - returns the max of the two integers passed
int max(int x, int y)
{
    if (x > y)
    {
        return x;
    }
    else
        return y;
}

// Sparce price cut rod algorithm from question 1a
int sparcePriceCutRod(int prices[], int length)
{
    int r[length + 1]; // HOLDS THE MEMOIZATION, +1 because we start at 0 (next line)
    r[0] = 0;          // THE ZERO START, NO ROD = 0 MONEY
    int i, j;          // indexing for the loops

    for (i = 1; i <= length; i++) // this loop contains other loop to find sol, and then adds it to our memoization array r
    {

        int max_val = INT_MIN;  // q = -infinity, probably could have just done -1 but the website I yoinked the base from used this so I thought it was cool
                                // max_val resets for comparison in next loop
        for (j = 0; j < i; j++) // loop that checks the memoization
        {
            max_val = max(max_val, prices[j] + r[i - j - 1]);
        }
        r[i] = max_val; // add optimal solution to array
    }

    return r[length];
}

// Result from 1b, can take lengths longer.
int longSparcePriceCutRod(int prices[], int length, int sizeOfPrices)
{
    int r[length + 1]; // HOLDS THE MEMOIZATION, +1 because we start at 0 (next line)
    r[0] = 0;          // THE ZERO START, NO ROD = 0 MONEY
    int i, j;          // indexing for the loops

    for (i = 1; i <= length; i++) // this loop contains other loop to find sol, and then adds it to our memoization array r
    {

        int max_val = INT_MIN;  // q = -infinity
                                // max_val resets for comparison in next loop
        for (j = 0; j < i; j++) // loop that checks the memoization
        {
            max_val = max(max_val, prices[j % sizeOfPrices] + r[i - (j % sizeOfPrices) - 1]);
        }
        r[i] = max_val; // add optimal solution to array
    }

    return r[length];
}

// ORIGINAL COIN CHANGE FUNCTION
double coinChange(int x, int coins[], int num_coins)
{
    int coin_value;
    double ways[x + 1] = {0};

    ways[0] = 1;
    for (int i = 0; i <= num_coins - 1; i++)
    {
        coin_value = coins[i];
        for (int j = coin_value; j <= x; j++)
        {
            ways[j] = ways[j - coin_value] + ways[j];
        }
    }
    return ways[x];
}

// KEEPING THE UNIQUE WAYS COIN CHANGE
int coinChangeCombos(int x, int coins[], int num_coins)
{
    int coin_value;
    int ways[x + 1] = {0};
    int i, j;
    int sum = 0;

    int tmp; // tmp int
    vector<vector<int>> vecVec;
    vector<int> vec1; // base vector
    for (int k = 0; k < x; k++)
    {
        vec1.push_back(1);
    }
    vecVec.push_back(vec1); // The first vector in the vectorvector is vec1

    ways[0] = 1;
    for (i = 0; i <= num_coins - 1; i++) // go through each coin
    {
        int tmpval = 0;        // here so it resets after every coin
        coin_value = coins[i]; // update current coin value
        for (j = coin_value; j <= x; j++)
        {

            tmp = ways[j - 1];

            ways[j] = ways[j - coin_value] + ways[j];

            if (tmp < ways[j]) // the amnt of ways we can have increased, so get those combination(s)
            {
                vector<int> tmpVec = vecVec.at(tmpval);
                tmpval++; // increment for the coin

                for (sum = 0; sum < coin_value;)
                {
                    sum = sum + tmpVec.back();
                    tmpVec.pop_back();
                }
                tmpVec.insert(tmpVec.begin(), coin_value);
                vecVec.push_back(tmpVec); // add it to our vec of vec
            }
        }
    }
    for (size_t ig = 0; ig < vecVec.size(); ig++)
    {
        cout << endl;
        cout << "Way " << ig + 1 << ": ";
        for (size_t ih = 0; ih < vecVec[ig].size(); ih++)
        {

            cout << vecVec.at(ig).at(ih) << " ";
        }
        cout << endl;
    }
    return ways[x];
}

// MAIN
int main(int argc, char const *argv[])
{
    //  PART 1 - ROD CUTTING
    //  QUESTION 1a
    //  NO DIRECT CHANGES TO CODE -- REPRESENT NO PRICES WITH 0 IN ARRAY

    int prices[] = {1, 5, 0, 9, 0, 17, 0, 20, 0, 30, 0, 36, 0, 0, 0, 48};
    int len = 16;
    int testArr[] = {4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096};
    cout << "Question 1a - Max revenue from rod length " << len << " is: " << sparcePriceCutRod(prices, len) << endl;

    // QUESTION 1b
    // Needed an added parameter - the size of the price list
    int sizeOfPrices = 16; // size of our prices array

    cout << "Four tests for rods of any length starting: " << endl;
    for (size_t i = 0; i < 11; i++) // little loop to test four rod lengths
    {
        len = testArr[i];
        cout << "Question 1b - Max revenue from rod length " << len << " is: ";
        auto start = chrono::high_resolution_clock::now(); // TIMING START
        int result = longSparcePriceCutRod(prices, len, sizeOfPrices);
        auto stop = chrono::high_resolution_clock::now(); // TIMING END
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        cout << result << " with a time of " << duration.count() << " microseconds" << endl;
    }
    // PART 2 - COIN CHANGE
    int coins[] = {1, 5, 10, 25, 50, 100, 200, 500, 1000, 2000};
    int wizCoins[] = {1, 29, 493};
    int amts[] = {10, 50, 100, 500, 1000, 1500, 2000, 3000, 5000};
    int amts2[] = {10, 25, 50, 100};
    int changeamt;
    double ways;

    cout << endl;
    cout << "Coin change" << endl;
    for (size_t i = 0; i < 9; i++)
    {
        cout << "With change: ";
        changeamt = amts[i];
        cout << changeamt << endl;
        cout << "The ways in U.S. currency is: ";
        auto start = chrono::high_resolution_clock::now(); // TIMING START
        ways = coinChange(changeamt, coins, 10);
        auto stop = chrono::high_resolution_clock::now(); // TIMING END
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        cout << ways << " with a time of " << duration.count() << " microseconds " << endl;

        // Test Wizard Currency Now
        cout << "The ways in wizard currency is: ";
        start = chrono::high_resolution_clock::now(); // TIMING START
        ways = coinChange(changeamt, wizCoins, 3);
        stop = chrono::high_resolution_clock::now(); // TIMING END
        duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        cout << ways << "with a time of " << duration.count() << " microseconds " << endl
             << endl;
    }

    cout << endl
         << endl
         << "Now for coin change + combos" << endl;
    for (size_t i = 0; i < 4; i++)
    {
        /* code */

        cout << "With change: ";
        changeamt = amts2[i];
        cout << changeamt << endl;
        cout << "The ways in U.S. currency is: ";
        auto start = chrono::high_resolution_clock::now(); // TIMING START
        ways = coinChangeCombos(changeamt, coins, 10);
        auto stop = chrono::high_resolution_clock::now(); // TIMING END
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        cout << " with a time of " << duration.count() << " microseconds " << endl
             << endl;

        // Test Wizard Currency Now
        cout << "Wizard currency: ";
        start = chrono::high_resolution_clock::now(); // TIMING START
        ways = coinChangeCombos(changeamt, wizCoins, 3);
        stop = chrono::high_resolution_clock::now(); // TIMING END
        duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        cout << "with a time of " << duration.count() << " microseconds " << endl
             << endl;
    }
    return 0;
} // main