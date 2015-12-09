#include "cTransform.h"
#include "entity.h"
#include "../glm/gtx/euler_angles.hpp"
#include "../glm/gtx/transform.hpp"

const int CTransform::familyId = (int)ComponentTypes::TRANSFORM;

CTransform::CTransform():
	position( glm::vec3(0) ),
	scale( glm::vec3(1) ),
	rotation( glm::quat( glm::vec3(0) ) ),
	transform( glm::mat4(1.0) ),
	worldTransform( glm::mat4(1.0) ),
	parent(NULL)
{}

CTransform::~CTransform()
{
	 std::vector< CTransform* >::const_iterator iter = children.begin();
	 std::vector< CTransform* >::const_iterator end = children.end();
	 for ( ; iter != end; ++iter)
	 {
		delete ( (*iter)->entity );
	 }
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

void CTransform::SetWorldTransform( const glm::mat4 &trans )
{
	worldTransform = trans;
}

// TODO(Valentinas): TEST LOOK AT
void CTransform::LookAt( const glm::vec3 &target, const glm::vec3& up )
{
	// position should be WorldTransform position
	// target should be WorldTransform position
	SetWorldTransform ( glm::inverse( glm::lookAt( position, target, VectorUp() ) ) );
	// Decompose to local rotation
	
}

//Private
void CTransform::Update()
{
	glm::mat4 mTranslation = glm::translate( position );
	glm::mat4 mRotation = glm::mat4_cast( rotation );
	glm::mat4 mScale = glm::scale( scale );
	transform = (mTranslation * mRotation * mScale);
	
	// localTransform = parentWorldTransform.inverse() * worldTransform;
	// M_loc = M_parent_inv * M
	glm::mat4 world = (parent != NULL ) ? (parent->GetWorldTransform() * transform) : transform;
	SetWorldTransform(world);
	
	std::vector< CTransform* >::const_iterator iter = children.begin();
	std::vector< CTransform* >::const_iterator end = children.end();
	for( ; iter != end; ++iter )
	{
		(*iter)->Update();
	}
}

void CTransform::AddChild( CTransform* c )
{
	if ( c->parent != NULL )
	{
		c->parent->RemoveChild( c );
	}
	children.push_back(c);
	c->parent = this;
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
			break;
		}
	}
}

const CTransform* CTransform::GetParent() const
{
	return parent;
}

const std::vector< CTransform* >& CTransform::GetChildren() const
{
	return children;
}

// POSITION
void CTransform::Translate( const glm::vec3& translation )
{
	this->position += translation;
	Update();
}

void CTransform::Translate( const float& x, const float& y, const float& z )
{
	this->position.x += x;
	this->position.y += y;
	this->position.z += z;
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


glm::vec3 CTransform::GetPosition() const
{
	return position;
}

void CTransform::SetPosition( const glm::vec3& position )
{
	this->position = position;
	Update();
}

void CTransform::SetPosition( const float& x, const float& y, const float& z )
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
	Update();
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

// ROTATION
void CTransform::Rotate( const float& angle, const glm::vec3& rotation )
{
	this->rotation = glm::rotate( this->rotation, glm::radians( angle ), rotation );
	Update();
}

glm::vec3 CTransform::GetRotation() const
{
	return glm::eulerAngles( rotation );
}

void CTransform::SetRotation( const glm::vec3& rotation )
{
	
	this->rotation = glm::rotate(this->rotation, glm::radians( rotation.x ), VectorRight() );
	this->rotation = glm::rotate(this->rotation, glm::radians( rotation.y ), VectorUp() );
	this->rotation = glm::rotate(this->rotation, glm::radians( rotation.z ), VectorForward() );
	Update();
}

void CTransform::Pitch( const float& angle )
{
	Rotate( angle, VectorRight() );
}

void CTransform::Yaw( const float& angle )
{
	Rotate( angle, VectorUp() );
}

void CTransform::Roll( const float& angle )
{
	Rotate( angle, VectorForward() );
}

const float CTransform::GetPitchEuler() const
{
	float value = eulerRotation.x / glm::pi<float>() * 2;
	return glm::mod( value, 1.0f );
}

const float CTransform::GetYawEuler() const
{
	float value = eulerRotation.y / glm::pi<float>() * 2;
	return  glm::mod( value, 1.0f );
}

const float CTransform::GetRollEuler() const
{
	float value = eulerRotation.z / glm::pi<float>() * 2;
	return glm::mod( value, 1.0f );
}

const float CTransform::GetPitch() const
{
	return glm::pitch( rotation );
}

const float CTransform::GetYaw() const
{
	return glm::yaw( rotation );
}

const float CTransform::GetRoll() const
{
	return glm::roll( rotation );
}

void CTransform::SetPitchEuler( const float& angle )
{
	eulerRotation.x = angle * glm::pi<float>() * 2;
	rotation = glm::angleAxis( eulerRotation.x, VectorRight() );
	//float pitch = glm::eulerAngles( glm::angleAxis( angle * glm::pi<float>() * 2, VectorRight() ) ).x;
	//glm::vec3 euler = glm::eulerAngles(rotation);
	//rotation = glm::quat_cast( glm::eulerAngleYXZ(euler.y, pitch, euler.z) );
	
	Update();
}

void CTransform::SetYawEuler( const float& angle )
{
	eulerRotation.y = angle * glm::pi<float>() * 2;
	rotation = glm::angleAxis( eulerRotation.y, VectorUp() );
	
	//rotation = glm::angleAxis( angle * glm::pi<float>() * 2, VectorUp() );
	Update();
}

void CTransform::SetRollEuler( const float& angle )
{
	eulerRotation.z = angle * glm::pi<float>() * 2;
	rotation = glm::angleAxis( eulerRotation.z, VectorForward() );
	
	//float roll = glm::eulerAngles( glm::angleAxis( angle * glm::pi<float>() * 2, VectorForward() ) ).z;
	//glm::vec3 euler = glm::eulerAngles(rotation);
	//rotation = glm::quat_cast( glm::eulerAngleYXZ(euler.y, euler.x, roll) );

	
	//float value = (2 * angle - 1) * 2*glm::pi<float>() / 360.0f;
	//glm::mat4 newRotation(1);
	//rotation = glm::quat( glm::rotate(newRotation, value, VectorForward() ) );
	//rotation = glm::quat( glm::eulerAngleZ(angle) );
	//rotation = glm::angleAxis( angle * glm::pi<float>() * 2, VectorForward() );
	Update();
}

void CTransform::SetPitch( const float& angle )
{
	glm::mat4 newRotation(1);
	rotation = glm::quat( glm::rotate(newRotation, angle , VectorRight() ) );
	Update();
}


void CTransform::SetYaw( const float& angle )
{
	glm::mat4 newRotation(1);
	rotation = glm::quat( glm::rotate(newRotation, angle, VectorUp() ) );
	Update();
}

void CTransform::SetRoll( const float& angle )
{
	glm::mat4 newRotation(1);
	rotation = glm::quat( glm::rotate(newRotation, angle, VectorForward() ) );
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