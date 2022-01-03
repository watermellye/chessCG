#include "Model1.h"

Model1::Model1(const char* file) {
    // Make a JSON object
    std::string text = get_file_contents(file);
    JSON = json::parse(text);

    // Get the binary data
    Model1::file = file;
    data = getData();
    // std::cout << "55";
    //  Traverse all nodes
    traverseNode(0);
    // std::cout << "65";
}

void Model1::Draw(Shader1& shader, Camera1& camera, glm::vec3 translation, glm::quat rotation, glm::vec3 scale) {
    // Go over all meshes and draw each one
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Mesh1::Draw(shader, camera, matricesMeshes[i], translation, rotation, scale);
    }
}

void Model1::Draw1(Shader1& shader, Camera1& camera, chess_info info) {
    //处理chessinfo传入的信息
    float x1 = gradient_x[info.chessType] * info.xpos + intercept_x[info.chessType];
    float z1 = gradient_x[info.chessType] * info.ypos + intercept_x[info.chessType];
    float y1 = y[info.chessType];
    y1 += info.height * distance;
    // 8
    this->Draw(shader, camera, glm::vec3(x1, y1, z1), glm::quat(0.0f, 1.0f, 0.0f, 0.0f), glm::vec3(0.5694760, 0.5694760, 0.5694760));
}
// 7
void Model1::Draw2(Shader1& shader, Camera1& camera, chess_info info) {
    float x1 = gradient_x[info.chessType] * info.xpos + intercept_x[info.chessType];
    float z1 = gradient_x[info.chessType] * info.ypos + intercept_x[info.chessType];
    float y1 = y[info.chessType];
    // std::cout << std::endl << x1 << y1 << z1 << std::endl;
    if (info.moveType == Selected) {
        y1 += info.height * distance * 2;
    }
    if (info.moveType == Selected) this->Draw(shader, camera, glm::vec3(-x1, y1 * outlining_y[info.chessType], -z1), glm::quat(0.0f, 0.0f, 1.0f, 0.0f), glm::vec3(0.5694760, 0.5694760, 0.5694760));
}
void Model1::loadMesh(unsigned int indMesh) {
    // Get all accessor indices
    unsigned int posAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
    unsigned int normalAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
    unsigned int texAccInd;
    if (JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"].is_null())
        texAccInd = 0;
    else
        texAccInd = JSON["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
    unsigned int indAccInd = JSON["meshes"][indMesh]["primitives"][0]["indices"];

    // Use accessor indices to get all vertices components
    std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
    std::vector<glm::vec3> positions = groupFloatsVec3(posVec);
    std::vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
    std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);
    std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
    std::vector<glm::vec2> texUVs = groupFloatsVec2(texVec);

    // Combine all the vertex components and also get the indices and textures
    std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
    std::vector<GLuint> indices = getIndices(JSON["accessors"][indAccInd]);
    std::vector<Texture1> textures = getTextures();

    // Combine the vertices, indices, and textures into a mesh
    meshes.push_back(Mesh1(vertices, indices, textures));
}

void Model1::traverseNode(unsigned int nextNode, glm::mat4 matrix) {
    // Current node
    json node = JSON["nodes"][nextNode];

    // Get translation if it exists
    glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
    if (node.find("translation") != node.end()) {
        float transValues[3];
        for (unsigned int i = 0; i < node["translation"].size(); i++) transValues[i] = (node["translation"][i]);
        translation = glm::make_vec3(transValues);
    }
    // Get quaternion if it exists
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    if (node.find("rotation") != node.end()) {
        float rotValues[4] = {node["rotation"][3], node["rotation"][0], node["rotation"][1], node["rotation"][2]};
        rotation = glm::make_quat(rotValues);
    }
    // Get scale if it exists
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    if (node.find("scale") != node.end()) {
        float scaleValues[3];
        for (unsigned int i = 0; i < node["scale"].size(); i++) scaleValues[i] = (node["scale"][i]);
        scale = glm::make_vec3(scaleValues);
    }
    // Get matrix if it exists
    glm::mat4 matNode = glm::mat4(1.0f);
    if (node.find("matrix") != node.end()) {
        float matValues[16];
        for (unsigned int i = 0; i < node["matrix"].size(); i++) matValues[i] = (node["matrix"][i]);
        matNode = glm::make_mat4(matValues);
    }

    // Initialize matrices
    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 rot = glm::mat4(1.0f);
    glm::mat4 sca = glm::mat4(1.0f);

    // Use translation, rotation, and scale to change the initialized matrices
    trans = glm::translate(trans, translation);
    rot = glm::mat4_cast(rotation);
    sca = glm::scale(sca, scale);

    // Multiply all matrices together
    glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

    // Check if the node contains a mesh and if it does load it
    if (node.find("mesh") != node.end()) {
        translationsMeshes.push_back(translation);
        rotationsMeshes.push_back(rotation);
        scalesMeshes.push_back(scale);
        matricesMeshes.push_back(matNextNode);

        loadMesh(node["mesh"]);
    }

    // Check if the node has children, and if it does, apply this function to them with the matNextNode
    if (node.find("children") != node.end()) {
        for (unsigned int i = 0; i < node["children"].size(); i++) traverseNode(node["children"][i], matNextNode);
    }
}

std::vector<unsigned char> Model1::getData() {
    // Create a place to store the raw text, and get the uri of the .bin file
    std::string bytesText;

    // Store raw text data into bytesText
    std::string fileStr = std::string(file);
    // std::cout << "22";

    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
    if (JSON["buffers"][0]["uri"].is_null()) {
        bytesText = get_file_contents(fileStr.c_str());
    } else {
        std::string uri = JSON["buffers"][0]["uri"];
        std::cout << uri;
        bytesText = get_file_contents((fileDirectory + uri).c_str());
    }
    // std::cout << "33";

    // Transform the raw text data into bytes and put them in a vector
    std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
    return data;
}

std::vector<float> Model1::getFloats(json accessor) {
    std::vector<float> floatVec;

    // Get properties from the accessor
    unsigned int buffViewInd = accessor.value("bufferView", 1);
    unsigned int count = accessor["count"];
    unsigned int accByteOffset = accessor.value("byteOffset", 0);
    std::string type = accessor["type"];

    // Get properties from the bufferView
    json bufferView = JSON["bufferViews"][buffViewInd];
    unsigned int byteOffset = bufferView["byteOffset"];

    // Interpret the type and store it into numPerVert
    unsigned int numPerVert;
    if (type == "SCALAR")
        numPerVert = 1;
    else if (type == "VEC2")
        numPerVert = 2;
    else if (type == "VEC3")
        numPerVert = 3;
    else if (type == "VEC4")
        numPerVert = 4;
    // else if (type == "MAT2") numPerVert = 5;//不会使用到
    // else if (type == "MAT3") numPerVert = 6;
    // else if (type == "MAT4") numPerVert = 7;
    else
        throw std::invalid_argument("Type is invalid (not SCALAR, VEC2, VEC3, or VEC4)");

    // Go over all the bytes in the data at the correct place using the properties from above
    unsigned int beginningOfData = byteOffset + accByteOffset;
    unsigned int lengthOfData = count * 4 * numPerVert;
    for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i) {
        unsigned char bytes[] = {data[i++], data[i++], data[i++], data[i++]};
        float value;
        std::memcpy(&value, bytes, sizeof(float));
        floatVec.push_back(value);
    }

    return floatVec;
}

