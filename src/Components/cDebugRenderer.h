#ifndef cDebugRenderer_h
#define cDebugRenderer_h

#include "component.h"
#include <vector>
#include "../glm/vec3.hpp"

enum class DrawMode
{
	NONE,
	LINES,
	TRIANGLES
};

struct Line
{
	glm::vec3 start;
	glm::vec3 end;
	glm::vec3 colour;
	Line( glm::vec3 p0, glm::vec3 p1, glm::vec3 c = glm::vec3(0) ) :
		start( p0 ),
		end( p1 ),
		colour( c )
	{}
};

struct Triangle
{
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;
	glm::vec3 colour;
	Triangle( glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 c = glm::vec3(0) ) :
		v1( p0 ),
		v2( p1 ),
		v3( p2 ),
		colour( c )
	{}
};

class CDebugRenderer : public Component
{
	public:
	
		static const int familyId;
	
		CDebugRenderer();
		~CDebugRenderer();
	
		void Call();
		void AddLine( Line line );
		void AddTriangle( Triangle triangle );
		void PushToGPU();
		void Clear();
		void SetDrawPoints( bool enabled );
		void SetPointSize( float size );
		const float& GetPointSize() const;
		inline bool IsDrawingPoints() { return isDrawingPoints; }
		inline bool IsBuffered() { return isBuffered; }
		static std::vector< CDebugRenderer* >* GetDebugRendererList();
	
		unsigned int vao;
		int count;
		DrawMode mode;
	
	private:
	
		unsigned int vbo;
		float pointSize;
		bool isBuffered;
		bool isDrawingPoints;
		std::vector< Line > lines;
		std::vector< Triangle >	triangles;
		static std::vector< CDebugRenderer* > list;
};

#endif /* cLineRenderer_h */
