#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

typedef unsigned long long bigNum;

bigNum eulerFunction(bigNum p, bigNum q) {
    return (p - 1) * (q - 1);
}

// finds primes from 0 to n (not used in the release)
vector<bool> primes(int n) {
    vector<bool> primes(n + 1);
    fill(primes.begin(), primes.end(), true);
    
    for (int i = 2; i <= n; i++)
        if (primes[i] == true)
            for (int j = i * i; j <= n; j += i)
                primes[j] = false;

    return primes;
}

// Euclid's algorithm for finding gcd
bigNum gcd(bigNum a, bigNum b) {
    return b ? gcd (b, a % b) : a;
}

bigNum randE(bigNum phi) {
    vector<bool> coprimes(phi + 1);
    fill(coprimes.begin(), coprimes.end(), false);
    
    for (int i = 0; i <= phi; i++)
        if (gcd(i, phi) == 1)
            coprimes[i] = true;
    
    bigNum e = 0;
    while(true) {
        e = rand() % phi;
        if (coprimes[e])
            break;
    }
    
    return e;
}

bigNum calcD(bigNum phi, bigNum e) {
    bigNum k = 0;
    while(((k * phi + 1) % e) != 0)
        k++;
    
    return (k * phi + 1) / e;
}

// special algorithm for working with large numbers
bigNum func(bigNum symbol, bigNum factor, bigNum n) {
    bigNum result = 1;
    while (factor) {
        if (factor % 2)
            result = (result * symbol) % n;
        symbol = (symbol * symbol) % n;
        factor /= 2;
    }
    return result;
}

int main() {
    // choose two primes
    bigNum p, q;
    cout << "Enter two primes: ";
    cin >> p >> q;
    
    // calculate the product
    bigNum n = p * q;
    
    // calculate the Euler function
    bigNum phi = eulerFunction(p, q);
    
    // choose an arbitrary coprime number with phi (0 < e < phi) for the public key
    bigNum e = randE(phi);
    
    // calculate the number d for the private key
    bigNum d = calcD(phi, e);
    
    string data;
    cout << "Enter data: ";
    cin >> data;
    
    cout << "\nInitial data: ";
    for (auto symbol : data)
        cout << (bigNum) symbol << " ";
    cout << endl;
    
    vector<bigNum> encryptedData, decryptedData;
    
    cout << "Encrypted data: ";
    for (bigNum symbol : data) {
        encryptedData.push_back(func(symbol, e, n));
        cout << encryptedData.back() << " ";
    }
    cout << endl;
    
    cout << "Decrypted data: ";
    for (bigNum symbol : encryptedData) {
        decryptedData.push_back(func(symbol, d, n));
        cout << decryptedData.back() << " ";
    }
    cout << "\n\n";
    
    return 0;
}
