#include "functions-header.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string>
using std::string;

#include <cstdlib>
using std::exit;

#include "cryptopp/cryptlib.h"
using CryptoPP::Exception;

#include "cryptopp/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "cryptopp/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::HashVerificationFilter;

#include "cryptopp/secblock.h"
using CryptoPP::SecByteBlock;


#include "cryptopp/sha.h"
using CryptoPP::SHA1;
using CryptoPP::HashFilter;

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptopp/md5.h"

#include "cryptopp/hmac.h"
using CryptoPP::HMAC;

string sha1_digest(std::string text){
    string encoded;
    SHA1 sha1;
    encoded.clear();
    StringSource s1(text, true, new HashFilter(sha1, new HexEncoder(new StringSink(encoded))));
    return encoded;
}
string md5(std::string plain) {
    byte digest[ CryptoPP::Weak::MD5::DIGESTSIZE ];
    
    CryptoPP::Weak::MD5 hash;
    hash.CalculateDigest( digest, (const byte*)plain.c_str(), plain.length() );
    
    CryptoPP::HexEncoder encoder;
    string output;
    
    encoder.Attach( new CryptoPP::StringSink( output ) );
    encoder.Put( digest, sizeof(digest) );
    encoder.MessageEnd();
    
    return output;
}
string PrettyPrint(byte arr[], int arraySize) {
    string encoded = "";
    encoded.clear();
    StringSource(arr, arraySize, true, new HexEncoder(new StringSink(encoded)));
    return encoded;
}
string PrettyPrint(string text) {
    string encoded = "";
    encoded.clear();
    StringSource(text, true, new HexEncoder(new StringSink(encoded)));
    return encoded;
}
string HMAC_SHA_1(string text, byte key[], int keySize) {
    string mac = "";
    try
    {
        HMAC< SHA1 > hmac(key, keySize);
        StringSource(text, true, new HashFilter(hmac, new StringSink(mac)));
    }
    catch(const CryptoPP::Exception& e)
    {
        cerr << e.what() << endl;
        exit(1);
    }
    return PrettyPrint(mac);
                     
}