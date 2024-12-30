#ifndef __RKALGO_POLYNOMIAL_H__
#define __RKALGO_POLYNOMIAL_H__

#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* �����ṹ�� */
typedef struct doubleComplex {
    double re;  // ʵ��
    double im;  // �鲿
} RkAlgoDoubleComplex;

/*
    [Function]      : ����ʵϵ�����η���0 = a0 + a1 * x + a2 * x^2 + a3 * x^3��ʵ��
    [Author Name]   : Sunbeam Huang
    [Build Date]    : 2024-04-16
    [input]         : cubeCoefs: ���η��̵�ʵϵ��������a0, a1, a2, a3˳����������
    [output]        : cubeRoots: ���η��̵�ʵ��
*/
int solveCubic(const double* cubeCoefs, double* cubeRoots);

/*
    [Function]      : ���㸴ϵ��N�׶���ʽ����0 = a0 + a1 * x + a2 * x^2 + a3 * x^3 + ... + an *
   x^n�ĸ����� ����ԭ��ʹ��Durand-Kerner���� [Author Name]   : Sunbeam Huang [Build Date]    :
   2024-04-16 [input]         : polyRank: ���̵���߽��� maxIters: ���������� cpxCoefs:
   ���̵ĸ�ϵ��������a0, a1, a2, a3, ... , an˳���������� [output]        : cpxRoots:
   ���̵�N��������
*/
void solveComplexPolyFunc(int polyRank, int maxIters, const RkAlgoDoubleComplex* cpxCoefs,
                          RkAlgoDoubleComplex* cpxRoots);

/*
    [Function]      : ���ʵϵ��N�׶���ʽ����y = a0 + a1 * x + a2 * x^2 + a3 * x^3 + ... + an *
   x^n��ϵ�� ����ԭ��At * A * C = At * Y������AtΪA��ת�þ���CΪϵ������ [Author Name]   :
   Sunbeam Huang [Build Date]    : 2024-04-16 [input]         : xData: ���ߵ�x�������� yData:
   ���ߵ�y�������� dataNum: x-y�������ݵĶ��� polyRank: ���ߵ���߽��� [output]        : realCoefs:
   ���ߵ�ʵϵ��������a0, a1, a2, a3, ... , an˳����������
*/
void fitRealPolyCurve(const double* xData, const double* yData, int dataNum, int polyRank,
                      double* realCoefs);

/*
    [Function]      : ����N�׶���ʽ����y = a0 + a1 * x + a2 * x^2 + a3 * x^3 + ... + an * x^n��ֵ
    [Author Name]   : Sunbeam Huang
    [Build Date]    : 2024-04-16
    [input]         : realCoefs: ���ߵ�ʵϵ��������a0, a1, a2, a3, ... , an˳����������
                      xData: ����x����
                      polyRank: ���ߵ���߽���
    [output]        : yData: ����y����
*/
void evalRealPolyCurve(const double* realCoefs, int polyRank, double xData, double* yData);

#endif  // !__RKALGO_POLYNOMIAL_H__
