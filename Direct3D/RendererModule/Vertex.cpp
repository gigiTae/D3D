#include "RendererPCH.h"
#include "Vertex.h"

GrapicsEngine::BaseVertex::BaseVertex()
	:position(),normal(),tangent(),uv()
{

}

GrapicsEngine::BaseVertex::~BaseVertex()
{

}

GrapicsEngine::SimpleVertex::SimpleVertex()
	:position(),color()
{

}

GrapicsEngine::SimpleVertex::~SimpleVertex()
{

}
