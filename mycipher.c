#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

unsigned short getbit(unsigned short bits, int index);
int getshiftp10(int index);
unsigned short circleftshift5bits(unsigned short bits);
unsigned short getleft5bits(unsigned short bits);
unsigned short getright5bits(unsigned short bits);
unsigned short combine5bits(unsigned short bitsleft, unsigned short bitsright);
unsigned short getinkey(char * binary, int size);
unsigned short permute(unsigned short bits, int arr[], int size, int shift10);
int getshift8(int index);
int getshift4(int index);
unsigned short getF(unsigned short bits, unsigned short sk);
unsigned short s0(unsigned short bits, unsigned short a[][4]);
unsigned short getleft4bits(unsigned short bits);
unsigned short getright4bits(unsigned short bits);
unsigned short getfk(unsigned short leftbits, unsigned short rightbits, unsigned short sk);
unsigned short switch8bits(unsigned short bits);
unsigned char encrypt(unsigned char text, unsigned char iv, unsigned short key1,unsigned short key2);
unsigned char decrypt(unsigned char cipher, unsigned char iv, unsigned short key1, unsigned short key2);

int p10array[10] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
int p8array[8] = {6, 3, 7, 4, 8, 5, 10, 9};
int iparray[8] = {2, 6, 3, 1, 4, 8, 5, 7};
int ip1array[8] = {4, 1, 3, 5, 7, 2, 8, 6};

int main(int argc, char* argv[]){

	if(argc < 5 || argc > 6){
		fprintf(stderr, "Usage: %s [-d] <init_key> <init_vector> <original_file> <result_file>\n", argv[0]);
		return 1;
	}

//	if(argc == 5){			//encrypt
//		
//	}else{					//decrypt
//		
//	}



	FILE *fp, *fpwrite;
	char c[1];
	int fd, fdwrite, count_r, count_w;
	int ec;

	if(argc == 5) ec = 1;
	else ec = 0;

	if(ec){
		if((fpwrite = fopen(argv[4],"wb")) == NULL){ printf("File create error\n"); return 1; }
	}else{
		if((fpwrite = fopen(argv[5],"wb")) == NULL){ printf("File create error\n"); return 1; }
	}
//	fclose(fp);
	

//	printf("here1\n");
//	if((fp = fopen("testwrite","wb")) == NULL){ printf("File create error"); return 1; }
//   fclose(fp);

//	printf("here2\n");

	unsigned short iv;
	unsigned short tempiv;
	if(ec)
		iv = getinkey(argv[2],strlen(argv[2]));
	else
		iv = getinkey(argv[3],strlen(argv[3]));

	unsigned short inkey;
	if(ec)
		inkey = getinkey(argv[1],strlen(argv[1]));
	else
		inkey = getinkey(argv[2],strlen(argv[2]));
//	printf("inkey: %hu\n", inkey);
	unsigned short p10 = permute(inkey, p10array, 10, 1);
//	printf("p10: %hu\n", p10);
	unsigned short ls1highbits = circleftshift5bits(getleft5bits(p10));
//	printf("ls1highbits: %hu\n", ls1highbits);
	unsigned short ls1lowbits = circleftshift5bits(getright5bits(p10));
//	printf("ls1lowbits: %hu\n", ls1lowbits);
	unsigned short k1 = permute(combine5bits(ls1highbits,ls1lowbits), p8array, 8, 1);
//	printf("key1: %hu\n", k1);
	unsigned short ls2highbits = circleftshift5bits(circleftshift5bits(ls1highbits));
//	printf("ls2highbits: %hu\n", ls2highbits);
	unsigned short ls2lowbits = circleftshift5bits(circleftshift5bits(ls1lowbits));
//	printf("ls2lowbits: %hu\n", ls2lowbits);
	unsigned short k2 = permute(combine5bits(ls2highbits,ls2lowbits), p8array, 8, 1);
//	printf("key2: %hu\n", k2);


	if(ec)
//		fd = open(argv[3], O_RDONLY);
		fp = fopen(argv[3], "rb");
	else
//		fd = open(argv[4], O_RDONLY);
		fp = fopen(argv[4], "rb");
	if(fd == -1){ printf("file read open error\n"); return 1;}


//	if(ec)
//		fdwrite = open(argv[4], O_WRONLY);
//	else
//		fdwrite = open(argv[5], O_WRONLY);

//   if(fd == -1){ printf("file write open error\n"); return 1;}

	int i = 0;
//	while((count_r = read(fd, c, 1))>0){
	while((count_r = fread(c, 1, 1, fp)>0)){
//		printf("i: %i\n", i);
//		count_w = 0;
//		bufptr = buf;
//		while (count_w < count_r){
//			count_r -= count_w;
//			bufptr += count_w;
//			count_w = send(newsockfd, bufptr, count_r, 0);
//			if (count_w == -1) { perror("Socket write error"); return;}
		if(ec){
			c[0] = encrypt(c[0], iv, k1, k2);
			iv = c[0];
		}else{
			tempiv = c[0];
			c[0] = decrypt(c[0], iv, k1, k2);
			iv = tempiv;
		}
//		count_w = write(fdwrite, c, 1);
		count_w = fwrite(c, 1, 1, fpwrite);
		if (count_w == -1) { printf("write error\n"); return 1; }
//		}// end while2
//		printf("%c", c[0]);
//		i++;
	}// end while1

	if(ec)
		printf("Done!...File %s contains encrypted data from file %s\n", argv[4], argv[3]);
	else
		printf("Done!...File %s contains decrypted data from file %s\n", argv[5], argv[4]);


//	unsigned short iv = getinkey(argv[2],strlen(argv[2]));
//	printf("iv: %hu\n", iv);
//	printf("encrypt(00000001,k1,k2): %i\n", encrypt(1, iv, k1, k2));
//	printf("decrypt: %i\n", decrypt(encrypt(1, iv, k1, k2), iv, k1, k2));


//	unsigned short x = 164;
//	printf("x: %hu ip(x): %hu ip-1(ip(x)): %hu\n", x, permute(x, iparray, 8, 0), permute(permute(x, iparray, 8, 0), ip1array, 8, 0));

//	printf("F(9,0): %hu\n", getF(9, 0));

//	printf("fk(1011, 0110, 10110011): %hu\n", getfk(11,6,179));

//	printf("switch8bits(10110110): %hu\n", switch8bits(182));

//	circleftshift5bits(20);
//	printf("%hu\n", getleft5bits(inkey));
//	printf("%hu\n", getright5bits(inkey));
//	printf("%hu\n", circleftshift5bits(getleft5bits(p10(inkey))));
//	printf("%hu\n", circleftshift5bits(getright5bits(p10(inkey))));
//	printf("%hu\n", combine5bits(circleftshift5bits(getleft5bits(p10(inkey))),circleftshift5bits(getright5bits(p10(inkey)))));

	fclose(fp);
	fclose(fpwrite);
	return 0;

}


