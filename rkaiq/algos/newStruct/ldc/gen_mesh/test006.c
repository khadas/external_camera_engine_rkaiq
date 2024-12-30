
#include "../rkAlgoGenMesh/rkAlgoGenFloatMeshRK/rkAlgoGenFloatMeshFromRK.h" /* ���ܣ�RK���� ---> ����� */
#include "../rkAlgoGenMesh/rkAlgoGenLdchLdcvMesh/rkAlgoGenLdchLdcvMesh.h" /* ���ܣ������ ---> LDCH+LDCV���㻯�� */
#include "../rkAlgoGenMesh/rkAlgoGenLdchMesh/rkAlgoGenLdchMesh.h" /* ���ܣ������ ---> LDCH���㻯�� */
#include "../rkAlgoMapSeparateLib/include/rkAlgoMapSeparate.h" /* ���ܣ�ȫ�ֱ��ʸ���� ---> ����У���²���������˲��ֲ��ṩԴ�룬ֻ�ṩ�� */

/* ������Windows C++ ����������֤Ч�� */
#if defined(__cplusplus) && (defined(WIN32) || defined(_WIN32))
#include <opencv2/opencv.hpp>
#endif

/* �ּ�У��-LDCH on + LDCV on: RK���� ---> �²�������� ---> ȫ�ֱ��ʸ���� --->
 * ����У���²�������� ---> LDCH��LDCV���㻯�� */
