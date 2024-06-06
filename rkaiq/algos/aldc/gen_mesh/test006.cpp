// clang-format off

/* LDCH on + LDCV on: RKģ�Ͳ��� ---> ԭʼȫ�ֱ��ʸ���� ---> �ֲ�У�����²�������� --->
 * LDCH��LDCV���㻯�� */
void test006_1() {
    printf("%s:\n", __func__);
    int32_t ret     = 0;
    uint32_t srcWid = 2688; /* ����ͼ���� */
    uint32_t srcHgt = 1520;
    uint32_t dstWid = 2688; /* ���ͼ���� */
    uint32_t dstHgt = 1520;

    char saveFilePrefix[64] = "OS04A10_6IR";                          /* �����ļ�����ǰ׺ */
    char srcBGRPath[256]    = "../data/OS04A10_6IR_2688x1520_01.bmp"; /* ����BGRͼ��·������ */

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

    RKALGO_GEN_FLOAT_MESH_INFO_S genOriFloatMeshInfo; /* ԭʼȫ�ֱ��ʸ�������ز��� */
    RKALGO_SEPARATE_MESH_INFO_S stSeparateMeshInfo;   /* �ֲ�У���²����������ز��� */
    RKALGO_LDCH_LDCV_INFO_S stLdchLdcvInfo;           /* LDCH��LDCV���㻯����ز��� */

    float* pf32MapxOri  = NULL; /* ԭʼȫ�ֱ��ʸ���� */
    float* pf32MapyOri  = NULL;
    float* pf32MeshX    = NULL; /* ���ת������²�������� */
    float* pf32MeshY    = NULL;
    uint16_t* pu16MeshX = NULL; /* LDCH���㻯�� */
    uint16_t* pu16MeshY = NULL; /* LDCV���㻯�� */

    /* ����������Ҫ����Ķ��㻯������� */
    enMeshType    = RKALGO_MESH_TYPE_LDCH_LDCV; /* ���ö��㻯������� */
    enFecVersion  = RKALGO_FEC_VERSION_NO_NEED; /* ����Ӳ��FEC�汾��������Ҫ�ı�������ΪNO_NEED */
    enLdchVersion = RKALGO_LDCH_VERSION_1;      /* ����Ӳ��LDCH�汾��������Ҫ�ı�������ΪNO_NEED */
    enLdchLdcvVersion =
        RKALGO_LDCH_LDCV_VERSION_3576; /* ����Ӳ��LDCH + LDCV�汾��������Ҫ�ı�������ΪNO_NEED */

    /* �����������ɵĶ��㻯������ͣ����úͼ��㸡������ز��� */
    genOriFloatMeshInfo.enMeshType        = enMeshType;
    genOriFloatMeshInfo.enFecVersion      = enFecVersion;
    genOriFloatMeshInfo.enLdchVersion     = enLdchVersion;
    genOriFloatMeshInfo.enLdchLdcvVersion = enLdchLdcvVersion;
    genOriFloatMeshInfo.srcW              = srcWid;
    genOriFloatMeshInfo.srcH              = srcHgt;
    genOriFloatMeshInfo.dstW              = dstWid;
    genOriFloatMeshInfo.dstH              = dstHgt;
    genOriFloatMeshInfo.mapStepW =
        1; /* ����ԭʼ�����Ĳ����������˴����밴��LDCH+LDCVҪ�����ò������� */
    genOriFloatMeshInfo.mapStepH    = 1;
    genOriFloatMeshInfo.correctX    = 1; /* ˮƽx����У��: 1����У��, 0����У�� */
    genOriFloatMeshInfo.correctY    = 1; /* ��ֱy����У��: 1����У��, 0����У�� */
    genOriFloatMeshInfo.saveMaxFovX = 0; /* ����ˮƽx�������FOV: 1������, 0�������� */
    ret                             = calcFloatMeshSizeRK(&genOriFloatMeshInfo);
    if (-1 == ret) {
        printf("%s: failed to calcFloatMeshSize\n", __func__);
        goto exit;
    }

    /* ����ֲ�У���²����������ز��� */
    ret = calcSeparateMeshInfo(srcWid, srcHgt, dstWid, dstHgt, &stSeparateMeshInfo);
    if (-1 == ret) {
        printf("%s: failed to calcSeparateMeshInfo\n", __func__);
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

    /* ԭʼȫ�ֱ��ʸ������ڴ����� */
    pf32MapxOri = new float[genOriFloatMeshInfo.u64MapBufSize];
    pf32MapyOri = new float[genOriFloatMeshInfo.u64MapBufSize];

    /* �²����������ڴ����� */
    pf32MeshX = new float[stSeparateMeshInfo.u64LdchFloatMeshBufSize];
    pf32MeshY = new float[stSeparateMeshInfo.u64LdcvFloatMeshBufSize];

    /* LDCH��LDCV���㻯����ڴ����� */
    pu16MeshX = new uint16_t[stLdchLdcvInfo.u64LdchMeshBufSize];
    pu16MeshY = new uint16_t[stLdchLdcvInfo.u64LdcvMeshBufSize];

    /* ����ԭʼȫ�ֱ��ʸ����ĳ�ʼ�� */
    ret = genFloatMeshNLevelInit(&camCoeff, &genOriFloatMeshInfo);
    if (-1 == ret) {
        printf("%s: failed to genFloatMeshNLevelInit\n", __func__);
        goto exit;
    }

    uint32_t level        = 0; /* level��Χ: 0-255 */
    uint32_t levelValue[] = {0, 64, 128, 192, 255};
    for (uint32_t levelIdx = 0; levelIdx < 5; ++levelIdx) {
        level = levelValue[levelIdx];
        printf("level = %d\n", level);

        /* ����ԭʼȫ�ֱ��ʸ���� */
        ret = genFloatMeshNLevel(&camCoeff, &genOriFloatMeshInfo, level, pf32MapxOri, pf32MapyOri);
        if (-1 == ret) {
            printf("%s: failed to genFloatMeshNLevel\n", __func__);
            goto exit;
        }

        /* ԭʼȫ�ֱ��ʸ���� ---> �ֲ�У���²�������� */
        ret = genSeparateMeshFromOriMap(&stSeparateMeshInfo, pf32MapxOri, pf32MapyOri, pf32MeshX,
                                        pf32MeshY);
        if (-1 == ret) {
            printf("%s: failed to genSeparateMeshFromOriMap\n", __func__);
            goto exit;
        }

        /* �ֲ�У���²�������� ---> LDCH��LDCV���㻯�� */
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

        /* ����ԭʼȫ�ֱ��ʸ���� */
        {
            char mapxOriName[512];
            char mapyOriName[512];
            sprintf(mapxOriName, "../data/%s_mapxOri_src%dx%d_dst%dx%d_%s_level%03d.bin",
                    saveFilePrefix, srcWid, srcHgt, genOriFloatMeshInfo.mapW,
                    genOriFloatMeshInfo.mapH, __func__, level);
            sprintf(mapyOriName, "../data/%s_mapyOri_src%dx%d_dst%dx%d_%s_level%03d.bin",
                    saveFilePrefix, srcWid, srcHgt, genOriFloatMeshInfo.mapW,
                    genOriFloatMeshInfo.mapH, __func__, level);
            FILE* fpMapxOri = fopen(mapxOriName, "wb");
            if (NULL != fpMapxOri) {
                fwrite(pf32MapxOri, sizeof(float), genOriFloatMeshInfo.u64MapBufSize, fpMapxOri);
                fclose(fpMapxOri);
            }
            FILE* fpMapyOri = fopen(mapyOriName, "wb");
            if (NULL != fpMapyOri) {
                fwrite(pf32MapyOri, sizeof(float), genOriFloatMeshInfo.u64MapBufSize, fpMapyOri);
                fclose(fpMapyOri);
            }
        }

        /* ����LDCH���㻯�� */
        {
            char ldchMeshPath[1024];
            sprintf(ldchMeshPath, "../data/%s_LDCH_meshX_src%dx%d_dst%dx%d_%s_level%03d.bin",
                    saveFilePrefix, srcWid, srcHgt, dstWid, dstHgt, __func__, level);
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
            sprintf(ldcvMeshPath, "../data/%s_LDCV_meshY_src%dx%d_dst%dx%d_%s_level%03d.bin",
                    saveFilePrefix, srcWid, srcHgt, dstWid, dstHgt, __func__, level);
            FILE* fpMeshY = fopen(ldcvMeshPath, "wb");
            if (fpMeshY == NULL) {
                printf("%s error: failed to save LDCV mesh\n", __func__);
                goto exit;
            }
            fwrite(pu16MeshY, sizeof(uint16_t), stLdchLdcvInfo.u64LdcvMeshBufSize, fpMeshY);
            fclose(fpMeshY);
        }

        /* ����rk3576 LDCH��LDCV��cmodelЧ�� */
        {
            /* ��ʾ��֤ */
            cv::Mat cvMatMapxOri =
                cv::Mat(genOriFloatMeshInfo.mapH, genOriFloatMeshInfo.mapW, CV_32FC1, pf32MapxOri);
            cv::Mat cvMatMapyOri =
                cv::Mat(genOriFloatMeshInfo.mapH, genOriFloatMeshInfo.mapW, CV_32FC1, pf32MapyOri);
            cv::Mat cvMatFloatMeshX =
                cv::Mat(stSeparateMeshInfo.ldchFloatMeshH, stSeparateMeshInfo.ldchFloatMeshW,
                        CV_32FC1, pf32MeshX);
            cv::Mat cvMatFloatMeshY =
                cv::Mat(stSeparateMeshInfo.ldcvFloatMeshH, stSeparateMeshInfo.ldcvFloatMeshW,
                        CV_32FC1, pf32MeshY);
            cv::Mat cvMatMeshX = cv::Mat(stLdchLdcvInfo.ldchMeshH, stLdchLdcvInfo.ldchAlignMeshW,
                                         CV_16UC1, pu16MeshX);
            cv::Mat cvMatMeshY =
                cv::Mat(stLdchLdcvInfo.ldcvMeshH, stLdchLdcvInfo.ldcvMeshW, CV_16UC1, pu16MeshY);

            cv::Mat srcImgBGR = cv::imread(srcBGRPath);

            /* RGB ---> YUV444 */
            cv::Mat srcImgYUV444;
            cv::cvtColor(srcImgBGR, srcImgYUV444, cv::COLOR_BGR2YUV);
            uint8_t* pSrcYuv444 = srcImgYUV444.data;

            int dstSize         = dstWid * dstHgt;
            uint8_t* pDstYuv444 = NULL;
            pDstYuv444          = new uint8_t[dstSize * 3];

            /* ����LDCH��LDCV��cmodel */
            uint8_t enLdch = 1;
            uint8_t enLdcv = 1;
            LDCH_LDCV_Cmodel(srcWid, srcHgt, dstWid, dstHgt, pu16MeshX, pu16MeshY, enLdch, enLdcv,
                             pSrcYuv444, pDstYuv444);

            /* YUV444 ---> RGB */
            cv::Mat dstImgYUV444 = cv::Mat(dstHgt, dstWid, CV_8UC3, pDstYuv444);
            cv::Mat dstImgBGR;
            cv::cvtColor(dstImgYUV444, dstImgBGR, cv::COLOR_YUV2BGR);

            /* ���� */
            char saveDstImgPath[1024];
            sprintf(saveDstImgPath, "../data/%s_result_ldch%d_ldcv%d_%s_level%03d.bmp",
                    saveFilePrefix, enLdch, enLdcv, __func__, level);
            cv::imwrite(saveDstImgPath, dstImgBGR);

            if (NULL != pDstYuv444) {
                delete[] pDstYuv444;
                pDstYuv444 = NULL;
            }
        }
    }

exit:
    /* ȫ�ֱ��ʸ�����ڴ��ͷ� */
    if (NULL != pf32MapxOri) {
        delete[] pf32MapxOri;
        pf32MapxOri = NULL;
    }
    if (NULL != pf32MapyOri) {
        delete[] pf32MapyOri;
        pf32MapyOri = NULL;
    }
    /* ���ת������²���������ڴ��ͷ� */
    if (NULL != pf32MeshX) {
        delete[] pf32MeshX;
        pf32MeshX = NULL;
    }
    if (NULL != pf32MeshY) {
        delete[] pf32MeshY;
        pf32MeshY = NULL;
    }
    /* LDCH���㻯���ڴ��ͷ� */
    if (NULL != pu16MeshX) {
        delete[] pu16MeshX;
        pu16MeshX = NULL;
    }
    /* LDCV���㻯���ڴ��ͷ� */
    if (NULL != pu16MeshY) {
        delete[] pu16MeshY;
        pu16MeshY = NULL;
    }
}

/* LDCH off + LDCV on: RKģ�Ͳ��� ---> �²�������� ---> LDCV���㻯�� */
void test006_2() {
    printf("%s:\n", __func__);
    int32_t ret     = 0;
    uint32_t srcWid = 2688; /* ����ͼ���� */
    uint32_t srcHgt = 1520;
    uint32_t dstWid = 2688; /* ���ͼ���� */
    uint32_t dstHgt = 1520;

    char saveFilePrefix[64] = "OS04A10_6IR";                          /* �����ļ�����ǰ׺ */
    char srcBGRPath[256]    = "../data/OS04A10_6IR_2688x1520_01.bmp"; /* ����BGRͼ��·������ */

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
    genFloatMeshInfo.mapStepW =
        16; /* ����ԭʼ�����Ĳ����������˴����밴��LDCH+LDCVҪ�����ò������� */
    genFloatMeshInfo.mapStepH    = 16;
    genFloatMeshInfo.correctX    = 1; /* ˮƽx����У��: 1����У��, 0����У�� */
    genFloatMeshInfo.correctY    = 1; /* ��ֱy����У��: 1����У��, 0����У�� */
    genFloatMeshInfo.saveMaxFovX = 0; /* ����ˮƽx�������FOV: 1������, 0�������� */
    ret                          = calcFloatMeshSizeRK(&genFloatMeshInfo);
    if (-1 == ret) {
        printf("%s: failed to calcFloatMeshSize\n", __func__);
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
    pf32MeshY = new float[genFloatMeshInfo.u64MapBufSize];

    /* LDCH��LDCV���㻯����ڴ����� */
    pu16MeshY = new uint16_t[stLdchLdcvInfo.u64LdcvMeshBufSize];

    /* �����²��������ĳ�ʼ�� */
    ret = genFloatMeshNLevelInit(&camCoeff, &genFloatMeshInfo);
    if (-1 == ret) {
        printf("%s: failed to genFloatMeshNLevelInit\n", __func__);
        goto exit;
    }

    uint32_t level        = 0; /* level��Χ: 0-255 */
    uint32_t levelValue[] = {0, 64, 128, 192, 255};
    for (uint32_t levelIdx = 0; levelIdx < 5; ++levelIdx) {
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
            sprintf(ldcvMeshPath, "../data/%s_LDCV_meshY_src%dx%d_dst%dx%d_%s_level%03d.bin",
                    saveFilePrefix, srcWid, srcHgt, dstWid, dstHgt, __func__, level);
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
                    "../data/%s_ldcvFloatMesh_src%dx%d_dst%dx%d_%s_level%03d.bin", saveFilePrefix,
                    srcWid, srcHgt, dstWid, dstHgt, __func__, level);
            FILE* fpFloatMeshY = fopen(ldcvFloatMeshPath, "wb");
            if (fpFloatMeshY == NULL) {
                printf("%s error: failed to save LDCV float mesh\n", __func__);
                goto exit;
            }
            fwrite(pf32MeshY, sizeof(float), genFloatMeshInfo.u64MapBufSize, fpFloatMeshY);
            fclose(fpFloatMeshY);
        }

        /* ����rk3576 LDCH��LDCV��cmodelЧ�� */
        {
            /* ��ʾ��֤ */
            cv::Mat cvMatFloatMeshY = cv::Mat(ldcvFloatMeshH, ldcvFloatMeshW, CV_32FC1, pf32MeshY);
            cv::Mat cvMatMeshY =
                cv::Mat(stLdchLdcvInfo.ldcvMeshH, stLdchLdcvInfo.ldcvMeshW, CV_16UC1, pu16MeshY);

            cv::Mat srcImgBGR = cv::imread(srcBGRPath);

            /* RGB ---> YUV444 */
            cv::Mat srcImgYUV444;
            cv::cvtColor(srcImgBGR, srcImgYUV444, cv::COLOR_BGR2YUV);
            uint8_t* pSrcYuv444 = srcImgYUV444.data;

            int dstSize         = dstWid * dstHgt;
            uint8_t* pDstYuv444 = NULL;
            pDstYuv444          = new uint8_t[dstSize * 3];

            /* ����LDCH��LDCV��cmodel */
            uint8_t enLdch = 0;
            uint8_t enLdcv = 1;
            LDCH_LDCV_Cmodel(srcWid, srcHgt, dstWid, dstHgt, NULL, pu16MeshY, enLdch, enLdcv,
                             pSrcYuv444, pDstYuv444);

            /* YUV444 ---> RGB */
            cv::Mat dstImgYUV444 = cv::Mat(dstHgt, dstWid, CV_8UC3, pDstYuv444);
            cv::Mat dstImgBGR;
            cv::cvtColor(dstImgYUV444, dstImgBGR, cv::COLOR_YUV2BGR);

            /* ���� */
            char saveDstImgPath[1024];
            sprintf(saveDstImgPath, "../data/%s_result_ldch%d_ldcv%d_%s_level%03d.bmp",
                    saveFilePrefix, enLdch, enLdcv, __func__, level);
            cv::imwrite(saveDstImgPath, dstImgBGR);

            if (NULL != pDstYuv444) {
                delete[] pDstYuv444;
                pDstYuv444 = NULL;
            }
        }
    }

exit:

    /* �²���������ڴ��ͷ� */
    if (NULL != pf32MeshY) {
        delete[] pf32MeshY;
        pf32MeshY = NULL;
    }
    /* LDCV���㻯���ڴ��ͷ� */
    if (NULL != pu16MeshY) {
        delete[] pu16MeshY;
        pu16MeshY = NULL;
    }
}

