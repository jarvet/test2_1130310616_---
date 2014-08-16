#include<iostream>
#include<stdio.h>
#include <stdlib.h>
#include "StreamBuffer.h"// for Code::Block

using namespace std;
//-------------------
 StreamBuffer::StreamBuffer()
 {
 //  to do��  some initialization work
   m_pData = head = tail = new char[512*1024];
   m_iBufferLen = 512*1024;
   m_offset = 0;

 }
  StreamBuffer::StreamBuffer(int iLen)
 {
 //  to do��  some initialization work
   m_pData =head = tail =  new char[iLen];
   m_iBufferLen = iLen;
   m_offset = 0;
 }
StreamBuffer::~StreamBuffer()
 {
 //  to do��  some initialization work
  if( NULL !=  m_pData)
   delete []m_pData;

 }

 /*
���ܣ����ղ������ݴ��뻺����
������
    offset���������ļ��е�ƫ����
    bytes������������ֽ���
    pData��ָ�����ݵ�ָ��
����ֵ���������ݵ��ֽ���
*/
int StreamBuffer::ReceiveDate(unsigned int offset, unsigned int bytes, char *pData)
{

    unsigned int iBytes = 0;


    if (offset + bytes > m_offset + m_iBufferLen)// ���������ռ䲻��������ǰ��
    {
        memcpy(m_pData, head, m_iBufferLen - (head - m_pData));
        tail -= head - m_pData;
        m_offset += head - m_pData;
        head = m_pData;
    }


    for ( ; iBytes<bytes; iBytes++)
        m_pData[iBytes+offset-m_offset] = pData[iBytes];//���뻺����

    packInfo.push(make_pair(offset, bytes));    // �����ݰ���Ϣ�������ȶ���

   return iBytes;// bytes the buffer saved
}

/*
���ܣ���ȡ���������ź�������ݳ��ȡ���һ���ֽڵ�ƫ����������ָ��
������
    iDataOffset�����ڷ����ź�������ݿ��е�һ���ֽڵ�ƫ������ֵ
    pData:����ָ��
����ֵ�����������ź�������ݳ���
*/
int StreamBuffer::ContinueBytes(unsigned int &iDataOffset, char* &pData)
{
  int iContinueBytes = 0;

    // ���ȶ��еĶ���Ԫ�ؼ�¼������ƫ������С�����ݰ���Ϣ
    // ��ǰ�������ֺ����ȶ��ж�������¼�����ݰ�����ʱ�����µ�ǰ�������֣�����������Ԫ��
    while ((m_offset + (tail - m_pData)) == packInfo.top().first)
    {
        tail += packInfo.top().second;
        packInfo.pop();
    }

    iDataOffset = m_offset + (head - m_pData);//����ƫ����
    pData = head;

    iContinueBytes = tail-head;//�������ֳ��ȣ�����βָ��ָ��ĳ���

  return iContinueBytes;
};

/*
���ܣ��ӻ�������ɾ������
������
    iBytes��ɾ�����ֽ���
����ֵ��ɾ�����ֽ���
*/
int StreamBuffer::RemoveData(int iBytes)
{
  int iBytesRemoved=0;

    //�ӻ������а�����"ɾ��"

    iBytesRemoved=iBytes;
    head += iBytes;

   return iBytesRemoved;
};
