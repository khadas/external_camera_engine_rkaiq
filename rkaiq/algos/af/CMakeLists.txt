set(THIRDAF_SRC
    rk_aiq_algo_af_itf_thirdalgo.c
    rk_aiq_uapi_af_int_thirdalgo.c
    )

dec_algo(thirdaf rk_aiq_algo_af_itf_thirdalgo.c ${THIRDAF_SRC})

if (RKAIQ_HAVE_AF_V20 OR RKAIQ_HAVE_AF_V30 OR RKAIQ_HAVE_AF_V31 OR RKAIQ_HAVE_AF_V32_LITE OR RKAIQ_HAVE_AF_V33)
set(AF_SRC
    rk_aiq_algo_af_itf.c
    af.c
    af_search.c
    af_trigger.c
    af_utils.c
    rk_aiq_uapi_af_int.c
    )

dec_algo(af rk_aiq_algo_af_itf.c ${AF_SRC})

if(RKAIQ_HAVE_PDAF)
    if (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    if("${ARCH}" STREQUAL "arm")
        file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/android/arm/librkaiq_pdaf.a	DESTINATION ${LIBRARY_OUTPUT_PATH})
    endif()
    if("${ARCH}" STREQUAL "aarch64")
        file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/android/arm64/librkaiq_pdaf.a  DESTINATION ${LIBRARY_OUTPUT_PATH})
    endif()
    else ()
    if("${ARCH}" STREQUAL "arm")
        file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/linux/arm/librkaiq_pdaf.a	DESTINATION ${LIBRARY_OUTPUT_PATH})
    endif()
    if("${ARCH}" STREQUAL "aarch64")
        file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/linux/aarch64/librkaiq_pdaf.a  DESTINATION ${LIBRARY_OUTPUT_PATH})
    endif()
    endif()

    add_library(rkaiq_pdaf STATIC IMPORTED GLOBAL)
    set_target_properties(rkaiq_pdaf PROPERTIES IMPORTED_LOCATION 	"${PREFIX}rkaiq_pdaf.${SUFFIX}")
    set_target_properties(rkaiq_pdaf PROPERTIES IMPORTED_IMPLIB 	"${PREFIX}rkaiq_pdaf.${SUFFIX}")
endif()

endif()
