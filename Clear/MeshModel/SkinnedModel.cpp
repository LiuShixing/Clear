#include "SkinnedModel.h"
#include "LoadM3d.h"
#include"..\MyUtil\MyUtil.h"
#include<stdio.h>

SkinnedModel::SkinnedModel(ID3D11Device* device, TextureMgr& texMgr, const std::string& modelFilename, const std::wstring& texturePath)
{
	std::vector<M3dMaterial> mats;
	M3DLoader m3dLoader;

	int start = clock();

	bool is = true;  //Load BM3d
	//		bool is = false;
	if (is)
	{
		m3dLoader.LoadBinM3d(modelFilename, Vertices, Subsets, mats, SkinnedData);
		//   	write(mats);
	}
	else
	{
		m3dLoader.LoadM3d(modelFilename, Vertices, Subsets, mats, SkinnedData);
		writebM3d(mats);
	}

	ModelMesh.SetVertices(device, &Vertices[0], Vertices.size());
	ModelMesh.SetSubsetTable(Subsets);

	SubsetCount = mats.size();

	for (UINT i = 0; i < SubsetCount; ++i)
	{
		Mat.push_back(mats[i].Mat);

		ID3D11ShaderResourceView* diffuseMapSRV = texMgr.CreateTexture(texturePath + mats[i].DiffuseMapName);
		DiffuseMapSRV.push_back(diffuseMapSRV);
		IsAlphaClips.push_back(mats[i].AlphaClip);
	}

	//

}
void SkinnedModel::write(std::vector<M3dMaterial>& mats)
{
	std::ofstream fs;
	fs.open("write.txt", std::ios::trunc);
	using namespace std;
	fs << "***************m3d-File-Header*******************" << endl
		<< "#Materials " << Subsets.size() << endl
		<< "#Vertices " << Vertices.size() << endl
		<< "#Bones " << SkinnedData.mBoneOffsets.size() << endl
		<< "#AnimationClips " << SkinnedData.mAnimations.size() << endl;

	fs << endl
		<< "***************Materials*******************" << endl;
	for (int i = 0; i < Subsets.size(); i++)
	{
		char dname[50];
		MyUtil::WstringToCharArry(dname, mats[i].DiffuseMapName);

		fs << "Ambient: " << mats[i].Mat.Ambient.x << " " << mats[i].Mat.Ambient.y << " " << mats[i].Mat.Ambient.z << endl
			<< "Diffuse: " << mats[i].Mat.Diffuse.x << " " << mats[i].Mat.Diffuse.y << " " << mats[i].Mat.Diffuse.z << endl
			<< "Specular: " << mats[i].Mat.Specular.x << " " << mats[i].Mat.Specular.y << " " << mats[i].Mat.Specular.z << endl
			<< "SpecPower: " << mats[i].Mat.Specular.w << endl
			<< "Reflect: " << mats[i].Mat.Reflect.x << " " << mats[i].Mat.Reflect.y << " " << mats[i].Mat.Reflect.z << endl
			<< "AlphaClip: " << mats[i].AlphaClip << endl
			<< "DiffuseMap: " << dname << endl
			<< endl;

	}

	fs << endl
		<< "***************SubsetTable*******************" << endl;
	for (int i = 0; i < Subsets.size(); i++)
	{
		fs << "SubsetID: " << i << " " << "VertexStart: " << Subsets[i].VertexStart << " " << "VertexCount: " << Subsets[i].VertexCount << endl;
	}

	fs << endl
		<< "***************Vertices*******************" << endl;
	for (int i = 0; i < Vertices.size(); i++)
	{
		fs << "Pos: " << Vertices[i].Pos.x << " " << Vertices[i].Pos.y << " " << Vertices[i].Pos.z << endl
			<< "Normal: " << Vertices[i].Normal.x << " " << Vertices[i].Normal.y << " " << Vertices[i].Normal.z << endl
			<< "Tex: " << Vertices[i].Tex.x << " " << Vertices[i].Tex.y << endl
			<< "Weight: " << Vertices[i].Weights.x << " " << Vertices[i].Weights.y << " " << Vertices[i].Weights.z << " " << 1.0f - Vertices[i].Weights.x - Vertices[i].Weights.y - Vertices[i].Weights.z << endl
			<< "BoneIndices: " << (int)Vertices[i].BoneIndices[0] << " " << (int)Vertices[i].BoneIndices[1] << " " << (int)Vertices[i].BoneIndices[2] << " " << (int)Vertices[i].BoneIndices[3] << endl
			<< endl;
	}


	fs << "***************BoneOffsets*******************" << endl;
	for (int i = 0; i < SkinnedData.mBoneOffsets.size(); i++)
	{
		fs << "BoneOffset" << i
			<< " " << SkinnedData.mBoneOffsets[i]._11 << " " << SkinnedData.mBoneOffsets[i]._12
			<< " " << SkinnedData.mBoneOffsets[i]._13 << " " << SkinnedData.mBoneOffsets[i]._14
			<< " " << SkinnedData.mBoneOffsets[i]._21 << " " << SkinnedData.mBoneOffsets[i]._22
			<< " " << SkinnedData.mBoneOffsets[i]._23 << " " << SkinnedData.mBoneOffsets[i]._24
			<< " " << SkinnedData.mBoneOffsets[i]._31 << " " << SkinnedData.mBoneOffsets[i]._32
			<< " " << SkinnedData.mBoneOffsets[i]._33 << " " << SkinnedData.mBoneOffsets[i]._34
			<< " " << SkinnedData.mBoneOffsets[i]._41 << " " << SkinnedData.mBoneOffsets[i]._42
			<< " " << SkinnedData.mBoneOffsets[i]._43 << " " << SkinnedData.mBoneOffsets[i]._44
			<< endl;

	}

	fs << endl;
	fs << "***************BoneHierarchy*****************" << endl;
	for (int i = 0; i < SkinnedData.mBoneHierarchy.size(); i++)
	{
		fs << "ParentIndexOfBone" << i << ": " << SkinnedData.mBoneHierarchy[i] << endl;
	}

	fs << endl;
	fs << "***************AnimationClips****************" << endl
		<< "AnimationClip" << " " << " ClipName " << endl
		<< "{" << endl;

	char ClipNames[8][50] = { "Idle1", "Idle2", "Idle3", "Run", "Attack1", "Attack2", "Attack3", "Defen" };

	for (int clip = 0; clip < 8; clip++)
	{
		for (int i = 0; i < SkinnedData.mAnimations[ClipNames[clip]].BoneAnimations.size(); i++)
		{
			BoneAnimation ba = SkinnedData.mAnimations[ClipNames[clip]].BoneAnimations[i];
			fs << "         Bone" << i << " #Keyframes: " << ba.Keyframes.size() << endl
				<< "         {" << endl;;
			for (int key = 0; key < ba.Keyframes.size(); key++)
			{
				fs << "             Time: " << ba.Keyframes[key].TimePos
					<< " Pos: " << ba.Keyframes[key].Translation.x << " " << ba.Keyframes[key].Translation.y << " " << ba.Keyframes[key].Translation.z
					<< " Scale: " << ba.Keyframes[key].Scale.x << " " << ba.Keyframes[key].Scale.y << " " << ba.Keyframes[key].Scale.z
					<< " Quat: " << ba.Keyframes[key].RotationQuat.x << " " << ba.Keyframes[key].RotationQuat.y << " " << ba.Keyframes[key].RotationQuat.z << " " << ba.Keyframes[key].RotationQuat.w << endl;

			}
			fs << "         }" << endl;
		}
	}
	fs << "}" << endl;
	exit(0);
}
void SkinnedModel::writebM3d(std::vector<M3dMaterial>& mats)
{
	FILE* fw = NULL;
	fopen_s(&fw, "dianjuguai.binm3d", "wb");
	if (fw == NULL)
	{
		MyUtil::WriteString("open leona.bm3d failure");
		return;
	}
	//File-Header
	int numMat = Subsets.size();
	int numVertex = Vertices.size();
	int numBone = SkinnedData.mBoneOffsets.size();
	int numAnimationClip = SkinnedData.mAnimations.size();
	fwrite(&numMat, sizeof(numMat), 1, fw);
	fwrite(&numVertex, sizeof(numVertex), 1, fw);
	fwrite(&numBone, sizeof(numBone), 1, fw);
	fwrite(&numAnimationClip, sizeof(numAnimationClip), 1, fw);

	////Materials
	char diffName[6][50] = { "3b6c645d.dds", "575d1bef.dds", "169aba19.dds", "91bb8e40.dds", "c4e48af6.dds", "c4e48af6.dds" };
	for (int i = 0; i < numMat; i++)
	{
		fwrite(&mats[i].Mat.Ambient, sizeof(mats[i].Mat.Ambient), 1, fw);
		fwrite(&mats[i].Mat.Diffuse, sizeof(mats[i].Mat.Diffuse), 1, fw);
		fwrite(&mats[i].Mat.Specular, sizeof(mats[i].Mat.Specular), 1, fw);
		fwrite(&mats[i].Mat.Reflect, sizeof(mats[i].Mat.Reflect), 1, fw);
		fwrite(&mats[i].AlphaClip, sizeof(mats[i].AlphaClip), 1, fw);

		int len = sizeof(diffName[i]);//字符串长度
		fwrite(&len, sizeof(len), 1, fw);
		fwrite(&diffName[i], sizeof(diffName[i]), 1, fw);
	}

	//SubsetTable
	for (int i = 0; i < numMat; i++)
	{
		fwrite(&i, sizeof(i), 1, fw);
		fwrite(&Subsets[i].VertexStart, sizeof(Subsets[i].VertexStart), 1, fw);
		fwrite(&Subsets[i].VertexCount, sizeof(Subsets[i].VertexCount), 1, fw);
	}

	//Vertices
	for (int i = 0; i < numVertex; i++)
	{
		fwrite(&Vertices[i].Pos, sizeof(Vertices[i].Pos), 1, fw);
		fwrite(&Vertices[i].Normal, sizeof(Vertices[i].Normal), 1, fw);
		fwrite(&Vertices[i].Tex, sizeof(Vertices[i].Tex), 1, fw);
		fwrite(&Vertices[i].Weights, sizeof(Vertices[i].Weights), 1, fw);
		fwrite(&Vertices[i].BoneIndices, sizeof(Vertices[i].BoneIndices), 1, fw);
	}


	//BoneOffsets
	for (int i = 0; i < numBone; i++)
	{
		fwrite(&SkinnedData.mBoneOffsets[i], sizeof(SkinnedData.mBoneOffsets[i]), 1, fw);
	}

	//BoneHierarchy
	for (int i = 0; i < numBone; i++)
	{
		fwrite(&SkinnedData.mBoneHierarchy[i], sizeof(SkinnedData.mBoneHierarchy[i]), 1, fw);
	}

	//AnimationClips
	char ClipNames[1][50] = { "Take001" };

	for (int i = 0; i < numAnimationClip; i++)
	{
		AnimationClip clip = SkinnedData.mAnimations[ClipNames[i]];

		int len = sizeof(ClipNames[i]);
		fwrite(&len, sizeof(len), 1, fw);
		fwrite(&ClipNames[i], sizeof(ClipNames[i]), 1, fw);
		for (int bone = 0; bone < numBone; bone++)
		{
			BoneAnimation ba = clip.BoneAnimations[bone];
			int numKey = ba.Keyframes.size();
			fwrite(&numKey, sizeof(numKey), 1, fw);
			for (int key = 0; key < numKey; key++)
			{
				fwrite(&ba.Keyframes[key].TimePos, sizeof(ba.Keyframes[key].TimePos), 1, fw);
				fwrite(&ba.Keyframes[key].Translation, sizeof(ba.Keyframes[key].Translation), 1, fw);
				fwrite(&ba.Keyframes[key].Scale, sizeof(ba.Keyframes[key].Scale), 1, fw);
				fwrite(&ba.Keyframes[key].RotationQuat, sizeof(ba.Keyframes[key].RotationQuat), 1, fw);

			}
		}
	}
	exit(0);
}

