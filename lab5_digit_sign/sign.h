#ifndef CREATE_X509_SIGN_H
#define CREATE_X509_SIGN_H


#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <fstream>
#include <vector>

typedef unsigned char byte;

#define UNUSED(x) ((void)(x))

const char hn[] = "SHA256";

int make_keys(EVP_PKEY **skey, EVP_PKEY **vkey);
int sign(const byte *msg, size_t mlen, byte **sig, size_t *slen, EVP_PKEY *pkey);
int verify(const byte *msg, size_t mlen, const byte *sig, size_t slen, EVP_PKEY *pkey);
void print_labeled(const char *label, const byte *buff, size_t len);
void write_keys(EVP_PKEY *public_k, EVP_PKEY *private_k);
void read_keys(EVP_PKEY **public_k, EVP_PKEY **private_k);
void write_sign(byte *sign, size_t len, char *filename);
size_t read_sign(byte **sign, char *filename);

#endif //CREATE_X509_SIGN_H
