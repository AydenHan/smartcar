#ifndef _MK60_FLASH_H_
#define _MK60_FLASH_H_

#include "common.h"

#if defined(MK60DZ10)
#define     FLASH_SECTOR_SIZE       (2*1024)                //������С Ϊ 2k �ֽ�
#define     FLASH_SECTOR_NUM        (256)                   //������

#define     FLASH_ALIGN_ADDR        4                       //��ַ����������
typedef     uint32                  FLASH_WRITE_TYPE;       //flash_write ����д�� ����������


#elif defined(MK60F15)
#define     FLASH_SECTOR_SIZE       (4*1024)                //������С Ϊ 4k �ֽ�
#define     FLASH_SECTOR_NUM        (128)                   //������

#define     FLASH_ALIGN_ADDR        8                       //��ַ����������
typedef     uint64                  FLASH_WRITE_TYPE;       //flash_write ����д�� ����������

#endif


__RAMFUNC    void    flash_init();                                                                               //��ʼ��Flash

__RAMFUNC    uint8   flash_erase_sector  (uint16 sectorNo);                                                      //����ָ��flash����

__RAMFUNC    uint8   flash_write         (uint16 sectorNo, uint16 offset, FLASH_WRITE_TYPE data);                //д��flash����
__RAMFUNC    uint8   flash_write_buf     (uint16 sectorNo, uint16 offset, uint16 cnt, uint8 buf[]);              //�ӻ�����д��flash����

#define     flash_read(sectorNo,offset,type)        (*(type *)((uint32)(((sectorNo)*FLASH_SECTOR_SIZE) + (offset))))          //��ȡ����
#endif //_FLASH_H_