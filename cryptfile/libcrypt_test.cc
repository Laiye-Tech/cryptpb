#include <gtest/gtest.h>
#include <dlfcn.h>
#include <string>

typedef std::string (*CBCMode_Encrypt_t)(const std::string &text);

typedef void (*CBCMode_Decrypt_t)(const std::string &cipher, std::string &plain);

std::mutex lib_crypt_lock;

void *getHandler() {
    const std::lock_guard<std::mutex> lock(lib_crypt_lock);
    static void *handle = dlopen("../../libcryptfile.so", RTLD_LAZY);
    return handle;
}

void decryptCBC(std::string cipher, std::string &plain) {
    auto handler = getHandler();
    if (!handler) {
        std::cout<< "failed to load libcryptfile.so" << std::endl;
    }
    dlerror();
    auto cbcModeDecrypt = (CBCMode_Decrypt_t) dlsym(handler, "CBCMode_Decrypt");
    const char *dlsym_encrypt_err = dlerror();
    if (dlsym_encrypt_err) {
        std::cout << "failed to load CBCMode_Decrypt function symbol" << std::endl;
    }
    cbcModeDecrypt(cipher, plain);
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

    std::string plain_text;
    decryptCBC(cipher, plain_text);
    ASSERT_EQ(origin_text, plain_text);

    cbcModeEncrypt = getEncryptFunc();
    cbcModeEncrypt(origin_text);

}
