#ifndef __RKALGO_GEN_FLOAT_MESH_H__
#define __RKALGO_GEN_FLOAT_MESH_H__
// clang-format off

#include "rkAlgoGenMeshComm.h"
#include "rkAlgoGenFloatMeshFromRK.h"

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
    [Function]      : ���ݲ������������²����ĸ���mesh��Ŀ��
    [Author Name]   : Sunbeam Huang
    [Build Date]    : 2023-02-16
    [input]         : enMeshType: ָ�������mesh������: DEBUG, FEC, LDCH, LDCV, etc., ��Ҫ�Ͷ��뷽ʽ�йء�
                      dstW, dstH: Ŀ�����ͼ�Ŀ��
                      meshStepW, meshStepH: ������������meshStepW == 1 && meshStepH == 1ʱ���˻���ȫ�ֱ��ʵĸ���mesh��
    [output]        : meshW, meshH: ����mesh��Ŀ��
*/
int32_t calcFloatMeshSize(
    const RKALGO_MESH_TYPE_E enMeshType, const uint32_t dstW, const uint32_t dstH,
    const uint32_t meshStepW, const uint32_t meshStepH, uint32_t *meshW, uint32_t *meshH
);


int32_t calcFloatMeshSizeRK(RKALGO_GEN_FLOAT_MESH_INFO_S *genFloatMeshInfo);

// clang-format on
#endif  // !__RKALGO_GEN_FLOAT_MESH_H__
