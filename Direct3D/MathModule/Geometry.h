#pragma once

namespace DM
{
	/// <summary>
	///  기하관련 클래스
	/// </summary>
	class Geometry
	{
	public:
		static std::vector<Vertex1> GetBoxVertices();
		static std::vector<unsigned int> GetBoxIndices();
		static void GetCylinder(
			float bottomRadius, float topRadius, float height
			, unsigned int sliceCount, unsigned int stackCount
			, std::vector<Vertex1>& vertex, std::vector<unsigned int>& index);

	};






};