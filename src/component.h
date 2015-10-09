#ifndef component_h
#define component_h

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"

typedef int FamilyId;

enum class ComponentTypes
{
    CAMERA = 1,
    LIGHT,
    MATERIAL,
    MESH,
    TEXTURE,
    TRANSFORM
};

struct Component
{
};

struct CompCamera : public Component
{
    static const FamilyId familyId = (int)ComponentTypes::CAMERA;
    // projection matrix =  glm::perspective(glm::radians(fov), aspectRatio, near plane, far plane);
    //                      glm::ortho(T left, T right, T bottom, T top, near, far);
    // view matrix = glm::look( Eye vector, Center vector, Up vector);
    CompCamera() {};
};

struct CompLight : public Component
{
    static const FamilyId familyId = (int)ComponentTypes::LIGHT;
    // direction
    // color
    // intensity
    CompLight() {};
};

struct CompMaterial : public Component
{
    static const FamilyId familyId = (int)ComponentTypes::MATERIAL;
    // shader *
    // texture *
    // parameters / uniforms[]
    CompMaterial() {};
};

struct CompMesh : public Component
{
    static const FamilyId familyId = (int)ComponentTypes::MESH;
    // VertexBuffer id
    CompMesh() {};
};

struct CompTexture : public Component
{
    static const FamilyId familyId = (int)ComponentTypes::TEXTURE;
    // Buffered Texture id
    CompTexture() {};
};

struct CompTransform : public Component
{
    static const FamilyId familyId = (int)ComponentTypes::TRANSFORM;
    CompTransform() : base(0), position(0), scale(1) {}
    glm::mat4 base;
    glm::vec3 position;
    glm::quat rotation;
    glm::vec3 scale;
};

#endif /* component_h */