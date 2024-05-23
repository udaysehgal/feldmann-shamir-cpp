#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <random>
#include <set>
#include <bits/stdc++.h>

using namespace std;

typedef struct Share
{
    int y;
    int x;
    vector<int> coeffecients;
} Share;

int q = 127; // use in main
bool isPrime(int n)
{
    if (n <= 1)
        return false;
    for (int i = 2; i <= sqrt(n); i++)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}

// Function to generate a random prime number p (q is fixed at 127)
int generate_p()
{
    int p = rand() % 100000 + 127;
    while (!isPrime(p) || (p - 1) % 127 != 0)
    {
        p = rand() % 100000 + 127;
    }
    cout << "final value of p is " << p << endl;
    return p;
}

int generate_r(int p, int q)
{
    return (p - 1) / q;
}
int modular(int base, unsigned int exp, unsigned int mod)
{
    int x = 1;
    int power = base % mod;

    for (int i = 0; i < sizeof(int) * 8; i++)
    {
        int least_sig_bit = 0x00000001 & (exp >> i);
        if (least_sig_bit)
            x = (x * power) % mod;
        power = (power * power) % mod;
    }

    return x;
}
vector<int> computeG(int p, int q)
{
    vector<int> group(p - 1);
    for (int i = 0; i < p - 1; i++)
    {
        group[i] = i + 1;
    }

    vector<int> subGroup;
    for (auto it = group.begin(); it != group.end(); it++)
    {
        int x = *it;
        int y = (int)(modular(x, (unsigned int)q, (unsigned int)p));
        if (y == 1)
        {
            subGroup.push_back(x);
        }
    }
    return subGroup;
}

int generateRandomNumber(int q)
{
    std::mt19937 generator(time(0));
    std::uniform_int_distribution<int> distribution(1, q - 1);
    return distribution(generator);
}

// Function to evaluate a polynomial at a given x
int evaluate_polynomial(vector<int> coefficients, int x, int p, int q)
{
    int result = 0;
    int power = 1;
    for (int i = 0; i < coefficients.size(); i++)
    {
        result = (result + (coefficients[i] * power) % p) % p;
        power = (power * x) % p;
    }
    return result % q;
}

vector<Share> generate_shares(vector<int> secret, int p)
{
    int n = 4, k = 2;
    vector<int> coefficients;
    vector<Share> shares;
    coefficients.push_back(secret[0]);

    // Generate random coefficients for the polynomial
    for (int i = 0; i < k - 1; i++)
    {
        int r = generateRandomNumber(q);
        coefficients.push_back(r);
        cout << "printing coeff " << r << endl;
    }

    // Generate shares for each byte of the secret
    for (int i = 0; i < n; i++)
    {
        Share share;
        share.x = i + 1;
        share.y = evaluate_polynomial(coefficients, share.x, p, q) % q;
        shares.push_back(share);
        cout << "shares " << shares[i].x << " " << shares[i].y << endl;
    }
    return shares;
}

vector<int> reconstruct(const vector<pair<int, int>> &values, int mod)
{
    int n = values.size();
    vector<int> result(2);
    for (int i = 0; i <= 1; i++)
    {
        int numerator = 1;
        int denominator = 1;
        for (int j = 0; j < n; j++)
        {
            if (j == i)
                continue;
            int diff = values[j].first - values[i].first;
            numerator = (numerator * diff) % mod;
            denominator = (denominator * (values[i].first - values[j].first)) % mod;
        }
        result[i] = (values[i].second * numerator * modular(denominator, mod - 2, mod)) % mod;
        cout << "result elements are " << result[i];
    }
    return result;
}

int main()
{
    int p = generate_p();
    cout << p << endl;
    vector<int> inputArr(32);
    inputArr[0] = 3756;
    for (int i = 1; i < inputArr.size(); i++)
    {
        inputArr[i] = i;
    }

    vector<int> secret;
    int a, b;

    for (int i = 0; i < 32; i++)
    {
        secret.push_back(inputArr[i]);
    }

    // Read the two shares to use to recover the secret
    cin >> a >> b;
    a--;
    b--;

    int r = generate_r(p, 127);
    cout << "r is" << r;

    vector<int> res1;
    cout << "computing G";
    res1 = computeG(p, q);

    int rand = res1[0]; // make this a random selection from res1

    vector<Share> shares;
    shares = generate_shares(secret, p);
    cout << "shares generated";

    vector<pair<int, int>> values(2);
    values[0].first = shares[a].x;
    values[0].second = shares[a].y;
    values[0].first = shares[b].y;
    values[0].second = shares[b].y;

    vector<int> recovered;
    recovered = reconstruct(values, q);
    cout << recovered[0] << endl;
}
