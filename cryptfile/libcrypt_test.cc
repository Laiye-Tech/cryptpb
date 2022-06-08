#include <gtest/gtest.h>
#include <dlfcn.h>
#include <string>
#include <mutex>
#include <utility>

typedef std::string (*CBCMode_Encrypt_t)(const std::string &text);

typedef void(*CBCMode_Decrypt_t)(const char *cipher, char **plain);

std::mutex lib_crypt_lock;

void *getHandler() {
    const std::lock_guard<std::mutex> lock(lib_crypt_lock);
    static void *handle = dlopen("../../libcryptfile.so", RTLD_LAZY);
    return handle;
}

void decryptCBC(const char *cipher, char **plain) {
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
    return cbcModeDecrypt(cipher, plain);
}

CBCMode_Encrypt_t getEncryptFunc() {
    auto cbcModeEncrypt = (CBCMode_Encrypt_t) dlsym(getHandler(), "CBCMode_Encrypt");
    return cbcModeEncrypt;
}

TEST(LibcryptTest, BasicEncryptAndDecrypt) {
    auto cbcModeEncrypt = getEncryptFunc();
    const char *dlsym_encrypt_err = dlerror();
    ASSERT_FALSE(dlsym_encrypt_err);

    std::string origin_text = "test_data";
    auto cipher = cbcModeEncrypt(origin_text);

    char *plain_text[1];
    decryptCBC(cipher.c_str(), plain_text);
    std::string s(plain_text[0]);
    ASSERT_EQ(origin_text, s);

    cbcModeEncrypt = getEncryptFunc();
    cbcModeEncrypt(origin_text);

}
