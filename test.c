#include<stdio.h>

#define u8 unsigned char
#define u32 unsigned long




void getRK(); // 轮密钥获取算法
void encryptSM4(); // 加密算法
void decryptSM4(); // 解密算法
void Work();

int main(void) {




	getRK();
	//Loop1:

	Work();


	//printf("%08x %08x %08x %08x\n", Y[0], Y[1], Y[2], Y[3]);
	//printf("************** 生成明文 *****************\n");
	//decryptSM4();
	//printf("%08x %08x %08x %08x\n", X[0], X[1], X[2], X[3]);
	return 0;
}
