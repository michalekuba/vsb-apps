#pragma once

#include <opencv2/core/mat.hpp>

// Structure definition for exchanging data between Host and Device
struct CudaImg
{
    uint3 m_size;             // size of picture
    union {
        void* m_p_void;       // data of picture
        uchar1* m_p_uchar1;   // data of picture
        uchar3* m_p_uchar3;   // data of picture
        uchar4* m_p_uchar4;   // data of picture
    };

    // Method to access uchar1 pixel at (y, x)
    uchar1& at1(int y, int x)
    {
        return m_p_uchar1[y * m_size.x + x];
    }

    // Method to access uchar3 pixel at (y, x)
    uchar3& at3(int y, int x)
    {
        return m_p_uchar3[y * m_size.x + x];
    }

    // Method to access uchar4 pixel at (y, x)
    uchar4& at4(int y, int x)
    {
        return m_p_uchar4[y * m_size.x + x];
    }
};
