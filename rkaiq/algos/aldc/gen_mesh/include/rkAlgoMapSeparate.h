#ifndef __RKALGO_MAP_SEPARATE_H__
#define __RKALGO_MAP_SEPARATE_H__
// clang-format off

#include "rkAlgoMapSeparateVersion.h"
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <string>
#include <algorithm>


#if defined(_WIN32) || defined(__CYGWIN__)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif


#ifndef CLIP
#define CLIP(a, min, max) (((a) < (min)) ? (min) : (((a) > (max)) ? (max) : (a)))
#endif

/*
* @struct   RKALGO_SEPARATE_MESH_INFO_S
* @brief    define the info of map separation for RK LDCH and LDCV
*/
typedef struct RKALGO_SEPARATE_MESH_INFO
{
    uint32_t srcW;                                          /* ����ͼ��� */
    uint32_t srcH;                                          /* ����ͼ��� */
    uint32_t dstW;                                          /* ���ͼ��� */
    uint32_t dstH;                                          /* ���ͼ��� */
    uint32_t mapWidAlign;                                   /* ԭʼӳ���Ŀ���Ҫ16������ + 1 */
    uint32_t mapHgtAlign;                                   /* ԭʼӳ���ĸߣ���Ҫ16������ + 1 */
    /* LDCH�²�������mesh�����Ϣ */
    uint32_t ldchFloatMeshStepW;                            /* LDCH�²�������mesh���� */
    uint32_t ldchFloatMeshStepH;                            /* LDCH�²�������mesh�߲��� */
    uint32_t ldchFloatMeshW;                                /* LDCH�²�������mesh��Ŀ� */
    uint32_t ldchFloatMeshH;                                /* LDCH�²�������mesh��ĸ� */
    uint64_t u64LdchFloatMeshBufSize;                       /* LDCH�²�������mesh���buffer��С */
    /* LDCV�²�������mesh�����Ϣ */
    uint32_t ldcvFloatMeshStepW;                            /* LDCV�²�������mesh���� */
    uint32_t ldcvFloatMeshStepH;                            /* LDCV�²�������mesh�߲��� */
    uint32_t ldcvFloatMeshW;                                /* LDCV�²�������mesh��Ŀ� */
    uint32_t ldcvFloatMeshH;                                /* LDCV�²�������mesh��ĸ� */
    uint64_t u64LdcvFloatMeshBufSize;                       /* LDCV�²�������mesh���buffer��С */
}RKALGO_SEPARATE_MESH_INFO_S;

/*
    [Function]      : ������ز�����ԭʼȫ�ֱ��ʸ���� ---> �ֲ�У�����²�������� ����Ĳ�����Ϣ
    [Author Name]   : Sunbeam Huang
    [Build Date]    : 2023-07-06
    [input]         : srcW, srcH: ����ͼ��Ŀ��
                      dstW, dstH: ���ͼ��Ŀ��
    [output]        : pstMapSepaInfo: �����ز���
*/
EXPORT int32_t calcSeparateMeshInfo(
    const uint32_t srcW, const uint32_t srcH,
    const uint32_t dstW, const uint32_t dstH,
    RKALGO_SEPARATE_MESH_INFO_S *pstMapSepaInfo
);

/*
    [Function]      : ԭʼȫ�ֱ��ʸ���� ---> �ֲ�У�����²��������
    [Author Name]   : Sunbeam Huang
    [Build Date]    : 2023-07-05
    [input]         : pstMapSepaInfo: ���ת����ز���
                      pf32MapxOri, pf32MapyOri: ԭʼ����ȫ�ֱ��ʱ�
    [output]        : pf32MapxNew, pf32MapyNew: �ֲ�У�����²��������
*/
EXPORT int32_t genSeparateMeshFromOriMap(
    const RKALGO_SEPARATE_MESH_INFO_S *pstMapSepaInfo,
    const float *pf32MapxOri, const float *pf32MapyOri,
    float *pf32MapxNew, float *pf32MapyNew
);

// clang-format on
#endif  // !__RKALGO_MAP_SEPARATE_H__
