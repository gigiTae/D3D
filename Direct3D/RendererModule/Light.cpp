#include "RendererPCH.h"
#include "Light.h"

PointLight::PointLight()
	:diffuse(),specular(),att(),range(),position(),pad(),ambient()
{
	ZeroMemory(this, sizeof(this));
}

DirectionalLight::DirectionalLight()
	:diffuse(), specular(), pad(), ambient(), direction()
{
	ZeroMemory(this, sizeof(this)); 
}

SpotLight::SpotLight()
	:diffuse(), specular(), att(), range(), position(), pad(), ambient(),direction(),spot()
{
	 ZeroMemory(this, sizeof(this)); 
}
