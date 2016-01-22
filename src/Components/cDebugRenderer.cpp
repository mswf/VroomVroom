#include "cDebugRenderer.h"
#include "../DataStructure/mesh.h"
#include "../console.h"

const int CDebugRenderer::familyId = static_cast<int>(ComponentTypes::DEBUG_RENDERER);
std::vector< CDebugRenderer* > CDebugRenderer::list;

const int POINT_COUNT = 4096;
const char* WARNING_BUFFER_SIZE = "DebugRenderer is trying to have more data than the buffer is initialized!";

CDebugRenderer::CDebugRenderer() :
	vao(0),
	vbo(0),
	numberOfPoints( POINT_COUNT ),
	pointSize(5.0f),
	mode( DrawMode::NONE ),
	isBuffered(false),
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
	bufferSizeAmount = int( numberOfPoints / GetCountPerPrimitive() );
	CreateDynamicBuffer(vao, vbo, GetBufferSize() );
}

void CDebugRenderer::AddPoint( glm::vec3 point, glm::vec4 colour )
{
	points.push_back( point );
	colours.push_back( colour );
}

void CDebugRenderer::AddPrimivite( Point p )
{
	if ( mode == DrawMode::NONE)
	{
		mode = DrawMode::POINTS;
		Initialize();
	}
	if ( bufferSizeAmount < points.size() )
	{
		Terminal.Warning( WARNING_BUFFER_SIZE );
	}
	AddPoint( p.point, p.colour );
}

void CDebugRenderer::AddPrimivite( Line line )
{
	if ( mode == DrawMode::NONE)
	{
		mode = DrawMode::LINES;
		Initialize();
	}
	if ( bufferSizeAmount < points.size() )
	{
		Terminal.Warning( WARNING_BUFFER_SIZE );
	}
	AddPoint(line.start, line.colour);
	AddPoint(line.end, line.colour);
}

void CDebugRenderer::AddPrimivite( Triangle triangle )
{
	if ( mode == DrawMode::NONE)
	{
		mode = DrawMode::TRIANGLES;
		Initialize();
	}
	if ( bufferSizeAmount < points.size() )
	{
		Terminal.Warning( WARNING_BUFFER_SIZE );
	}
	AddPoint(triangle.v1, triangle.colour);
	AddPoint(triangle.v2, triangle.colour);
	AddPoint(triangle.v3, triangle.colour);
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
	unsigned int offset = sizeof(glm::vec3) * static_cast<unsigned int>(points.size());
	BufferUpdate( vbo, 0, static_cast<unsigned int>(points.size()), &points.front() );
	BufferUpdate( vbo, offset, static_cast<unsigned int>(colours.size()), &colours.front() );
}

void CDebugRenderer::ResizeBuffer()
{
	DeleteVertexArray( vao );
	DeleteArrayBuffer( vbo );
	CreateDynamicBuffer(vao, vbo, GetBufferSize() );
	isBuffered = false;
	UpdateBuffer();
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
	return static_cast<int>(points.size());
}

const DrawMode& CDebugRenderer::GetDrawMode() const
{
	return mode;
}

const int CDebugRenderer::GetCountPerPrimitive() const
{
	int primitive = 0;
	switch (mode)
	{
		case DrawMode::POINTS: 		{ primitive = 1; break; }
		case DrawMode::LINES: 		{ primitive = 2; break; }
		case DrawMode::TRIANGLES:	{ primitive = 3; break; }
		case DrawMode::NONE: 		{ primitive = 1; break; }
		default: break;
	}
	return primitive;
}

void CDebugRenderer::SetDrawPoints( bool enabled )
{
	isDrawingPoints = enabled;
}

void CDebugRenderer::SetPointSize( float size )
{
	pointSize = size;
}

void CDebugRenderer::SetNumberOfPoints( int number )
{
	numberOfPoints = number;
	ResizeBuffer();
}

std::vector< CDebugRenderer* >* CDebugRenderer::GetDebugRendererList()
{
	return &list;
}