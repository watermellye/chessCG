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
/*    ���ǻ���Ҫ��ԴHDRͼ��ת��Ϊ��������ͼ����Ϊ��ʵ����һ�㣬���Ǳ����ͬһ����������Ⱦ���Σ�ÿ������������һ���棬����֡��������¼����*/
void SetupFrambuffer(unsigned int& captureFBO, unsigned int& captureRBO);
// pbr: load the HDR environment map
 // ---------------------------------
/*   stb_image.h �Զ��� HDR ֵӳ�䵽һ���������б�Ĭ������£�ÿ��ͨ��32λ��ÿ����ɫ 3 ��ͨ����*/
void StbMapHDR(unsigned int& hdrTexture);
// pbr: setup cubemap to render to and attach to framebuffer
// ---------------------------------------------------------
//���Ǵ�ʱ�Ϳ���������Ӧ����������ͼ�ˣ�����Ϊ��������Ԥ�ȷ����ڴ棺
void SetCubmaprenger(unsigned int& envCubemap);
// pbr: convert HDR equirectangular environment map to cubemap equivalent
 // ----------------------------------------------------------------------
//��������������������������ͬ����ͼ���󣬸���ͶӰ����� fov Ϊ 90 ���Բ�׽�����棬����Ⱦ���������Σ�������洢�ڸ���֡�����У�
void SetupCubeMap
(
    Shader1& equirectangularToCubemapShader,
    unsigned int& hdrTexture,
    unsigned int& captureFBO,
    unsigned int& envCubemap
);
// pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
    // --------------------------------------------------------------------------------
    /*���� OpenGL ��Ⱦ���룬�Ա����ǿ��Զ�֮ǰ��׽�� envCubemap ������
    �������Ǵ���һ�����ն���������ͼ
    ������ֻ��Ҫ����Ⱦѭ��֮ǰִ��һ�Σ�*/
void CeateIrradiance(unsigned int& irradianceMap, unsigned int& captureFBO, unsigned int& captureRBO);

// pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
 // -----------------------------------------------------------------------------
 /*����ʹ�þ����ɫ�������Ͳ�׽������������ͼ���Ƶķ�ʽ�������Ի�����ͼ������*/
void ConvolutionIttadianShader
(
    Shader1& irradianceShader,
    unsigned int& envCubemap,
    unsigned int& captureFBO,
    unsigned int& irradianceMap
);

// pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
 // --------------------------------------------------------------------------------
 /*Ԥ�˲�������ͼ�ķ��������ǶԷ������ͼ�����ķ����ǳ����ơ�
     ���ھ����ÿ���ֲڶȼ������ǽ���˳���ģ����Ľ���洢��Ԥ�˲���ͼ�� mipmap �С�
     ���ȣ�������Ҫ����һ���µ���������ͼ������Ԥ���˵Ļ�����ͼ���ݡ�
     Ϊ��ȷ��Ϊ�� mip ��������㹻���ڴ棬һ���򵥷����ǵ��� glGenerateMipmap��*/

void CreatePrefileter(unsigned int& prefilterMap);

// pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
  // ----------------------------------------------------------------------------------------------------
  //�� OpenGL �ڶ�� mipmap �������Բ�ͬ�Ĵֲڶ�ֵԤ���˻�����ͼ��
 
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
 /*Ϊ�˴洢 BRDF ��������������Ҫ����һ�� 2D ����*/
void StoreBRDF
(
    Shader1& brdfShader,
    unsigned int& brdfLUTTexture,
    unsigned int& captureFBO,
    unsigned int& captureRBO
);
