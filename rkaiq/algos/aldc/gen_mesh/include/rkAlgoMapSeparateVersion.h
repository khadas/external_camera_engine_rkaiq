#ifndef __RKALGO_MAP_SEPARATE_VERSION_H__
#define __RKALGO_MAP_SEPARATE_VERSION_H__
// clang-format off

/*!
 * ==================== VERSION HISTORY ====================
 *
* v1.0.0
*  - date: 2023-02-16
*  - content:
                1. base version of rkAlgoGenMeshTool
                2. generate float mesh
                3. generate fixed mesh from float mesh for RK FEC & LDCH



*/

/* �汾ģʽ */
#ifdef _DEBUG
    #define MAP_SEPARATE_VER_MODE "debug"
#elif NDEBUG
    #define MAP_SEPARATE_VER_MODE "release"
#endif

#define MAP_SEPARATE_VER_NUM "rkAlgoMapSeparate Version: 1.0.0"

#define MAP_SEPARATE_VER_DATE "date: 2023-07-07"

/* �汾���ַ�����lib�汾��.��������.�汾ģʽ */
#define RKALGO_MAP_SEPARATE_VER (MAP_SEPARATE_VER_NUM " " MAP_SEPARATE_VER_DATE " " MAP_SEPARATE_VER_MODE)

// clang-format on
#endif  // !__RKALGO_MAP_SEPARATE_VERSION_H__
