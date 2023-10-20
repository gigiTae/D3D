#include "RendererPCH.h"
#include "Vertex.h"

RendererModule::BaseVertex::BaseVertex()
	:position(),normal(),tangent(),uv()
{

}

RendererModule::BaseVertex::~BaseVertex()
{

}

RendererModule::SimpleVertex::SimpleVertex()
	:position(),color()
{

}

RendererModule::SimpleVertex::~SimpleVertex()
{

}
