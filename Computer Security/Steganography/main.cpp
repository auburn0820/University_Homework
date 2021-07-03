#include <iostream>
#include <fstream>
#include <Windows.h>
#include <cstdio>
#include <string>

using namespace std;

const char* BMP_PATH = "origin.bmp";
const char* BMP_MODIFIED_PATH = "stego.bmp";

// 몇 번째 비트에 어떠한 값이 들어있는지 반환해주는 함수
unsigned short getBit(int x, char ch) {
	unsigned short ch_convert = (unsigned short)ch;

	return (ch_convert & (1 << x)) >> x;
}

void encode_bmp_file() {
	FILE* image_file = NULL;
	FILE* output_image_file = NULL;
	
	BITMAPFILEHEADER bf_header;
	BITMAPINFOHEADER bi_header;
	RGBQUAD hRGB[256];

	// origin.bmp를 담을 배열
	unsigned char input_image[314][559][3];
	// stegano.bmp를 담을 배열
	unsigned char output_image[314][559][3];

	cin.clear();
	string str;
	getline(cin, str);

	int length = (int)str.length();

	if (fopen_s(&image_file, BMP_PATH, "rb") == 0 && fopen_s(&output_image_file, BMP_MODIFIED_PATH, "wb") == 0) {
		// bmp 파일 헤더의 정보를 가져온다.
		fread(&bf_header, sizeof(BITMAPFILEHEADER), 1, image_file);
		fread(&bi_header, sizeof(BITMAPINFOHEADER), 1, image_file);

		int width = bi_header.biWidth;
		int height = bi_header.biHeight;

		fread(&hRGB, sizeof(RGBQUAD), 256, image_file);
		fread(input_image, height * width * 3, 1, image_file);

		// origin이미지의 데이터를 스테가노그래피를 저장할 배열에 가져온다.
		for (int i = 0; i < 314; i++) {
			for (int j = 0; j < 559; j++) {
				for (int k = 0; k < 3; k++) {
					output_image[i][j][k] = input_image[i][j][k];
				}
			}
		}

		// 길이 숫자를 최대 2byte 수로 저장한다.
		// bmp 파일은 배열 하나 당 1byte이므로, 나눠서 저장해야 한다.
		unsigned short upper = 0;
		unsigned short lower = 0;

		// 앞 1byte, 뒤 1byte를 나누는 과정
		upper = length >> 8;
		lower = length;

		unsigned short ret = 0;

		
		for (int i = 7; i >= 0; i--) {
			ret = ret << 1;
			ret |= getBit(i, lower);
		}

		lower = ret;

		output_image[0][0][0] = upper;
		output_image[0][0][1] = lower;

		// 1byte정보를 각각의 lsb에 저장한다.
		int idx = 0;
		int lsb_count = 7;

		for (int i = 1; i < 314; i++) {
			for (int j = 1; j < 559; j++) {
				for (int k = 0; k < 3; k++) {
					if (idx < length) {
						// 저장할 ASCII Code의 정보를 한 비트씩 가져온다.
						unsigned char ch_bit_data = (unsigned char)getBit(lsb_count, str[idx]);
						// 비트가 0일 경우
						if (ch_bit_data == 0) {
							// lsb를 0으로 바꿔준다.
							output_image[i][j][k] &= ~1;
						}
						// 비트가 1일 경우
						else if (ch_bit_data == 1) {
							// lsb를 1로 바꿔준다.
							output_image[i][j][k] |= 1;
						}
						
						lsb_count--;
						// 한 개의 ASCII Code를 담았을 경우, 다음 ASCII Code를 저장하기 위해 idx를 증가한다.
						if (lsb_count == -1) {
							idx++;
							lsb_count = 7;
						}
					}
					// 모든 텍스트 정보를 담았을 경우, 기존 이미지 정보를 그대로 담는다.
					else
						output_image[i][j][k] = input_image[i][j][k];
				}
			}
		}
		
		// bmp 파일 형식으로 저장한다.
		fwrite(&bf_header, sizeof(BITMAPFILEHEADER), 1, output_image_file);
		fwrite(&bi_header, sizeof(BITMAPINFOHEADER), 1, output_image_file);
		fwrite(&hRGB, sizeof(RGBQUAD), 256, output_image_file);
		fwrite(output_image, height * width * 3, 1, output_image_file);

		fclose(output_image_file);
		fclose(image_file);
	}
}

void decode_bmp_file() {
	FILE* modified_image = NULL;
	FILE* output_text = NULL;

	BITMAPFILEHEADER bf_header;
	BITMAPINFOHEADER bi_header;
	RGBQUAD hRGB[256];
	char* text_buffer;

	unsigned char modified_image_arr[314][559][3];

	if (fopen_s(&modified_image, BMP_MODIFIED_PATH, "rb") == 0) {
		fread(&bf_header, sizeof(BITMAPFILEHEADER), 1, modified_image);
		fread(&bi_header, sizeof(BITMAPINFOHEADER), 1, modified_image);

		int width = bi_header.biWidth;
		int height = bi_header.biHeight;

		fread(&hRGB, sizeof(RGBQUAD), 256, modified_image);
		fread(modified_image_arr, height * width * 3, 1, modified_image);

		// 상위 1byte의 길이 정보를 가져온다.
		int gotLength = (int)modified_image_arr[0][0][0] << 8;
		// 가져온 상위 1byte 길이 정보와 하위 1byte 길이 정보를 or 비트 연산한다.
		gotLength |= modified_image_arr[0][0][1];

		// 해독된 텍스트 정보를 담을 버퍼를 동적할당한다.
		text_buffer = new char[gotLength];

		int idx = 0;
		int ascii_code = 0;
		int lsb_count = 0;

		for (int i = 1; i < 314; i++) {
			for (int j = 1; j < 559; j++) {
				for (int k = 0; k < 3; k++) {
					if (gotLength >= idx) {
						// 해독할 ASCII Code의 정보를 lsb에서 가져온다.
						int ch_bit_data = getBit(0, modified_image_arr[i][j][k]);
						ascii_code = ascii_code << 1;

						if (ch_bit_data == 0) {
							ascii_code &= ~1;
						}
						else if (ch_bit_data == 1) {
							ascii_code |= 1;
						}
						
						lsb_count++;
						lsb_count %= 8;

						// ASCII Code의 정보를 다 가져왔을 경우, buffer에 입력한다. 이후 다음 ASCII Code를 가져온다.
						if (lsb_count == 0) {
							text_buffer[idx] = (unsigned char)ascii_code;
							idx++;
							ascii_code = 0;
						}
					}
				}
			}
		}
		// 해독된 정보를 출력한다.
		cout.write(text_buffer, gotLength);
		cout << '\n';
	}
}

int main(int argc, const char* argv[]) {
	if (strcmp(argv[1], "e") == 0) {
		encode_bmp_file();
	}
	else if (strcmp(argv[1], "d") == 0) {
		decode_bmp_file();
	}

	return 0;
}