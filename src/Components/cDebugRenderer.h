#ifndef cDebugRenderer_h
#define cDebugRenderer_h

#include "component.h"
#include <vector>
#include "../glm/vec3.hpp"
#include "../glm/vec4.hpp"

const glm::vec4 BLACK = glm::vec4( 0.0f,0.0f,0.0f,1.0f );

enum class DrawMode
{
	NONE,
	POINTS,
	LINES,
	TRIANGLES
};

struct Point
{
	glm::vec3 point;
	glm::vec4 colour;
	Point( glm::vec3 p, glm::vec4 c = BLACK ) :
		point(p),
		colour( c )
	{}
};


struct Line
{
	glm::vec3 start;
	glm::vec3 end;
	glm::vec4 colour;
	Line( glm::vec3 p0, glm::vec3 p1, glm::vec4 c = BLACK ) :
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
	glm::vec4 colour;
	Triangle( glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec4 c = BLACK ) :
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
	
		void Call() override;
		void Initialize();
		void AddPrimivite( Point point );
		void AddPrimivite( Line line );
		void AddPrimivite( Triangle triangle );
		void AddPoint( glm::vec3 point, glm::vec4 colour );
		void UpdateBuffer();
		void ResizeBuffer();
		const bool ContainsPrimitives() const;
		void Clear();
		void SetNumberOfPoints( int number );
		void SetDrawPoints( bool enabled );
		void SetPointSize( float size );
		const float& GetPointSize() const;
		const int GetDrawCount() const;
		const int GetCountPerPrimitive() const;
		const DrawMode& GetDrawMode() const;
		inline bool IsDrawingPoints() { return isDrawingPoints; }
		inline bool IsBuffered() { return isBuffered; }
		inline unsigned long GetBufferSize()
 		{
			return sizeof(glm::vec3) * bufferSizeAmount + sizeof(glm::vec4) * bufferSizeAmount;
		}
		static std::vector< CDebugRenderer* >* GetDebugRendererList();
	
		unsigned int vao;
	
	
	private:
	
		unsigned int vbo;
		unsigned int numberOfPoints;
		unsigned int bufferSizeAmount;
		float pointSize;
		DrawMode mode;
		bool isBuffered;
		bool isDrawingPoints;
		std::vector< glm::vec3 > points;
		std::vector< glm::vec4 > colours;
		static std::vector< CDebugRenderer* > list;
};

#endif /* cLineRenderer_h */