SkinnedModel::~SkinnedModel()
{
}
void SkinnedModel::Draw(ID3D11DeviceContext* dc, ID3DX11EffectTechnique* tech, ID3DX11EffectTechnique* techAlphaClip, UINT passIndex, ID3DX11EffectVariable* pEffectMat, ID3DX11EffectShaderResourceVariable* pEffectSRV)
{
	UINT offset = 0;
	dc->IASetVertexBuffers(0, 1, &ModelMesh.mVB, &ModelMesh.mVertexStride, &offset);
	for (int subset = 0; subset < Subsets.size(); subset++)
	{
		pEffectMat->SetRawValue(&Mat[subset], 0, sizeof(Material));

		pEffectSRV->SetResource(DiffuseMapSRV[subset]);

		if (IsAlphaClips[subset])
		{
			techAlphaClip->GetPassByIndex(passIndex)->Apply(0, dc);
		}
		else
		{
			tech->GetPassByIndex(passIndex)->Apply(0, dc);
		}
		ModelMesh.Draw(dc, subset);
	}
}


SkinnedModelInstance::SkinnedModelInstance() :mUpSpeed(0.0f), mVerticalState(VERTICAL_STATE::DOWN)
{
	TimePos = 0.0f;
	::XMMATRIX O(::XMMatrixIdentity());
	::XMStoreFloat4x4(&World, O);
	Scale = 1.0f;

	RotAngle = 0.0f;
	Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	Direction = DIRECT::PZ;
	mActionAddSpeed = 1.0f;
}

