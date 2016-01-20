#include "cTransform.h"
//#include "entity.h"
#include "../glm/gtx/euler_angles.hpp"
#include "../glm/gtx/transform.hpp"
//#include "../glm/gtx/quaternion.hpp"

const int CTransform::familyId = static_cast<int>(ComponentTypes::TRANSFORM);

CTransform::CTransform():
	transform( glm::mat4(1.0) ),
	rotation( glm::quat( glm::vec3(0) ) ),
	position( glm::vec3(0) ),
	scale( glm::vec3(1) ),
	parent(nullptr)
{}

CTransform::~CTransform()
{
}

void CTransform::Call()
{
	//Update();
}

	const glm::mat4& CTransform::GetTransform()
{
	return transform;
}

void CTransform::SetTransform( const glm::mat4 &transform )
{
	this->transform = transform;
	Update();
}

void CTransform::LookAt( const glm::vec3 &target, const glm::vec3& up )
{
	SetTransform( glm::lookAt(position, glm::normalize(target - position), up) );
}

//Private
void CTransform::Update()
{
	glm::mat4 mTranslation = glm::translate( position );
	glm::mat4 mRotation = glm::mat4_cast( rotation );
	glm::mat4 mScale = glm::scale( scale );
	transform = (mTranslation * mRotation * mScale);
	
	transform = ( parent != nullptr ) ? ( parent->GetTransform() * transform ) : transform;
	
	std::vector< CTransform* >::const_iterator iter = children.begin();
	std::vector< CTransform* >::const_iterator end = children.end();
	for( ; iter != end; ++iter )
	{
		(*iter)->Update();
	}
}

void CTransform::AddChild( CTransform* c )
{
	if ( c->parent != nullptr )
	{
		c->parent->RemoveChild( c );
	}
	c->parent = this;

	children.push_back(c);
	Update();

	//c->Update();
}

void CTransform::RemoveChild( CTransform *c )
{
	std::vector< CTransform* >::const_iterator iter = children.begin();
	std::vector< CTransform* >::const_iterator end = children.end();
	for ( ; iter != end; ++iter)
	{
		if ( c == (*iter) )
		{
			children.erase(iter);
			c->parent = nullptr;
			c->Update();
			break;
		}
	}
}

const CTransform* CTransform::GetParent() const
{
	return parent;
}

std::vector< CTransform* >& CTransform::GetChildren()
{
	return children;
}

const glm::vec3 CTransform::VectorRight() const
{
	return glm::vec3( transform[0] );
}

const glm::vec3 CTransform::VectorUp() const
{
	return glm::vec3( transform[1] );
}

const glm::vec3 CTransform::VectorForward() const
{
	return glm::vec3( transform[2] );
}

// POSITION
void CTransform::Translate( const glm::vec3& translation )
{
	this->position += translation;
	Update();
}

void CTransform::Translate( const float& x, const float& y, const float& z )
{
	Translate( glm::vec3( x, y, z ) );
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

glm::vec3 CTransform::GetPosition()
{
	return glm::vec3( transform[3][0], transform[3][1], transform[3][2] );
}

glm::vec3 CTransform::GetLocalPosition()
{
	glm::mat4 localTransform = glm::inverse( parent->GetTransform() ) * transform;
	return glm::vec3( localTransform[3][0], localTransform[3][1], localTransform[3][2] );
}

void CTransform::SetPosition( const glm::vec3& position )
{
	this->position = position;
	Update();
}

void CTransform::SetPosition( const float& x, const float& y, const float& z )
{
	SetPosition( glm::vec3( x, y, z ) );
}

const float CTransform::GetPositionX() const
{
	return position.x;
}

const float CTransform::GetPositionY() const
{
	return position.y;
}

const float CTransform::GetPositionZ() const
{
	return position.z;
}

void CTransform::SetPositionX( const float& x )
{
	position.x = x;
	Update();
}

void CTransform::SetPositionY( const float& y )
{
	position.y = y;
	Update();
}

void CTransform::SetPositionZ( const float& z )
{
	position.z = z;
	Update();
}

// SCALE

void CTransform::Scale( const glm::vec3& scale )
{
	this->scale *= scale;
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

glm::vec3 CTransform::GetScale() const
{
	return scale;
}

void CTransform::SetScale( const glm::vec3& scale )
{
	this->scale = scale;
	Update();
}

const float CTransform::GetScaleX() const
{
	return scale.x;
}

const float CTransform::GetScaleY() const
{
	return scale.y;
}

const float CTransform::GetScaleZ() const
{
	return scale.z;
}

void CTransform::SetScaleX( const float& x )
{
	scale.x = x;
	Update();
}

void CTransform::SetScaleY( const float& y )
{
	scale.y = y;
	Update();
}

void CTransform::SetScaleZ( const float& z )
{
	scale.z = z;
	Update();
}

// ROTATION
void CTransform::Rotate( const float& angle, const glm::vec3& rotation )
{
	this->rotation = glm::rotate( this->rotation, glm::radians( angle ), rotation );
	eulerRotation = glm::eulerAngles(this->rotation);
	Update();
}

const glm::quat CTransform::GetRotation() const
{
	return rotation;
}

const glm::vec3 CTransform::GetEulerAngles() const
{
	return eulerRotation;
}

void CTransform::SetRotation( const glm::vec3& rotation )
{
	glm::quat newRot;
	newRot = glm::rotate(newRot, glm::radians( eulerRotation.z ), glm::vec3(0,0,1) );
	newRot = glm::rotate(newRot, glm::radians( eulerRotation.y ), glm::vec3(0,1,0) );
	newRot = glm::rotate(newRot, glm::radians( eulerRotation.x ), glm::vec3(1,0,0) );
	this->rotation = newRot;
	Update();
}

void CTransform::Pitch( const float& angle )
{
	Rotate( angle, glm::vec3(1,0,0) );
}

void CTransform::Yaw( const float& angle )
{
	Rotate( angle, glm::vec3(0,1,0) );
}

void CTransform::Roll( const float& angle )
{
	Rotate( angle, glm::vec3(0,0,1) );
}

const float CTransform::GetPitch() const
{
	return eulerRotation.x / 360;
}

const float CTransform::GetYaw() const
{
	return eulerRotation.y / 360;
}

const float CTransform::GetRoll() const
{
	return eulerRotation.z / 360;
}

void CTransform::SetPitch( const float& angle )
{
	eulerRotation.x = angle * 360;
	SetRotation( eulerRotation );
	Update();
}

void CTransform::SetYaw( const float& angle )
{
	eulerRotation.y = angle * 360;
	SetRotation( eulerRotation );
	Update();
}

void CTransform::SetRoll( const float& angle )
{
	eulerRotation.z = angle * 360;
	SetRotation( eulerRotation );
	Update();
}