#ifndef _DS1307_H_
#define _DS1307_H_

#include "PINDEF.H"

// ������� ������ �������� ������ ���������� DS1307
void DS1307_write(unsigned char addr, unsigned char value);

// ������� ������ �������� ������ ���������� DS1307
unsigned char DS1307_read(unsigned char addr);

// ������� ��������� �������� �������
// ����� ������ timestr ������ ���� ����� 9
void DS1307_gettime(char* timestr);

// ������� ������� �������� �������
void DS1307_settime( unsigned char hour, unsigned char minute, unsigned char second );

// ������� ��������� �������� ����
// ����� ������ datestr ������ ���� ����� 9
void DS1307_getdate(char* datestr);

// ������� ������� �������� ����
void DS1307_setdate( unsigned char day, unsigned char month, unsigned char year );

#endif
