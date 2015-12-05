#ifndef cLineRenderer_h
#define cLineRenderer_h

#include "component.h"

class CLineRenderer : public Component
{
	public:
		static const int familyId;
	
		CLineRenderer();
		~CLineRenderer();
	
		void Call();
	
	private:
		//static std::vector< CLineRenderer* > list;
};

#endif /* cLineRenderer_h */
