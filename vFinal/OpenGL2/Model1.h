#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include "json.h"
#include "Mesh1.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//
#include <cstring>
#include <cmath>
#include <minmax.h>
const float PI = 3.1415926535;

using json = nlohmann::json;

enum ChessType { Pawn, Rook, Knight, King, Queen, Bishop };
// 5
enum MoveType { None, Selected, Fade, Move };

enum FrameType { Once, Repeat };

struct inform {
    int xpos = -1, ypos = -1;  // 坐标
    int currentFrame = 0;      // 当前动画的起始帧
    MoveType moveType = None;
    bool alive = 1;
    int oxpos = -1, oypos = -1;
};
const float distance = -0.006;
const int max_frame[] = {240, 60, 60, 120};
const FrameType frameType[] = {Repeat, Repeat, Once, Once};
const float gradient_x[6] = {0.014228472, 0.014100714, 0.014121638, 0.014414577, 0.013577447, 0.014057325};
const float intercept_x[6] = {-0.049799651, -0.049423683, -0.049425733, -0.050451018, -0.047521064, -0.049200638};
const float gradient_z[6] = {-0.019844644, -0.020051819, -0.019651265, -0.019852161, -0.019857628, -0.019637355};
const float intercept_z[6] = {0.069456254, 0.070711835, 0.06877943, 0.069482564, 0.069501698, 0.068730742};
const float y[6] = {-0.003820614, -0.010267652, -0.007045557, -0.011803529, -0.012430522, -0.013714691};
const float outlining_y[6] = {1, 1 / 3, 1 / 4, 2 / 3, 1 / 2, 1 / 2};
const int pieceNum[12] = {8, 2, 2, 1, 1, 2, 8, 2, 2, 1, 1, 2};
// 4

struct chess_info {
    float xpos, ypos;     // 坐标
    char color;           // 颜色
    ChessType chessType;  // 棋子类型
    MoveType moveType;
    float height = 0;
};

class Model1 {
   public:
    // Loads in a model from a file and stores tha information in 'data', 'JSON', and 'file'
    Model1(const char* file);

    void Draw(Shader1& shader, Camera1& camera, glm::vec3 translation, glm::quat rotation, glm::vec3 scale);

    void Draw1(Shader1& shader, Camera1& camera, chess_info info);
    // 6
    void Draw2(Shader1& shader, Camera1& camera, chess_info info);

   private:
    // Variables for easy access
    const char* file;
    std::vector<unsigned char> data;
    json JSON;

    // All the meshes and transformations
    std::vector<Mesh1> meshes;
    std::vector<glm::vec3> translationsMeshes;
    std::vector<glm::quat> rotationsMeshes;
    std::vector<glm::vec3> scalesMeshes;
    std::vector<glm::mat4> matricesMeshes;

    // Prevents textures from being loaded twice
    std::vector<std::string> loadedTexName;
    std::vector<Texture1> loadedTex;

    // Loads a single mesh by its index
    void loadMesh(unsigned int indMesh);

    // Traverses a node recursively, so it essentially traverses all connected nodes
    void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

    // Gets the binary data from a file
    std::vector<unsigned char> getData();
    // Interprets the binary data into floats, indices, and textures
    std::vector<float> getFloats(json accessor);
    std::vector<GLuint> getIndices(json accessor);
    std::vector<Texture1> getTextures();

    // Assembles all the floats into vertices
    std::vector<Vertex> assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs);

    // Helps with the assembly from above by grouping floats
    std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
    std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
    std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};

class Model2 {
   public:
    // Loads in a model from a file and stores tha information in 'data', 'JSON', and 'file'
    Model2(const char* file);

    void Draw(Shader1& shader, Camera1& camera, glm::vec3 translation, glm::quat rotation, glm::vec3 scale);

   private:
    // Variables for easy access
    const char* file;
    std::vector<unsigned char> data;
    json JSON;

    // All the meshes and transformations
    std::vector<Mesh1> meshes;
    std::vector<glm::vec3> translationsMeshes;
    std::vector<glm::quat> rotationsMeshes;
    std::vector<glm::vec3> scalesMeshes;
    std::vector<glm::mat4> matricesMeshes;

    // Prevents textures from being loaded twice
    std::vector<std::string> loadedTexName;
    std::vector<Texture1> loadedTex;

    // Loads a single mesh by its index
    void loadMesh(unsigned int indMesh);

    // Traverses a node recursively, so it essentially traverses all connected nodes
    void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

    // Gets the binary data from a file
    std::vector<unsigned char> getData();
    // Interprets the binary data into floats, indices, and textures
    std::vector<float> getFloats(json accessor);
    std::vector<GLuint> getIndices(json accessor);
    std::vector<Texture1> getTextures();

    // Assembles all the floats into vertices
    std::vector<Vertex> assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs);

    // Helps with the assembly from above by grouping floats
    std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
    std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
    std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};

