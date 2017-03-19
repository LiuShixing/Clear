#ifndef SKINNEDMODEL_H
#define SKINNEDMODEL_H

#include "..\SkinnedData\SkinnedData.h"
#include "MeshGeometry.h"
#include "TextureMgr.h"
#include"..\Common\Geometry.h"
#include "LoadM3d.h"

const float CON_SCALE = 1.0f;
struct M3dFileHeader
{
	unsigned int Materials;
	unsigned int Vertices;
	unsigned int Bones;
	unsigned int AnimationClips;
};

class SkinnedModel
{
public:
	SkinnedModel(ID3D11Device* device, TextureMgr& texMgr, const std::string& modelFilename, const std::wstring& texturePath);
	~SkinnedModel();

	void Draw(ID3D11DeviceContext* dc, ID3DX11EffectTechnique* tech, ID3DX11EffectTechnique* techAlphaClip, UINT passIndex, ID3DX11EffectVariable* pEffectMat, ID3DX11EffectShaderResourceVariable* pEffectSRV);
	void write(std::vector<M3dMaterial>& mats);
	void writebM3d(std::vector<M3dMaterial>& mats);
	UINT SubsetCount;

	std::vector<Material> Mat;
	std::vector<ID3D11ShaderResourceView*> DiffuseMapSRV;

	// Keep CPU copies of the mesh data to read from.  
	std::vector<Vertex::PosNormalTexSkinned> Vertices;
	std::vector<MeshGeometry::Subset> Subsets;
	std::vector<int> IsAlphaClips;

	MeshGeometry ModelMesh;
	SkinnedData SkinnedData;
};
struct CollidePoint
{
	float y;
	float z;
};
class SkinnedModelInstance
{
public:
	SkinnedModelInstance();
	
	void SetPosition(XMFLOAT3& pos);
	void SetPosition(float x, float y, float z);
	void SetPositionY(float y);
	void SetPositionZ(float z);
	void MovePosition(float offsetX, float offsetY, float offsetZ)
	{
		Position.x += offsetX;
		Position.y += offsetY;
		Position.z += offsetZ;
	}
	XMFLOAT3& GetPosition(){ return Position; }
	XMFLOAT4X4& GetWorld(){ return World; }
	void SetDirection(int direct);
	int  GetDirection(){ return Direction; }
	float GetScale(){ return Scale; }

	int GetVerticalState(){ return mVerticalState; }
	void SetVerticalStateLevel()
	{
		mVerticalState = VERTICAL_STATE::LEVEL;
		mUpSpeed = 0.0f;
	}
	void SetVerticalStateUp()
	{
		mVerticalState = VERTICAL_STATE::UP;
	}
	void SetVerticalStateDown()
	{
		mVerticalState = VERTICAL_STATE::DOWN;
	}
//	float GetRotAngle(){ return RotAngle; }
	
//	void CaculateRotAngle(XMFLOAT3& direction);

	SkinnedModel* Model;
	std::vector<XMFLOAT4X4> FinalTransforms;
	enum DIRECT
	{
		PZ = 1,//+z
		NZ = -1//-z
	};
	enum VERTICAL_STATE //垂直方向状态
	{
		DOWN = 0,
		UP = 1,
		LEVEL = 2
	};
protected:
	int UpdateAnimation(float dt);
	void UpdateWorld();
//	void HorizontalMove(float offset);

	//UpdateAnimation(float dt)返回时调用  
	virtual void DoAtionEnd(){ }
	void SetAction(std::string& clipName, float actBegin, float actEnd);

	enum ACTION
	{
		NOTEND = 0,
		END = 1
	};
	
	XMFLOAT4X4 World;
	XMFLOAT3 Position;
	float RotAngle;
	float Scale;

	int Direction;
	int mVerticalState;
	float mUpSpeed;
	float mActionAddSpeed;
private:
	//当前动作
	std::string ClipName;
	//当前Clip中当前动画块的起始时间
	float ActBeginTime;
	float ActEndTime;
	//当前时间
	float TimePos;
};

#endif // SKINNEDMODEL_H