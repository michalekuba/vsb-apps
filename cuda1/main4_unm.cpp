#include <stdio.h>
#include <cuda_device_runtime_api.h>
#include <cuda_runtime.h>
#include <opencv2/opencv.hpp>

#include "uni_mem_allocator.h"
#include "cuda_img.h"

namespace cv {
}

// Function prototype from .cu file
void cu_run_grayscale(CudaImg t_bgr_cuda_img, CudaImg t_bw_cuda_img);

int main(int t_numarg, char** t_arg)
{
    // Uniform Memory allocator for Mat
    UniformAllocator allocator;
    cv::Mat::setDefaultAllocator(&allocator);

    if (t_numarg < 2)
    {
        printf("Enter picture filename!\n");
        return 1;
    }

    // Load image
    cv::Mat l_bgr_cv_img = cv::imread(t_arg[1], cv::IMREAD_COLOR); // CV_LOAD_IMAGE_COLOR );

    if (!l_bgr_cv_img.data)
    {
        printf("Unable to read file '%s'\n", t_arg[1]);
        return 1;
    }

    // create empty BW image
    cv::Mat l_bw_cv_img(l_bgr_cv_img.size(), CV_8U);

    // data for CUDA
    CudaImg l_bgr_cuda_img, l_bw_cuda_img;
    l_bgr_cuda_img.m_size.x = l_bw_cuda_img.m_size.x = l_bgr_cv_img.size().width;
    l_bgr_cuda_img.m_size.y = l_bw_cuda_img.m_size.y = l_bgr_cv_img.size().height;
    l_bgr_cuda_img.m_p_uchar3 = (uchar3*)l_bgr_cv_img.data;
    l_bw_cuda_img.m_p_uchar1 = (uchar1*)l_bw_cv_img.data;

    // Function calling from .cu file
    cu_run_grayscale(l_bgr_cuda_img, l_bw_cuda_img);

    // Access individual pixels using at1, at3, and at4 methods
    for (int y = 0; y < l_bw_cv_img.rows; y++)
    {
        for (int x = 0; x < l_bw_cv_img.cols; x++)
        {
            uchar1& pixel1 = l_bw_cuda_img.at1(y, x);
            uchar3& pixel3 = l_bgr_cuda_img.at3(y, x);
            uchar4& pixel4 = l_bgr_cuda_img.at4(y, x);

            // Modify the pixels if needed
            // For example, accessing the grayscale value:
            // uchar grayscaleValue = pixel1.x;
        }
    }

    // Show the Color and BW image
    cv::imshow("Color", l_bgr_cv_img);
    cv::imshow("GrayScale", l_bw_cv_img);
    cv::waitKey(0);
}
