#include <iostream>
#include <fstream>
#include <Windows.h>
#include <cstdio>
#include <string>

using namespace std;

const char* BMP_PATH = "origin.bmp";
const char* BMP_MODIFIED_PATH = "stego.bmp";

// �� ��° ��Ʈ�� ��� ���� ����ִ��� ��ȯ���ִ� �Լ�
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

	// origin.bmp�� ���� �迭
	unsigned char input_image[314][559][3];
	// stegano.bmp�� ���� �迭
	unsigned char output_image[314][559][3];

	cin.clear();
	string str;
	getline(cin, str);

	int length = (int)str.length();

	if (fopen_s(&image_file, BMP_PATH, "rb") == 0 && fopen_s(&output_image_file, BMP_MODIFIED_PATH, "wb") == 0) {
		// bmp ���� ����� ������ �����´�.
		fread(&bf_header, sizeof(BITMAPFILEHEADER), 1, image_file);
		fread(&bi_header, sizeof(BITMAPINFOHEADER), 1, image_file);

		int width = bi_header.biWidth;
		int height = bi_header.biHeight;

		fread(&hRGB, sizeof(RGBQUAD), 256, image_file);
		fread(input_image, height * width * 3, 1, image_file);

		// origin�̹����� �����͸� ���װ���׷��Ǹ� ������ �迭�� �����´�.
		for (int i = 0; i < 314; i++) {
			for (int j = 0; j < 559; j++) {
				for (int k = 0; k < 3; k++) {
					output_image[i][j][k] = input_image[i][j][k];
				}
			}
		}

		// ���� ���ڸ� �ִ� 2byte ���� �����Ѵ�.
		// bmp ������ �迭 �ϳ� �� 1byte�̹Ƿ�, ������ �����ؾ� �Ѵ�.
		unsigned short upper = 0;
		unsigned short lower = 0;

		// �� 1byte, �� 1byte�� ������ ����
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

		// 1byte������ ������ lsb�� �����Ѵ�.
		int idx = 0;
		int lsb_count = 7;

		for (int i = 1; i < 314; i++) {
			for (int j = 1; j < 559; j++) {
				for (int k = 0; k < 3; k++) {
					if (idx < length) {
						// ������ ASCII Code�� ������ �� ��Ʈ�� �����´�.
						unsigned char ch_bit_data = (unsigned char)getBit(lsb_count, str[idx]);
						// ��Ʈ�� 0�� ���
						if (ch_bit_data == 0) {
							// lsb�� 0���� �ٲ��ش�.
							output_image[i][j][k] &= ~1;
						}
						// ��Ʈ�� 1�� ���
						else if (ch_bit_data == 1) {
							// lsb�� 1�� �ٲ��ش�.
							output_image[i][j][k] |= 1;
						}
						
						lsb_count--;
						// �� ���� ASCII Code�� ����� ���, ���� ASCII Code�� �����ϱ� ���� idx�� �����Ѵ�.
						if (lsb_count == -1) {
							idx++;
							lsb_count = 7;
						}
					}
					// ��� �ؽ�Ʈ ������ ����� ���, ���� �̹��� ������ �״�� ��´�.
					else
						output_image[i][j][k] = input_image[i][j][k];
				}
			}
		}
		
		// bmp ���� �������� �����Ѵ�.
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

		// ���� 1byte�� ���� ������ �����´�.
		int gotLength = (int)modified_image_arr[0][0][0] << 8;
		// ������ ���� 1byte ���� ������ ���� 1byte ���� ������ or ��Ʈ �����Ѵ�.
		gotLength |= modified_image_arr[0][0][1];

		// �ص��� �ؽ�Ʈ ������ ���� ���۸� �����Ҵ��Ѵ�.
		text_buffer = new char[gotLength];

		int idx = 0;
		int ascii_code = 0;
		int lsb_count = 0;

		for (int i = 1; i < 314; i++) {
			for (int j = 1; j < 559; j++) {
				for (int k = 0; k < 3; k++) {
					if (gotLength >= idx) {
						// �ص��� ASCII Code�� ������ lsb���� �����´�.
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

						// ASCII Code�� ������ �� �������� ���, buffer�� �Է��Ѵ�. ���� ���� ASCII Code�� �����´�.
						if (lsb_count == 0) {
							text_buffer[idx] = (unsigned char)ascii_code;
							idx++;
							ascii_code = 0;
						}
					}
				}
			}
		}
		// �ص��� ������ ����Ѵ�.
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