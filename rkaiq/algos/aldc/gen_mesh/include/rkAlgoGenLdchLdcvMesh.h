#ifndef __RKALGO_GEN_LDCH_LDCV_MESH_H__
#define __RKALGO_GEN_LDCH_LDCV_MESH_H__
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
// #include <opencv2/opencv.hpp>


/*
* @struct   RKALGO_LDCH_LDCV_INFO_S
* @brief    define the RK LDCH and LDCV info
*/
typedef struct RKALGO_LDCH_LDCV_INFO
{
    uint32_t srcW;                                      /* ����ͼ��� */
    uint32_t srcH;                                      /* ����ͼ��� */
    uint32_t dstW;                                      /* ���ͼ��� */
    uint32_t dstH;                                      /* ���ͼ��� */
    RKALGO_LDCH_LDCV_VERSION_E enLdchLdcvVersion;       /* LDCH��LDCV�İ汾 */
    /* LDCH���㻯mesh�����Ϣ */
    uint32_t ldchDstWidAlign;                           /* LDCHҪ��ԭʼȫ�ֱ��ʸ���map��������16������ + 1 */
    uint32_t ldchDstHgtAlign;                           /* LDCHҪ��ԭʼȫ�ֱ��ʸ���map��������8������ + 1 */
    uint32_t ldchMeshStepW;                             /* LDCH���㻯mesh���� */
    uint32_t ldchMeshStepH;                             /* LDCH���㻯mesh�߲��� */
    uint32_t ldchMeshW;                                 /* LDCH���㻯mesh��Ŀ� */
    uint32_t ldchMeshH;                                 /* LDCH���㻯mesh��ĸ� */
    uint32_t ldchAlignMeshW;                            /* mesh��Ŀ���2���� */
    uint32_t ldchFixBit;                                /* С�����㻯λ�� */
    uint64_t u64LdchMeshBufSize;                        /* LDCH���㻯mesh�������buffer��С */
    /* LDCV���㻯mesh�����Ϣ */
    uint32_t ldcvDstWidAlign;                           /* LDCVҪ��ԭʼȫ�ֱ��ʸ���map��������16������ + 1 */
    uint32_t ldcvDstHgtAlign;                           /* LDCVҪ��ԭʼȫ�ֱ��ʸ���map��������16������ + 1 */
    uint32_t ldcvMeshStepW;                             /* LDCV���㻯mesh���� */
    uint32_t ldcvMeshStepH;                             /* LDCV���㻯mesh�߲��� */
    uint32_t ldcvMeshW;                                 /* LDCV���㻯mesh��Ŀ� */
    uint32_t ldcvMeshH;                                 /* LDCV���㻯mesh��ĸ�(������0�д������) */
    uint32_t ldcvFixBit;                                /* С�����㻯λ�� */
    uint64_t u64LdcvMeshBufSize;                        /* LDCV���㻯mesh�������buffer��С */

}RKALGO_LDCH_LDCV_INFO_S;


/*
    [Function]      : ����LDCH��LDCV�İ汾������LDCH��LDCV���㻯mesh�������buffer��С
    [Author Name]   : Sunbeam Huang
    [Build Date]    : 2023-06-30
    [input]         : pstLdchLdcvInfo->enLdchLdcvVersion: ָ��LDCH��LDCV�汾: rk3576, etc.
                      pstLdchLdcvInfo->srcW, pstLdchLdcvInfo->srcH: ����ͼ��Ŀ��
                      pstLdchLdcvInfo->dstW, pstLdchLdcvInfo->dstH: ���ͼ��Ŀ��
    [output]        : pstLdchLdcvInfo: LDCH��LDCV���㻯mesh��������Ϣ
*/
int32_t calcLdchLdcvMeshSize(RKALGO_LDCH_LDCV_INFO_S *pstLdchLdcvInfo);

/*
    [Function]      : �ֲ�У����ȫ�ֱ��ʸ���� ---> LDCH��LDCV�Ķ��㻯mesh��
    [Author Name]   : Sunbeam Huang
    [Build Date]    : 2023-06-30
    [input]         : pstLdchLdcvInfo: LDCH��LDCV���㻯��������Ϣ
                      bigMapWid, bigMapWid: �ֲ�У����ȫ�ֱ��ʸ����Ŀ��
                      newBigMapX, newBigMapY: �ֲ�У����ȫ�ֱ��ʸ����
    [output]        : pu16MeshX, pu16MeshY: LDCH��LDCV�Ķ��㻯mesh��
*/
/*
 * int32_t genLdchLdcvMeshFromBigMap(
 *     const RKALGO_LDCH_LDCV_INFO_S *pstLdchLdcvInfo,
 *     const uint32_t bigMapWid, const uint32_t bigMapHgt,
 *     const cv::Mat &newBigMapX, const cv::Mat &newBigMapY,
 *     uint16_t *pu16MeshX, uint16_t *pu16MeshY
 * );
 */

/*
    [Function]      : LDCH/LDCV�²�������� ---> LDCH/LDCV���㻯��
    [Author Name]   : Sunbeam Huang
    [Build Date]    : 2023-07-06
    [input]         : pstLdchLdcvInfo: LDCH/LDCV���㻯��������Ϣ
                      ldchFloatMeshW, ldchFloatMeshH: LDCH�²��������Ŀ��
                      ldcvFloatMeshW, ldcvFloatMeshH: LDCV�²��������Ŀ��
                      pf32MapxNew, pf32MapyNew: ������²��������
    [input/output]  : pu16MeshX, pu16MeshY: �����LDCH/LDCV�Ķ��㻯��, ����buffer��ָ���Ƿ�Ϊ��, �������������ϣ����䲻ͬ����:
                                (1)NULL!=pf32MapxNew && NULL!=pu16MeshX: ���������LDCH�Ķ��㻯��;
                                (2)NULL!=pf32MapyNew && NULL!=pu16MeshY: ���������LDCV�Ķ��㻯��;
                                (3)����(1)��(2)����ͬʱ�����
*/
int32_t genLdchLdcvMeshFromFloatMesh(
    const RKALGO_LDCH_LDCV_INFO_S *pstLdchLdcvInfo,
    const uint32_t ldchFloatMeshW, const uint32_t ldchFloatMeshH,
    const uint32_t ldcvFloatMeshW, const uint32_t ldcvFloatMeshH,
    const float *pf32MapxNew, const float *pf32MapyNew,
    uint16_t *pu16MeshX, uint16_t *pu16MeshY
);

// clang-format on
#endif  // !__RKALGO_GEN_LDCH_LDCV_MESH_H__
