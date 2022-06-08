#include <gtest/gtest.h>
#include <dlfcn.h>
#include <string>
#include <mutex>
#include <utility>
#include "fileutils.h"

typedef std::string (*CBCMode_Encrypt_t)(const std::string &text);

typedef unsigned long (*CBCMode_Decrypt_t)(const char *cipher, char **plain, unsigned long size);

std::mutex lib_crypt_lock;

void *getHandler() {
    const std::lock_guard<std::mutex> lock(lib_crypt_lock);
    static void *handle = dlopen("../../libcryptfile.so", RTLD_LAZY);
    return handle;
}

void decryptCBC(const char *cipher, string &plain, unsigned long size) {
    auto handler = getHandler();
    if (!handler) {
        std::cout << "failed to load libcryptfile.so" << std::endl;
    }
    dlerror();
    auto cbcModeDecrypt = (CBCMode_Decrypt_t) dlsym(handler, "CBCMode_Decrypt");
    const char *dlsym_encrypt_err = dlerror();
    if (dlsym_encrypt_err) {
        std::cout << "failed to load CBCMode_Decrypt function symbol" << std::endl;
    }
    char *plain_char[1];
    unsigned long result_size = cbcModeDecrypt(cipher, plain_char, size);
    plain = string(plain_char[0], result_size);
}

CBCMode_Encrypt_t getEncryptFunc() {
    auto cbcModeEncrypt = (CBCMode_Encrypt_t) dlsym(getHandler(), "CBCMode_Encrypt");
    return cbcModeEncrypt;
}

TEST(LibcryptTest, BasicEncryptAndDecrypt) {
    auto cbcModeEncrypt = getEncryptFunc();
    const char *dlsym_encrypt_err = dlerror();
    ASSERT_FALSE(dlsym_encrypt_err);

    auto origin_text = FileUtils::read(
            "/Users/meijie/Work/serving/tensorflow_serving/servables/tensorflow/testdata/saved_model_half_plus_two_cpu/00000123/saved_model.pb");
    std::cout << origin_text << std::endl;
    auto cipher = cbcModeEncrypt(origin_text);
    string plain;
    decryptCBC(cipher.c_str(), plain, cipher.length());
    ASSERT_EQ(origin_text, plain);
}
