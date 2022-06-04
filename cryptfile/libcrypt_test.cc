#include <gtest/gtest.h>
#include <dlfcn.h>
#include <string>
#include "cryptopp/aes.h"
using CryptoPP::AES;

TEST(LibcryptTest, BasicEncryptAndDecrypt)
{
    void *handle = dlopen("../../libcryptfile.so", RTLD_LAZY);
    ASSERT_TRUE(handle);

    typedef std::string (*CBCMode_Encrypt_t)(const std::string &text);

    dlerror();

    CBCMode_Encrypt_t cbcModeEncrypt = (CBCMode_Encrypt_t) dlsym(handle, "CBCMode_Encrypt");
    const char *dlsym_encrypt_err = dlerror();
    ASSERT_FALSE(dlsym_encrypt_err);

    std::string origin_text = "test_data";
    auto cipher = cbcModeEncrypt(origin_text);
    typedef std::string (*CBCMode_Decrypt_t)(const std::string &cipher);

    dlerror();

    CBCMode_Decrypt_t cbcModeDecrypt = (CBCMode_Decrypt_t) dlsym(handle, "CBCMode_Decrypt");
    const char *dlsym_decrypt_err = dlerror();
    ASSERT_FALSE(dlsym_decrypt_err);
    auto plain_text = cbcModeDecrypt(cipher);
    ASSERT_EQ(origin_text, plain_text
    );
    dlclose(handle);
}

