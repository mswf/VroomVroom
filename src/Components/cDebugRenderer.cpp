#include "cDebugRenderer.h"
#include "../DataStructure/mesh.h"

const int CDebugRenderer::familyId = (int)ComponentTypes::DEBUG_RENDERER;
std::vector< CDebugRenderer* > CDebugRenderer::list;

// TODO(Valentinas): ADD WARNING FOR NUMBER OF POINTS!!!

CDebugRenderer::CDebugRenderer() :
	isBuffered(false),
	mode( DrawMode::NONE ),
	vao(0),
	vbo(0),
	numberOfPoints( 4096 ),
	pointSize(5.0f),
	isDrawingPoints(false)
{
	list.push_back( this );
}

CDebugRenderer::~CDebugRenderer()
{
	std::vector<CDebugRenderer*>::iterator it = list.begin();
	std::vector<CDebugRenderer*>::iterator end = list.end();
	for ( ; it != end; ++it )
	{
		if( (*it) == this)
		{
			list.erase(it);
			break;
		}
	}
}

void CDebugRenderer::Call()
{

}

void CDebugRenderer::Initialize()
{
	int primitive = (mode == DrawMode::LINES) ? 2 : 3;
	unsigned long bSize = sizeof(glm::vec3) * int( numberOfPoints / primitive ) * 2;
	CreateDynamicBuffer(vao, vbo, bSize);
}

void CDebugRenderer::AddPrimivite( Line line )
{
	if ( mode == DrawMode::NONE)
	{
		mode = DrawMode::LINES;
		Initialize();
	}
	points.push_back(line.start);
	colours.push_back(line.colour);
	points.push_back(line.end);
	colours.push_back(line.colour);
}

void CDebugRenderer::AddPrimivite( Triangle triangle )
{
	if ( mode == DrawMode::NONE)
	{
		mode = DrawMode::TRIANGLES;
		Initialize();
	}
	points.push_back(triangle.v1);
	colours.push_back(triangle.colour);
	points.push_back(triangle.v2);
	colours.push_back(triangle.colour);
	points.push_back(triangle.v3);
	colours.push_back(triangle.colour);
}

const bool CDebugRenderer::ContainsPrimitives() const
{
	return ( points.size() > 0 );
}

void CDebugRenderer::UpdateBuffer()
{
	if (!isBuffered)
	{
		BufferPoints( vao, vbo, points, colours );
		isBuffered = true;
	}
	unsigned int offset = sizeof(glm::vec3) * (unsigned int)points.size();
	BufferUpdate( vbo, 0, (unsigned int)points.size(), &points.front() );
	BufferUpdate( vbo, offset, (unsigned int)colours.size(), &colours.front() );
}

void CDebugRenderer::Clear()
{
	if ( points.size() != 0 ) points.clear();
	if ( colours.size() != 0 ) colours.clear();
	isBuffered = false;
}

const float& CDebugRenderer::GetPointSize() const
{
	return pointSize;
}

const int CDebugRenderer::GetDrawCount() const
{
	return (int)points.size();
}

const DrawMode& CDebugRenderer::GetDrawMode() const
{
	return mode;
}

void CDebugRenderer::SetDrawPoints( bool enabled )
{
	isDrawingPoints = enabled;
}

void CDebugRenderer::SetPointSize( float size )
{
	pointSize = size;
}

std::vector< CDebugRenderer* >* CDebugRenderer::GetDebugRendererList()
{
	return &list;
}