int SkinnedModelInstance::UpdateAnimation(float dt)
{

	Model->SkinnedData.GetFinalTransforms(ClipName, TimePos, FinalTransforms);

	TimePos += dt*mActionAddSpeed;;

	if (TimePos > ActEndTime)
	{
		TimePos = ActBeginTime;//默认循环动作
		return ACTION::END;
	}
	else
	{
		return ACTION::NOTEND;
	}
}
void SkinnedModelInstance::UpdateWorld()
{
	XMMATRIX worldMat = XMMatrixTranslation(Position.x, Position.y, Position.z);
	XMMATRIX rotMat = XMMatrixRotationY(RotAngle);
//	XMMATRIX rotMat = XMMatrixRotationY(XM_PI);
	worldMat = rotMat*worldMat;

	XMStoreFloat4x4(&World, worldMat);
}
//void SkinnedModelInstance::HorizontalMove(float offset)
//{
//	XMVECTOR POS = XMLoadFloat3(&Position);
//	XMVECTOR DIRECT = XMLoadFloat3(&Direction);
//	POS += DIRECT*offset;
//	XMStoreFloat3(&Position, POS);
//}
//void SkinnedModelInstance::CaculateRotAngle(XMFLOAT3& direction)//默认绕Y旋转
//{
//	if (Direction.x == direction.x && Direction.y == direction.y && Direction.z == direction.z)
//		return;
//	Direction = direction;
//	//由mDirection计算物体旋转角
//	XMFLOAT3 rawDiret = XMFLOAT3(0.0f, 0.0f, 1.0f);
//	XMVECTOR RawDiret = XMLoadFloat3(&rawDiret);
//	XMVECTOR NewDiret = XMLoadFloat3(&Direction);
//	NewDiret = ::XMVector3Normalize(NewDiret);
//
//	XMVECTOR Angle = ::XMVector3AngleBetweenNormals(RawDiret, NewDiret);//弧度角存在Angle的每个成分中
//	XMFLOAT3 caculAngle;
//	XMStoreFloat3(&caculAngle, Angle);
//
//	RotAngle = caculAngle.x;
//	if (Direction.x<0)//-------------
//	{
//		RotAngle = -RotAngle;
//	}
//}
void SkinnedModelInstance::SetAction(std::string& clipName, float actBegin, float actEnd)
{
	ClipName = clipName;
	ActBeginTime = actBegin;
	ActEndTime = actEnd;
	TimePos = ActBeginTime;
}
void SkinnedModelInstance::SetPosition(float x, float y, float z)
{
	Position.x = x;
	Position.y = y;
	Position.z = z;
}
void SkinnedModelInstance::SetPosition(XMFLOAT3& pos)
{
	SetPosition(pos.x, pos.y, pos.z);
}
void SkinnedModelInstance::SetPositionY(float y)
{
	Position.y = y;
}
void SkinnedModelInstance::SetPositionZ(float z)
{
	Position.z = z;
}
void SkinnedModelInstance::SetDirection(int direct)
{
	if (direct == DIRECT::PZ)
	{
		Direction = DIRECT::PZ;
		RotAngle = 0.0f;
	}
	if (direct == DIRECT::NZ)
	{
		RotAngle = XM_PI;
		Direction = DIRECT::NZ;
	}
}