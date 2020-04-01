#include "cryptopp/aes.h"
using CryptoPP::AES;

#include "cryptopp/modes.h"
using CryptoPP::CBC_Mode;

#include "functions.h"
#include <cxxopts.hpp>
#include "fileutils.h"
#include <chrono>
#include <iomanip>
// #include <boost/algorithm/hex.hpp>
// using namespace boost::algorithm;

using namespace std::chrono_literals;
using Clock = std::chrono::system_clock;

// using CryptoPP::ArraySink;

string CBCMode_Encrypt(const string &text, byte key[], int keySize, byte iv[])
{
    string cipher = "";
    //Encryption
    try
    {
        CBC_Mode<AES>::Encryption e;
        e.SetKeyWithIV(key, keySize, iv);
        // The StreamTransformationFilter adds padding
        //  as required. ECB and CBC Mode must be padded
        //  to the block size of the cipher.
        StringSource(text, true, new StreamTransformationFilter(e, new StringSink(cipher))); // StringSource
    }
    catch (const CryptoPP::Exception &e)
    {
        cerr << e.what() << endl;
        exit(1);
    }
    return cipher;
}
string CBCMode_Decrypt(const string &cipher, byte key[], int keySize, byte iv[])
{
    string recovered = "";
    //Decryption
    try
    {
        CBC_Mode<AES>::Decryption d;
        d.SetKeyWithIV(key, keySize, iv);
        // The StreamTransformationFilter removes
        //  padding as required.
        StringSource s(cipher, true, new StreamTransformationFilter(d, new StringSink(recovered))); // StringSource
    }
    catch (const CryptoPP::Exception &e)
    {
        cerr << e.what() << endl;
        exit(1);
    }
    return recovered;
}

string tp2str(const Clock::time_point &tp)
{
    std::time_t t_c = Clock::to_time_t(tp);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t_c), "%F %T");
    return ss.str();
}

Clock::time_point str2tp(const string &expired_date)
{
    std::tm tm = {};
    std::stringstream ss(expired_date);
    ss >> std::get_time(&tm, "%y-%m-%d %H:%M:%S");
    return Clock::from_time_t(std::mktime(&tm));
}

void print_tp(const Clock::time_point &tp)
{
    std::time_t t_c = Clock::to_time_t(tp);
    std::cout << std::put_time(std::localtime(&t_c), "%F %T") << '\n';
}

std::string transform(const std::string &input, const std::string &key)
{
    std::string output(input);
    for (unsigned int i = 0; i < input.size(); i++)
        output[i] = input[i] ^ key[i % (key.size())];

    return output;
}

int main(int argc, char *argv[])
{
    string mod("enc");
    string input("tmp_input");
    string output("tmp_output");

    cxxopts::Options options("cryptfile", "文件加解密");
    options.positional_help("[optional args]")
        .show_positional_help();
    options.add_options()
        ("m,mod", "mode: enc/dec", cxxopts::value<string>(mod))
        ("i,input", "input file name", cxxopts::value<string>(input))
        ("o,output", "output file name", cxxopts::value<string>(output))
        ("help", "Print help");
    auto result = options.parse(argc, argv);
    if (result.count("help") || result.arguments().size() == 0)
    {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    if (!FileUtils::file_exists(input))
    {
        std::cout << "input file is not exists." << std::endl;
        exit(0);
    }

    //Define the key and iv
    byte key[AES::DEFAULT_KEYLENGTH] = {0x8b, 0x8b, 0x8b, 0x8b, 0x8b, 0x8b, 0x8b, 0x8b, 0x8b, 0x8b, 0x8b, 0x8b, 0x8b, 0x8b, 0x8b, 0x8b};
    byte iv[AES::BLOCKSIZE] = {0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37, 0x37};

    if (mod == "enc")
    {
        auto data = FileUtils::read(input);
        auto cipher = CBCMode_Encrypt(data, key, sizeof(key), iv);
        FileUtils::write(output, cipher);
        cout << "加密完成" << endl;
        exit(0);
    }
    else if (mod == "dec")
    {
        auto data = FileUtils::read(input);
        auto plain = CBCMode_Decrypt(data, key, sizeof(key), iv);
        FileUtils::write(output, plain);
        cout << "解密完成" << endl;
        exit(0);
    }
    else
    {
        cout << "invalid mod" << endl;
    }

    return 0;
}