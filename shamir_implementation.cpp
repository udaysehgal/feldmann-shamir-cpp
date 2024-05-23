#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <random>

using namespace std;

typedef struct Share
{
    int y;
    vector<int> coefficients;
    int x;
} Share;

// Function to check if a number is prime or not
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

int generate_random(int p)
{
    return rand() % p;
}

// Function to evaluate a polynomial at a given x
int evaluate_polynomial(vector<int> coefficients, int x)
{
    int result = coefficients[0];
    for (int i = 1; i < coefficients.size(); i++)
    {
        result = (result + coefficients[i] * (int)pow(x, i));
    }
    return result;
}

// Function to generate shares for a secret
vector<Share> generate_shares(vector<int> secret, int p)
{
    int n = 4, k = 2;
    vector<int> coefficients;
    vector<Share> shares;

    coefficients.push_back(secret[0]);
    // Generate random coefficients for the polynomial
    for (int i = 0; i < k - 1; i++)
    {
        coefficients.push_back(generate_random(p));
    }

    // Generate shares for each byte of the secret
    for (int i = 0; i < n; i++)
    {
        Share share;
        share.x = i + 1;
        share.y = evaluate_polynomial(coefficients, share.x);
        // share.coefficients = coefficients;
        shares.push_back(share);
        cout << "shares" << shares[i].x << " " << shares[i].y << endl;
    }
    return shares;
}

// Function to recover the secret from given shares
int recover_secret(vector<Share> shares, int a, int b)
{
    int x1 = shares[a].x;
    int y1 = shares[a].y;
    int x2 = shares[b].x;
    int y2 = shares[b].y;
    double result = ((0 - x2) * y1 + (x1 - 0) * y2) / (x1 - x2);

    return result;
}

/*vector<int> recover_secret(vector<Share> shares, int a, int b, int p)
{
    int n = 4, k = 2;
    int num_bytes = shares.size() / n;
    vector<int> secret;

    vector<int> x_values, y_values;
    for (int i = 0; i < shares.size(); i++)
    {
        x_values.push_back(shares[i].x);
        y_values.push_back(shares[i].y);
    }

    for (int j = 0; j < num_bytes; j++)
    {
        int offset = j * n;
        int x_a = shares[offset + a].x;
        int y_a = shares[offset + a].y;
        int x_b = shares[offset + b].x;
        int y_b = shares[offset + b].y;

        int numerator = (y_b - y_a + p) % p;
        int denominator = (x_a - x_b + p) % p;

        int denominator_inv = 1;
        for (int i = 0; i < p - 2; i++)
        {
            denominator_inv = (denominator_inv * denominator) % p;
        }

        int byte = (numerator * denominator_inv) % p;
        secret.push_back(byte);
    }
    return secret;
}
*/

int main()
{
    int p = generate_p();
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
        // cin >> inputArr[i];
        secret.push_back(inputArr[i]);
    }

    // Read the two shares to use to recover the secret
    cin >> a >> b;
    a--;
    b--;

    vector<Share> shares = generate_shares(secret, p);

    // Verify if the shares are correct
    /*for (int i = 0; i < 4; i++)
    {
        if (evaluate_polynomial(shares[i].coefficients, shares[i].x) != shares[i].y)
        {
            cout << "ERROR" << endl;
            return 0;
        }
    }
    */

    // Recover secret from the shares
    int recovered_secret = recover_secret(shares, a, b);
    cout << "this is the recovered secret:"
         << " " << recovered_secret << endl;

    for (int i = 0; i < 4; i++)
    {
        cout << shares[i].y << " ";
        for (int j = 0; j < shares[i].coefficients.size(); j++)
        {
            cout << shares[i].coefficients[j] << " ";
        }
        cout << shares[i].x << endl;
    }

    return 0;
}
