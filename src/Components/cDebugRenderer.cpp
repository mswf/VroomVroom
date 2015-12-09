#include "cDebugRenderer.h"
#include "../DataStructure/mesh.h"

const int CDebugRenderer::familyId = (int)ComponentTypes::DEBUG_RENDERER;
std::vector< CDebugRenderer* > CDebugRenderer::list;

CDebugRenderer::CDebugRenderer() :
	isBuffered(false),
	mode(DrawMode::NONE),
	vao(0),
	vbo(0),
	pointSize(0.0f)
{
	list.push_back( this );
}

CDebugRenderer::~CDebugRenderer()
{
	
}

void CDebugRenderer::Call()
{

}

void CDebugRenderer::AddLine( Line line )
{
	if ( mode == DrawMode::NONE )
	{
		mode = DrawMode::LINES;
	}
	if ( mode == DrawMode::LINES )
	{
		lines.push_back(line);
	}
}

void CDebugRenderer::AddTriangle( Triangle triangle )
{
	if ( mode == DrawMode::NONE )
	{
		mode = DrawMode::TRIANGLES;
	}
	if ( mode == DrawMode::TRIANGLES )
	{
		triangles.push_back(triangle);
	}
}

bool CDebugRenderer::ContainsPrimitives()
{
	return (lines.size() != 0 || triangles.size() != 0 );
}

void CDebugRenderer::PushToGPU()
{
	
	std::vector< glm::vec3 > points, colours;
	
	if ( mode == DrawMode::NONE )
	{
		printf("No primitives present!");
		if (lines.size() > 0)
		{
			mode = DrawMode::LINES;
		}
		if (triangles.size() > 0)
		{
			mode = DrawMode::TRIANGLES;
		}

	}
	if (mode == DrawMode::LINES)
	{
		std::vector< Line >::const_iterator iter_line = lines.begin();
		std::vector< Line >::const_iterator end_line = lines.end();
		for ( ; iter_line != end_line; ++iter_line)
		{
			points.push_back( (*iter_line).start );
			colours.push_back( (*iter_line).colour );
			points.push_back( (*iter_line).end );
			colours.push_back( (*iter_line).colour );
		}
	}
	if (mode == DrawMode::TRIANGLES)
	{
		std::vector< Triangle >::const_iterator iter_triangle = triangles.begin();
		std::vector< Triangle >::const_iterator end_triangle = triangles.end();
		for ( ; iter_triangle != end_triangle; ++iter_triangle)
		{
			points.push_back( (*iter_triangle).v1 );
			colours.push_back( (*iter_triangle).colour );
			points.push_back( (*iter_triangle).v2 );
			colours.push_back( (*iter_triangle).colour );
			points.push_back( (*iter_triangle).v3 );
			colours.push_back( (*iter_triangle).colour );
		}
	}
	
	BufferPoints( vao, vbo, points, colours );
	count = (int)points.size();
	isBuffered = true;
}

void CDebugRenderer::Clear()
{
	if ( triangles.size() != 0 ) triangles.clear();
	if ( lines.size() != 0 ) lines.clear();
	BufferClear(vao, vbo);
	mode = DrawMode::NONE;
}

const float& CDebugRenderer::GetPointSize() const
{
	return pointSize;
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