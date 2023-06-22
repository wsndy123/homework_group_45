
#define Openssl

#ifdef Openssl

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/ec.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <chrono>

using namespace std;

// 计时函数
template<typename Func, typename... Args>
void timing(Func func, Args&&... args)
{
    auto start = chrono::system_clock::now();
    func(args...);
    auto end = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "执行时间：" << duration.count() << " 微秒" << endl;
}


int sm4_enc(unsigned char* keyStr, unsigned char* surbuf, int surlen, unsigned char* enbuf);
int sm4_dec(unsigned char* sm4PriKey, unsigned char* cEnStr, int cEnstrlen, unsigned char* deStr);



int sm4_ecb_openssl(const char enc_before[], const char enc_after[])
{
    unsigned char sm4_en[512], sm4_de[512];
    int sm4enStrLen, sm4deStrLen;
    //密钥
    unsigned char keyStr[16] = { 0x15,0x67,0x28,0xe1,0x5f,0x9a,0xfc,0x01,0xd4,0xb6,0x1b,0x4e,0x44,0x5d,0xbb,0x26 };


    // 打开输入文件
    std::ifstream inFile(enc_before, ios::binary);
    std::ofstream outFile(enc_after);

    //// 每次读取16字节，并将其转换为16进制数存储到数组中
    char buffer[17] = { 0 }; // 用于存储每次读入的16个字节

    while (inFile.read(buffer, 16)) {
        //加密中
        sm4enStrLen = sm4_enc(keyStr, (unsigned char*)buffer, 17, sm4_en);

        outFile << sm4_en;//写入加密后的文件
    }
    printf("end\n");

    // 关闭文件
    inFile.close();
    outFile.close();

    return 0;
}
    
int main(){
    
    timing(sm4_ecb_openssl, "video.mp4", "enc2.txt");

}


int sm4_enc(unsigned char* keyStr, unsigned char* surbuf, int surlen, unsigned char* enbuf)
{
    unsigned char* out_buf = enbuf;
    int out_len;
    int out_padding_len;
    int i;
    unsigned char* iv=NULL;
    EVP_CIPHER_CTX* ctx;
    ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit(ctx, EVP_sm4_ecb(), keyStr, iv);
    if (0 == surlen % 16)
    {
        EVP_CIPHER_CTX_set_padding(ctx, 0);
    }
    out_len = 0;
    EVP_EncryptUpdate(ctx, out_buf, &out_len, surbuf, surlen);
    out_padding_len = 0;
    EVP_EncryptFinal(ctx, out_buf + out_len, &out_padding_len);

    EVP_CIPHER_CTX_free(ctx);
    return out_len + out_padding_len;
}


int sm4_dec(unsigned char* sm4PriKey, unsigned char* cEnStr, int cEnstrlen, unsigned char* deStr)
{
    unsigned char* iv=NULL;
    EVP_CIPHER_CTX* ctx;
    int len;
    int temlen;
    int deStrLen;
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        printf("EVP_CIPHER_CTX_new failed");
    }
    if (1 != EVP_DecryptInit(ctx, EVP_sm4_ecb(), sm4PriKey, iv)) {
        printf("EVP_DecryptInit_ex failed");
    }


    if (1 != EVP_DecryptUpdate(ctx, deStr, &len, cEnStr, cEnstrlen)) {
        printf("EVP_DecryptUpdate failed");
    }
    if (0 == len % 16)
    {
        EVP_CIPHER_CTX_set_padding(ctx, 0);
        len += 16;

    }
    if (!EVP_DecryptFinal(ctx, deStr + len, &temlen))
    {
        printf("EVP_DecryptFinal failed");
        return EXIT_FAILURE;
    }
    deStrLen = len + temlen;
    EVP_CIPHER_CTX_free(ctx);
    return deStrLen;
}

#endif