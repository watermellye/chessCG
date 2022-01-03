
const glm::vec4 lightColor_1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
void Bloom1::shine(Model1 type, Shader1 shader, Camera1& camera, chess_info info) {
    if (info.max_frame < info.currentFrame) return;
    int endFrame = info.max_frame;
    int color_r = (rand() % 3);
    shader.Activate();
    glm::vec4 lightColor_1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    if (info.currentFrame <= (endFrame)*2 / 3)
        lightColor_1 = glm::vec4(color_r, color_r, color_r, 1.0f);
    else {
        color_r = endFrame - info.currentFrame;
        lightColor_1 = glm::vec4(color_r, color_r, color_r, 1.0f);
    }
    shader.setVec4("lightColor_1", lightColor_1);
    type.Draw1(shader, info);
    if (info.currentFrame >= endFrame) {
        shader.Delete();
    }
}
