#if !defined  SREAM_BUFFER_H_HIT_CPP
#define SREAM_BUFFER_H_HIT_CPP
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cstring>
#include <queue>
#include <utility>
using namespace std;

class StreamBuffer
{
 public:
    StreamBuffer();
    ~StreamBuffer();
    StreamBuffer(int iLen);
	int ReceiveDate(unsigned int offset, unsigned int bytes, char *pData);
	int ContinueBytes(unsigned int &iDataOffset, char* &pData);
	int RemoveData(int iBytes);

 private:
    char *m_pData;//�����ݵ�buffer��Ҳ������ָ��
    int m_iBufferLen;//����������
    unsigned int m_offset;//�������������ļ��е�ƫ����
    char *head, *tail;  // head��tailΪ��ǰ�������ݵ���βָ��
    priority_queue <pair <unsigned int, unsigned int>, vector<pair <unsigned int, unsigned int> >, greater<pair <unsigned int, unsigned int> > > packInfo;
    //������ѧ�ġ���
    // ʹ�����ȶ��м�¼���ݰ���Ϣ��������Ԫ��Ϊpair��
    // pair���������ݷֱ�Ϊ���ݰ�ƫ���������ݰ���С
    // ���ݰ�ƫ������С������ʼ���ڶ���(���ȶ����ص�)
};

#endif
