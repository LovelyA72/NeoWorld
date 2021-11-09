#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "wavread.h"

#pragma warning(disable:4996)

/* wavread�֐��̈ڐA */
double * wavread(char* filename, int *fs, int *Nbit, int *waveLength)
{
	FILE *fp;
	char dataCheck[5]; // �������߂�
	unsigned char forIntNumber[4];
	double tmp, signBias, zeroLine;
	int quantizationByte;
	double *waveForm;
	int i;
	dataCheck[4] = '\0'; // ������ƍ��̂��߁C�Ō�ɏI������������D
//	fp = fopen(filename, "rb");
	fp = fopen(filename, "rb");
	if(NULL == fp) 
	{
		printf("�t�@�C���̃��[�h�Ɏ��s\n");
		return NULL;
	}
	//�w�b�_�̃`�F�b�N
	fread(dataCheck, sizeof(char), 4, fp); // "RIFF"
	if(0 != strcmp(dataCheck,"RIFF"))
	{
		fclose(fp);
		printf("�w�b�_RIFF���s��\n");
		return NULL;
	}
	fseek(fp, 4, SEEK_CUR); // 4�o�C�g��΂�
	fread(dataCheck, sizeof(char), 4, fp); // "WAVE"
	if(0 != strcmp(dataCheck,"WAVE"))
	{
		fclose(fp);
		printf("�w�b�_WAVE���s��\n");
		return NULL;
	}
	fread(dataCheck, sizeof(char), 4, fp); // "fmt "
	if(0 != strcmp(dataCheck,"fmt "))
	{
		fclose(fp);
		printf("�w�b�_fmt ���s��\n");
		return NULL;
	}
	fread(dataCheck, sizeof(char), 4, fp); //1 0 0 0
	if(!(16 == dataCheck[0] && 0 == dataCheck[1] && 0 == dataCheck[2] && 0 == dataCheck[3]))
	{
		fclose(fp);
		printf("�w�b�_fmt (2)���s��\n");
		return NULL;
	}
	fread(dataCheck, sizeof(char), 2, fp); //1 0
	if(!(1 == dataCheck[0] && 0 == dataCheck[1]))
	{
		fclose(fp);
		printf("�t�H�[�}�b�gID���s��\n");
		return NULL;
	}
	fread(dataCheck, sizeof(char), 2, fp); //1 0
	if(!(1 == dataCheck[0] && 0 == dataCheck[1]))
	{
		fclose(fp);
		printf("�X�e���I�ɂ͑Ή����Ă��܂���\n");
		return NULL;
	}

	// �T���v�����O���g��
	fread(forIntNumber, sizeof(char), 4, fp);
	*fs = 0;
	for(i = 3;i >= 0;i--)
	{
		*fs = *fs*256 + forIntNumber[i];
	}
	// �ʎq���r�b�g��
	fseek(fp, 6, SEEK_CUR); // 6�o�C�g��΂�
	fread(forIntNumber, sizeof(char), 2, fp);
	*Nbit = forIntNumber[0];
	// �w�b�_
	fread(dataCheck, sizeof(char), 4, fp); // "data"
	if(0 != strcmp(dataCheck,"data"))
	{
		fclose(fp);
		printf("�w�b�_data���s��\n");
		return NULL;
	}
	// �T���v���_�̐�
	fread(forIntNumber, sizeof(char), 4, fp); // "data"
	*waveLength = 0;
	for(i = 3;i >= 0;i--)
	{
		*waveLength = *waveLength*256 + forIntNumber[i];
	}
	*waveLength /= (*Nbit/8);

	// �g�`�����o��
	waveForm = (double *)malloc(sizeof(double) * *waveLength);
	if(waveForm == NULL) return NULL;

	quantizationByte = *Nbit/8;
	zeroLine = pow(2.0,*Nbit-1);
	for(i = 0;i < *waveLength;i++)
	{
		signBias = 0.0;
		tmp = 0.0;
		fread(forIntNumber, sizeof(char), quantizationByte, fp); // "data"
		// �����̊m�F
		if(forIntNumber[quantizationByte-1] >= 128)
		{
			signBias = pow(2.0,*Nbit-1);
			forIntNumber[quantizationByte-1] = forIntNumber[quantizationByte-1] & 0x7F;
		}
		// �f�[�^�̓ǂݍ���
		for(int j = quantizationByte-1;j >= 0;j--)
		{
			tmp = tmp*256.0 + (double)(forIntNumber[j]);
		}
		waveForm[i] = (double)((tmp - signBias) / zeroLine);
	}
	// ����
	fclose(fp);
	return waveForm;
}

