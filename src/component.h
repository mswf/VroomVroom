#ifndef component_h
#define component_h

typedef int FamilyId;

enum class ComponentTypes
{
    UNKNOWN = 0,
    TRANSFORM = 1,
    CAMERA,
    LIGHT,
    MATERIAL,
    MESH,
    TEXTURE,
    
};

struct Component
{
};

/*

struct CompCamera : public Component
{
    // projection matrix =  glm::perspective(glm::radians(fov), aspectRatio, near plane, far plane);
    //                      glm::ortho(T left, T right, T bottom, T top, near, far);
    // view matrix = glm::look( Eye vector, Center vector, Up vector);
    static const FamilyId familyId;
    CompCamera() {};
};

struct CompLight : public Component
{
    // direction
    // color
    // intensity
    static const FamilyId familyId;
    CompLight() {};
};

struct CompMaterial : public Component
{
    // shader *
    // texture *
    // parameters / uniforms[]
    static const FamilyId familyId;
    CompMaterial() {};
};

struct CompMesh : public Component
{
    // VertexBuffer id
    static const FamilyId familyId;
    CompMesh() {};
};

struct CompTexture : public Component
{
    // Buffered Texture id
    static const FamilyId familyId;
    CompTexture() {};
};
*/

#endif /* component_h */