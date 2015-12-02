#include "cTransform.h"
#include "entity.h"
#include "../glm/gtc/quaternion.hpp"
#include "../glm/gtx/transform.hpp"

const int CTransform::familyId = (int)ComponentTypes::TRANSFORM;

CTransform::CTransform():
	transform( glm::mat4(1.0) ),
	worldTransform( glm::mat4(1.0) )
{}

CTransform::~CTransform()
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

void CTransform::Translate( const glm::vec3& translation )
{
	glm::vec4 p = glm::translate( transform, translation ) * glm::vec4( GetPosition(), 1.0f );
	SetPosition( glm::vec3( p.x, p.y, p.z ) );
	Update();
}

void CTransform::Rotate( const glm::vec3& rotation )
{
	glm::quat rotate(rotation);
	transform = glm::rotate(transform, glm::radians( glm::eulerAngles(rotate).x ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
	transform = glm::rotate(transform, glm::radians( glm::eulerAngles(rotate).y ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
	transform = glm::rotate(transform, glm::radians( glm::eulerAngles(rotate).z ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
	Update();
	//glm::rotate(const tquat<T, P> &q, const T &angle, const tvec3<T, P> &v);
	//glm::rotate(const tmat4x4<T, P> &m, T angle, const tvec3<T, P> &v);
	//glm::rotate(T angle, const tvec3<T, P> &v);
}

void CTransform::Scale( const glm::vec3& scale )
{
	glm::scale(transform, scale);
	Update();
}

//Private
void CTransform::Update()
{
	if ( entity != NULL ) entity->Update();
}

// POSITION
glm::vec3 CTransform::GetPosition() const
{
	return glm::vec3( transform[3] );
}

void CTransform::SetPosition( const glm::vec3& position )
{
	transform[3][0] = position.x;
	transform[3][1] = position.y;
	transform[3][2] = position.z;
	Update();
}

const float CTransform::GetPositionX() const
{
	return transform[3][0];
}

const float CTransform::GetPositionY() const
{
	return transform[3][1];
}

const float CTransform::GetPositionZ() const
{
	return transform[3][2];
}

void CTransform::SetPositionX( const float& x )
{
	transform[3][0] = x;
	Update();
}

void CTransform::SetPositionY( const float& y )
{
	transform[3][1] = y;
	Update();
}

void CTransform::SetPositionZ( const float& z )
{
	transform[3][2] = z;
	Update();
}

void CTransform::TranslateX( const float& x )
{
	glm::vec4 p = glm::translate( transform, glm::vec3( x, 0.0f, 0.0f ) ) * glm::vec4( GetPosition(), 1.0f );
	SetPosition( glm::vec3( p.x, GetPositionY(), GetPositionZ() ) );
	Update();
}

void CTransform::TranslateY( const float& y )
{
	glm::vec4 p = glm::translate( transform, glm::vec3( 0.0f, y, 0.0f ) ) * glm::vec4( GetPosition(), 1.0f );
	SetPosition( glm::vec3( GetPositionX(), p.y, GetPositionZ() ) );
	Update();
}

void CTransform::TranslateZ( const float& z )
{
	glm::vec4 p = glm::translate( transform, glm::vec3( 0.0f, 0.0f, z ) ) * glm::vec4( GetPosition(), 1.0f );
	SetPosition( glm::vec3( GetPositionX(), GetPositionY(), p.z ) );
	Update();
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
	Update();
}

const float CTransform::GetPitch() const
{
	return glm::pitch( glm::quat_cast(transform) );
}

const float CTransform::GetYaw() const
{
	return glm::yaw( glm::quat_cast(transform) );
}

const float CTransform::GetRoll() const
{
	return glm::roll( glm::quat_cast(transform) );
}

void CTransform::SetPitch( const float& angle )
{
	glm::mat4 rot(1.0f);
	rot = glm::rotate(rot, glm::radians( angle ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
	SetRotationMatrix(rot);
	Update();
}

void CTransform::SetYaw( const float& angle )
{
	glm::mat4 rot(1.0f);
	rot = glm::rotate(rot, glm::radians( angle ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
	SetRotationMatrix(rot);
	Update();
}

void CTransform::SetRoll( const float& angle )
{
	glm::mat4 rot(1.0f);
	rot = glm::rotate(rot, glm::radians( angle ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
	SetRotationMatrix(rot);
	Update();
}

void CTransform::Pitch( const float& angle )
{
	transform = glm::rotate(transform, glm::radians( angle ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
	Update();
}

void CTransform::Yaw( const float& angle )
{
	transform = glm::rotate(transform, glm::radians( angle ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
	Update();
}

void CTransform::Roll( const float& angle )
{
	transform = glm::rotate(transform, glm::radians( angle ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
	Update();
}

//Private
void CTransform::SetRotationMatrix( const glm::mat4& rot)
{
	transform[0][0] = rot[0][0];	transform[0][1] = rot[0][1];	transform[0][2] = rot[0][2];
	transform[1][0] = rot[1][0];	transform[1][1] = rot[1][1];	transform[1][2] = rot[1][2];
	transform[2][0] = rot[2][0];	transform[2][1] = rot[2][1];	transform[2][2] = rot[2][2];
}

// SCALE
glm::vec3 CTransform::GetScale() const
{
	return glm::vec3( transform[0][3], transform[1][3], transform[2][3] );
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
	glm::scale(transform, glm::vec3( x, 0.0f, 0.0f ) );
	Update();
}

void CTransform::ScaleY( const float& y )
{
	glm::scale(transform, glm::vec3( 0.0f, y, 0.0f ) );
	Update();
}

void CTransform::ScaleZ( const float& z )
{
	glm::scale(transform, glm::vec3( 0.0f, 0.0f, z ) );
	Update();
}
