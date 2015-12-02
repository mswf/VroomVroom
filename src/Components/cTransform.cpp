#include "cTransform.h"
#include "entity.h"

const int CTransform::familyId = (int)ComponentTypes::TRANSFORM;

CTransform::CTransform()
:	transform( glm::mat4(1.0) )
{
}

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

glm::vec3 CTransform::GetPosition() const
{
	return glm::vec3( transform[3] );
}

glm::vec3 CTransform::GetScale() const
{
	return glm::vec3( transform[0][3], transform[1][3], transform[2][3] );
}

glm::vec3 CTransform::GetRotation() const
{
	return glm::eulerAngles( glm::quat_cast(transform) );
}

float CTransform::GetPitch() const
{
	return glm::pitch( glm::quat_cast(transform) );
}

float CTransform::GetYaw() const
{
	return glm::yaw( glm::quat_cast(transform) );
}

float CTransform::GetRoll() const
{
	return glm::roll( glm::quat_cast(transform) );
}

void CTransform::SetTransform( const glm::mat4 &trans )
{
	transform = trans;
}

void CTransform::SetWorldTransform( const glm::mat4 &trans )
{
	worldTransform = trans;
}


void CTransform::SetPosition( glm::vec3 position )
{
	transform[3][0] = position.x;
	transform[3][1] = position.y;
	transform[3][2] = position.z;
	if ( entity != NULL ) entity->Update();
}

void CTransform::SetScale( glm::vec3 scale )
{
	transform[0][3] = scale.x;
	transform[1][3] = scale.y;
	transform[2][3] = scale.z;
	if ( entity != NULL ) entity->Update();
}


void CTransform::SetRotation( glm::vec3 rotation )
{
	glm::mat4 rot(1.0f);
	rot = glm::rotate(rot, glm::radians( rotation.x ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
	rot = glm::rotate(rot, glm::radians( rotation.y ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
	rot = glm::rotate(rot, glm::radians( rotation.z ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
	SetRotationMatrix(rot);
	if ( entity != NULL ) entity->Update();
}

void CTransform::SetRotationMatrix(glm::mat4 rot)
{
	transform[0][0] = rot[0][0];	transform[0][1] = rot[0][1];	transform[0][2] = rot[0][2];
	transform[1][0] = rot[1][0];	transform[1][1] = rot[1][1];	transform[1][2] = rot[1][2];
	transform[2][0] = rot[2][0];	transform[2][1] = rot[2][1];	transform[2][2] = rot[2][2];
}

void CTransform::SetPitch( float angle )
{
	glm::mat4 rot(1.0f);
	rot = glm::rotate(rot, glm::radians( angle ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
	SetRotationMatrix(rot);
	if ( entity != NULL ) entity->Update();
}

void CTransform::SetYaw( float angle )
{
	glm::mat4 rot(1.0f);
	rot = glm::rotate(rot, glm::radians( angle ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
	SetRotationMatrix(rot);
	if ( entity != NULL ) entity->Update();
}

void CTransform::SetRoll( float angle )
{
	glm::mat4 rot(1.0f);
	rot = glm::rotate(rot, glm::radians( angle ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
	SetRotationMatrix(rot);
	if ( entity != NULL ) entity->Update();
}

void CTransform::Pitch( float angle )
{
	transform = glm::rotate(transform, glm::radians( angle ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
	if ( entity != NULL ) entity->Update();
}
void CTransform::Yaw( float angle )
{
	transform = glm::rotate(transform, glm::radians( angle ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
	if ( entity != NULL ) entity->Update();
}
void CTransform::Roll( float angle )
{
	transform = glm::rotate(transform, glm::radians( angle ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
	if ( entity != NULL ) entity->Update();
}


void CTransform::Rotate( glm::vec3 rotation )
{
	glm::quat rotate(rotation);
	transform = glm::rotate(transform, glm::radians( glm::eulerAngles(rotate).x ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
	transform = glm::rotate(transform, glm::radians( glm::eulerAngles(rotate).y ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
	transform = glm::rotate(transform, glm::radians( glm::eulerAngles(rotate).z ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
	if ( entity != NULL ) entity->Update();
	//glm::rotate(const tquat<T, P> &q, const T &angle, const tvec3<T, P> &v);
	//glm::rotate(const tmat4x4<T, P> &m, T angle, const tvec3<T, P> &v);
	//glm::rotate(T angle, const tvec3<T, P> &v);
}

void CTransform::Scale( glm::vec3 scale )
{
	glm::scale(transform, scale);
	if ( entity != NULL ) entity->Update();
}

void CTransform::Translate( glm::vec3 translation )
{
	glm::translate(transform, translation);
	if ( entity != NULL ) entity->Update();
}