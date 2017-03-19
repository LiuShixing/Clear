#ifndef LOADM3D_H
#define LOADM3D_H

#include "MeshGeometry.h"
//#include "LightHelper.h"
#include "..\SkinnedData\SkinnedData.h"
#include "..\Common\Geometry.h"

struct M3dMaterial
{
	Material Mat;
	bool AlphaClip;
	std::wstring DiffuseMapName;
};

class M3DLoader
{
public:
	bool LoadM3d(const std::string& filename, 
		std::vector<Vertex::PosNormTex>& vertices,
		std::vector<MeshGeometry::Subset>& subsets,
		std::vector<M3dMaterial>& mats);
	bool LoadM3d(const std::string& filename, 
		std::vector<Vertex::PosNormalTexSkinned>& vertices,
		std::vector<MeshGeometry::Subset>& subsets,
		std::vector<M3dMaterial>& mats,
		SkinnedData& skinInfo);
	bool LoadBinM3d(const std::string& filename,
		std::vector<Vertex::PosNormalTexSkinned>& vertices,
		std::vector<MeshGeometry::Subset>& subsets,
		std::vector<M3dMaterial>& mats,
		SkinnedData& skinInfo);

private:
	void ReadMaterials(std::ifstream& fin, UINT numMaterials, std::vector<M3dMaterial>& mats);
	void ReadSubsetTable(std::ifstream& fin, UINT numSubsets, std::vector<MeshGeometry::Subset>& subsets);
	void ReadVertices(std::ifstream& fin, UINT numVertices, std::vector<Vertex::PosNormTex>& vertices);
	void ReadSkinnedVertices(std::ifstream& fin, UINT numVertices, std::vector<Vertex::PosNormalTexSkinned>& vertices);
	void ReadBoneOffsets(std::ifstream& fin, UINT numBones, std::vector<XMFLOAT4X4>& boneOffsets);
	void ReadBoneHierarchy(std::ifstream& fin, UINT numBones, std::vector<int>& boneIndexToParentIndex);
	void ReadAnimationClips(std::ifstream& fin, UINT numBones, UINT numAnimationClips, std::map<std::string, AnimationClip>& animations);
	void ReadBoneKeyframes(std::ifstream& fin, UINT numBones, BoneAnimation& boneAnimation);
};



#endif // LOADM3D_H