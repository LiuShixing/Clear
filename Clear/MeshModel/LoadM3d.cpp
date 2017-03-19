#include "LoadM3d.h"
#include"SkinnedModel.h"
#include"../MyUtil/MyUtil.h"
bool M3DLoader::LoadM3d(const std::string& filename, 
						std::vector<Vertex::PosNormTex>& vertices,
						std::vector<MeshGeometry::Subset>& subsets,
						std::vector<M3dMaterial>& mats)
{
	std::ifstream fin(filename);

	UINT numMaterials = 0;
	UINT numVertices  = 0;
	UINT numBones     = 0;
	UINT numAnimationClips = 0;

	std::string ignore;

	if( fin )
	{
		fin >> ignore; // file header text
		fin >> ignore >> numMaterials;
		fin >> ignore >> numVertices;
		fin >> ignore >> numBones;
		fin >> ignore >> numAnimationClips;
 
		ReadMaterials(fin, numMaterials, mats);
		ReadSubsetTable(fin, numMaterials, subsets);
	    ReadVertices(fin, numVertices, vertices);
 
		return true;
	 }
    return false;
}

bool M3DLoader::LoadM3d(const std::string& filename, 
						std::vector<Vertex::PosNormalTexSkinned>& vertices,
						std::vector<MeshGeometry::Subset>& subsets,
						std::vector<M3dMaterial>& mats,
						SkinnedData& skinInfo)
{
    std::ifstream fin(filename);

	UINT numMaterials = 0;
	UINT numVertices  = 0;
	UINT numBones     = 0;
	UINT numAnimationClips = 0;

	std::string ignore;

	if( fin )
	{
		fin >> ignore; // file header text
		fin >> ignore >> numMaterials;
		fin >> ignore >> numVertices;
		fin >> ignore >> numBones;
		fin >> ignore >> numAnimationClips;
 
		std::vector<XMFLOAT4X4> boneOffsets;
		std::vector<int> boneIndexToParentIndex;
		std::map<std::string, AnimationClip> animations;

		ReadMaterials(fin, numMaterials, mats);
		ReadSubsetTable(fin, numMaterials, subsets);
	    ReadSkinnedVertices(fin, numVertices, vertices);
		ReadBoneOffsets(fin, numBones, boneOffsets);
	    ReadBoneHierarchy(fin, numBones, boneIndexToParentIndex);
	    ReadAnimationClips(fin, numBones, numAnimationClips, animations);
 
		skinInfo.Set(boneIndexToParentIndex, boneOffsets, animations);

	    return true;
	}
    return false;
}
 bool M3DLoader::LoadBinM3d(const std::string& filename,
		std::vector<Vertex::PosNormalTexSkinned>& vertices,
		std::vector<MeshGeometry::Subset>& subsets,
		std::vector<M3dMaterial>& mats,
		SkinnedData& skinInfo)
{
	 FILE* fr = NULL;
	 fopen_s(&fr, filename.c_str(), "rb");
	 if (fr == NULL)
	 {
		 MyUtil::WriteString("open leona.bm3d failure");
		 return false;
	 }
//	 MyUtil::WriteString("open leona.bm3d success");
	 //File-Header
	 int numMat = 0;
	 int numVertex = 0;
	 int numBone = 0;
	 int numAnimationClip = 0;
	 
	 fread(&numMat, sizeof(numMat), 1, fr);
	 fread(&numVertex, sizeof(numVertex), 1, fr);
	 fread(&numBone, sizeof(numBone), 1, fr);
	 fread(&numAnimationClip, sizeof(numAnimationClip), 1, fr);

	 //Materials
	 mats.resize(numMat);
	 for (int i = 0; i < numMat; i++)
	 {
		 fread(&mats[i].Mat.Ambient, sizeof(mats[i].Mat.Ambient), 1, fr);
		 fread(&mats[i].Mat.Diffuse, sizeof(mats[i].Mat.Diffuse), 1, fr);
		 fread(&mats[i].Mat.Specular, sizeof(mats[i].Mat.Specular), 1, fr);
		 fread(&mats[i].Mat.Reflect, sizeof(mats[i].Mat.Reflect), 1, fr);
		 fread(&mats[i].AlphaClip, sizeof(mats[i].AlphaClip), 1, fr);

		 int len; //字符串长度 不包括\0
		 fread(&len, sizeof(len), 1, fr);
		 char dname[50];
		 fread(&dname, sizeof(dname), 1, fr);

		 std::string diffname(dname);

		 mats[i].DiffuseMapName.resize(diffname.size(), ' ');
		 std::copy(diffname.begin(), diffname.end(), mats[i].DiffuseMapName.begin());
	 }
//	 MyUtil::WriteString("Materials success");
	 //SubsetTable
	 subsets.resize(numMat);
	 for (int i = 0; i < numMat; i++)
	 {
		 fread(&i, sizeof(i), 1, fr);
		 fread(&subsets[i].VertexStart, sizeof(subsets[i].VertexStart), 1, fr);
		 fread(&subsets[i].VertexCount, sizeof(subsets[i].VertexCount), 1, fr);
	 }
//	 MyUtil::WriteString("SubsetTable success");
	 //Vertices
	 vertices.resize(numVertex);
	 for (int i = 0; i < numVertex; i++)
	 {
		 fread(&vertices[i].Pos, sizeof(vertices[i].Pos), 1, fr);
		 fread(&vertices[i].Normal, sizeof(vertices[i].Normal), 1, fr);
		 fread(&vertices[i].Tex, sizeof(vertices[i].Tex), 1, fr);
		 fread(&vertices[i].Weights, sizeof(vertices[i].Weights), 1, fr);
		 fread(&vertices[i].BoneIndices, sizeof(vertices[i].BoneIndices), 1, fr);
	 }

//	 MyUtil::WriteString("Vertices success");
	 //BoneOffsets
	 skinInfo.mBoneOffsets.resize(numBone);
	 for (int i = 0; i < numBone; i++)
	 {
		 fread(&skinInfo.mBoneOffsets[i], sizeof(skinInfo.mBoneOffsets[i]), 1, fr);
	 }
//	 MyUtil::WriteString("BoneOffsets success");
	 //BoneHierarchy
	 skinInfo.mBoneHierarchy.resize(numBone);
	 for (int i = 0; i < numBone; i++)
	 {
		 fread(&skinInfo.mBoneHierarchy[i], sizeof(skinInfo.mBoneHierarchy[i]), 1, fr);
	 }
//	 MyUtil::WriteString("BoneHierarchy success");
	 //AnimationClips
//	 MyUtil::WriteData("numAnimationClip", numAnimationClip);
	
	 AnimationClip clip;
	 for (int i = 0; i < numAnimationClip; i++)
	 {	 
		 clip.BoneAnimations.resize(numBone);
		
		 int len;
		 fread(&len, sizeof(len), 1, fr);
	//	 char* cname = new char[len];
		 char cname[50];
		 fread(&cname, len, 1, fr);
		 std::string clipName(cname);

//		 MyUtil::WriteString(clipName);

		 int numKey = 0;
		 for (int bone = 0; bone < numBone; bone++)
		 {
			 fread(&numKey, sizeof(numKey), 1, fr);
			 clip.BoneAnimations[bone].Keyframes.resize(numKey);
			 int key;
			 for (key = 0; key < numKey; key++)
			 {

				/* fread(&clip.BoneAnimations[bone].Keyframes[key].TimePos, sizeof(clip.BoneAnimations[bone].Keyframes[key].TimePos), 1, fr);

				 fread(&clip.BoneAnimations[bone].Keyframes[key].Translation.x, sizeof(clip.BoneAnimations[bone].Keyframes[key].Translation.x), 1, fr);
				 fread(&clip.BoneAnimations[bone].Keyframes[key].Translation.y, sizeof(clip.BoneAnimations[bone].Keyframes[key].Translation.y), 1, fr);
				 fread(&clip.BoneAnimations[bone].Keyframes[key].Translation.z, sizeof(clip.BoneAnimations[bone].Keyframes[key].Translation.z), 1, fr);

				 fread(&clip.BoneAnimations[bone].Keyframes[key].Scale.x, sizeof(clip.BoneAnimations[bone].Keyframes[key].Scale.x), 1, fr);
				 fread(&clip.BoneAnimations[bone].Keyframes[key].Scale.y, sizeof(clip.BoneAnimations[bone].Keyframes[key].Scale.y), 1, fr);
				 fread(&clip.BoneAnimations[bone].Keyframes[key].Scale.z, sizeof(clip.BoneAnimations[bone].Keyframes[key].Scale.z), 1, fr);

				 fread(&clip.BoneAnimations[bone].Keyframes[key].RotationQuat.x, sizeof(clip.BoneAnimations[bone].Keyframes[key].RotationQuat.x), 1, fr);
				 fread(&clip.BoneAnimations[bone].Keyframes[key].RotationQuat.y, sizeof(clip.BoneAnimations[bone].Keyframes[key].RotationQuat.y), 1, fr);
				 fread(&clip.BoneAnimations[bone].Keyframes[key].RotationQuat.z, sizeof(clip.BoneAnimations[bone].Keyframes[key].RotationQuat.z), 1, fr);
				 fread(&clip.BoneAnimations[bone].Keyframes[key].RotationQuat.w, sizeof(clip.BoneAnimations[bone].Keyframes[key].RotationQuat.w), 1, fr);*/
				 fread(&clip.BoneAnimations[bone].Keyframes[key].TimePos, sizeof(clip.BoneAnimations[bone].Keyframes[key].TimePos), 1, fr);
				 fread(&clip.BoneAnimations[bone].Keyframes[key].Translation, sizeof(clip.BoneAnimations[bone].Keyframes[key].Translation), 1, fr);
				 fread(&clip.BoneAnimations[bone].Keyframes[key].Scale, sizeof(clip.BoneAnimations[bone].Keyframes[key].Scale), 1, fr);
				 fread(&clip.BoneAnimations[bone].Keyframes[key].RotationQuat, sizeof(clip.BoneAnimations[bone].Keyframes[key].RotationQuat), 1, fr);
			 }
			/* MyUtil::WriteData("bone", bone);
			 MyUtil::WriteData("key", key);
			 MyUtil::WriteDataFloat("time", clip.BoneAnimations[bone].Keyframes[key - 1].RotationQuat.y, true);*/
		 }
		 skinInfo.mAnimations[clipName] = clip;
	 }
//	 MyUtil::WriteString("AnimationClips success");
	 fclose(fr);
	 return true;
}
//bool M3DLoader::LoadBM3d(const std::string& filename,
//	std::vector<Vertex::PosNormalTexSkinned>& vertices,
//	std::vector<MeshGeometry::Subset>& subsets,
//	std::vector<M3dMaterial>& mats,
//	SkinnedData& skinInfo)
//{
//	FILE* fr;
//	fopen_s(&fr, filename.c_str(), "r");
//	if (fr == NULL)
//	{
//		MyUtil::WriteString("file open failure");
//		return false;
//	}
//	//m3d-File-Header
//	M3dFileHeader header;
//	fread(&header, sizeof(header), 1, fr);
//
//	MyUtil::WriteData("header.Materials", header.Materials);
//	MyUtil::WriteData("header.Vertices", header.Vertices);
//	MyUtil::WriteData("header.Bones", header.Bones);
//	MyUtil::WriteData("header.AnimationClips", header.AnimationClips);
//
//	//分配空间
//	M3dMaterial* matArry = new M3dMaterial[header.Materials];
//	MeshGeometry::Subset* subsetArry = new MeshGeometry::Subset[header.Materials];
//	Vertex::PosNormalTexSkinned* vertexArry = new Vertex::PosNormalTexSkinned[header.Vertices];
//	XMFLOAT4X4* boneOffsetArry = new XMFLOAT4X4[header.Bones];
//	int* boneHierarchyArry = new int[header.Bones];
//
//	//Materials
//	/*int* lens = new int[header.Materials];
//	fread(lens, sizeof(lens), 1, fr);
//	for (int i = 0; i < header.Materials; i++)
//	{
//		matArry[i].DiffuseMapName.resize(lens[i]);
//	}
//	delete[] lens;*/
//	fread(matArry, sizeof(matArry), 1, fr);
//	//SubsetTable
//	fread(subsetArry, sizeof(subsetArry),1, fr);
//	//Vertices
//	fread(vertexArry, sizeof(vertexArry), 1, fr);
//	//BoneOffsets
//	fread(boneOffsetArry, sizeof(boneOffsetArry),1, fr);
//	//BoneHierarchy
//	fread(boneHierarchyArry, sizeof(boneHierarchyArry), 1, fr);
//	MyUtil::WriteString("begin read clip");
//	//animation
//	std::string clipName;
//	for (int i = 0; i < header.AnimationClips; i++)
//	{
//		AnimationClip animationClip;
//		animationClip.BoneAnimations.resize(header.Bones);
//		//clipName
//	/*	int strLen = 0;
//		fread(&strLen, sizeof(int), 1, fr);
//		clipName.resize(strLen);*/
//		fread(&clipName, sizeof(clipName), 1, fr);
//		MyUtil::WriteString(clipName);
//		for (int b = 0; b < header.Bones; b++)
//		{
//			//keyFrames
//			BoneAnimation ba;
//			UINT size = 0;
//			fread(&size, sizeof(size), 1, fr);
//
//			Keyframe* keyframeArry = new Keyframe[size];
//			fread(keyframeArry, sizeof(keyframeArry), 1, fr);
//			ba.Keyframes.resize(size);
//			for (int i = 0; i < size; i++)
//			{
//				ba.Keyframes[i] = keyframeArry[i];
//			}
//			animationClip.BoneAnimations[b] = ba;
//			delete[] keyframeArry;
//		}
//		skinInfo.mAnimations[clipName] = animationClip;
//	}
//	fclose(fr);
//
//	mats.resize(header.Materials);
//	subsets.resize(header.Materials);
//	vertices.resize(header.Vertices);
//	skinInfo.mBoneOffsets.resize(header.Bones);
//	skinInfo.mBoneHierarchy.resize(header.Bones);
//
//	MyUtil::WriteDataFloat("matArry[0].Mat.Ambient.x", matArry[0].Mat.Ambient.x,true);
//	MyUtil::WriteDataFloat("matArry[0].Mat.Ambient.y", matArry[0].Mat.Ambient.y, true);
//	MyUtil::WriteDataFloat("matArry[0].Mat.Ambient.z", matArry[0].Mat.Ambient.z, true);
//	MyUtil::WriteDataFloat("matArry[0].Mat.Diffuse.x", matArry[0].Mat.Diffuse.x, true);
//	MyUtil::WriteDataFloat("matArry[0].Mat.Diffuse.y", matArry[0].Mat.Diffuse.y, true);
//	MyUtil::WriteDataFloat("matArry[0].Mat.Diffuse.z", matArry[0].Mat.Diffuse.z, true);
//
//	for (int i = 0; i < header.Materials; i++)
//	{
//		mats[i] = matArry[i];
//		subsets[i] = subsetArry[i];
//	}
//	for (int i = 0; i < header.Vertices; i++)
//	{
//		vertices[i] = vertexArry[i];
//	}
//	for (int i = 0; i < header.Bones; i++)
//	{
//		skinInfo.mBoneOffsets[i] = boneOffsetArry[i];
//		skinInfo.mBoneHierarchy[i] = boneHierarchyArry[i];
//	}
//	MyUtil::WriteString("copy end");
//	delete[] matArry;
//	delete[] subsetArry;
//	delete[] vertexArry;
//	delete[] boneOffsetArry;
//	delete[] boneHierarchyArry;
//
//	return true;
//}
void M3DLoader::ReadMaterials(std::ifstream& fin, UINT numMaterials, std::vector<M3dMaterial>& mats)
{
	 std::string ignore;
     mats.resize(numMaterials);

	 std::string diffuseMapName;

     fin >> ignore; // materials header text
	 for(UINT i = 0; i < numMaterials; ++i)
	 {
			fin >> ignore >> mats[i].Mat.Ambient.x  >> mats[i].Mat.Ambient.y  >> mats[i].Mat.Ambient.z;
			fin >> ignore >> mats[i].Mat.Diffuse.x  >> mats[i].Mat.Diffuse.y  >> mats[i].Mat.Diffuse.z;
			fin >> ignore >> mats[i].Mat.Specular.x >> mats[i].Mat.Specular.y >> mats[i].Mat.Specular.z;
			fin >> ignore >> mats[i].Mat.Specular.w;
			fin >> ignore >> mats[i].Mat.Reflect.x >> mats[i].Mat.Reflect.y >> mats[i].Mat.Reflect.z;
			fin >> ignore >> mats[i].AlphaClip;
			fin >> ignore >> diffuseMapName;

			mats[i].DiffuseMapName.resize(diffuseMapName.size(), ' ');
			std::copy(diffuseMapName.begin(), diffuseMapName.end(), mats[i].DiffuseMapName.begin());
		}
}