unsigned char encrypt(unsigned char text, unsigned char iv, unsigned short key1, unsigned short key2){
	unsigned short x = (short) (iv ^ text);
	x = permute(x, iparray, 8, 2);
	x = getfk(getleft4bits(x), getright4bits(x), key1);
	x = switch8bits(x);
	x = getfk(getleft4bits(x), getright4bits(x), key2);
	x = permute(x, ip1array, 8, 2);
	return (char) x;
}


unsigned char decrypt(unsigned char cipher, unsigned char iv, unsigned short key1, unsigned short key2){
	unsigned short x = permute(cipher, iparray, 8, 2);
	x = getfk(getleft4bits(x), getright4bits(x), key2);
	x = switch8bits(x);
	x = getfk(getleft4bits(x), getright4bits(x), key1);
	x = permute(x, ip1array, 8, 2);
	x = iv ^ x;
	return (char) x;
}


unsigned short switch8bits(unsigned short bits){
	unsigned short left = getleft4bits(bits);
	unsigned short right = getright4bits(bits);
	return (right<<4) | left;
}

unsigned short getfk(unsigned short leftbits, unsigned short rightbits, unsigned short sk){
	return ((leftbits ^ getF(rightbits, sk))<<4) | rightbits;
}

unsigned short getF(unsigned short bits, unsigned short sk){
	int eparray[8] = {4, 1, 2, 3, 2, 3, 4, 1};
	unsigned short x = permute(bits, eparray, 8, 3);
	x = x ^ sk;
	unsigned short a[4][4] = {
		{1, 0, 3, 2},
		{3, 2, 1, 0},
		{0, 2, 1, 3},
		{3, 1, 3, 2}
	};
	unsigned short a2[4][4] = {
		{0, 1, 2, 3},
		{2, 0, 1, 3},
		{3, 0, 1, 0},
		{2, 1, 0, 3}
	};
	unsigned short z = 0;
	z = z | s0(getleft4bits(x), a);
//	printf("s0: %hu\n", s0(getleft4bits(x), a));
	z = z<<2;
	z = z | s0(getright4bits(x), a2);
//	printf("s1: %hu\n", s0(getright4bits(x), a2));
	int p4array[4] = {2, 4, 3, 1};
	z = permute(z, p4array, 4, 3);
	return z;
}

