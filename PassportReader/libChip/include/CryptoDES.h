/*******************************************************************************
 * FileName: CryptoDES.h
 *   Author: wenlei����
 *     Date: 2010.12.30
 *  COMMENT: ֧�� DES  ���ܻ���ܹ���
 *           ֧�� 3DES �����ܱ���ʽ ���ܻ���ܹ���
 *           ֧�� DES CBC  ���ķ������ӷ�ʽ �����ݼ������ӣ����������
 *           ֧�� 3DES CBC ���ķ������ӷ�ʽ �����ݼ������ӣ����������
 *
 *           ʹ�ú궨�壬����ȡ������ֹ��ܡ�ע��3DES_CBC��Ҫ3DES��֧�֡�
 *
 ******************************************************************************/
#ifndef _CRYPTO_DES_H_
#define _CRYPTO_DES_H_

//------------------------------------------------------------------------------
// DES��3DES���ܻ���ܷ�ʽ
//------------------------------------------------------------------------------
#define CRYPTO_ENCRYPT		0x01		// ����
#define CRYPTO_DECRYPT		0x02		// ����


//------------------------------------------------------------------------------
// Functions Support
//------------------------------------------------------------------------------
#define CRYPTO_DES_ECB					// �����Ƿ�֧�ֺ�������
#define CRYPTO_DES_CBC
#define CRYPTO_3DES_ECB
#define CRYPTO_3DES_CBC

//****************************************************************************** 
// IN:		cMode						// CRYPTO_ENCRYPT = ����; CRYPTO_DECRYPT = ����.
// IN:		*cpKey						// 8�ֽ���Կ
// IN OUT:	*cpnData					// ���ܻ���ܵ�����
// RETURN:	void
// COMMENT:	DES���ܻ���ܹ���, �ⲿ���á�
//******************************************************************************
void Crypto_DES(unsigned char cMode, unsigned char *cpKey, unsigned char *cpData);


//******************************************************************************
// IN:		cMode						// CRYPTO_ENCRYPT = ����; CRYPTO_DECRYPT = ����.
// IN:		*cpKey						// 8�ֽ���Կ
// IN:		iInLen						// ���ܻ���ܵ����ݳ��ȣ�������8����������
// IN OUT:	*cpData						// ���ܻ�������ݵ�����ڣ�������ڡ�
// RETURN:	unsigned char 				// SUCCESS = 0x00; FAILURE = 0xFF.
// COMMENT:	DES ECB���ܻ���ܹ���, �ⲿ���á�
//******************************************************************************
unsigned char Crypto_DES_ECB(unsigned char cMode, unsigned char *cpKey, unsigned int iInLen, unsigned char *cpData);

//******************************************************************************
// IN:		cMode						// CRYPTO_ENCRYPT = ����; CRYPTO_DECRYPT = ����.
// IN:		*cpKey						// 8�ֽ���Կ
// IN OUT:	*cpIV						// ��ʼ����
// IN:		iInLen						// ���ܻ���ܵ����ݳ��ȣ�������8����������
// IN OUT:	*cpData						// ���ܻ�������ݵ�����ڣ�������ڡ�
// RETURN:	unsigned char 				// SUCCESS = 0x00; FAILURE = 0xFF.
// COMMENT: DES CBC���ܻ���ܹ���
//******************************************************************************
unsigned char Crypto_DES_CBC(unsigned char cMode, unsigned char *cpKey, unsigned char *cpIV, unsigned int iInLen, unsigned char *cpData);


//******************************************************************************
// IN:		cMode						// CRYPTO_ENCRYPT = ����; CRYPTO_DECRYPT = ����.
// IN:		*cpKey						// 16�ֽ���Կ
// IN:		iInLen						// ���ܻ���ܵ����ݳ��ȣ�������8����������
// IN OUT:	*cpData						// ���ܻ�������ݵ�����ڣ�������ڡ�
// RETURN:	unsigned char 				// SUCCESS = 0x00; FAILURE = 0xFF.
// COMMENT: 3DES ECB���ܻ���ܹ���
//******************************************************************************
unsigned char Crypto_3DES_ECB(unsigned char cMode, unsigned char *cpKey, unsigned int iInLen, unsigned char *cpData);


//******************************************************************************
// IN:		cMode						// ENCRYPT = ����; DECRYPT = ����.
// IN:		*cpKey						// 16�ֽ���Կ
// IN OUT:	*cpIV						// ��ʼ����
// IN:		iInLen						// ���ܻ���ܵ����ݳ��ȣ�������8����������
// IN OUT:	*cpData						// ���ܻ�������ݵ�����ڣ�������ڡ�
// RETURN:	unsigned char				// SUCCESS = 0; FAILURE = 1.
// COMMENT:	3DES��CBCģʽ
//******************************************************************************
unsigned char Crypto_3DES_CBC(unsigned char cMode, unsigned char *cpKey, unsigned char *cpIV, unsigned int iInLen, unsigned char *cpData);

#endif