void M3DLoader::ReadSubsetTable(std::ifstream& fin, UINT numSubsets, std::vector<MeshGeometry::Subset>& subsets)
{
    std::string ignore;
	subsets.resize(numSubsets);

	fin >> ignore; // subset header text
	for(UINT i = 0; i < numSubsets; ++i)
	{
        fin >> ignore >> subsets[i].Id;
		fin >> ignore >> subsets[i].VertexStart;
		fin >> ignore >> subsets[i].VertexCount;
    }
}

void M3DLoader::ReadVertices(std::ifstream& fin, UINT numVertices, std::vector<Vertex::PosNormTex>& vertices)
{
	std::string ignore;
    vertices.resize(numVertices);

    fin >> ignore; // vertices header text
    for(UINT i = 0; i < numVertices; ++i)
    {
	    fin >> ignore >> vertices[i].pos.x      >> vertices[i].pos.y      >> vertices[i].pos.z;
	    fin >> ignore >> vertices[i].normal.x   >> vertices[i].normal.y   >> vertices[i].normal.z;
	    fin >> ignore >> vertices[i].tex.x      >> vertices[i].tex.y;
    }
}

void M3DLoader::ReadSkinnedVertices(std::ifstream& fin, UINT numVertices, std::vector<Vertex::PosNormalTexSkinned>& vertices)
{
	std::string ignore;
    vertices.resize(numVertices);

    fin >> ignore; // vertices header text
	int boneIndices[4];
	float weights[4];
    for(UINT i = 0; i < numVertices; ++i)
    {
	    fin >> ignore >> vertices[i].Pos.x        >> vertices[i].Pos.y          >> vertices[i].Pos.z;
	    fin >> ignore >> vertices[i].Normal.x     >> vertices[i].Normal.y       >> vertices[i].Normal.z;
	    fin >> ignore >> vertices[i].Tex.x        >> vertices[i].Tex.y;
		fin >> ignore >> weights[0]     >> weights[1]     >> weights[2]     >> weights[3];
		fin >> ignore >> boneIndices[0] >> boneIndices[1] >> boneIndices[2] >> boneIndices[3];

		vertices[i].Weights.x = weights[0];
		vertices[i].Weights.y = weights[1];
		vertices[i].Weights.z = weights[2];

		vertices[i].BoneIndices[0] = (BYTE)boneIndices[0]; 
		vertices[i].BoneIndices[1] = (BYTE)boneIndices[1]; 
		vertices[i].BoneIndices[2] = (BYTE)boneIndices[2]; 
		vertices[i].BoneIndices[3] = (BYTE)boneIndices[3]; 
    }
}

 
void M3DLoader::ReadBoneOffsets(std::ifstream& fin, UINT numBones, std::vector<XMFLOAT4X4>& boneOffsets)
{
	std::string ignore;
    boneOffsets.resize(numBones);

    fin >> ignore; // BoneOffsets header text
    for(UINT i = 0; i < numBones; ++i)
    {
        fin >> ignore >> 
            boneOffsets[i](0,0) >> boneOffsets[i](0,1) >> boneOffsets[i](0,2) >> boneOffsets[i](0,3) >>
            boneOffsets[i](1,0) >> boneOffsets[i](1,1) >> boneOffsets[i](1,2) >> boneOffsets[i](1,3) >>
            boneOffsets[i](2,0) >> boneOffsets[i](2,1) >> boneOffsets[i](2,2) >> boneOffsets[i](2,3) >>
            boneOffsets[i](3,0) >> boneOffsets[i](3,1) >> boneOffsets[i](3,2) >> boneOffsets[i](3,3);
    }
}

