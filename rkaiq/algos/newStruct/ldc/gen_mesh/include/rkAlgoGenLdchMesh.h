#ifndef __RKALGO_GEN_LDCH_MESH_H__
#define __RKALGO_GEN_LDCH_MESH_H__

#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rkAlgoGenMeshComm.h"

#ifdef __cplusplus
#include <algorithm>
#include <string>
#endif  // __cplusplus

/*
    [Function]      : ����LDCH�İ汾������LDCH mesh�������buffer��С
    [Author Name]   : Sunbeam Huang
    [Build Date]    : 2023-02-16
    [input]         : enLdchVersion: ָ��LDCH�汾: 1109, 1126, 3588, etc.
                      dstW, dstH: ���ͼ��Ŀ��
    [output]        : u64LdchMeshBufSize: ��ʾLDCH mesh�������buffer��С
*/
int32_t calcLdchMeshSize(const RKALGO_LDCH_VERSION_E enLdchVersion, const uint32_t dstW,
                         const uint32_t dstH, uint64_t* pu64LdchMeshBufSize);

/*
    [Function]      : float���͵�mesh��ת����LDCH mesh��
    [Author Name]   : Sunbeam Huang
    [Build Date]    : 2023-02-16
    [input]         : enLdchVersion: ָ��LDCH�汾��1109, 1126, 3588, etc.
                      srcW, srcH: ����ͼ�Ŀ��
                      dstW, dstH: ���ͼ��Ŀ��
                      pf32MeshX: float���͵�mesh��
    [output]        : pu16MeshX: LDCH mesh��
*/
int32_t genLdchMeshFromFloatMesh(const RKALGO_LDCH_VERSION_E enLdchVersion, const uint32_t srcW,
                                 const uint32_t srcH, const uint32_t dstW, const uint32_t dstH,
                                 const float* pf32MeshX, uint16_t* pu16MeshX);

/*
    [Function]      : �����޻���У����LDCH mesh��
    [Author Name]   : Sunbeam Huang
    [Build Date]    : 2023-03-06
    [input]         : enLdchVersion: ָ��LDCH�汾��1109, 1126, 3588, etc.
                      srcW, srcH: ����ͼ�Ŀ��
                      dstW, dstH: ���ͼ��Ŀ��
    [output]        : pu16MeshX: LDCH mesh��
*/
int32_t genLdchMeshZeroCorrect(const RKALGO_LDCH_VERSION_E enLdchVersion, const uint32_t srcW,
                               const uint32_t srcH, const uint32_t dstW, const uint32_t dstH,
                               uint16_t* pu16MeshX);

#endif  // !__RKALGO_GEN_LDCH_MESH_H__
