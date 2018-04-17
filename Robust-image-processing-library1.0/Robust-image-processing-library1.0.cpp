/*Robustͼ�����
*�汾��v1.0
*���ߣ���־��
*4.17 19��09
*/
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
//���Ͷ���
//8bit  
typedef char UINT_8;
//16bit  
typedef char UINT_16[2];
//22bit  
typedef char UINT_22[3];
//256bit  
typedef char UINT_256[256];
//32bit  
typedef int  UINT_32;
//data  
typedef char* DATA;
//64 fbit  
typedef double FUINT_64;
//16bit int  
typedef short SUINT_16;
//�ṹ��
//ͼ��ṹ�壬��ֹ�ṹ���������ȡ�ļ���ʱ����������pragma����ǿ�ƶ���
#pragma pack(1)  
typedef struct image_struct{
	UINT_16   image_pil;                //�ļ���ʶ��  
	UINT_32   image_Size;               //�ļ���С  
	UINT_16   image_Reserved1;          //�������  
	UINT_16   image_Reserved2;          //�������  
	UINT_32   image_Offbits;            //ͷ�ļ���ͼ������ƫ����  
	UINT_32   image_Stsize;             //�ṹ�������С  
	UINT_32   image_Width;              //ͼ����  
	UINT_32   image_Height;             //ͼ��߶�  
	UINT_16   image_Planes;             //Ŀ���豸λ����  
	SUINT_16  image_Bitcount;           //���ص�ռ�ö���bitλ  
	UINT_32   image_Compression;        //ͼ��ѹ������  
	UINT_32   image_Sizeimage;          //ͼ��Ĵ�С  
	UINT_32   image_Xpelspermeter;      //ˮƽ�ֱ���  
	UINT_32   image_Ypelspermeter;      //��ֱ�ֱ���  
	UINT_32   image_ClrUsed;            //��ɫ��������  
	UINT_32   image_Clrlmportant;       //ͼ����ʾ��Ҫ��ɫ������Ŀ  
	DATA      image_Data;               //ͼ������ 
	UINT_32   image_Data_Size;			//ͼ�����ݴ�С
}image; 
#pragma pack()  
//����
//����ͼ�� 
int image_load(struct image_struct** im, char *path){
	FILE *image_path_fp;
	image_path_fp = fopen(path, "rb");
	if (image_path_fp == NULL){
		return -1;
	}
	//ȡ�ļ���С
	fseek(image_path_fp, 0, SEEK_END); //��λ���ļ�ĩ 
	int nFileLen = ftell(image_path_fp); //�ļ�����
	fseek(image_path_fp, 0, SEEK_SET);	//�ָ����ļ�ͷ����ֹ���ļ�β��ȡ����
	//��ȡͷ��Ϣ
	if (fread((*im), (sizeof(struct image_struct) - ((sizeof(DATA/*image_Data*/)+(sizeof(UINT_32)/*image_Data_Size*/)))/*�ݲ���ȡ���ݣ��޷���ͷ�ļ��л�ȡ����ƫ��������ֹ���ݻ���*/), 1, image_path_fp) == 0){
		return -2;
	}
	//��data���������ڴ�
	(*im)->image_Data = (DATA)malloc(nFileLen-(*im)->image_Offbits/*���������ݴ�С*/);
	//�ж��Ƿ����ɹ�
	if ((*im)->image_Data == NULL){	//���û�п��ö��ڴ���malloc����NULL
		return -3;
	}
	//��ȡ����
	//��ȡǰ���ļ�ָ��Ų�Ƶ��ļ�ͷ��Ϣ���ҵ���ȷ�����ݴ洢��
	fseek(image_path_fp, 0, SEEK_SET);	//�ָ����ļ�ͷ����Ϊ�Ѿ�freadһ���ˣ����������ļ�ָ�뷢�����
	fseek(image_path_fp, (*im)->image_Offbits, SEEK_CUR);	//����ͷ����
	if (fread((*im)->image_Data, (nFileLen - (*im)->image_Offbits/*file - off = ʵ�ʴ�С*/), 1, image_path_fp) == 0){
		return -4;
	}
	//�����ļ���С,�����д����
	(*im)->image_Data_Size = (nFileLen - (*im)->image_Offbits/*file - off = ʵ�ʴ�С*/);
	//�ļ�ָ���ͷţ���ֹռ���ļ��ں˵��ٽ���Դ
	fclose(image_path_fp);
	image_path_fp == NULL;
	return 0;
}
//��ͼ�����ݷ����ڴ�
int image_malloc(struct image_struct** im){
	*im = (struct image_struct*)malloc(sizeof(struct image_struct));
	if (*im == NULL){
		return -1;
	}
	return 0;
}
//��ͼ��ת���ɷ���ͼ_�÷���ֻ���������ͼ
int image_reverse_rgb(struct image_struct** im){
	if ((*im) == NULL){	//�жϴ��ݽ�����ͼ��ָ���Ƿ�Ϊ��
		return -1;
	}
	//ת���ɷ���ͼ�ܼ�ֻ��Ҫ��ÿ��ͼ��������ص�ת���ɸ��ľͿ�����,ע����һ��24λ��ͼ���ļ���һ���ֽڶ�Ӧһ����ɫֵ�����ֽ���Ϊһ�����������ص㣬��������һ��һ�����ص��ת���Ϳ�����
	//�㷨��ʽΪ��S=-R-G-B
	for (int i = 0; i < (*im)->image_Data_Size; ++i){
			if ((*im)->image_Data[i] == (int)0){	//�������0�򲻴���
				continue;	//��ʼ��һ��ѭ��
			}//i*(*im)->image_Width + j
			(*im)->image_Data[i] = -(*im)->image_Data[i];	//���ú꺯��ת��
	}
	return 0;
}
//����ͼ�����ݵ��ļ�
int image_save_file(struct image_struct** im, char *path){
	FILE* file_fp = fopen(path, "wb+");	//�Զ����ƿɶ�д��ʽ��
	if (file_fp == NULL){	//�ж��ļ�ָ���Ƿ�Ϊ��
		return -1;
	}
	//д��ͷ��Ϣ
	fwrite((*im), (*im)->image_Offbits/*ֱ��д��ͷ�ļ������ݵ�ƫ������С����*/, 1, file_fp);
	//д���ļ�����
	fwrite((*im)->image_Data, (*im)->image_Data_Size, 1, file_fp);
	return 0;

}
int main(){
	image *imga;
	image_malloc(&imga);
	image_load(&imga, "test.bmp");
	image_reverse_rgb(&imga);
	image_save_file(&imga, "test1.bmp");
	printf("ת�����");
	getchar();
}