/* LDCH on + LDCV off: RKģ�Ͳ��� ---> �²�������� ---> LDCH���㻯�� */
void test006_3() {
    int32_t ret     = 0;
    uint32_t srcWid = 2688; /* ����ͼ���� */
    uint32_t srcHgt = 1520;
    uint32_t dstWid = 2688; /* ���ͼ���� */
    uint32_t dstHgt = 1520;

    char saveFilePrefix[64] = "OS04A10_4IR";                          /* �����ļ�����ǰ׺ */
    char srcBGRPath[256]    = "../data/OS04A10_4IR_2688x1520_01.bmp"; /* ����BGRͼ��·������ */

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
    genFloatMeshInfo.mapStepW =
        16; /* ����ԭʼ�����Ĳ����������˴����밴��LDCH+LDCVҪ�����ò������� */
    genFloatMeshInfo.mapStepH    = 8;
    genFloatMeshInfo.correctX    = 1; /* ˮƽx����У��: 1����У��, 0����У�� */
    genFloatMeshInfo.correctY    = 1; /* ��ֱy����У��: 1����У��, 0����У�� */
    genFloatMeshInfo.saveMaxFovX = 0; /* ����ˮƽx�������FOV: 1������, 0�������� */
    ret                          = calcFloatMeshSizeRK(&genFloatMeshInfo);
    if (-1 == ret) {
        printf("%s: failed to calcFloatMeshSize\n", __func__);
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
    pf32MeshX = new float[genFloatMeshInfo.u64MapBufSize];

    /* LDCH���㻯����ڴ����� */
    pu16MeshX = new uint16_t[stLdchLdcvInfo.u64LdchMeshBufSize];

    /* �����²��������ĳ�ʼ�� */
    ret = genFloatMeshNLevelInit(&camCoeff, &genFloatMeshInfo);
    if (-1 == ret) {
        printf("%s: failed to genFloatMeshNLevelInit\n", __func__);
        goto exit;
    }

    uint32_t level        = 0; /* level��Χ: 0-255 */
    uint32_t levelValue[] = {0, 64, 128, 192, 255};
    for (uint32_t levelIdx = 0; levelIdx < 5; ++levelIdx) {
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
            sprintf(ldchMeshPath, "../data/%s_LDCH_meshX_src%dx%d_dst%dx%d_%s_level%03d.bin",
                    saveFilePrefix, srcWid, srcHgt, dstWid, dstHgt, __func__, level);
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
                    "../data/%s_ldchFloatMesh_src%dx%d_dst%dx%d_%s_level%03d.bin", saveFilePrefix,
                    srcWid, srcHgt, dstWid, dstHgt, __func__, level);
            FILE* fpFloatMeshY = fopen(ldchFloatMeshPath, "wb");
            if (fpFloatMeshY == NULL) {
                printf("%s error: failed to save LDCH float mesh\n", __func__);
                goto exit;
            }
            fwrite(pf32MeshX, sizeof(float), genFloatMeshInfo.u64MapBufSize, fpFloatMeshY);
            fclose(fpFloatMeshY);
        }

        /* ����rk3576 LDCH��LDCV��cmodelЧ�� */
        {
            /* ��ʾ��֤ */
            cv::Mat cvMatFloatMeshX = cv::Mat(ldchFloatMeshH, ldchFloatMeshW, CV_32FC1, pf32MeshX);
            cv::Mat cvMatMeshX = cv::Mat(stLdchLdcvInfo.ldchMeshH, stLdchLdcvInfo.ldchAlignMeshW,
                                         CV_16UC1, pu16MeshX);

            cv::Mat srcImgBGR = cv::imread(srcBGRPath);

            /* RGB ---> YUV444 */
            cv::Mat srcImgYUV444;
            cv::cvtColor(srcImgBGR, srcImgYUV444, cv::COLOR_BGR2YUV);
            uint8_t* pSrcYuv444 = srcImgYUV444.data;

            int dstSize         = dstWid * dstHgt;
            uint8_t* pDstYuv444 = NULL;
            pDstYuv444          = new uint8_t[dstSize * 3];

            /* ����LDCH��LDCV��cmodel */
            uint8_t enLdch = 1;
            uint8_t enLdcv = 0;
            LDCH_LDCV_Cmodel(srcWid, srcHgt, dstWid, dstHgt, pu16MeshX, NULL, enLdch, enLdcv,
                             pSrcYuv444, pDstYuv444);

            /* YUV444 ---> RGB */
            cv::Mat dstImgYUV444 = cv::Mat(dstHgt, dstWid, CV_8UC3, pDstYuv444);
            cv::Mat dstImgBGR;
            cv::cvtColor(dstImgYUV444, dstImgBGR, cv::COLOR_YUV2BGR);

            /* ���� */
            char saveDstImgPath[1024];
            sprintf(saveDstImgPath, "../data/%s_result_ldch%d_ldcv%d_%s_level%03d.bmp",
                    saveFilePrefix, enLdch, enLdcv, __func__, level);
            cv::imwrite(saveDstImgPath, dstImgBGR);

            if (NULL != pDstYuv444) {
                delete[] pDstYuv444;
                pDstYuv444 = NULL;
            }
        }
    }

exit:

    /* �²���������ڴ��ͷ� */
    if (NULL != pf32MeshX) {
        delete[] pf32MeshX;
        pf32MeshX = NULL;
    }
    /* LDCH���㻯���ڴ��ͷ� */
    if (NULL != pu16MeshX) {
        delete[] pu16MeshX;
        pu16MeshX = NULL;
    }
}
// clang-format on