void test006_1() {
    printf("%s:\n", __func__);

    /* �������ձ�����ļ���·�� */
    rkAlgoCreatFileDir(__func__);

    /* ��ʱ��� */
    uint64_t t0, t1, t2, t3, t4, t5, t6;       /* ʱ�� */
    float ctAll, ct1, ct2, ct3, ct4, ct5, ct6; /* ��ʱ */
    ctAll = ct1 = ct2 = ct3 = ct4 = ct5 = ct6 = 0;

    int32_t ret     = 0;
    uint32_t srcWid = 2688; /* ����ͼ���� */
    uint32_t srcHgt = 1520;
    uint32_t dstWid = 2688; /* ���ͼ���� */
    uint32_t dstHgt = 1520;

    uint32_t level        = 0;                      /* �ּ�У����level����Χ: 0-255 */
    uint32_t levelValue[] = {0, 64, 128, 192, 255}; /* �ּ�У����levelȡֵ */
    uint32_t levelNum     = sizeof(levelValue) / sizeof(levelValue[0]); /* �ּ�У����level�ĸ��� */
    printf("%s: levelNum = %d\n", __func__, levelNum);

    char saveFilePrefix[64] = "OS04A10_6IR"; /* �����ļ�����ǰ׺ */

    /* RKģ�Ͳ��� */
    RKALGO_CAM_COEFF_INFO_S camCoeff;
    /* group_010_OS04A10_4IR_2688x1520 */
    // camCoeff.a0 =
    // -1830.2636074851754983683349564671516418457031250000000000000000000000000000000000000000000000000000000000;
    // camCoeff.a2 =
    // 0.0004237951284061040315917501253295540664112195372581481933593750000000000000000000000000000000000000;
    // camCoeff.a3 =
    // -0.0000002507674855122118848025746917079770170744268398266285657882690429687500000000000000000000000000;
    // camCoeff.a4 =
    // 0.0000000001272470035935948196069050985539572624916093701585850794799625873565673828125000000000000000;
    // camCoeff.cx =
    // 1351.1178239999999277642928063869476318359375000000000000000000000000000000000000000000000000000000000000;
    // camCoeff.cy =
    // 739.4860800000000153886503539979457855224609375000000000000000000000000000000000000000000000000000000000;

    /* group_011_OS04A10_6IR_2688x1520 */
    camCoeff.a0 =
        -2653.9299617503911576932296156883239746093750000000000000000000000000000000000000000000000000000000000000;
    camCoeff.a2 =
        0.0001171174803799108549888854935439042037614854052662849426269531250000000000000000000000000000000000;
    camCoeff.a3 =
        -0.0000000150166870224219489918825648283248752612450971355428919196128845214843750000000000000000000000;
    camCoeff.a4 =
        0.0000000000087465891629053077103521832658175594581856371334538380324374884366989135742187500000000000;
    camCoeff.cx =
        1333.2695039999998698476701974868774414062500000000000000000000000000000000000000000000000000000000000000;
    camCoeff.cy =
        728.1043200000000297222868539392948150634765625000000000000000000000000000000000000000000000000000000000;

    RKALGO_MESH_TYPE_E enMeshType;     /* ������Ҫ����Ķ��㻯������ͣ�FEC/LDCH/LDCV */
    RKALGO_FEC_VERSION_E enFecVersion; /* Ӳ��FEC�汾������rv1109, rv1126��rk3588 */
    RKALGO_LDCH_VERSION_E enLdchVersion; /* Ӳ��LDCH�汾(��û��LDCV)������rv1109��rv1126��rk356x��rk3588��1106��rk3562
                                          */
    RKALGO_LDCH_LDCV_VERSION_E enLdchLdcvVersion; /* Ӳ��LDCH + LDCV�汾������rk3576 */

    uint32_t ldchFloatMeshW, ldchFloatMeshH; /* LDCH�²��������Ŀ�� */
    uint32_t ldcvFloatMeshW, ldcvFloatMeshH; /* LDCV�²��������Ŀ�� */

    RKALGO_GEN_FLOAT_MESH_INFO_S genFloatMeshInfo;  /* �²�����������ز��� */
    RKALGO_SEPARATE_MESH_INFO_S stSeparateMeshInfo; /* �ֲ�У���²����������ز��� */
    RKALGO_LDCH_LDCV_INFO_S stLdchLdcvInfo;         /* LDCH��LDCV���㻯����ز��� */

    float* pf32MeshxDn  = NULL; /* �²�������� */
    float* pf32MeshyDn  = NULL;
    float* pf32MeshxUp  = NULL; /* �ϲ������ȫ�ֱ��ʸ���� */
    float* pf32MeshyUp  = NULL;
    float* pf32MeshX    = NULL; /* ���ת������²�������� */
    float* pf32MeshY    = NULL;
    uint16_t* pu16MeshX = NULL; /* LDCH���㻯�� */
    uint16_t* pu16MeshY = NULL; /* LDCV���㻯�� */

    uint32_t mapUpWid, mapUpHgt; /* �ϲ������ȫ�ֱ��ʸ������ */
    uint64_t mapUpBufSize;

    /* ����������Ҫ����Ķ��㻯������� */
    enMeshType    = RKALGO_MESH_TYPE_LDCH_LDCV; /* ���ö��㻯������� */
    enFecVersion  = RKALGO_FEC_VERSION_NO_NEED; /* ����Ӳ��FEC�汾��������Ҫ�ı�������ΪNO_NEED */
    enLdchVersion = RKALGO_LDCH_VERSION_1;      /* ����Ӳ��LDCH�汾��������Ҫ�ı�������ΪNO_NEED */
    enLdchLdcvVersion =
        RKALGO_LDCH_LDCV_VERSION_3576; /* ����Ӳ��LDCH + LDCV�汾��������Ҫ�ı�������ΪNO_NEED */

    /* �����²�����������ز��� */
    genFloatMeshInfo.enMeshType        = enMeshType;
    genFloatMeshInfo.enFecVersion      = enFecVersion;
    genFloatMeshInfo.enLdchVersion     = enLdchVersion;
    genFloatMeshInfo.enLdchLdcvVersion = enLdchLdcvVersion;
    genFloatMeshInfo.srcW              = srcWid;
    genFloatMeshInfo.srcH              = srcHgt;
    genFloatMeshInfo.dstW              = dstWid;
    genFloatMeshInfo.dstH              = dstHgt;
    genFloatMeshInfo.mapStepW =
        16; /* �����²��������Ĳ����������˴�����LDCH+LDCVҪ�����ò������� */
    genFloatMeshInfo.mapStepH    = 8;
    genFloatMeshInfo.correctX    = 1; /* ˮƽx����У��: 1����У��, 0����У�� */
    genFloatMeshInfo.correctY    = 1; /* ��ֱy����У��: 1����У��, 0����У�� */
    genFloatMeshInfo.saveMaxFovX = 0; /* ����ˮƽx�������FOV: 1������, 0�������� */

    /* ����LDCH+LDCV���㻯�����ز��� */
    stLdchLdcvInfo.enLdchLdcvVersion = enLdchLdcvVersion;
    stLdchLdcvInfo.srcW              = srcWid;
    stLdchLdcvInfo.srcH              = srcHgt;
    stLdchLdcvInfo.dstW              = dstWid;
    stLdchLdcvInfo.dstH              = dstHgt;

    /* �����²�����������ز��� */
    ret = calcFloatMeshSizeRK(&genFloatMeshInfo);
    if (-1 == ret) {
        printf("%s: failed to calcFloatMeshSizeRK\n", __func__);
        goto exit;
    }

    /* �����ϲ������ȫ�ֱ��ʸ�������ز��� */
    mapUpWid     = dstWid + 1; /* �ϲ������ȫ�ֱ��ʸ�����ߣ������1 */
    mapUpHgt     = dstHgt + 1;
    mapUpBufSize = mapUpWid * mapUpHgt;

    /* ������ת������²����������ز��� */
    ret = calcSeparateMeshInfo(srcWid, srcHgt, dstWid, dstHgt, &stSeparateMeshInfo);
    if (-1 == ret) {
        printf("%s: failed to calcSeparateMeshInfo\n", __func__);
        goto exit;
    }

    /* ����LDCH��LDCV���㻯�����ز��� */
    ret = calcLdchLdcvMeshSize(&stLdchLdcvInfo);
    if (-1 == ret) {
        printf("%s: failed to calcLdchLdcvMeshSize\n", __func__);
        goto exit;
    }

    /* �²����������ڴ����� */
    RKALGO_MALLOC(pf32MeshxDn, float, genFloatMeshInfo.u64MapBufSize, exit);
    RKALGO_MALLOC(pf32MeshyDn, float, genFloatMeshInfo.u64MapBufSize, exit);

    /* �ϲ������ȫ�ֱ��ʸ������ڴ����� */
    RKALGO_MALLOC(pf32MeshxUp, float, mapUpBufSize, exit);
    RKALGO_MALLOC(pf32MeshyUp, float, mapUpBufSize, exit);

    /* ���ת������²����������ڴ����� */
    RKALGO_MALLOC(pf32MeshX, float, stSeparateMeshInfo.u64LdchFloatMeshBufSize, exit);
    RKALGO_MALLOC(pf32MeshY, float, stSeparateMeshInfo.u64LdcvFloatMeshBufSize, exit);

    /* LDCH��LDCV���㻯����ڴ����� */
    RKALGO_MALLOC(pu16MeshX, uint16_t, stLdchLdcvInfo.u64LdchMeshBufSize, exit);
    RKALGO_MALLOC(pu16MeshY, uint16_t, stLdchLdcvInfo.u64LdcvMeshBufSize, exit);

    /* �����²��������ĳ�ʼ�� */
    ret = genFloatMeshNLevelInit(&camCoeff, &genFloatMeshInfo);
    if (-1 == ret) {
        printf("%s: failed to genFloatMeshNLevelInit\n", __func__);
        goto exit;
    }

    for (uint32_t levelIdx = 0; levelIdx < levelNum; ++levelIdx) {
        level = levelValue[levelIdx];
        printf("level = %d\n", level);

        t0 = rkAlgoToolGetTime();

        /* �����²�������� */
        ret = genFloatMeshNLevel(&camCoeff, &genFloatMeshInfo, level, pf32MeshxDn, pf32MeshyDn);
        if (-1 == ret) {
            printf("%s: failed to genFloatMeshNLevel\n", __func__);
            goto exit;
        }

        t1 = rkAlgoToolGetTime();

        /* �²�������� ---> ԭʼȫ�ֱ��ʸ���� */
        floatMeshUpSample(pf32MeshxDn, pf32MeshyDn, pf32MeshxUp, pf32MeshyUp, genFloatMeshInfo.mapW,
                          genFloatMeshInfo.mapH, mapUpWid, mapUpHgt, genFloatMeshInfo.mapStepW,
                          genFloatMeshInfo.mapStepH);

        t2 = rkAlgoToolGetTime();

        /* ԭʼȫ�ֱ��ʸ���� ---> ���ת������²�������� */
        ret = genSeparateMeshFromOriMap(&stSeparateMeshInfo, pf32MeshxUp, pf32MeshyUp, pf32MeshX,
                                        pf32MeshY);
        if (-1 == ret) {
            printf("%s: failed to genSeparateMeshFromOriMap\n", __func__);
            goto exit;
        }

        t3 = rkAlgoToolGetTime();

        /* ���ת������²�������� ---> LDCH��LDCV���㻯�� */
        ldchFloatMeshW = stSeparateMeshInfo.ldchFloatMeshW;
        ldchFloatMeshH = stSeparateMeshInfo.ldchFloatMeshH;
        ldcvFloatMeshW = stSeparateMeshInfo.ldcvFloatMeshW;
        ldcvFloatMeshH = stSeparateMeshInfo.ldcvFloatMeshH;
        ret = genLdchLdcvMeshFromFloatMesh(&stLdchLdcvInfo, ldchFloatMeshW, ldchFloatMeshH,
                                           ldcvFloatMeshW, ldcvFloatMeshH, pf32MeshX, pf32MeshY,
                                           pu16MeshX, pu16MeshY);
        if (-1 == ret) {
            printf("%s: failed to genLdchLdcvMeshFromSeparateMesh\n", __func__);
            goto exit;
        }

        t4    = rkAlgoToolGetTime();
        ct1   = (float)(t1 - t0) / 1e3f;
        ct2   = (float)(t2 - t1) / 1e3f;
        ct3   = (float)(t3 - t2) / 1e3f;
        ct4   = (float)(t4 - t3) / 1e3f;
        ctAll = (float)(t4 - t0) / 1e3f;
        printf(
            "%s: ctAll = %0.2f ms, ct1(NLevel) = %0.2f ms, ct2(upsample) = %0.2f ms, ct3(separate) "
            "= %0.2f ms, ct4(fixed point) = %0.2f ms\n",
            __func__, ctAll, ct1, ct2, ct3, ct4);

        /* �����ϲ������ԭʼȫ�ֱ��ʸ���� */
        {
            char mapxUpName[512];
            char mapyUpName[512];
            sprintf(mapxUpName, "../data/%s/%s_mapxOri_src%dx%d_dst%dx%d_level%03d.bin", __func__,
                    saveFilePrefix, srcWid, srcHgt, mapUpWid, mapUpHgt, level);
            sprintf(mapyUpName, "../data/%s/%s_mapyOri_src%dx%d_dst%dx%d_level%03d.bin", __func__,
                    saveFilePrefix, srcWid, srcHgt, mapUpWid, mapUpHgt, level);
            FILE* fpMapxUp = fopen(mapxUpName, "wb");
            if (NULL != fpMapxUp) {
                fwrite(pf32MeshxUp, sizeof(float), mapUpBufSize, fpMapxUp);
                fclose(fpMapxUp);
            }
            FILE* fpMapyUp = fopen(mapyUpName, "wb");
            if (NULL != fpMapyUp) {
                fwrite(pf32MeshyUp, sizeof(float), mapUpBufSize, fpMapyUp);
                fclose(fpMapyUp);
            }
        }

        /* ����LDCH���㻯�� */
        {
            char ldchMeshPath[1024];
            sprintf(ldchMeshPath, "../data/%s/%s_LDCH_meshX_src%dx%d_dst%dx%d_level%03d.bin",
                    __func__, saveFilePrefix, srcWid, srcHgt, dstWid, dstHgt, level);
            FILE* fpMeshX = fopen(ldchMeshPath, "wb");
            if (fpMeshX == NULL) {
                printf("%s error: failed to save LDCH mesh\n", __func__);
                goto exit;
            }
            fwrite(&dstWid, sizeof(uint16_t), 1, fpMeshX);
            fwrite(&dstHgt, sizeof(uint16_t), 1, fpMeshX);
            fwrite(&stLdchLdcvInfo.ldchAlignMeshW, sizeof(uint16_t), 1, fpMeshX);
            fwrite(&stLdchLdcvInfo.ldchMeshH, sizeof(uint16_t), 1, fpMeshX);
            fwrite(&stLdchLdcvInfo.ldchMeshStepW, sizeof(uint16_t), 1, fpMeshX);
            fwrite(&stLdchLdcvInfo.ldchMeshStepH, sizeof(uint16_t), 1, fpMeshX);
            fwrite(pu16MeshX, sizeof(uint16_t), stLdchLdcvInfo.u64LdchMeshBufSize, fpMeshX);
            fclose(fpMeshX);
        }

        /* ����LDCV���㻯�� */
        {
            char ldcvMeshPath[1024];
            sprintf(ldcvMeshPath, "../data/%s/%s_LDCV_meshY_src%dx%d_dst%dx%d_level%03d.bin",
                    __func__, saveFilePrefix, srcWid, srcHgt, dstWid, dstHgt, level);
            FILE* fpMeshY = fopen(ldcvMeshPath, "wb");
            if (fpMeshY == NULL) {
                printf("%s error: failed to save LDCV mesh\n", __func__);
                goto exit;
            }
            fwrite(pu16MeshY, sizeof(uint16_t), stLdchLdcvInfo.u64LdcvMeshBufSize, fpMeshY);
            fclose(fpMeshY);
        }

        /* ������Windows C++�����������rk3576 LDCH��LDCV��cmodelЧ�� */
#if defined(__cplusplus) && (defined(WIN32) || defined(_WIN32))
        {
            /* ��ʾ��֤ */
            cv::Mat cvMatMapxDn =
                cv::Mat(genFloatMeshInfo.mapH, genFloatMeshInfo.mapW, CV_32FC1, pf32MeshxDn);
            cv::Mat cvMatMapyDn =
                cv::Mat(genFloatMeshInfo.mapH, genFloatMeshInfo.mapW, CV_32FC1, pf32MeshyDn);
            cv::Mat cvMatMapxUp = cv::Mat(mapUpHgt, mapUpWid, CV_32FC1, pf32MeshxUp);
            cv::Mat cvMatMapyUp = cv::Mat(mapUpHgt, mapUpWid, CV_32FC1, pf32MeshyUp);
            cv::Mat cvMatFloatMeshX =
                cv::Mat(stSeparateMeshInfo.ldchFloatMeshH, stSeparateMeshInfo.ldchFloatMeshW,
                        CV_32FC1, pf32MeshX);
            cv::Mat cvMatFloatMeshY =
                cv::Mat(stSeparateMeshInfo.ldcvFloatMeshH, stSeparateMeshInfo.ldcvFloatMeshW,
                        CV_32FC1, pf32MeshY);
            cv::Mat cvMatMeshX = cv::Mat(stLdchLdcvInfo.ldchMeshH, stLdchLdcvInfo.ldchAlignMeshW,
                                         CV_16UC1, pu16MeshX);
            cv::Mat cvMatMeshY = cv::Mat(stLdchLdcvInfo.ldcvMeshH, stLdchLdcvInfo.ldcvAlignMeshW,
                                         CV_16UC1, pu16MeshY);

            cv::Mat srcImgBGR;
            cv::Mat srcImgYUV444;
            cv::Mat dstImgYUV444;
            cv::Mat dstImgBGR;
            uint8_t* pSrcYuv444 = NULL;
            uint8_t* pDstYuv444 = NULL;
            uint8_t enLdch      = 1;
            uint8_t enLdcv      = 1;

            char srcImgPath[256] = "../data/OS04A10_6IR_2688x1520_01.bmp";
            char dstImgPath[256];
            sprintf(dstImgPath, "../data/%s/%s_result_ldch%d_ldcv%d_level%03d.bmp", __func__,
                    saveFilePrefix, enLdch, enLdcv, level);

            /* ����ͼ����ص�buffer */
            uint64_t dstSize = dstWid * dstHgt * 3;
            RKALGO_MALLOC(pDstYuv444, uint8_t, dstSize, exit);

            /* ��ȡͼ�� */
            srcImgBGR = cv::imread(srcImgPath);

            /* RGB ---> YUV444 */
            cv::cvtColor(srcImgBGR, srcImgYUV444, cv::COLOR_BGR2YUV);
            pSrcYuv444 = srcImgYUV444.data;

            /* ����LDCH��LDCV��cmodel */
            LDCH_LDCV_Cmodel(srcWid, srcHgt, dstWid, dstHgt, pu16MeshX, pu16MeshY, enLdch, enLdcv,
                             pSrcYuv444, pDstYuv444);

            /* YUV444 ---> RGB */
            dstImgYUV444 = cv::Mat(dstHgt, dstWid, CV_8UC3, pDstYuv444);
            cv::cvtColor(dstImgYUV444, dstImgBGR, cv::COLOR_YUV2BGR);

            /* �������ͼ�� */
            cv::imwrite(dstImgPath, dstImgBGR);

        exit2:
            /* �ͷ�ͼ���ڴ� */
            RKALGO_FREE(pDstYuv444);
        }
#endif
    }

exit:
    /* �²���������ڴ��ͷ� */
    RKALGO_FREE(pf32MeshxDn);
    RKALGO_FREE(pf32MeshyDn);
    /* �ϲ������ȫ�ֱ��ʸ�����ڴ��ͷ� */
    RKALGO_FREE(pf32MeshxUp);
    RKALGO_FREE(pf32MeshyUp);
    /* ���ת������²���������ڴ��ͷ� */
    RKALGO_FREE(pf32MeshX);
    RKALGO_FREE(pf32MeshY);
    /* LDCH���㻯���ڴ��ͷ� */
    RKALGO_FREE(pu16MeshX);
    /* LDCV���㻯���ڴ��ͷ� */
    RKALGO_FREE(pu16MeshY);
}

