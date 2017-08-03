#include <Arduino.h>
#include <AES.h>
#include <ebase64.h>

// helper method for printing array of bytes
void printArray(String name, byte *arr, int length)
{
    Serial.print(name + ": ");
    for (int i = 0; i < length; i++)
    {
        Serial.write(arr[i]);
    }
    Serial.println();
}

void testAES128()
{
    // password used for encypt/decrypt
    byte key[] = {'m', 'y', 's', 'e', 'c', 'r', 'e', 't', 'p', 'a', 's', 's', 'w', 'o', 'r', 'd'};
    // message
    String msg = "{\"temp\":\"28.5\", \"id\":\"1\" }";

    AES aes;

    // init vector, for now just constant
    byte iv[N_BLOCK] = {65, 66, 67, 68, 65, 66, 67, 68, 65, 66, 67, 68, 65, 66, 67, 68};
    // plain message in array of bytes
    byte plain[200];
    // encrypted message
    byte cipher[200];

    // BASE64 encoded data, which are going to be transported to server/storage
    // those are used in nodejs decrypting example
    char ivb64[200];
    char cipherb64[200];

    // set password
    aes.set_key(key, sizeof(key));

    // transform string to byte[]
    msg.getBytes(plain, sizeof(plain));
    printArray("Plain message", plain, msg.length());

    // BASE64 encode init vector
    byte ivb64len = base64_encode(ivb64, (char *)iv, N_BLOCK);
    printArray("IV", iv, 16);
    Serial.println("IV in B64: " + String(ivb64));

    // encrypt message with AES128 CBC pkcs7 padding with key and IV
    aes.do_aes_encrypt(plain, strlen((char *)plain), cipher, key, 128, iv);
    printArray("Encrypted message", cipher, aes.get_size());
    Serial.println("Encrypted message size: " + String(aes.get_size()));

    // BASE64 encode ciphered message
    byte cipherb64len = base64_encode(cipherb64, (char *)cipher, aes.get_size());
    Serial.println("Encrypted message in B64: " + String(cipherb64));

    aes.clean();

    // decode IV from BASE64
    base64_decode((char *)iv, ivb64, ivb64len);
    printArray("IV B64-decoded", iv, 16);

    // decrypt message with AES128 CBC pkcs7 padding with key and IV
    aes.do_aes_decrypt(cipher, aes.get_size(), plain, key, 128, iv);
    printArray("Decrypted message", plain, msg.length());

    Serial.println();
    Serial.println();
}

/********************************************************************/
void setup(void)
{
    Serial.begin(9600);
}
void loop(void)
{
    testAES128();
    delay(10000);
}