#ifndef _DELAY_H_
#define _DELAY_H_

#include "COMMON.h"

// ��������������� ������� ��� ���������� ��������
void delay_func_mcs( unsigned short count);

// ��������������� ������� ��� ���������� ��������
void delay_func_ms( unsigned short count);

// ������ ������ ��������� � ������� ��������� ������� �����, 
// ������� � �������� ���������� �����������
// ����������� �������� ����� - 24 ����� = 24 * TACT_TIME_MCS ���
// �������� ��� ������� ���������� 14.7456 - ����������� �������� 2 ���
#define DELAY_MCS( mcs ) delay_func_mcs( ( mcs - 17 * TACT_TIME_MCS ) / ( 6 * TACT_TIME_MCS ) );

// �������� ������ ������� +- 0.1 �� 
// ����������� ������� � ������� ���������� ����� �� ������
#define DELAY_MS( ms ) delay_func_ms( ( 1000.0 * ms - 28 * TACT_TIME_MCS ) / ( 1472 * TACT_TIME_MCS ) );

#endif
