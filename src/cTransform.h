#ifndef cTransform_h
#define cTransform_h

#include "component.h"
#include <vector>
#include "glm/mat4x4.hpp"
//#include "glm/vec3.hpp"
//#include "glm/gtc/quaternion.hpp"

//TODO: Change components to classes
//TODO: Add Spaces: Self & World
struct CTransform : public Component
{
    public:
        static const int familyId;
    
        CTransform();
        ~CTransform();
        //std::vector< CTransform* > children;
        glm::mat4 tranform;
    
        //CTransform GetChild( int32_t index );
        //void LookAt( CTransform target, glm::vec3 worldUp );
        //void Rotate( glm::vec3 eulerAngles, CTransform relativeTo );
        //void RotateAround( glm::vec3 point, glm::vec3 axis, float angle );
        //void SetParent( CTransform parent, bool worldPositionStays = true );
        //void TransformDirection( glm::vec3 direction );
        //void TransformPoint( glm::vec3 position );
        //void TransformVector( glm::vec3 vector );
        //void Translate( glm::vec3 translation, CTransform relativeTo );
    
        //glm::vec3 position;   // position of the transform in world space
        //glm::vec3 localPosition; // position of the transform relative to the parent transform's.
        //glm::quat rotation;   // rotation of the transform in world space stored as a Quaternion
        //glm::quat localRotation; // rotation of the transform relative to the parent transform's rotation
        //glm::vec3 scale;      // global scale of the object
        //glm::vec3 localScale; // relative to parent
        //glm::vec3 eulerAngles;
        //glm::vec3 localEulerAngles;
        //glm::vec3 forward;
        //glm::vec3 right;
        //glm::vec3 up;
    
        //CTransform parent;
    
    private:
    

};


#endif /* cTransform_h */