std::vector<GLuint> Model1::getIndices(json accessor) {
    std::vector<GLuint> indices;

    // Get properties from the accessor
    unsigned int buffViewInd = accessor.value("bufferView", 0);
    unsigned int count = accessor["count"];
    unsigned int accByteOffset = accessor.value("byteOffset", 0);
    unsigned int componentType = accessor["componentType"];

    // Get properties from the bufferView
    json bufferView = JSON["bufferViews"][buffViewInd];
    unsigned int byteOffset;
    if (bufferView["byteOffset"].is_null())
        byteOffset = 0;
    else
        byteOffset = bufferView["byteOffset"];

    // Get indices with regards to their type: unsigned int, unsigned short, or short
    unsigned int beginningOfData = byteOffset + accByteOffset;
    if (componentType == 5125) {
        for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 4; i) {
            unsigned char bytes[] = {data[i++], data[i++], data[i++], data[i++]};
            unsigned int value;
            std::memcpy(&value, bytes, sizeof(unsigned int));
            indices.push_back((GLuint)value);
        }
    } else if (componentType == 5123) {
        for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i) {
            unsigned char bytes[] = {data[i++], data[i++]};
            unsigned short value;
            std::memcpy(&value, bytes, sizeof(unsigned short));
            indices.push_back((GLuint)value);
        }
    } else if (componentType == 5122) {
        for (unsigned int i = beginningOfData; i < byteOffset + accByteOffset + count * 2; i) {
            unsigned char bytes[] = {data[i++], data[i++]};
            short value;
            std::memcpy(&value, bytes, sizeof(short));
            indices.push_back((GLuint)value);
        }
    }

    return indices;
}

