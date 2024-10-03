#pragma once

#include <iostream>

#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/rand.h>


#include <limits>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

#include <fstream>
#include <cstring>


std::vector<unsigned char> encrypt(const std::string& plaintext, const unsigned char* key, const unsigned char* iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    std::vector<unsigned char> ciphertext(plaintext.size() + AES_BLOCK_SIZE);
    int len;

    EVP_EncryptUpdate(ctx, ciphertext.data(), &len, reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.size());
    int ciphertext_len = len;

    EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len);
    ciphertext_len += len;
    ciphertext.resize(ciphertext_len);

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext;
};

// Decryption Function
std::string decrypt(const std::vector<unsigned char>& ciphertext, const unsigned char* key, const unsigned char* iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);

    std::vector<unsigned char> plaintext(ciphertext.size());
    int len;

    EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size());
    int plaintext_len = len;

    EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len);
    plaintext_len += len;
    plaintext.resize(plaintext_len);

    EVP_CIPHER_CTX_free(ctx);

    return std::string(plaintext.begin(), plaintext.end());
};
std::string toHex(const std::vector<unsigned char>& data) {
    std::ostringstream oss;
    for (unsigned char c : data) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    }
    return oss.str();
}
std::vector<unsigned char> fromHex(const std::string& hexStr) {
    std::vector<unsigned char> bytes;
    for (size_t i = 0; i < hexStr.length(); i += 2) {
        std::string byteString = hexStr.substr(i, 2);
        unsigned char byte = (unsigned char) strtol(byteString.c_str(), nullptr, 16);
        bytes.push_back(byte);
    }
    return bytes;
}

// Function to load the keys from PEM-like file
void read_key_from_pem(unsigned char* key, unsigned char* iv) {
    std::ifstream pem_file("keys.pem");
    std::string line;
    std::string key_str, iv_str;

    // Read through the file and extract the key and IV
    while (std::getline(pem_file, line)) {
        if (line == "-----BEGIN AES-256 KEY-----") {
            std::getline(pem_file, key_str);  // Read key line
        } else if (line == "-----BEGIN AES-128 IV-----") {
            std::getline(pem_file, iv_str);   // Read IV line
        }
    }
    pem_file.close();

    // Copy the key and IV from strings into the unsigned char arrays
    memcpy(key, key_str.c_str(), 32);  // AES-256 key is 32 bytes
    memcpy(iv, iv_str.c_str(), 16);    // AES-128 IV is 16 bytes
}
