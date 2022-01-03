#include "skybox.h"
#include "chessRule.h"
// #include "bloom1.h"

// lights
// ------
glm::vec3 lightPositions[] = {
    glm::vec3(-10.0f, 10.0f, 10.0f),
    glm::vec3(10.0f, 10.0f, 10.0f),
    glm::vec3(-10.0f, -10.0f, 10.0f),
    glm::vec3(10.0f, -10.0f, 10.0f),
};
glm::vec3 lightColors[] = {glm::vec3(300.0f, 300.0f, 300.0f), glm::vec3(300.0f, 300.0f, 300.0f), glm::vec3(300.0f, 300.0f, 300.0f), glm::vec3(300.0f, 300.0f, 300.0f)};

int nrRows = 7;
int nrColumns = 7;
float spacing = 2.5;
int now, cnt, nowPos = -1, nowFrame;

void RenderLignt(Shader1& pbrShader) {
    for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i) {
        glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
        newPos = lightPositions[i];
        pbrShader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
        pbrShader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
    }
}

int main() {
    /*
    inform info[12][8] = {{{0, 0, 0, None, 1}, {1, 0, 0, Selected, 1}, {2, 0, 2, None, 1}, {3, 0, 3, None, 1}, {4, 0, 4, None, 1}, {5, 0, 5, None, 1}, {6, 0, 6, None, 1}, {7, 0, 7, None, 1}},
                          {{0, 1, 0, None, 1}, {1, 1, 0, None, 1}},
                          {{2, 1, 0, None, 1}, {3, 1, 0, None, 1}},
                          {{4, 1, 0, None, 1}},
                          {{5, 1, 0, None, 1}},
                          {{6, 1, 0, None, 1}, {7, 1, 0, None, 1}},
                          {{0, 2, 0, None, 1}, {1, 2, 0, None, 1}, {2, 2, 0, None, 1}, {3, 2, 0, None, 1}, {4, 2, 0, None, 1}, {5, 2, 0, None, 1}, {6, 2, 0, None, 1}, {7, 2, 0, None, 1}},
                          {{0, 3, 0, None, 1}, {1, 3, 0, None, 1}},
                          {{2, 3, 0, None, 1}, {3, 3, 0, None, 1}},
                          {{4, 3, 0, None, 1}},
                          {{5, 3, 0, None, 1}},
                          {{6, 3, 0, None, 1}, {7, 3, 0, None, 1}}};
    */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //加载window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GroupWork", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // -----------------------------
    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);
    // set depth function to less than AND equal for skybox depth trick.
    /*xyww 可以确保渲染的立方体片段的深度值总是 1.0，即最大深度，如立方体贴图教程中所述。注意我们需要将深度比较函数更改为 GL_LEQUAL：*/
    glDepthFunc(GL_LEQUAL);
    // enable seamless cubemap sampling for lower mip levels in the pre-filter map.
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    // 1wwww
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // build and compile shaders
    // -------------------------
    // pbr，环境贴图
    Shader1 pbrShader("./HLSL_Shader/pbr.vert", "./HLSL_Shader/pbr.frag");
    //特殊显示
    Shader1 outliningProgram("./HLSL_Shader/outlining.vert", "./HLSL_Shader/outlining.frag");
    //等距柱状投影图
    Shader1 equirectangularToCubemapShader("./HLSL_Shader/cubemap.vert", "./HLSL_Shader/equirectangular_to_cubemap.frag");
    //辐照度，用于计算环境光照
    Shader1 irradianceShader("./HLSL_Shader/cubemap.vert", "./HLSL_Shader/irradiance_convolution.frag");
    //镜面反射,prefilter,brd,预计算的光照数据
    //用于计算镜面反射，根据表面粗糙度在合适的 mip 级别采样，以使更粗糙的表面产生更模糊的镜面反射。
    Shader1 prefilterShader("./HLSL_Shader/cubemap.vert", "./HLSL_Shader/prefilter.frag");
    Shader1 brdfShader("./HLSL_Shader/brdf.vert", "./HLSL_Shader/brdf.frag");
    Shader1 backgroundShader("./HLSL_Shader/background.vert", "./HLSL_Shader/background.frag");

    pbrShader.Activate();
    pbrShader.setInt("irradianceMap", 3);  //同一个shader有多张图时，不得重复，mesh中texture共有三张，所以从3开始
    pbrShader.setInt("prefilterMap", 4);
    pbrShader.setInt("brdfLUT", 5);

    Shader1 shaderBloom("./HLSL_Shader/bloom.vert", "./HLSL_Shader/bloom.frag");
    shaderBloom.Activate();
    shaderBloom.setVec4("lightColor_1", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    //天空盒
    backgroundShader.Activate();
    backgroundShader.setInt("environmentMap", 0);

    // pbr: setup framebuffer
    unsigned int captureFBO;
    unsigned int captureRBO;
    SetupFrambuffer(captureFBO, captureRBO);
    // pbr: load the HDR environment map
    unsigned int hdrTexture;
    StbMapHDR(hdrTexture);
    // pbr: setup cubemap to render to and attach to framebuffer
    unsigned int envCubemap;
    SetCubmaprenger(envCubemap);
    SetupCubeMap(equirectangularToCubemapShader, hdrTexture, captureFBO, envCubemap);
    // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
    unsigned int irradianceMap;
    CeateIrradiance(irradianceMap, captureFBO, captureRBO);
    // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
    ConvolutionIttadianShader(irradianceShader, envCubemap, captureFBO, irradianceMap);
    // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
    unsigned int prefilterMap;
    CreatePrefileter(prefilterMap);
    // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
    RunQuasiPrefilter(prefilterShader, envCubemap, captureFBO, captureRBO, prefilterMap);
    // pbr: generate a 2D LUT from the BRDF equations used.
    unsigned int brdfLUTTexture;
    StoreBRDF(brdfShader, brdfLUTTexture, captureFBO, captureRBO);
    // initialize static shader uniforms before rendering

    //相机
    Model1 model("models_new/BoardFixed/board.gltf");
    Model1 model2("models_new/Board/board.gltf");
    /* 棋子数组和要传的信息 */
    ChessPiece piece[12] = {{"models_new/Black/Pawn/ALL.gltf", Pawn}, {"models_new/Black/Rook/ALL.gltf", Rook},   {"models_new/Black/Knight/ALL.gltf", Knight},
                            {"models_new/Black/King/ALL.gltf", King}, {"models_new/Black/Queen/ALL.gltf", Queen}, {"models_new/Black/Bishop/ALL.gltf", Bishop},
                            {"models_new/White/Pawn/ALL.gltf", Pawn}, {"models_new/White/Rook/ALL.gltf", Rook},   {"models_new/White/Knight/ALL.gltf", Knight},
                            {"models_new/White/King/ALL.gltf", King}, {"models_new/White/Queen/ALL.gltf", Queen}, {"models_new/White/Bishop/ALL.gltf", Bishop}};

    int currentFrame = 0;  //待输入

    backgroundShader.Activate();
    backgroundShader.setMat4("projection", camera.projection);

    // then before rendering, configure the viewport to the original framebuffer's screen dimensions
    RetrieveSizeOfWindow(window);

    now = glfwGetTime() * 60;
    cnt = now;
    inform info[32] = {
        {6, 0, now, None, 1},       {6, 1, now - 30, None, 1},  {6, 2, now - 60, None, 1},  {6, 3, now - 90, None, 1},  {6, 4, now - 120, None, 1}, {6, 5, now - 150, None, 1},
        {6, 6, now - 180, None, 1}, {6, 7, now - 210, None, 1}, {7, 0, now, None, 1},       {7, 7, now - 210, None, 1}, {7, 1, now - 30, None, 1},  {7, 6, now - 180, None, 1},
        {7, 3, now - 90, None, 1},  {7, 4, now - 120, None, 1}, {7, 2, now - 60, None, 1},  {7, 5, now - 150, None, 1}, {1, 0, now - 210, None, 1}, {1, 1, now - 180, None, 1},
        {1, 2, now - 150, None, 1}, {1, 3, now - 120, None, 1}, {1, 4, now - 90, None, 1},  {1, 5, now - 60, None, 1},  {1, 6, now - 30, None, 1},  {1, 7, now, None, 1},
        {0, 0, now - 210, None, 1}, {0, 7, now, None, 1},       {0, 1, now - 180, None, 1}, {0, 6, now - 30, None, 1},  {0, 3, now - 120, None, 1}, {0, 4, now - 90, None, 1},
        {0, 2, now - 150, None, 1}, {0, 5, now - 60, None, 1},
    };
    ChessBoard qGame;

    int ccnt = 0;
    while (!glfwWindowShouldClose(window)) {
        // std::cout << now << std::endl;
        // glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //相机处理输入
        camera.Inputs(window);
        camera.width = SCR_WIDTH;
        camera.height = SCR_HEIGHT;
        camera.updateMatrix(45.0f, 0.01f, 100.0f);
        backgroundShader.Activate();
        backgroundShader.setMat4("projection", camera.projection);

        pbrShader.Activate();
        // bind pre-computed IBL data //0 1 2 已经属于三张贴图
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);

        //模型载入绘制。
        //如果对
        //棋盘绘制
        model.Draw(pbrShader, camera, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(0, 0, 0.0f, 1.0f), glm::vec3(1, 1, 1));
        model2.Draw(pbrShader, camera, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(0, 0, 0.0f, 1.0f), glm::vec3(1, 1, 1));
        //绘制棋子

        nowFrame += glfwGetTime() * 60 - now;
        now = glfwGetTime() * 60;
        if (now - cnt > 120) {  // 演示120 平时300
            qGame.getInfo(info, now);
            cnt = now;
            ++ccnt;
        }
        if (ccnt == 3 && now - cnt >= 60) {  // 演示60 平时200
            ccnt = 0;
            nowFrame = 0;
            nowPos *= -1;
        }

        int ind = 0;
        for (int i = 0; i < 12; i++)
            for (int j = 0; j < pieceNum[i]; j++) {
                inform inff = info[ind];
                if (inff.alive == 1) {
                    inff.xpos = info[ind].ypos;
                    inff.ypos = 7 - info[ind].xpos;
                    inff.oxpos = info[ind].oypos;
                    inff.oypos = 7 - info[ind].oxpos;

                    inff.currentFrame = now - inff.currentFrame;
                    if (inff.currentFrame >= max_frame[inff.moveType]) {
                        switch (inff.moveType) {
                            case Fade:
                                info[ind].alive = 0;
                                inff.alive = 0;
                                break;
                            case Move:
                                info[ind].moveType = None;
                                inff.moveType = None;
                                info[ind].currentFrame = now;
                                inff.currentFrame = 0;
                                break;
                            default:
                                inff.currentFrame %= max_frame[inff.moveType];
                        }
                    }
                    if (inff.alive) {
                        // printf("type=%d uid=%d pos=(%d,%d) currentFrame=%d moveType=%d\n", i, ind, inff.xpos, inff.ypos, inff.currentFrame, inff.moveType);
                        if (inff.moveType == Fade)
                            (piece[i]).Draw(shaderBloom, outliningProgram, camera, inff);
                        else
                            (piece[i]).Draw(pbrShader, outliningProgram, camera, inff);
                    }
                }
                ind++;
            }

        RenderLignt(pbrShader);
        // render skybox (render as last to prevent overdraw)
        backgroundShader.Activate();
        backgroundShader.setMat4("view", camera.view);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
        renderCube();

        // glDepthFunc(GL_LESS);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    pbrShader.Delete();
    outliningProgram.Delete();
    equirectangularToCubemapShader.Delete();
    irradianceShader.Delete();
    prefilterShader.Delete();
    brdfShader.Delete();
    backgroundShader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
