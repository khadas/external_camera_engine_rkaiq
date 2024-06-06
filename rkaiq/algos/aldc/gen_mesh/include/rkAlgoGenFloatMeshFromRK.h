#ifndef __RKALGO_GEN_FLOAT_MESH_FROM_RK_H__
#define __RKALGO_GEN_FLOAT_MESH_FROM_RK_H__
// clang-format off

#include "rkAlgoGenMeshComm.h"
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <string>
#include <algorithm>

#define INV_POLY_COEFF_NUM 21                               /* ����ʽϵ������, ��ߴ���(INV_POLY_COEFF_NUM-1)�� */

/* ������� */
typedef struct RKALGO_CAM_COEFF_INFO
{
    double cx, cy;                                          /* ��ͷ�Ĺ��� */
    double a0, a2, a3, a4;                                  /* ��ͷ�Ļ���ϵ�� */
    double c, d, e;                                         /* �ڲ�[c d;e 1] */
    double sf;                                              /* sf�����ӽ�, sfԽ���ӽ�Խ�� */

    int invPolyTanNum0;                                     /* level = 0ʱ��rho-tanTheta����ʽ��ϵ������ */
    double invPolyTanCoeff0[INV_POLY_COEFF_NUM];            /* level = 0ʱ��rho-tanTheta����ʽϵ��, ��ߴ���(INV_POLY_COEFF_NUM-1)�� */

    int invPolyCotNum0;                                     /* level = 0ʱ��rho-cotTheta����ʽ��ϵ������ */
    double invPolyCotCoeff0[INV_POLY_COEFF_NUM];            /* level = 0ʱ��rho-cotTheta����ʽϵ��, ��ߴ���(INV_POLY_COEFF_NUM-1)�� */

    int invPolyTanNum255;                                   /* level = 255ʱ��rho-tanTheta����ʽ��ϵ������ */
    double invPolyTanCoeff255[INV_POLY_COEFF_NUM];          /* level = 255ʱ��rho-tanTheta����ʽϵ��, ��ߴ���(INV_POLY_COEFF_NUM-1)�� */

    int invPolyCotNum255;                                   /* level = 255ʱ��rho-cotTheta����ʽ��ϵ������ */
    double invPolyCotCoeff255[INV_POLY_COEFF_NUM];          /* level = 255ʱ��rho-cotTheta����ʽϵ��, ��ߴ���(INV_POLY_COEFF_NUM-1)�� */
}RKALGO_CAM_COEFF_INFO_S;

typedef struct RKALGO_CAM_COEFF_ONE_LEVEL_INFO
{
    double cx, cy;                                          /* ��ͷ�Ĺ��� */
    double a0, a2, a3, a4;                                  /* ��ͷ�Ļ���ϵ�� */
    double c, d, e;                                         /* �ڲ�[c d;e 1] */
    double sf;                                              /* sf�����ӽ�, sfԽ���ӽ�Խ�� */
    uint32_t level;                                         /* ��ǰ����У���ȼ� */
    int invPolyTanNum;                                      /* rho-tanTheta����ʽ��ϵ������ */
    double invPolyTanCoeff[INV_POLY_COEFF_NUM];             /* level = 0ʱ��rho-tanTheta����ʽϵ��, ��ߴ���(INV_POLY_COEFF_NUM-1)�� */
    int invPolyCotNum;                                      /* rho-cotTheta����ʽ��ϵ������ */
    double invPolyCotCoeff[INV_POLY_COEFF_NUM];             /* level = 0ʱ��rho-cotTheta����ʽϵ��, ��ߴ���(INV_POLY_COEFF_NUM-1)�� */
}RKALGO_CAM_COEFF_ONE_LEVEL_INFO_S;

typedef struct RKALGO_GEN_FLOAT_MESH_INFO
{
    uint32_t srcW;                                          /* ����ͼ��� */
    uint32_t srcH;                                          /* ����ͼ��� */
    uint32_t dstW;                                          /* ���ͼ��� */
    uint32_t dstH;                                          /* ���ͼ��� */
    uint32_t mapStepW;                                      /* ����ӳ���Ĳ������� */
    uint32_t mapStepH;
    uint32_t mapW;                                          /* ����ӳ���Ŀ� */
    uint32_t mapH;                                          /* ����ӳ���ĸ� */
    uint64_t u64MapBufSize;                                 /* ����ӳ����buffer��С */
    uint32_t maxLevel;                                      /* Ӳ��������У���Ļ���ȼ� */
    uint32_t correctX;										/* ˮƽx����У��: 1����У��, 0����У�� */
    uint32_t correctY;										/* ��ֱy����У��: 1����У��, 0����У�� */
    uint32_t saveMaxFovX;									/* ����ˮƽx�������FOV: 1������, 0�������� */
    RKALGO_MESH_TYPE_E enMeshType;                          /* ������Ҫ�Ķ��㻯������ͣ�FEC/LDCH/LDCV */
    RKALGO_FEC_VERSION_E enFecVersion;                      /* Ӳ��FEC�汾������rv1109, rv1126��rk3588 */
    RKALGO_LDCH_VERSION_E enLdchVersion;                    /* Ӳ��LDCH�汾(��û��LDCV)������rv1109��rv1126��rk356x��rk3588��1106��rk3562 */
    RKALGO_LDCH_LDCV_VERSION_E enLdchLdcvVersion;           /* Ӳ��LDCH + LDCV�汾������rk3576 */
    double cropStepW[2000], cropStepH[2000];
    double cropStartW[2000], cropStartH[2000];
}RKALGO_GEN_FLOAT_MESH_INFO_S;


/* �ⲿ�ӿڣ������²��������֮ǰ����ز�����ʼ�� */
int32_t genFloatMeshNLevelInit(
    RKALGO_CAM_COEFF_INFO_S *camCoeff,
    RKALGO_GEN_FLOAT_MESH_INFO_S *genFloatMeshInfo
);

/* �ⲿ�ӿڣ����ɲ�ͬУ���̶ȵ��²�������������������� */
int32_t genFloatMeshNLevel(
    const RKALGO_CAM_COEFF_INFO_S *camCoeff,
    const RKALGO_GEN_FLOAT_MESH_INFO_S *genFloatMeshInfo,
    uint32_t level,
    float *pf32MapxOri, float *pf32MapyOri
);

// clang-format on
#endif  // !__RKALGO_GEN_FLOAT_MESH_FROM_RK_H__
