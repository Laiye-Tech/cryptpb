#include "cryptopp/aes.h"

using CryptoPP::AES;

#include "cryptopp/modes.h"

using CryptoPP::CBC_Mode;

#include "functions.h"
#include <cxxopts.hpp>
#include "fileutils.h"
#include <chrono>
#include <iomanip>

using namespace std::chrono_literals;
using Clock = std::chrono::system_clock;

const byte iv[AES::BLOCKSIZE] = {0x37, 0x37, 0x37, 0x37,
                                 0x37, 0x37, 0x37, 0x37,
                                 0x37, 0x37, 0x37, 0x37,
                                 0x37, 0x37, 0x37, 0x37};
const int key_length = 16;

#ifndef LAIYE_MODEL_ENCRYPT_KEY
#define LAIYE_MODEL_ENCRYPT_KEY ""
#endif

// using CryptoPP::ArraySink;
string DO_CBCMode_Encrypt(const string &text, byte key[], int keySize) {
    string cipher;
    //Encryption
    try {
        CBC_Mode<AES>::Encryption e;
        e.SetKeyWithIV(key, keySize, iv);
        // The StreamTransformationFilter adds padding
        //  as required. ECB and CBC Mode must be padded
        //  to the block size of the cipher.
        StringSource(text, true, new StreamTransformationFilter(e, new StringSink(cipher))); // StringSource
    }
    catch (const CryptoPP::Exception &e) {
        cerr << e.what() << endl;
        exit(1);
    }
    return cipher;
}

extern "C" string CBCMode_Encrypt(const string &text) {
    byte *key = (byte *) LAIYE_MODEL_ENCRYPT_KEY;
    return DO_CBCMode_Encrypt(text, key, key_length);
}

void DO_CBCMode_Decrypt(const char *cipher, byte key[], int keySize, char **plain) {
    string plain_str;
    try {
        CBC_Mode<AES>::Decryption d;
        d.SetKeyWithIV(key, keySize, iv);
        // The StreamTransformationFilter removes
        //  padding as required.
        std::cout << "begin cipher " << std::endl;
        StringSource s(cipher, true,
                       new StreamTransformationFilter(d, new StringSink(plain_str))); // StringSource
    }
    catch (const CryptoPP::Exception &e) {
        cerr << e.what() << endl;
    }
    plain[0] = new char[plain_str.length() + 1];
    strcpy(plain[0], plain_str.c_str());
}

extern "C" void *CBCMode_Decrypt(const char *cipher, char **plain) {
    byte *key = (byte *) LAIYE_MODEL_ENCRYPT_KEY;
    DO_CBCMode_Decrypt(cipher, key, key_length, plain);

}

string tp2str(const Clock::time_point &tp) {
    std::time_t t_c = Clock::to_time_t(tp);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t_c), "%F %T");
    return ss.str();
}

Clock::time_point str2tp(const string &expired_date) {
    std::tm tm = {};
    std::stringstream ss(expired_date);
    ss >> std::get_time(&tm, "%y-%m-%d %H:%M:%S");
    return Clock::from_time_t(std::mktime(&tm));
}

void print_tp(const Clock::time_point &tp) {
    std::time_t t_c = Clock::to_time_t(tp);
    std::cout << std::put_time(std::localtime(&t_c), "%F %T") << '\n';
}

std::string transform(const std::string &input, const std::string &key) {
    std::string output(input);
    for (unsigned int i = 0; i < input.size(); i++)
        output[i] = input[i] ^ key[i % (key.size())];

    return output;
}

int main(int argc, char *argv[]) {
    string mod("enc");
    string input("tmp_input");
    string output("tmp_output");
    string key("key");

    cxxopts::Options options("cryptfile", "文件加解密");
    options.positional_help("[optional args]")
            .show_positional_help();
    options.add_options()
            ("m,mod", "mode: enc/dec", cxxopts::value<string>(mod))
            ("i,input", "input file name", cxxopts::value<string>(input))
            ("o,output", "output file name", cxxopts::value<string>(output))
            ("k, key", "key to enc/dec model", cxxopts::value<string>(key))
            ("help", "Print help");

    auto result = options.parse(argc, argv);
    if (result.count("help") || result.arguments().size() == 0) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    if (!FileUtils::file_exists(input)) {
        std::cout << "input file is not exists." << std::endl;
        exit(0);
    }


    byte *byte_key = (byte *) key.c_str();
    //Define the key and iv
    if (mod == "enc") {
        auto data = FileUtils::read(input);
        auto cipher = DO_CBCMode_Encrypt(data, byte_key, key_length);
        FileUtils::write(output, cipher);
        cout << "加密完成" << endl;
        exit(0);
    } else if (mod == "dec") {
        auto data = FileUtils::read(input);
        char *plain_char[1];
        DO_CBCMode_Decrypt(data.c_str(), byte_key, key_length, plain_char);
        string plain(plain_char[0]);
        if (plain == "") {
            cout << "加密失败" << std::endl;
            exit(1);
        }
        FileUtils::write(output, plain);
        cout << "解密完成" << endl;
        exit(0);
    } else {
        cout << "invalid mod" << endl;
    }

    return 0;
}