/* �ּ�У��-LDCH off + LDCV on: RK���� ---> �²��������(ֻʹ��LDCV������Ҫ���) ---> LDCV���㻯��
 */
void test006_2() {
    printf("%s:\n", __func__);

    /* �������ձ�����ļ���·�� */
    rkAlgoCreatFileDir(__func__);

    int32_t ret     = 0;
    uint32_t srcWid = 2688; /* ����ͼ���� */
    uint32_t srcHgt = 1520;
    uint32_t dstWid = 2688; /* ���ͼ���� */
    uint32_t dstHgt = 1520;

    uint32_t level        = 0;                      /* �ּ�У����level����Χ: 0-255 */
    uint32_t levelValue[] = {0, 64, 128, 192, 255}; /* �ּ�У����levelȡֵ */
    uint32_t levelNum     = sizeof(levelValue) / sizeof(levelValue[0]); /* �ּ�У����level�ĸ��� */
    printf("%s: levelNum = %d\n", __func__, levelNum);

    char saveFilePrefix[64] = "OS04A10_6IR"; /* �����ļ�����ǰ׺ */

    /* RKģ�Ͳ��� */
    RKALGO_CAM_COEFF_INFO_S camCoeff;
    /* group_010_OS04A10_4IR_2688x1520 */
    // camCoeff.a0 =
    // -1830.2636074851754983683349564671516418457031250000000000000000000000000000000000000000000000000000000000;
    // camCoeff.a2 =
    // 0.0004237951284061040315917501253295540664112195372581481933593750000000000000000000000000000000000000;
    // camCoeff.a3 =
    // -0.0000002507674855122118848025746917079770170744268398266285657882690429687500000000000000000000000000;
    // camCoeff.a4 =
    // 0.0000000001272470035935948196069050985539572624916093701585850794799625873565673828125000000000000000;
    // camCoeff.cx =
    // 1351.1178239999999277642928063869476318359375000000000000000000000000000000000000000000000000000000000000;
    // camCoeff.cy =
    // 739.4860800000000153886503539979457855224609375000000000000000000000000000000000000000000000000000000000;

    /* group_011_OS04A10_6IR_2688x1520 */
    camCoeff.a0 =
        -2653.9299617503911576932296156883239746093750000000000000000000000000000000000000000000000000000000000000;
    camCoeff.a2 =
        0.0001171174803799108549888854935439042037614854052662849426269531250000000000000000000000000000000000;
    camCoeff.a3 =
        -0.0000000150166870224219489918825648283248752612450971355428919196128845214843750000000000000000000000;
    camCoeff.a4 =
        0.0000000000087465891629053077103521832658175594581856371334538380324374884366989135742187500000000000;
    camCoeff.cx =
        1333.2695039999998698476701974868774414062500000000000000000000000000000000000000000000000000000000000000;
    camCoeff.cy =
        728.1043200000000297222868539392948150634765625000000000000000000000000000000000000000000000000000000000;

    RKALGO_MESH_TYPE_E enMeshType;     /* ������Ҫ����Ķ��㻯������ͣ�FEC/LDCH/LDCV */
    RKALGO_FEC_VERSION_E enFecVersion; /* Ӳ��FEC�汾������rv1109, rv1126��rk3588 */
    RKALGO_LDCH_VERSION_E enLdchVersion; /* Ӳ��LDCH�汾(��û��LDCV)������rv1109��rv1126��rk356x��rk3588��1106��rk3562
                                          */
    RKALGO_LDCH_LDCV_VERSION_E enLdchLdcvVersion; /* Ӳ��LDCH + LDCV�汾������rk3576 */

    uint32_t ldchFloatMeshW, ldchFloatMeshH; /* LDCH�²��������Ŀ�� */
    uint32_t ldcvFloatMeshW, ldcvFloatMeshH; /* LDCV�²��������Ŀ�� */

    RKALGO_GEN_FLOAT_MESH_INFO_S genFloatMeshInfo; /* �²�����������ز��� */
    RKALGO_LDCH_LDCV_INFO_S stLdchLdcvInfo;        /* LDCH��LDCV���㻯����ز��� */

    float* pf32MeshY    = NULL; /* �²�������� */
    uint16_t* pu16MeshY = NULL; /* LDCV���㻯�� */

    /* ����������Ҫ����Ķ��㻯������� */
    enMeshType    = RKALGO_MESH_TYPE_LDCV;       /* ���ö��㻯������� */
    enFecVersion  = RKALGO_FEC_VERSION_NO_NEED;  /* ����Ӳ��FEC�汾��������Ҫ�ı�������ΪNO_NEED */
    enLdchVersion = RKALGO_LDCH_VERSION_NO_NEED; /* ����Ӳ��LDCH�汾��������Ҫ�ı�������ΪNO_NEED */
    enLdchLdcvVersion =
        RKALGO_LDCH_LDCV_VERSION_3576; /* ����Ӳ��LDCH + LDCV�汾��������Ҫ�ı�������ΪNO_NEED */

    /* �����������ɵĶ��㻯������ͣ����úͼ��㸡������ز��� */
    genFloatMeshInfo.enMeshType        = enMeshType;
    genFloatMeshInfo.enFecVersion      = enFecVersion;
    genFloatMeshInfo.enLdchVersion     = enLdchVersion;
    genFloatMeshInfo.enLdchLdcvVersion = enLdchLdcvVersion;
    genFloatMeshInfo.srcW              = srcWid;
    genFloatMeshInfo.srcH              = srcHgt;
    genFloatMeshInfo.dstW              = dstWid;
    genFloatMeshInfo.dstH              = dstHgt;
    genFloatMeshInfo.mapStepW    = 16; /* ���ø����Ĳ����������˴����밴��LDCVҪ�����ò������� */
    genFloatMeshInfo.mapStepH    = 16;
    genFloatMeshInfo.correctX    = 1; /* ˮƽx����У��: 1����У��, 0����У�� */
    genFloatMeshInfo.correctY    = 1; /* ��ֱy����У��: 1����У��, 0����У�� */
    genFloatMeshInfo.saveMaxFovX = 0; /* ����ˮƽx�������FOV: 1������, 0�������� */
    ret                          = calcFloatMeshSizeRK(&genFloatMeshInfo);
    if (-1 == ret) {
        printf("%s: failed to calcFloatMeshSizeRK\n", __func__);
        goto exit;
    }

    /* ����LDCH + LDCV�İ汾������LDCH��LDCV���㻯�������buffer��С */
    stLdchLdcvInfo.enLdchLdcvVersion = enLdchLdcvVersion;
    stLdchLdcvInfo.srcW              = srcWid;
    stLdchLdcvInfo.srcH              = srcHgt;
    stLdchLdcvInfo.dstW              = dstWid;
    stLdchLdcvInfo.dstH              = dstHgt;
    ret                              = calcLdchLdcvMeshSize(&stLdchLdcvInfo);
    if (-1 == ret) {
        printf("%s: failed to calcLdchLdcvMeshSize\n", __func__);
        goto exit;
    }

    /* �²����������ڴ����� */
    RKALGO_MALLOC(pf32MeshY, float, genFloatMeshInfo.u64MapBufSize, exit);

    /* LDCH��LDCV���㻯����ڴ����� */
    RKALGO_MALLOC(pu16MeshY, uint16_t, stLdchLdcvInfo.u64LdcvMeshBufSize, exit);

    /* �����²��������ĳ�ʼ�� */
    ret = genFloatMeshNLevelInit(&camCoeff, &genFloatMeshInfo);
    if (-1 == ret) {
        printf("%s: failed to genFloatMeshNLevelInit\n", __func__);
        goto exit;
    }

    for (uint32_t levelIdx = 0; levelIdx < levelNum; ++levelIdx) {
        level = levelValue[levelIdx];
        printf("level = %d\n", level);

        /* �����²�������� */
        ret = genFloatMeshNLevel(&camCoeff, &genFloatMeshInfo, level, NULL, pf32MeshY);
        if (-1 == ret) {
            printf("%s: failed to genFloatMeshNLevel\n", __func__);
            goto exit;
        }

        /* �²�������� ---> LDCV���㻯�� */
        ldchFloatMeshW = 0; /* ����ֻ����LDCV���㻯��LDCH���㻯��Ŀ�߿�������Ϊ0 */
        ldchFloatMeshH = 0;
        ldcvFloatMeshW = genFloatMeshInfo.mapW;
        ldcvFloatMeshH = genFloatMeshInfo.mapH;
        ret = genLdchLdcvMeshFromFloatMesh(&stLdchLdcvInfo, ldchFloatMeshW, ldchFloatMeshH,
                                           ldcvFloatMeshW, ldcvFloatMeshH, NULL, pf32MeshY, NULL,
                                           pu16MeshY);
        if (-1 == ret) {
            printf("%s: failed to genLdchLdcvMeshFromSeparateMesh\n", __func__);
            goto exit;
        }

        /* ����LDCV���㻯mesh�� */
        {
            char ldcvMeshPath[1024];
            sprintf(ldcvMeshPath, "../data/%s/%s_LDCV_meshY_src%dx%d_dst%dx%d_level%03d.bin",
                    __func__, saveFilePrefix, srcWid, srcHgt, dstWid, dstHgt, level);
            FILE* fpMeshY = fopen(ldcvMeshPath, "wb");
            if (fpMeshY == NULL) {
                printf("%s error: failed to save LDCV mesh\n", __func__);
                goto exit;
            }
            fwrite(pu16MeshY, sizeof(uint16_t), stLdchLdcvInfo.u64LdcvMeshBufSize, fpMeshY);
            fclose(fpMeshY);
        }

        /* �����²�������� */
        {
            char ldcvFloatMeshPath[1024];
            sprintf(ldcvFloatMeshPath,
                    "../data/%s/%s_ldcvFloatMesh_src%dx%d_dst%dx%d_level%03d.bin", __func__,
                    saveFilePrefix, srcWid, srcHgt, dstWid, dstHgt, level);
            FILE* fpFloatMeshY = fopen(ldcvFloatMeshPath, "wb");
            if (fpFloatMeshY == NULL) {
                printf("%s error: failed to save LDCV float mesh\n", __func__);
                goto exit;
            }
            fwrite(pf32MeshY, sizeof(float), genFloatMeshInfo.u64MapBufSize, fpFloatMeshY);
            fclose(fpFloatMeshY);
        }

        /* ������Windows C++�����������rk3576 LDCH��LDCV��cmodelЧ�� */
#if defined(__cplusplus) && (defined(WIN32) || defined(_WIN32))
        {
            /* ��ʾ��֤ */
            cv::Mat cvMatFloatMeshY = cv::Mat(ldcvFloatMeshH, ldcvFloatMeshW, CV_32FC1, pf32MeshY);
            cv::Mat cvMatMeshY = cv::Mat(stLdchLdcvInfo.ldcvMeshH, stLdchLdcvInfo.ldcvAlignMeshW,
                                         CV_16UC1, pu16MeshY);

            cv::Mat srcImgBGR;
            cv::Mat srcImgYUV444;
            cv::Mat dstImgYUV444;
            cv::Mat dstImgBGR;
            uint8_t* pSrcYuv444 = NULL;
            uint8_t* pDstYuv444 = NULL;
            uint8_t enLdch      = 0;
            uint8_t enLdcv      = 1;

            char srcImgPath[256] = "../data/OS04A10_6IR_2688x1520_01.bmp";
            char dstImgPath[256];
            sprintf(dstImgPath, "../data/%s/%s_result_ldch%d_ldcv%d_level%03d.bmp", __func__,
                    saveFilePrefix, enLdch, enLdcv, level);

            /* ����ͼ����ص�buffer */
            uint64_t dstSize = dstWid * dstHgt * 3;
            RKALGO_MALLOC(pDstYuv444, uint8_t, dstSize, exit);

            /* ��ȡͼ�� */
            srcImgBGR = cv::imread(srcImgPath);

            /* RGB ---> YUV444 */
            cv::cvtColor(srcImgBGR, srcImgYUV444, cv::COLOR_BGR2YUV);
            pSrcYuv444 = srcImgYUV444.data;

            /* ����LDCH��LDCV��cmodel */
            LDCH_LDCV_Cmodel(srcWid, srcHgt, dstWid, dstHgt, NULL, pu16MeshY, enLdch, enLdcv,
                             pSrcYuv444, pDstYuv444);

            /* YUV444 ---> RGB */
            dstImgYUV444 = cv::Mat(dstHgt, dstWid, CV_8UC3, pDstYuv444);
            cv::cvtColor(dstImgYUV444, dstImgBGR, cv::COLOR_YUV2BGR);

            /* �������ͼ�� */
            cv::imwrite(dstImgPath, dstImgBGR);

        exit2:
            /* �ͷ�ͼ���ڴ� */
            RKALGO_FREE(pDstYuv444);
        }
#endif
    }
exit:
    /* �²���������ڴ��ͷ� */
    RKALGO_FREE(pf32MeshY);
    /* LDCV���㻯���ڴ��ͷ� */
    RKALGO_FREE(pu16MeshY);
}