unsigned short s0(unsigned short bits, unsigned short a[][4]){
	unsigned short row = 0;
	unsigned short col = 0;
	unsigned short temp;
	temp = getbit(bits, getshift4(1));
	row = row | temp;
	row = row<<1;
	temp = getbit(bits, getshift4(4));
	row = row | temp;

	temp = getbit(bits, getshift4(2));
	col = col | temp;
	col = col<<1;
	temp = getbit(bits, getshift4(3));
	col = col | temp;

//	printf("row: %hu col: %hu\n", row, col);

	return a[row][col];
}

unsigned short getright4bits(unsigned short bits){
	return bits & 15;
}

unsigned short getleft4bits(unsigned short bits){
	return bits>>4;
}

unsigned short permute(unsigned short bits, int arr[], int size, int shift){	//shift:1 = getshiftp10, shift:2 = getshift8, shift:3 = getshift4
	unsigned short x = 0;
	unsigned short temp;
	int i;
	for(i = 0; i<size; i++){
		if(shift == 1)
			temp = getbit(bits, getshiftp10(arr[i]));
		else if(shift == 2)
			temp = getbit(bits, getshift8(arr[i]));
		else
			temp = getbit(bits, getshift4(arr[i]));
		x = x | temp;
		x = x<<1;
	}
	x = x>>1;
	return x;
}

unsigned short getinkey(char * binary, int size){
	unsigned short x = 0;
	unsigned short temp = 1;
	int i;
	for(i = 0; i < size; i++){
		if(binary[i] == '0')
			x = x<<1;
		else{
			x = x | temp;
			x = x<<1;
		}
	}
	x = x>>1;
	return x;
}

unsigned short combine5bits(unsigned short bitsleft, unsigned short bitsright){
	unsigned short x = bitsleft;
	x = x<<5;
	x = x | bitsright;
	return x;
}

unsigned short getleft5bits(unsigned short bits){
	return bits >> 5;
}

unsigned short getright5bits(unsigned short bits){
	return bits & 31;
}

unsigned short circleftshift5bits(unsigned short bits){
	unsigned short x = bits;
	x = x << 1;
	unsigned short temp = getbit(x, 5);
	x = x | temp;
	x = x & 31;		//clear bit 6 and on (from right)
	return x;
}

unsigned short getbit(unsigned short bits, int index){
	unsigned short x = bits;
//	int shift = getshift(index);
//	x = x>>shift;
	x = x>>index;
	x = x & 1;
	return x;
}

int getshift4(int index){
	if(index == 4)
		return 0;
	else if(index == 3)
		return 1;
	else if(index == 2)
		return 2;
	else if(index == 1)
		return 3;
	else
		return -1;
}

int getshift8(int index){
	if(index == 8)
		return 0;
	else if(index == 7)
		return 1;
	else if(index == 6)
		return 2;
	else if(index == 5)
		return 3;
	else if(index == 4)
		return 4;
	else if(index == 3)
		return 5;
	else if(index == 2)
		return 6;
	else if(index == 1)
		return 7;
	else
		return -1;
}

int getshiftp10(int index){
	if(index == 10)
		return 0;
	else if(index == 9)
		return 1;
	else if(index == 8)
		return 2;
	else if(index == 7)
		return 3;
	else if(index == 6)
		return 4;
	else if(index == 5)
		return 5;
	else if(index == 4)
		return 6;
	else if(index == 3)
		return 7;
	else if(index == 2)
		return 8;
	else if(index == 1)
		return 9;
	else
		return -1;
}




















