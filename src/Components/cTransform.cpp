#include "cTransform.h"
#include "entity.h"
#include "../glm/gtc/quaternion.hpp"
//#include "../glm/gtx/euler_angles.hpp"
#include "../glm/gtx/transform.hpp"

const int CTransform::familyId = (int)ComponentTypes::TRANSFORM;

CTransform::CTransform():
	transform( glm::mat4(1.0) ),
	worldTransform( glm::mat4(1.0) )
{}

CTransform::~CTransform()
{
	
}

void CTransform::Call()
{
	
}

const glm::mat4& CTransform::GetTransform() const
{
	return transform;
}

const glm::mat4& CTransform::GetWorldTransform() const
{
	return worldTransform;
}

void CTransform::SetTransform( const glm::mat4 &trans )
{
	transform = trans;
}

void CTransform::SetWorldTransform( const glm::mat4 &trans )
{
	worldTransform = trans;
}

// ?
void CTransform::Translate( const glm::vec3& translation )
{
	transform = glm::translate( transform, translation );
	Update();
}

// ?
void CTransform::Rotate( const glm::vec3& rotation, const float& angle )
{
	glm::quat rotate(rotation);
	transform = glm::rotate(transform, glm::radians( angle ), rotation );
	Update();
}

// ?
void CTransform::Scale( const glm::vec3& scale )
{
	transform = glm::scale_slow(transform, scale);
	Update();
}

//Private
void CTransform::Update()
{
	//glm::mat4 mTranslation = glm::translate( position );
	//glm::mat4 mRotation = glm::eulerAngleXYZ( rotation.x, rotation.y, rotation.z );
	//glm::mat4 mScale = glm::scale( scale );
	//SetTransform( mTranslation * mRotation * mScale );
	if ( entity != NULL ) entity->Update();
}

// POSITION
// V
glm::vec3 CTransform::GetPosition() const
{
	return glm::vec3( transform[3] );
}
// V
void CTransform::SetPosition( const glm::vec3& position )
{
	transform[3][0] = position.x;
	transform[3][1] = position.y;
	transform[3][2] = position.z;
	Update();
}
// V
const float CTransform::GetPositionX() const
{
	return transform[3][0];
}
// V
const float CTransform::GetPositionY() const
{
	return transform[3][1];
}
// V
const float CTransform::GetPositionZ() const
{
	return transform[3][2];
}
// V
void CTransform::SetPositionX( const float& x )
{
	transform[3][0] = x;
	Update();
}
// V
void CTransform::SetPositionY( const float& y )
{
	transform[3][1] = y;
	Update();
}
// V
void CTransform::SetPositionZ( const float& z )
{
	transform[3][2] = z;
	Update();
}

void CTransform::TranslateX( const float& x )
{
	Translate( glm::vec3( x, 0.0, 0.0 ) );
}

void CTransform::TranslateY( const float& y )
{
	Translate( glm::vec3( 0.0, y, 0.0 ) );
}

void CTransform::TranslateZ( const float& z )
{
	Translate( glm::vec3( 0.0, 0.0, z ) );
}

// ROTATION
glm::vec3 CTransform::GetRotation() const
{
	return glm::eulerAngles( glm::quat_cast(transform) );
}

void CTransform::SetRotation( const glm::vec3& rotation )
{
	glm::mat4 rot(1.0f);
	rot = glm::rotate(rot, glm::radians( rotation.x ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
	rot = glm::rotate(rot, glm::radians( rotation.y ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
	rot = glm::rotate(rot, glm::radians( rotation.z ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
	SetRotationMatrix(rot);
}

const float CTransform::GetPitch() const
{
	return glm::pitch( glm::quat_cast( transform ) );
}

const float CTransform::GetYaw() const
{
	return glm::yaw( glm::quat_cast( transform ) );
}

const float CTransform::GetRoll() const
{
	return glm::roll( glm::quat_cast( transform ) );
}
// SET PITCH/YAW/ROLL

void CTransform::SetPitch( const float& angle )
{
	glm::mat4 rot(1.0f);
	rot = glm::rotate(rot, glm::radians( angle ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
	SetRotationMatrix(rot);
}

void CTransform::SetYaw( const float& angle )
{
	glm::mat4 rot(1.0f);
	rot = glm::rotate(rot, glm::radians( angle ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
	SetRotationMatrix(rot);
}

void CTransform::SetRoll( const float& angle )
{
	glm::mat4 rot(1.0f);
	rot = glm::rotate(rot, glm::radians( angle ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
	SetRotationMatrix(rot);
}


void CTransform::Pitch( const float& angle )
{
	Rotate( glm::vec3( 1.0, 0.0, 0.0 ), angle );
}

void CTransform::Yaw( const float& angle )
{
	Rotate( glm::vec3( 0.0, 1.0, 0.0 ), angle );
}

void CTransform::Roll( const float& angle )
{
	Rotate( glm::vec3( 0.0, 0.0, 1.0 ), angle );
}

//Private
void CTransform::SetRotationMatrix( const glm::mat4& rot)
{
	transform[0] = rot[0];
	transform[1] = rot[1];
	transform[2] = rot[2];
	Update();
}

// SCALE
glm::vec3 CTransform::GetScale() const
{
	return glm::vec3( transform[0][0], transform[1][1], transform[2][2] );
}

void CTransform::SetScale( const glm::vec3& scale )
{
	transform[0][0] = scale.x;
	transform[1][1] = scale.y;
	transform[2][2] = scale.z;
	Update();
}

const float CTransform::GetScaleX() const
{
	return transform[0][0];
}

const float CTransform::GetScaleY() const
{
	return transform[1][1];
}

const float CTransform::GetScaleZ() const
{
	return transform[2][2];
}

void CTransform::SetScaleX( const float& x )
{
	transform[0][0] = x;
	Update();
}

void CTransform::SetScaleY( const float& y )
{
	transform[1][1] = y;
	Update();
}

void CTransform::SetScaleZ( const float& z )
{
	transform[2][2] = z;
	Update();
}

void CTransform::ScaleX( const float& x )
{
	Scale( glm::vec3( 1.0 + x, 1.0, 1.0 ) );
}

void CTransform::ScaleY( const float& y )
{
	Scale( glm::vec3( 1.0, 1.0 + y, 1.0 ) );
}

void CTransform::ScaleZ( const float& z )
{
	Scale( glm::vec3( 1.0, 1.0, 1.0 + z ) );
}
