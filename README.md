this is an implementation of the feldmann verifiable secret sharing algorithm in c++. It generates a random prime number p and computes a subgroup of integers modulo p. Using a secret polynomial, it generates and evaluates shares of a secret for distribution. The generate_shares function creates shares from the secret using random coefficients.