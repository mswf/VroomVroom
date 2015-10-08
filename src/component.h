typedef int FamilyId;

struct Component
{
};


struct Transform : public Component
{
    static const FamilyId familyId = 1;
    Transform() : x(0), y(0), z(0) {}
    float x, y, z;
};

struct Texture : public Component
{
    static const FamilyId familyId = 2;
    //Camera() {};
};

struct Mesh : public Component
{
    static const FamilyId familyId = 3;
    //Mesh() {};
};

struct Camera : public Component
{
    static const FamilyId familyId = 4;
    //Camera() {};
};

struct Light : public Component
{
    static const FamilyId familyId = 5;
    //Light() {};
};

struct Material : public Component
{
    static const FamilyId familyId = 6;
    //Material() {};
};