void M3DLoader::ReadBoneHierarchy(std::ifstream& fin, UINT numBones, std::vector<int>& boneIndexToParentIndex)
{
	std::string ignore;
    boneIndexToParentIndex.resize(numBones);

    fin >> ignore; // BoneHierarchy header text
	for(UINT i = 0; i < numBones; ++i)
	{
	    fin >> ignore >> boneIndexToParentIndex[i];
	}
}

void M3DLoader::ReadAnimationClips(std::ifstream& fin, UINT numBones, UINT numAnimationClips, 
								   std::map<std::string, AnimationClip>& animations)
{
	std::string ignore;
    fin >> ignore; // AnimationClips header text
    for(UINT clipIndex = 0; clipIndex < numAnimationClips; ++clipIndex)
    {
        std::string clipName;
        fin >> ignore >> clipName;
        fin >> ignore; // {

		AnimationClip clip;
		clip.BoneAnimations.resize(numBones);

        for(UINT boneIndex = 0; boneIndex < numBones; ++boneIndex)
        {
            ReadBoneKeyframes(fin, numBones, clip.BoneAnimations[boneIndex]);
        }
        fin >> ignore; // }

        animations[clipName] = clip;
    }
}

void M3DLoader::ReadBoneKeyframes(std::ifstream& fin, UINT numBones, BoneAnimation& boneAnimation)
{
	std::string ignore;
    UINT numKeyframes = 0;
    fin >> ignore >> ignore >> numKeyframes;
    fin >> ignore; // {

    boneAnimation.Keyframes.resize(numKeyframes);
    for(UINT i = 0; i < numKeyframes; ++i)
    {
        float t    = 0.0f;
        XMFLOAT3 p(0.0f, 0.0f, 0.0f);
        XMFLOAT3 s(1.0f, 1.0f, 1.0f);
        XMFLOAT4 q(0.0f, 0.0f, 0.0f, 1.0f);
        fin >> ignore >> t;
        fin >> ignore >> p.x >> p.y >> p.z;
        fin >> ignore >> s.x >> s.y >> s.z;
        fin >> ignore >> q.x >> q.y >> q.z >> q.w;

	    boneAnimation.Keyframes[i].TimePos      = t;
        boneAnimation.Keyframes[i].Translation  = p;
	    boneAnimation.Keyframes[i].Scale        = s;
	    boneAnimation.Keyframes[i].RotationQuat = q;
    }

    fin >> ignore; // }
}