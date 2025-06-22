
#include "common.h"
int8_t SD_exist = 0;//���sd���Ƿ񱻳�ʼ����1Ϊ��ʼ��

uint16_t adc_result;

extern float cic_span;


extern float usart_vol;
extern float usart_acZ0,usart_acZ1,usart_acZ2;
float Pulling;

extern FATFS *fs[2];
FIL datafile;
extern int8_t acc_ableRead;
extern int8_t number_acz1;


char acc_float0[20];
char acc_float1[20];
char acc_float2[20];
char vol_float[20];
char adc_float[20];
char filename[20];
extern uint8_t SD_txtNumber;										  

/***********************************************************
��������SD_Insert
���ܣ�����SD������ʱ����(���жϺ���)
������None
����ֵ:1 ��ʼ��ʧ��
***********************************************************/
void SD_Insert()
{
	     exfuns_init();
		if(f_mount(fs[0],"0:",1)==0&&sd_init()==0)
		{
			SD_exist=1;
		}
	}
/***********************************************************
��������Para_Save Para_init Para_end
���ܣ���SD���������
������None
����ֵ: 1 �ļ��򿪴���
				2 �ļ���ȡ����
				3 �ļ�д�����
				10 SD��������
***********************************************************/
//int Para_Save(void)
//{
//if(SD_exist)
//{
//	if(acc_ableRead == 1&&number_acz1 == 40)
//	{
//	for(uint8_t i = 0;i<40;i++)
//		{if(f_printf(&datafile,"Acceleration_Z %.2f,",usart_acZ1[i])<0) return 3;
//		
////		if(f_printf(&datafile,"Pulling %.2f,",Pulling)<0) return 3;
////		if(f_printf(&datafile,"Velocity %.3f\n",Velocity)<0) return 3;
//		f_printf(&datafile,"\n");
////		}
////		acc_ableRead = 0;
//	}

//	return 0;
//}
//	else return 10;
//}
//}
void Para_init(void)
{
if(SD_exist)
{
	   sprintf(filename, "0:data%d.txt", SD_txtNumber);
		//���ļ�
		f_open(&datafile, filename, FA_OPEN_ALWAYS|FA_WRITE|FA_READ);
		f_rewind(&datafile);
//д���һ��
		f_printf(&datafile,"a0,a1,a2,v,pull\n");

}
}

int Para_end(void)
{
if(SD_exist)
{
	if(f_truncate(&datafile)) return 3;
		if(f_close(&datafile)) return 1;
return 0;
}
	else return 10;

}

	
/***********************************************************
��������Voltage_Pulling
����:����adc��������ѹֵ�����������N
������None
����ֵ: 0����
***********************************************************/
void Voltage_Pull(void)
{
	  	 adc_result = adc_get_result_average(ADC_CHANNEL_1, 10);      /* ��ֵ�˲���ȡADC��� */
       Pulling = ((adc_result * 3.3f) / 4095.0f)*4000.0f;                       /* ����ʵ�ʵ�ѹֵ������1000���� */

}
/***********************************************************
��������common_loop
����:��freertos�н���ѭ����ִ����sd��д���ݵ�����
������None
����ֵ: 0����
***********************************************************/
void common_loop(void)
{
// �ɼ��������ݴ浽���Ե�������
	//�ٽ���д��
	//���������㣨��Ҫ�б�־λ��
//Para_Save();
//	  Voltage_Pull(); 

		    sprintf(acc_float0, "%.4f", usart_acZ0);
		    sprintf(acc_float1, "%.4f", usart_acZ1);
		    sprintf(acc_float2, "%.4f", usart_acZ2);
		    sprintf(vol_float, "%.4f", usart_vol);
		    sprintf(adc_float, "%.4f", Pulling);


	f_printf(&datafile," %s,",acc_float0);
	f_printf(&datafile," %s,",acc_float1);
	f_printf(&datafile," %s,",acc_float2);
	f_printf(&datafile," %s,",vol_float);
	f_printf(&datafile," %s\n",adc_float);

}