std::vector<Texture1> Model1::getTextures() {
    std::vector<Texture1> textures;

    std::string fileStr = std::string(file);
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

    // Go over all images
    for (unsigned int i = 0; i < JSON["images"].size(); i++) {
        // uri of current texture
        // json x = JSON["images"];
        // std::string texPath = JSON["images"][i]["name"] + ".jpg";
        std::string texPath = JSON["images"][i]["uri"];
        // Check if the texture has already been loaded
        bool skip = false;
        for (unsigned int j = 0; j < loadedTexName.size(); j++) {
            if (loadedTexName[j] == texPath) {
                textures.push_back(loadedTex[j]);
                skip = true;
                break;
            }
        }

        // If the texture has been loaded, skip this
        if (!skip) {
            // Load diffuse texture
            if (texPath.find("baseColor") != std::string::npos) {
                Texture1 diffuse = Texture1((fileDirectory + texPath).c_str(), "albedoMap", loadedTex.size());
                textures.push_back(diffuse);
                loadedTex.push_back(diffuse);
                loadedTexName.push_back(texPath);
            }
            // Load specular texture
            else if (texPath.find("metallicRoughness") != std::string::npos) {
                Texture1 specular = Texture1((fileDirectory + texPath).c_str(), "metallicMap", loadedTex.size());
                textures.push_back(specular);
                loadedTex.push_back(specular);
                loadedTexName.push_back(texPath);
            }
            //加载normal法线贴图
            else if (texPath.find("normal") != std::string::npos) {
                Texture1 normal = Texture1((fileDirectory + texPath).c_str(), "normalMap", loadedTex.size());
                textures.push_back(normal);
                loadedTex.push_back(normal);
                loadedTexName.push_back(texPath);
            }
        }
    }

    return textures;
}

std::vector<Vertex> Model1::assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs) {
    std::vector<Vertex> vertices;
    for (size_t i = 0; i < positions.size(); i++) {
        if (i >= positions.size() || i < 0 || i >= normals.size() || i >= texUVs.size()) continue;
        vertices.push_back(Vertex{positions[i], normals[i],
                                  // glm::vec3(1.0f, 1.0f, 1.0f),
                                  texUVs[i]});
    }
    return vertices;
}

std::vector<glm::vec2> Model1::groupFloatsVec2(std::vector<float> floatVec) {
    std::vector<glm::vec2> vectors;
    for (int i = 0; i < floatVec.size() - 1; i) {
        vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
    }
    return vectors;
}

std::vector<glm::vec3> Model1::groupFloatsVec3(std::vector<float> floatVec) {
    std::vector<glm::vec3> vectors;
    for (int i = 0; i < floatVec.size(); i) {
        vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
    }
    return vectors;
}

std::vector<glm::vec4> Model1::groupFloatsVec4(std::vector<float> floatVec) {
    std::vector<glm::vec4> vectors;
    for (int i = 0; i < floatVec.size(); i) {
        vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
    }
    return vectors;
}
