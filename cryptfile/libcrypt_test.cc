#include <gtest/gtest.h>
#include <dlfcn.h>
#include <string>
#include "cryptfile/cryptopp/include/cryptopp/config.h"
#include "cryptopp/aes.h"

using CryptoPP::AES;

TEST(LibcryptTest, BasicEncryptAndDecrypt) {
    void *handle = dlopen("../../libcryptfile.so", RTLD_LAZY);
    ASSERT_TRUE(handle);
    typedef std::string (*CBCMode_Encrypt_t)(const std::string &text, byte key[], int keySize, byte kv[]);
    dlerror();
    CBCMode_Encrypt_t cbcModeEncrypt = (CBCMode_Encrypt_t) dlsym(handle, "CBCMode_Encrypt");
    const char *dlsym_encrypt_err = dlerror();
    ASSERT_FALSE(dlsym_encrypt_err);

    byte key[AES::DEFAULT_KEYLENGTH] = {0x8b, 0x8b, 0x8b, 0x8b,
                                        0x8b, 0x8b, 0x8b, 0x8b,
                                        0x8b, 0x8b, 0x8b, 0x8b,
                                        0x8b, 0x8b, 0x8b, 0x8b};

    byte iv[AES::BLOCKSIZE] = {0x37, 0x37, 0x37, 0x37,
                               0x37, 0x37, 0x37, 0x37,
                               0x37, 0x37, 0x37, 0x37,
                               0x37, 0x37, 0x37, 0x37};

    std::string origin_text = "test_data";
    auto cipher = cbcModeEncrypt(origin_text, key, sizeof(key), iv);

    typedef std::string  (*CBCMode_Decrypt_t)(const std::string &cipher, byte key[], int keySize, byte iv[]);
    dlerror();
    CBCMode_Decrypt_t cbcModeDecrypt = (CBCMode_Decrypt_t) dlsym(handle, "CBCMode_Decrypt");
    const char *dlsym_decrypt_err = dlerror();
    ASSERT_FALSE(dlsym_decrypt_err);
    auto plain_text = cbcModeDecrypt(cipher, key, sizeof(key), iv);
    ASSERT_EQ(origin_text, plain_text);
    dlclose(handle);
}
