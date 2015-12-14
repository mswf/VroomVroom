#include "cCollider.h"

const int CCollider::familyId = (int)ComponentTypes::COLLIDER;
std::vector< CCollider* > CCollider::list;

CCollider::CCollider()
{
	list.push_back( this );
}

CCollider::~CCollider()
{
	
}

void CCollider::Call()
{
	
}