class ChessPiece {
   public:
    ChessPiece(const char* _file, ChessType _type) : file(_file), type(_type) {
        std::string filename = file;
        model = new Model1(_file);
        if (filename.find("White") != std::string::npos)
            mPieceColor = 'W';
        else if (filename.find("Black") != std::string::npos)
            mPieceColor = 'B';
        filename = filename.replace(filename.find("new"), 3, "outlining");
        filename = filename.replace(filename.find("ALL"), 3, "outlining");
        outlining = new Model1(filename.c_str());
    }
    ~ChessPiece() {}
    void Draw(Shader1& shaderProgram, Shader1& outliningProgram, Camera1& camera, inform info) {
        if (info.alive == 1) {
            float height = 0.5;
            // std::cout << "currentFrame " << info.currentFrame << " max_frame " << max_frame[info.moveType];
            // std::cout << " Height " << height << std::endl;
            if (info.moveType == Selected) {
                //棋子上下浮动幅度变大+outlining，如果不需要outlining删掉除model.draw1之外的语句
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glStencilMask(0xFF);
                model->Draw1(shaderProgram, camera, {(float)info.xpos, (float)info.ypos, mPieceColor, type, info.moveType, height});
                glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
                glStencilMask(0x00);
                glDisable(GL_DEPTH_TEST);
                outliningProgram.Activate();
                outlining->Draw2(outliningProgram, camera, {(float)info.xpos, (float)info.ypos, mPieceColor, type, info.moveType, height});
                glStencilMask(0xFF);
                glStencilFunc(GL_ALWAYS, 0, 0xFF);
                glEnable(GL_DEPTH_TEST);
            } else if (info.moveType == None) {
                //上下浮动
                height = float(info.currentFrame) / float(max_frame[info.moveType]);
                height = 0.5 * (1 + sin(height * 2 * PI));
                model->Draw1(shaderProgram, camera, {(float)info.xpos, (float)info.ypos, mPieceColor, type, info.moveType, height});
            } else if (info.moveType == Fade) {
                int endFrame = max_frame[info.moveType];
                if (endFrame < info.currentFrame) return;
                float color_r = (rand() % 3) / 2;
                shaderProgram.Activate();
                glm::vec4 lightColor_1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                if (info.currentFrame > endFrame / 2) color_r = (1 - float(info.currentFrame) / float(endFrame)) * 2;
                if (mPieceColor == 'B')
                    lightColor_1 = glm::vec4(color_r / 2, color_r / 3, color_r / 4, 1.0f);
                else
                    lightColor_1 = glm::vec4(color_r, color_r, color_r, 1.0f);
                shaderProgram.setVec4("lightColor_1", lightColor_1);
                model->Draw1(shaderProgram, camera, {(float)info.xpos, (float)info.ypos, mPieceColor, type, info.moveType, height});
            } else if (info.moveType == Move) {
                float x = float(info.currentFrame) / float(max_frame[info.moveType]);
                if (x < 0.25) {
                    // [0,0.25)    升起 height 0.5~3 由快到慢 oxpos oypos height=0.5+2.5*sin(PI * 2 * x)
                    // printf("x=%.2f oxpos=%d oypos=%d xpos=%d ypos=%d height=%f\n", x, info.oxpos, info.oypos, info.xpos, info.ypos, float(0.5 + 2.5 * sin(PI * 2 * x)));
                    model->Draw1(shaderProgram, camera, {(float)info.oxpos, (float)info.oypos, mPieceColor, type, info.moveType, float(0.5 + 2.5 * sin(PI * 2 * x))});
                } else if (x > 0.75) {
                    // [0.75,1]    降落 height 3~0.5 由快到慢 xpos ypos height=3-2.5*sin(PI * 2 * (x - 0.75))
                    // printf("x=%.2f oxpos=%d oypos=%d xpos=%d ypos=%d height=%f\n", x, info.oxpos, info.oypos, info.xpos, info.ypos, float(0.5 + 2.5 * sin(PI * 2 * (x - 0.75))));
                    model->Draw1(shaderProgram, camera, {(float)info.xpos, (float)info.ypos, mPieceColor, type, info.moveType, float(3 - 2.5 * sin(PI * 2 * (x - 0.75)))});
                } else {
                    // [0.25,0.75) 移动 height=3     由快到慢 oxpos→xpos oypos→ypos x=oxpos+(xpos-oxpos)*[(x-0.25)*2] y=oypos+(ypos-oypos)*[(x-0.25)*2]
                    float rr = (x - 0.25) * 2;
                    rr = sin(PI / 2 * rr);
                    model->Draw1(shaderProgram, camera, {float(info.oxpos + (info.xpos - info.oxpos) * rr), float(info.oypos + (info.ypos - info.oypos) * rr), mPieceColor, type, info.moveType, 3.0f});
                }
            }
        }
    }

   protected:
    const char* file;
    Model1* model;
    Model1* outlining;
    char mPieceColor;
    ChessType type;
};
#endif