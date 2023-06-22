#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdio.h>

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


#define u8 unsigned char
#define u32 unsigned long
void getRK(u32 MK[], u32 K[], u32 RK[]); // 轮密钥获取算法
void encryptSM4(u32 X[], u32 RK[], u32 Y[]); // 加密算法
void decryptSM4(u32 X[], u32 RK[], u32 Y[]); // 解密算法

bool hexToDec(const char hex[32], u32 dec[4]);
bool decToHex(u32 dec[4], char hex[33]);

//#define test_1
//#define test_2

#ifdef test_1

int sm4_ecb(const char enc_before[], const char enc_after[]) {

	u32 X[4]; // 明文
	u32 MK[4]; // 密钥
	u32 RK[32]; // 轮密钥
	u32 K[4]; // 中间数据
	u32 Y[4]; // 密文
	short i; // 临时变量

	MK[0] = 0x01234657;
	MK[1] = 0x89abcdef;
	MK[2] = 0xfedcba98;
	MK[3] = 0x76543210;
	
	getRK(MK, K, RK);
	
	// 打开txt文件
	std::ifstream inFile(enc_before);
	std::ofstream outFile(enc_after);
	// 读取8个字符，并存储到字符串或char数组中
	int count = 0;

	char buffer[33] = { 0 };
	while (inFile.read(buffer, 32)) {
		hexToDec(buffer, X);//把读取的16进制文件转换为sm4输入的u32[4]
		
		//加密
		encryptSM4(X, RK, Y);

		char M1[33] = { 0 };
		decToHex(Y, M1);//把sm4加密输出的u32[4]数转换为16进制数

		outFile << M1;//写入加密后的文件
		
		
	}
	printf("end\n");
	inFile.close();
	outFile.close();
	return 0;
}


int main() {

	timing(sm4_ecb,"video.txt", "enc.txt");

	return 0;
}

#endif


#ifdef test_2
int main(void) {

	//return 0;
	u32 X[4]; // 明文
	u32 MK[4]; // 密钥
	u32 RK[32]; // 轮密钥
	u32 K[4]; // 中间数据
	u32 Y[4]; // 密文
	short i; // 临时变量

	MK[0] = 0x01234657;
	MK[1] = 0x89abcdef;
	MK[2] = 0xfedcba98;
	MK[3] = 0x76543210;

	getRK(MK, K, RK);

	char s[33] = "EFE5A6305C139D9814C5656D2FC40988";

	printf("待加密的32字节字符串：%s\n", s);
	hexToDec(s, X);//把读取的16进制文件转换为sm4输入的u32[4]

	//加密
	encryptSM4(X, RK, Y);

	char M[33] = { 0 };
	decToHex(Y, M);//把sm4加密输出的u32[4]数转换为16进制数
	printf("加密后的32字节字符串：%s\n", M);

	//解密

	decryptSM4(Y, RK, X);
	decToHex(X, M);//把sm4解密输出的u32[4]数转换为16进制数
	printf("解密后的32字节字符串：%s\n\n", M);
	printf("end\n");

	return 0;
}

#endif



bool hexToDec(const char hex[32], u32 dec[4]) {
	char c;
	for(int j = 0; j < 4; j++){
		for (int i = 0; i < 8; i++) {
			c = hex[i+j*8];
			if (c >= '0' && c <= '9') {
				dec[j] = dec[j] * 16 + (c - '0');
			}
			else if (c >= 'a' && c <= 'f') {
				dec[j] = dec[j] * 16 + (c - 'a' + 10);
			}
			else if (c >= 'A' && c <= 'F') {
				dec[j] = dec[j] * 16 + (c - 'A' + 10);
			}
			else {
				// 如果字符串中包含非16进制字符，则返回0
				return false;
			}
		}
	}
	return true;
}

bool decToHex(u32 dec[4], char hex[33]) {
	u32 d, r;
    for (int i = 0; i < 4; i++) {
		d = dec[i];
		for (int j = 7; j >= 0; j--) {
			r = d % 16;
			d = d / 16;
			if (r < 10) {
				hex[i * 8 + j] = r + '0';
			}
			else {
				hex[i * 8 + j] = r + 'a' - 10;
			}
		}
    }
    hex[32] = '\0'; // 给hex尾部添加空字符
    return true;
}


//int main() {
//	char s[33] = "00000001111fec0011011d01111fec00";
//	u32 m[4];
//	hexToDec(s, m);
//	printf("%d %d %d %d\n", m[0], m[1], m[2], m[3]);
//	decToHex(m, s);
//	printf("%s", s);
//}