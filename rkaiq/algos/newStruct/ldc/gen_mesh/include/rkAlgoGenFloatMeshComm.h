#ifndef __RKALGO_GEN_FLOAT_MESH_COMM_H__
#define __RKALGO_GEN_FLOAT_MESH_COMM_H__

#include "rkAlgoGenMeshComm.h"

/* ���������ʽ(rho-tanTheta)��ϵ�� */
void calcPolyTanCoeffInv(double a0, double a2, double a3, double a4, double radius,
                         int* invPolyTanNum, double* invPolyTanCoeff);

/* ���������ʽ(rho-cotTheta)��ϵ�� */
void calcPolyCotCoeffInv(double a0, double a2, double a3, double a4, double radius,
                         int* invPolyCotNum, double* invPolyCotCoeff);

/*
    [Function]      : �ж������Ƿ�Ϊ2���ݴ�
    [Author Name]   : Sunbeam Huang
    [Build Date]    : 2024-04-10
    [input]         : n: ��������
    [output]        : RKALGO_BOOL_E: ������
*/
RKALGO_BOOL_E isPowerOfTwo(uint32_t n);

/*
    [Function]      : ��3*3������г����б任������
    [Author Name]   : Sunbeam Huang
    [Build Date]    : 2022-07-18
    [input]         : srcMatrix: �������
    [output]        : matrixInv: ��������
*/
void calcMatrixInverse(const float srcMatrix[9], float matrixInv[9]);

/*
    [Function]      : �²��������˫���Բ�ֵΪȫ�ֱ��ʸ����(��������Ҫ��1)
    [Author Name]   : Sunbeam Huang
    [Build Date]    : 2024-03-15
*/
void floatMeshUpSample(const float* pSmallMapx, /* [input]: mapxС�� */
                       const float* pSmallMapy, /* [input]: mapyС�� */
                       float* pBigMapx,         /* [output]: mapx��� */
                       float* pBigMapy,         /* [output]: mapy��� */
                       int srcWid,              /* [input]: С��Ŀ� */
                       int srcHgt,              /* [input]: С��ĸ� */
                       int dstWid,              /* [input]: ���Ŀ���Ҫ��1 */
                       int dstHgt,              /* [input]: ���ĸߣ���Ҫ��1 */
                       int meshStepW,           /* [input]: С��Ŀ��� */
                       int meshStepH            /* [input]: С��ĸ߲��� */
);

#endif  // !__RKALGO_GEN_FLOAT_MESH_COMM_H__
