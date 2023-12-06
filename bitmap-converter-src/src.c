
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define CHECKBMP_BYTE (2)
#define FILESIZE_BYTE (4)
#define HEADERPOSITION_BYTE (4)
#define IMAGEWIDTH_BYTE (4)
#define IMAGEHEIGHT_BYTE (4)
#define BMPBIT_BYTE (2)
#define COMPRESSEDSTATUS_BYTE (2)
#define XXIV_BYTE (1)
#define XVI_BYTE (2)

#define CHECKBMP PRESET(0)
#define FILESIZE PRESET (2)
#define HEADERPOSITION_PRESET (10)
#define IMAGEWIDTH_PRESET (18)
#define IMAGEHEIGHT_PRESET (22)
#define BMPBIT_PRESET (28)
#define COMPRESSEDSTATUS_PRESET (30)
#define XVI_COLORMASK_PRESET (54)


#define Blue_MASK (0xF800)
#define Green_MASK (0x7e0)
#define Red_MASK (0x1f)

#define TARGET_FILE ( " . . /16bit . bmp " )


int main(void)
{

	FILE** fp1 = NULL;
	/*Opening a BinaryFile */

	errno_t errBMP = fopen_s(&fp1, TARGET_FILE, "r+");
	if (errBMP = 0) {
		printf("%s ", " File opened successfully . \n ");

		WORD CheckBMP[2], BMPBit[2], CompressedState[2];
		DWORD FileSize[2], HeaderPosition[2], ImageWidth[2], ImageHeight[2];
		DWORD wCheckBMP, wFileSize, wHeaderPosition, wimageWidth, wimageHeight, wBMPBit, wCompressedState;
		DWORD wCheckBMP_COUNTER, wFileSize_COUNTER, wHeaderPosition_COUNTER, wimageWidth_COUNTER, wimageHeight_COUNTER, wBMPBit_COUNTER, wCompressedState_COUNTER, PIXEL_COUNTER;


		fread(CheckBMP, 1, CHECKBMP_BYTE, fp1);
		if (CheckBMP[0] == 0x4d42)
		{
			printf("BMP file detected,strating processing... \n");

			/*File Size Confirmation*/

			fread(FileSize, 1, FILESIZE_BYTE, fp1);
			printf("このイメージのサイズは： %d Byte \n ", FileSize[0]);

			/*Header Position Confirmation*/
			fseek(fp1, HEADERPOSITION_PRESET, SEEK_SET);
			fread(HeaderPosition, 1, HEADERPOSITION_BYTE, fp1);
			printf("ヘッダーポジションは： %d \n ", HeaderPosition[0]);

			/*Image Width Confirmation*/
			fseek(fp1, IMAGEWIDTH_PRESET, SEEK_SET);
			fread(ImageWidth, 1, IMAGEWIDTH_BYTE, fp1);
			printf("イメージの横幅は %d px\n ", ImageWidth[0]);

			/* Image Height Confirmation*/
			fseek(fp1, IMAGEHEIGHT_PRESET, SEEK_SET);
			fread(ImageHeight, 1, IMAGEHEIGHT_BYTE, fp1);
			printf("イメージの縦幅は %d px\n ", ImageHeight[0]);

			/*Color Bits Confirmation*/
			fseek(fp1, BMPBIT_PRESET, SEEK_SET);
			fread(BMPBit, 1, BMPBIT_BYTE, fp1);
			printf("カラーは %d ビット\n ", BMPBit[0]);

			/*Compressed State*/
			fseek(fp1, COMPRESSEDSTATUS_PRESET, SEEK_SET);
			fread(CompressedState, 1, COMPRESSEDSTATUS_BYTE, fp1);
			if (CompressedState[0] == 0) {
				printf("無圧縮　\n", BMPBit[0]);
			}
			else {
				printf("圧縮あり \n", BMPBit[0]);
			}



			WORD Red[1], Green[1], Blue[1];
			WORD XVI_Red[1], XVI_Green[1], XVI_Blue[1];
			WORD XVI_color[1];


			WORD Pix2COLOR_BLUE = ((Blue_MASK) & (0 - Blue_MASK));
			WORD Pix2COLOR_GREEN = ((Green_MASK) & (0 - Green_MASK));
			WORD Pix2COLOR_RED = ((Red_MASK) & (0 - Red_MASK));

			printf(" Pix2COLOR_BLUE : %x\n ", Pix2COLOR_BLUE);
			printf(" Pix2COLOR_GREEN : %x\n ", Pix2COLOR_GREEN);
			printf(" Pix2COLOR_RED : %x\n", Pix2COLOR_RED);

			int Width_COUNTER, Height_COUNTER;

			if (24 == BMPBit[0]) {

				fseek(fp1, HeaderPosition[0], SEEK_SET);
				fread(Red, 1, XXIV_BYTE, fp1);
				fread(Green, 1, XXIV_BYTE, fp1);
				fread(Blue, 1, XXIV_BYTE, fp1);
				printf("%x\n ", Red[0]);
				printf("%x\n ", Green[0]);
				printf("%x\n ", Blue[0]);
				float coefficient = 32.0 / 255.0;

				XVI_Blue[0] = Blue[0] * coefficient * Pix2COLOR_BLUE;
				XVI_Green[0] = Red[0] * coefficient * Pix2COLOR_GREEN;
				XVI_Red[0] = Red[0] * coefficient * Pix2COLOR_RED;

				printf("XVI_Blue[0] : %x\n ", XVI_Blue[0]);
				printf("XVI_Green[0] : %x\n ", XVI_Green[0]);
				printf("XVI Red[0] : %x\n ", XVI_Red[0]);


			}
			else if (16 == BMPBit[0]) {
				fseek(fp1, HeaderPosition[0], SEEK_SET);
				FILE** fp2 = NULL;
				fopen_s(&fp2, ". /XVI2XXIV . bmp ", " w+ ");
				unsigned char BMP_TMP[3];
				for (PIXEL_COUNTER = 0; PIXEL_COUNTER < 240000; PIXEL_COUNTER++) {

					unsigned char BMP_TMP[3];
					fread(XVI_color, 1, 2, fp1);
					printf(" \n ");
					printf("XVI co-lor : %x\n ", XVI_color[0]);
					XVI_Blue[0] = (XVI_color[0] & Blue_MASK) * (255 / 63) / Pix2COLOR_BLUE;
					BMP_TMP[0] = XVI_Blue[0];
					printf("%x\n ", BMP_TMP[0]);
					XVI_Green[0] = (XVI_color[0] & Green_MASK) * (255 / 63) / Pix2COLOR_GREEN;
					BMP_TMP[1] = XVI_Green[0];
					printf("%x\n ", BMP_TMP[1]);
					XVI_Red[0] = (XVI_color[0] & Red_MASK) * (255 / 63) / Pix2COLOR_RED;
					BMP_TMP[2] = XVI_Red[0];
					printf("%x\n ", BMP_TMP[2]);
					printf("%d\n ", PIXEL_COUNTER);
					fwrite(BMP_TMP, 1, 3, fp2);
					// fwrite ( " \0 ", 1 , 1 , fp2) ;
				}
			}
		}
	}else {


			printf("BMP file detection failed . Check file type . \n ");
		}
		printf("%s ", "Attemp to open file has failed . Check file position before retry . \n ");
}