/* �ּ�У��-LDCH on + LDCV off: RK���� ---> �²��������(ֻʹ��LDCH������Ҫ���) ---> LDCH���㻯��
 */
void test006_3() {
    printf("%s:\n", __func__);

    /* �������ձ�����ļ���·�� */
    rkAlgoCreatFileDir(__func__);

    int32_t ret     = 0;
    uint32_t srcWid = 2688; /* ����ͼ���� */
    uint32_t srcHgt = 1520;
    uint32_t dstWid = 2688; /* ���ͼ���� */
    uint32_t dstHgt = 1520;

    uint32_t level        = 0;                      /* �ּ�У����level����Χ: 0-255 */
    uint32_t levelValue[] = {0, 64, 128, 192, 255}; /* �ּ�У����levelȡֵ */
    uint32_t levelNum     = sizeof(levelValue) / sizeof(levelValue[0]); /* �ּ�У����level�ĸ��� */
    printf("%s: levelNum = %d\n", __func__, levelNum);

    char saveFilePrefix[64] = "OS04A10_6IR"; /* �����ļ�����ǰ׺ */

    /* RKģ�Ͳ��� */
    RKALGO_CAM_COEFF_INFO_S camCoeff;
    /* group_010_OS04A10_4IR_2688x1520 */
    // camCoeff.a0 =
    // -1830.2636074851754983683349564671516418457031250000000000000000000000000000000000000000000000000000000000;
    // camCoeff.a2 =
    // 0.0004237951284061040315917501253295540664112195372581481933593750000000000000000000000000000000000000;
    // camCoeff.a3 =
    // -0.0000002507674855122118848025746917079770170744268398266285657882690429687500000000000000000000000000;
    // camCoeff.a4 =
    // 0.0000000001272470035935948196069050985539572624916093701585850794799625873565673828125000000000000000;
    // camCoeff.cx =
    // 1351.1178239999999277642928063869476318359375000000000000000000000000000000000000000000000000000000000000;
    // camCoeff.cy =
    // 739.4860800000000153886503539979457855224609375000000000000000000000000000000000000000000000000000000000;

    /* group_011_OS04A10_6IR_2688x1520 */
    camCoeff.a0 =
        -2653.9299617503911576932296156883239746093750000000000000000000000000000000000000000000000000000000000000;
    camCoeff.a2 =
        0.0001171174803799108549888854935439042037614854052662849426269531250000000000000000000000000000000000;
    camCoeff.a3 =
        -0.0000000150166870224219489918825648283248752612450971355428919196128845214843750000000000000000000000;
    camCoeff.a4 =
        0.0000000000087465891629053077103521832658175594581856371334538380324374884366989135742187500000000000;
    camCoeff.cx =
        1333.2695039999998698476701974868774414062500000000000000000000000000000000000000000000000000000000000000;
    camCoeff.cy =
        728.1043200000000297222868539392948150634765625000000000000000000000000000000000000000000000000000000000;

    RKALGO_MESH_TYPE_E enMeshType;     /* ������Ҫ����Ķ��㻯������ͣ�FEC/LDCH/LDCV */
    RKALGO_FEC_VERSION_E enFecVersion; /* Ӳ��FEC�汾������rv1109, rv1126��rk3588 */
    RKALGO_LDCH_VERSION_E enLdchVersion; /* Ӳ��LDCH�汾(��û��LDCV)������rv1109��rv1126��rk356x��rk3588��1106��rk3562
                                          */
    RKALGO_LDCH_LDCV_VERSION_E enLdchLdcvVersion; /* Ӳ��LDCH + LDCV�汾������rk3576 */

    uint32_t ldchFloatMeshW, ldchFloatMeshH; /* LDCH�²��������Ŀ�� */
    uint32_t ldcvFloatMeshW, ldcvFloatMeshH; /* LDCV�²��������Ŀ�� */

    RKALGO_GEN_FLOAT_MESH_INFO_S genFloatMeshInfo; /* �²�����������ز��� */
    RKALGO_LDCH_LDCV_INFO_S stLdchLdcvInfo;        /* LDCH��LDCV���㻯����ز��� */

    float* pf32MeshX    = NULL; /* �²�������� */
    uint16_t* pu16MeshX = NULL; /* LDCV���㻯�� */

    /* ����������Ҫ����Ķ��㻯������� */
    enMeshType    = RKALGO_MESH_TYPE_LDCH;      /* ���ö��㻯������� */
    enFecVersion  = RKALGO_FEC_VERSION_NO_NEED; /* ����Ӳ��FEC�汾��������Ҫ�ı�������ΪNO_NEED */
    enLdchVersion = RKALGO_LDCH_VERSION_1;      /* ����Ӳ��LDCH�汾��������Ҫ�ı�������ΪNO_NEED */
    enLdchLdcvVersion =
        RKALGO_LDCH_LDCV_VERSION_3576; /* ����Ӳ��LDCH + LDCV�汾��������Ҫ�ı�������ΪNO_NEED */

    /* �����������ɵĶ��㻯������ͣ����úͼ��㸡������ز��� */
    genFloatMeshInfo.enMeshType        = enMeshType;
    genFloatMeshInfo.enFecVersion      = enFecVersion;
    genFloatMeshInfo.enLdchVersion     = enLdchVersion;
    genFloatMeshInfo.enLdchLdcvVersion = enLdchLdcvVersion;
    genFloatMeshInfo.srcW              = srcWid;
    genFloatMeshInfo.srcH              = srcHgt;
    genFloatMeshInfo.dstW              = dstWid;
    genFloatMeshInfo.dstH              = dstHgt;
    genFloatMeshInfo.mapStepW    = 16; /* ���ø����Ĳ����������˴����밴��LDCHҪ�����ò������� */
    genFloatMeshInfo.mapStepH    = 8;
    genFloatMeshInfo.correctX    = 1; /* ˮƽx����У��: 1����У��, 0����У�� */
    genFloatMeshInfo.correctY    = 1; /* ��ֱy����У��: 1����У��, 0����У�� */
    genFloatMeshInfo.saveMaxFovX = 0; /* ����ˮƽx�������FOV: 1������, 0�������� */
    ret                          = calcFloatMeshSizeRK(&genFloatMeshInfo);
    if (-1 == ret) {
        printf("%s: failed to calcFloatMeshSizeRK\n", __func__);
        goto exit;
    }

    /* ����LDCH + LDCV�İ汾������LDCH��LDCV���㻯�������buffer��С */
    stLdchLdcvInfo.enLdchLdcvVersion = enLdchLdcvVersion;
    stLdchLdcvInfo.srcW              = srcWid;
    stLdchLdcvInfo.srcH              = srcHgt;
    stLdchLdcvInfo.dstW              = dstWid;
    stLdchLdcvInfo.dstH              = dstHgt;
    ret                              = calcLdchLdcvMeshSize(&stLdchLdcvInfo);
    if (-1 == ret) {
        printf("%s: failed to calcLdchLdcvMeshSize\n", __func__);
        goto exit;
    }

    /* �²����������ڴ����� */
    RKALGO_MALLOC(pf32MeshX, float, genFloatMeshInfo.u64MapBufSize, exit);

    /* LDCH���㻯����ڴ����� */
    RKALGO_MALLOC(pu16MeshX, uint16_t, stLdchLdcvInfo.u64LdchMeshBufSize, exit);

    /* �����²��������ĳ�ʼ�� */
    ret = genFloatMeshNLevelInit(&camCoeff, &genFloatMeshInfo);
    if (-1 == ret) {
        printf("%s: failed to genFloatMeshNLevelInit\n", __func__);
        goto exit;
    }

    for (uint32_t levelIdx = 0; levelIdx < levelNum; ++levelIdx) {
        level = levelValue[levelIdx];
        printf("level = %d\n", level);

        /* �����²�������� */
        ret = genFloatMeshNLevel(&camCoeff, &genFloatMeshInfo, level, pf32MeshX, NULL);
        if (-1 == ret) {
            printf("%s: failed to genFloatMeshNLevel\n", __func__);
            goto exit;
        }

        /* LDCH�²�������� ---> LDCH���㻯�� */
        ldchFloatMeshW = genFloatMeshInfo.mapW;
        ldchFloatMeshH = genFloatMeshInfo.mapH;
        ldcvFloatMeshW = 0; /* ����ֻ����LDCH���㻯��LDCV���㻯��Ŀ�߿�������Ϊ0 */
        ldcvFloatMeshH = 0;
        ret = genLdchLdcvMeshFromFloatMesh(&stLdchLdcvInfo, ldchFloatMeshW, ldchFloatMeshH,
                                           ldcvFloatMeshW, ldcvFloatMeshH, pf32MeshX, NULL,
                                           pu16MeshX, NULL);
        if (-1 == ret) {
            printf("%s: failed to genLdchLdcvMeshFromSeparateMesh\n", __func__);
            goto exit;
        }

        /* ����LDCH���㻯�� */
        {
            char ldchMeshPath[1024];
            sprintf(ldchMeshPath, "../data/%s/%s_LDCH_meshX_src%dx%d_dst%dx%d_level%03d.bin",
                    __func__, saveFilePrefix, srcWid, srcHgt, dstWid, dstHgt, level);
            FILE* fpMeshX = fopen(ldchMeshPath, "wb");
            if (fpMeshX == NULL) {
                printf("%s error: failed to save LDCH mesh\n", __func__);
                goto exit;
            }
            fwrite(&dstWid, sizeof(uint16_t), 1, fpMeshX);
            fwrite(&dstHgt, sizeof(uint16_t), 1, fpMeshX);
            fwrite(&stLdchLdcvInfo.ldchAlignMeshW, sizeof(uint16_t), 1, fpMeshX);
            fwrite(&stLdchLdcvInfo.ldchMeshH, sizeof(uint16_t), 1, fpMeshX);
            fwrite(&stLdchLdcvInfo.ldchMeshStepW, sizeof(uint16_t), 1, fpMeshX);
            fwrite(&stLdchLdcvInfo.ldchMeshStepH, sizeof(uint16_t), 1, fpMeshX);
            fwrite(pu16MeshX, sizeof(uint16_t), stLdchLdcvInfo.u64LdchMeshBufSize, fpMeshX);
            fclose(fpMeshX);
        }

        /* �����²�������� */
        {
            char ldchFloatMeshPath[1024];
            sprintf(ldchFloatMeshPath,
                    "../data/%s/%s_ldchFloatMesh_src%dx%d_dst%dx%d_level%03d.bin", __func__,
                    saveFilePrefix, srcWid, srcHgt, dstWid, dstHgt, level);
            FILE* fpFloatMeshY = fopen(ldchFloatMeshPath, "wb");
            if (fpFloatMeshY == NULL) {
                printf("%s error: failed to save LDCH float mesh\n", __func__);
                goto exit;
            }
            fwrite(pf32MeshX, sizeof(float), genFloatMeshInfo.u64MapBufSize, fpFloatMeshY);
            fclose(fpFloatMeshY);
        }

        /* ������Windows C++�����������rk3576 LDCH��LDCV��cmodelЧ�� */
#if defined(__cplusplus) && (defined(WIN32) || defined(_WIN32))
        {
            /* ��ʾ��֤ */
            cv::Mat cvMatFloatMeshX = cv::Mat(ldchFloatMeshH, ldchFloatMeshW, CV_32FC1, pf32MeshX);
            cv::Mat cvMatMeshX = cv::Mat(stLdchLdcvInfo.ldchMeshH, stLdchLdcvInfo.ldchAlignMeshW,
                                         CV_16UC1, pu16MeshX);

            cv::Mat srcImgBGR;
            cv::Mat srcImgYUV444;
            cv::Mat dstImgYUV444;
            cv::Mat dstImgBGR;
            uint8_t* pSrcYuv444 = NULL;
            uint8_t* pDstYuv444 = NULL;
            uint8_t enLdch      = 1;
            uint8_t enLdcv      = 0;

            char srcImgPath[256] = "../data/OS04A10_6IR_2688x1520_01.bmp";
            char dstImgPath[256];
            sprintf(dstImgPath, "../data/%s/%s_result_ldch%d_ldcv%d_level%03d.bmp", __func__,
                    saveFilePrefix, enLdch, enLdcv, level);
            /* ����ͼ����ص�buffer */
            uint64_t dstSize = dstWid * dstHgt * 3;
            RKALGO_MALLOC(pDstYuv444, uint8_t, dstSize, exit);

            /* ��ȡͼ�� */
            srcImgBGR = cv::imread(srcImgPath);

            /* RGB ---> YUV444 */
            cv::cvtColor(srcImgBGR, srcImgYUV444, cv::COLOR_BGR2YUV);
            pSrcYuv444 = srcImgYUV444.data;

            /* ����LDCH��LDCV��cmodel */
            LDCH_LDCV_Cmodel(srcWid, srcHgt, dstWid, dstHgt, pu16MeshX, NULL, enLdch, enLdcv,
                             pSrcYuv444, pDstYuv444);

            /* YUV444 ---> RGB */
            dstImgYUV444 = cv::Mat(dstHgt, dstWid, CV_8UC3, pDstYuv444);
            cv::cvtColor(dstImgYUV444, dstImgBGR, cv::COLOR_YUV2BGR);

            /* �������ͼ�� */
            cv::imwrite(dstImgPath, dstImgBGR);

        exit2:
            /* �ͷ�ͼ���ڴ� */
            RKALGO_FREE(pDstYuv444);
        }
#endif
    }

exit:
    /* �²���������ڴ��ͷ� */
    RKALGO_FREE(pf32MeshX);
    /* LDCH���㻯���ڴ��ͷ� */
    RKALGO_FREE(pu16MeshX);
}