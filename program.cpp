#include <iostream>
#include <string>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/modes.h>

using namespace CryptoPP;

std::string EncryptString(const std::string& plain, const std::string& key, const std::string& iv) {
    std::string cipher;

    try {
        AES::Encryption aesEncryption((byte *)key.c_str(), AES::DEFAULT_KEYLENGTH);
        CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, (byte *)iv.c_str());

        StreamTransformationFilter stfEncryptor(cbcEncryption, new StringSink(cipher));
        stfEncryptor.Put(reinterpret_cast<const unsigned char*>(plain.c_str()), plain.length() + 1);
        stfEncryptor.MessageEnd();
    }
    catch (const Exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    return cipher;
}

std::string DecryptString(const std::string& cipher, const std::string& key, const std::string& iv) {
    std::string recovered;

    try {
        AES::Decryption aesDecryption((byte *)key.c_str(), AES::DEFAULT_KEYLENGTH);
        CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, (byte *)iv.c_str());

        StreamTransformationFilter stfDecryptor(cbcDecryption, new StringSink(recovered));
        stfDecryptor.Put(reinterpret_cast<const unsigned char*>(cipher.c_str()), cipher.size());
        stfDecryptor.MessageEnd();
    }
    catch (const Exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    return recovered;
}

int main() {
    std::string key = "0123456789abcdef"; // 16 bytes for AES128
    std::string iv = "abcdef9876543210"; // AES block size in bytes

    std::string plain = "This is a test";
    std::string cipher = EncryptString(plain, key, iv);
    std::string decoded = DecryptString(cipher, key, iv);

    std::cout << "Cipher (Hex): ";
    StringSource ss1(cipher, true, new HexEncoder(new FileSink(std::cout)));
    std::cout << std::endl << "Decoded: " << decoded << std::endl;

    return 0;
}
