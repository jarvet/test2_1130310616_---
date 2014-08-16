#include<iostream>
#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include "StreamBuffer.h"
using namespace std;

#define TEST_FRAME  //���Դ����ܵ���ȷ��

void	GenDisOrder(int iSendOrder[],int iPacketNum)
{
    int i,j,k;

    for(i=0;i<iPacketNum;i++)//�Ȳ���˳������У�0��1,2��.....iPacketNum-1
        iSendOrder[i] = i;

	if( iPacketNum < 5)
		return;

    int n,temp;
    n =  rand()%(iPacketNum/5) +1;    //���ҵĴ��������20%*2�����ݰ�
   for(i=0;i< n; i++)
   {//����j��k�������ݰ���˳��
         j = rand()%(iPacketNum/2) + 1;
         k = rand()%( iPacketNum - j);
        temp =  iSendOrder[j];
        iSendOrder[j] = iSendOrder[k];
        iSendOrder[k]=temp;
   }
}

int main()
{
	char srcfileName[500]="../media/source.mp3";
	char dstfileName[500]="../media/dest.mp3";
	FILE* fpSrcFile = NULL;
	FILE* fpDstFile = NULL;
	const int MTU =  1500;//����䵥Ԫ�����������ʾһ�����ݰ������ߴ磬��λ���ֽ�
	//unsigned int iOffset;�������ܸ������ò���ò�������
	int iReadBytes = 0, iWantReadBytes;

    int  iContinueBytes;
    int iUseBytes;
	unsigned int iOutDataOffset;
	char  *pOutData;

	StreamBuffer MyBuffer;

	const int iMaxPacketNum = 20; //ÿ�ζ���20�����ݰ���Ȼ�����������ʽ��������ģ�飨StreamBuffer�ࣩ
	int iSendOrder[iMaxPacketNum];//��¼�·����ݰ���˳��
	unsigned int iPacketOffset[iMaxPacketNum];//��¼ÿ�����ݰ��е�һ���ֽ����ݵ�ƫ����
	unsigned int iPacketLen[iMaxPacketNum];//��¼ÿ�����ݰ��е����ݳ���
	char       (*pDataBuf)[MTU]; //���ݰ�������������������1��ָ��һλ�����ָ�룬��һ����Ϊ��һ��������һ����ά����
	//char     *pDataBuf[iMaxPacketNum];//  ��������2��ָ�����飬һ����Ϊ���������ռ䣬ÿ������һ��һά���飬�ͷ�ҲҪ���
	int   iPacketNum;
	int i;
	int iPackNo;

     srand(100);//�ù̶�ֵ��ʼ���������ɹ̶�����������У�������Գ��򣬷�����srand( (unsigned)time( NULL ) );

	 pDataBuf = new char[iMaxPacketNum][MTU];//����1
	 /*for( i=0;i<iMaxPacketNum;i++)//����2
     {
         pDataBuf[i] = new char[MTU];
     }*/


	fpSrcFile = fopen(srcfileName, "rb");
	if( fpSrcFile == NULL )
	{
	  cout<<"Cann't open file: "<<srcfileName<<endl;
	  return 1;
	}

	fpDstFile = fopen(dstfileName, "wb");
	if( fpDstFile == NULL )
	{
	  cout<<"Cann't create file: "<< dstfileName <<endl;
	  return 2;
	}

	iWantReadBytes = 1024;

   do
   {
        iPacketNum = 0;
        for( i=0;i<iMaxPacketNum;i++)//��ʼ�����ݰ�����Ϊ0����ʾû�ж�������
            iPacketLen[i] = 0;//2014.07.11  iPacketNum->i
        //	iReadBytes = fread(pDataBuf[iPacketNum], 1, iWantReadBytes, fpSrcFile);
		do
		{
			iPacketOffset[iPacketNum] = ftell(fpSrcFile);
			iReadBytes = fread(pDataBuf[iPacketNum], 1, iWantReadBytes, fpSrcFile);
            iPacketLen[iPacketNum] = iReadBytes; //��ǰ���ݰ���ȡ�ɹ�����¼���ݰ����ȣ�����������0
            iWantReadBytes =  (iPacketOffset[iPacketNum]+iPacketNum*iPacketNum)%500+400;//��һ�����ݰ���ȡ����
            iPacketNum++;
        }  while( iReadBytes > 0 && iPacketNum < iMaxPacketNum );//����һ�����ݰ�������ļ�������iReadBytes<1

        //�Ѹո��Ѿ�����һ�����ݰ��������·�������ģ��
        GenDisOrder(iSendOrder,iMaxPacketNum);
        for(i=0;i<iMaxPacketNum;i++)//ֻҪ���Ȳ�Ϊ0���ͷ�������ģ��
        {
            iPackNo = iSendOrder[i];
            if(iPacketLen[iPackNo] > 0 )//�����ݣ���������ģ��
            {
//#ifdef TEST_FRAME
                 iUseBytes = iPacketLen[iPackNo];
                 pOutData = pDataBuf[iPackNo];
                 iOutDataOffset = iPacketOffset[iPackNo];
//#else
				MyBuffer.ReceiveDate(iPacketOffset[iPackNo],iPacketLen[iPackNo],pDataBuf[iPackNo]);
                iContinueBytes = MyBuffer.ContinueBytes(iOutDataOffset, pOutData);
                iUseBytes =    iContinueBytes - 100;//��������һ����
                if( iContinueBytes  > 1024) //ʾ����ֵ�����Ե���
//#endif
                {
                   fseek(fpDstFile,iOutDataOffset,SEEK_SET);
                   fwrite(pOutData,iUseBytes,1,fpDstFile);
                   MyBuffer.RemoveData(iUseBytes);
                }
            }
        }
   }while(iReadBytes>0);//˵�������ļ��������ݣ�����....

	//����������ѻ�������ʣ���ź��������ȡ��
    iContinueBytes = MyBuffer.ContinueBytes(iOutDataOffset, pOutData);
	if( iContinueBytes > 0 )
	{
		fseek(fpDstFile,iOutDataOffset,SEEK_SET);
		fwrite(pOutData,iContinueBytes,1,fpDstFile);
	}

    fclose(fpDstFile);
    fclose(fpSrcFile);



	 delete []pDataBuf;//����1
	 /*for( i=0;i<iMaxPacketNum;i++) //����2
     {
         delete []pDataBuf[i];
     }*/
  //��������������������������������
  //   ���õ��ļ��ȶ������ BCompare���ܺ��ã�ǿ���Ƽ�
  //��������������������������������
     return 0;
	}
