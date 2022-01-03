#pragma once 

#include "Objectetc.h"

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
void renderCube();
// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
void renderQuad();

// pbr: setup framebuffer
// ----------------------
/*    我们还需要将源HDR图像转换为立方体贴图纹理。为了实现这一点，我们必须对同一个立方体渲染六次，每次面对立方体的一个面，并用帧缓冲对象记录其结果*/
void SetupFrambuffer(unsigned int& captureFBO, unsigned int& captureRBO);
// pbr: load the HDR environment map
 // ---------------------------------
/*   stb_image.h 自动将 HDR 值映射到一个浮点数列表：默认情况下，每个通道32位，每个颜色 3 个通道。*/
void StbMapHDR(unsigned int& hdrTexture);
// pbr: setup cubemap to render to and attach to framebuffer
// ---------------------------------------------------------
//我们此时就可以生成相应的立方体贴图了，首先为其六个面预先分配内存：
void SetCubmaprenger(unsigned int& envCubemap);
// pbr: convert HDR equirectangular environment map to cubemap equivalent
 // ----------------------------------------------------------------------
//面向立方体六个面设置六个不同的视图矩阵，给定投影矩阵的 fov 为 90 度以捕捉整个面，并渲染立方体六次，将结果存储在浮点帧缓冲中：
void SetupCubeMap
(
    Shader1& equirectangularToCubemapShader,
    unsigned int& hdrTexture,
    unsigned int& captureFBO,
    unsigned int& envCubemap
);
// pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
    // --------------------------------------------------------------------------------
    /*设置 OpenGL 渲染代码，以便我们可以对之前捕捉的 envCubemap 求卷积。
    首先我们创建一个辐照度立方体贴图
    （我们只需要在渲染循环之前执行一次）*/
void CeateIrradiance(unsigned int& irradianceMap, unsigned int& captureFBO, unsigned int& captureRBO);

// pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
 // -----------------------------------------------------------------------------
 /*我们使用卷积着色器——和捕捉环境立方体贴图类似的方式——来对环境贴图求卷积：*/
void ConvolutionIttadianShader
(
    Shader1& irradianceShader,
    unsigned int& envCubemap,
    unsigned int& captureFBO,
    unsigned int& irradianceMap
);

// pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
 // --------------------------------------------------------------------------------
 /*预滤波环境贴图的方法与我们对辐射度贴图求卷积的方法非常相似。
     对于卷积的每个粗糙度级别，我们将按顺序把模糊后的结果存储在预滤波贴图的 mipmap 中。
     首先，我们需要生成一个新的立方体贴图来保存预过滤的环境贴图数据。
     为了确保为其 mip 级别分配足够的内存，一个简单方法是调用 glGenerateMipmap。*/

void CreatePrefileter(unsigned int& prefilterMap);

// pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
  // ----------------------------------------------------------------------------------------------------
  //让 OpenGL 在多个 mipmap 级别上以不同的粗糙度值预过滤环境贴图。
 
void RunQuasiPrefilter
(
    Shader1& prefilterShader,
    unsigned int& envCubemap,
    unsigned int& captureFBO,
    unsigned int& captureRBO,
    unsigned int& prefilterMap
);
// pbr: generate a 2D LUT from the BRDF equations used.
 // ----------------------------------------------------
 /*为了存储 BRDF 卷积结果，我们需要生成一张 2D 纹理。*/
void StoreBRDF
(
    Shader1& brdfShader,
    unsigned int& brdfLUTTexture,
    unsigned int& captureFBO,
    unsigned int& captureRBO
);
