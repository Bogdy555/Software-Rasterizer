#include "..\Headers\BSR.hpp"



const bool BSR::Renderer::Material::HasFrontFace() const
{
	return Albedo != nullptr && Metalness != nullptr && Roughness != nullptr && AmbientOcclusion != nullptr && NormalMap != nullptr && Emission != nullptr;
}

const bool BSR::Renderer::Material::HasBackFace() const
{
	return AlbedoBack != nullptr && MetalnessBack != nullptr && RoughnessBack != nullptr && AmbientOcclusionBack != nullptr && NormalMapBack != nullptr && EmissionBack != nullptr;
}

const uint8_t BSR::Renderer::Material::GetCullingType() const
{
	if (HasFrontFace())
	{
		if (HasBackFace())
		{
			return Rasterizer::_NoCulling;
		}
		else
		{
			return Rasterizer::_ClockWiseCulling;
		}
	}
	else
	{
		if (HasBackFace())
		{
			return Rasterizer::_CounterClockWiseCulling;
		}
	}

	BSR_DEBUG_BREAK();

	return Rasterizer::_NoCulling;
}



const BSR::Math::Mat4f BSR::Renderer::Camera::GetViewMatrix() const
{
	return
		Math::Mat4f::GetRotation(-AngleTilt, Math::Vec3f(0.0f, 0.0f, 1.0f)) *
		Math::Mat4f::GetRotation(-AngleVertical, Math::Vec3f(1.0f, 0.0f, 0.0f)) *
		Math::Mat4f::GetRotation(-AngleFlat, Math::Vec3f(0.0f, 1.0f, 0.0f)) *
		Math::Mat4f::GetTranslation(-Position);
}

const BSR::Math::Mat4f BSR::Renderer::Camera::GetProjectionMatrix(const float _AspectRatio) const
{
	if (Perspective)
	{
		return Math::Mat4f::GetPerspective(FieldOfView, _AspectRatio, NearPlane, FarPlane);
	}

	return Math::Mat4f::GetOrtho(-FieldOfView / 2.0f * _AspectRatio, FieldOfView / 2.0f * _AspectRatio, -FieldOfView / 2.0f, FieldOfView / 2.0f, -NearPlane, -FarPlane);
}

const BSR::Math::Mat4f BSR::Renderer::Camera::GetCubeMapMatrix(const float _AspectRatio) const
{
	Camera _CubeMapCamera;

	_CubeMapCamera.Position = Math::Vec3f(0.0f, 0.0f, 0.0f);
	_CubeMapCamera.AngleFlat = AngleFlat;
	_CubeMapCamera.AngleVertical = AngleVertical;
	_CubeMapCamera.AngleTilt = AngleTilt;
	_CubeMapCamera.Perspective = true;
	if (Perspective)
	{
		_CubeMapCamera.FieldOfView = FieldOfView;
	}
	else
	{
		_CubeMapCamera.FieldOfView = 90.0f * Math::fDegreesToRadians;
	}
	_CubeMapCamera.NearPlane = 0.1f;
	_CubeMapCamera.FarPlane = 1000.0f;

	return _CubeMapCamera.GetProjectionMatrix(_AspectRatio) * _CubeMapCamera.GetViewMatrix();
}

const BSR::Math::Vec3f BSR::Renderer::Camera::GetForwardVector() const
{
	return Math::Mat3f::GetRotation(AngleFlat, Math::Vec3f(0.0f, 1.0f, 0.0f)) * Math::Mat3f::GetRotation(AngleVertical, Math::Vec3f(1.0f, 0.0f, 0.0f)) * Math::Vec3f(0.0f, 0.0f, -1.0f);
}



const BSR::Math::Mat4f BSR::Renderer::Transform::GetModelMatrix() const
{
	return
		Math::Mat4f::GetTranslation(Position) *
		Math::Mat4f::GetRotation(AngleFlat, Math::Vec3f(0.0f, -1.0f, 0.0f)) *
		Math::Mat4f::GetRotation(AngleVertical, Math::Vec3f(-1.0f, 0.0f, 0.0f)) *
		Math::Mat4f::GetRotation(AngleTilt, Math::Vec3f(0.0f, 0.0f, 1.0f)) *
		Math::Mat4f::GetScale(Scale.x, Scale.y, Scale.z, 1.0f) *
		Math::Mat4f::GetShear(ShearXByY, 0, 1) *
		Math::Mat4f::GetShear(ShearXByZ, 0, 2) *
		Math::Mat4f::GetShear(ShearYByZ, 1, 2) *
		Math::Mat4f::GetShear(ShearYByX, 1, 0) *
		Math::Mat4f::GetShear(ShearZByX, 2, 0) *
		Math::Mat4f::GetShear(ShearZByY, 2, 1);
}



BSR::Renderer::VertexBuffer::VertexBuffer() : Verteces()
{

}

BSR::Renderer::VertexBuffer::VertexBuffer(const VertexBuffer& _Other) : Verteces(_Other.Verteces)
{

}

BSR::Renderer::VertexBuffer::VertexBuffer(VertexBuffer&& _Other) noexcept : Verteces(std::move(_Other.Verteces))
{

}

BSR::Renderer::VertexBuffer::~VertexBuffer()
{

}

void BSR::Renderer::VertexBuffer::PushBack(const VertexData& _Vertex)
{
	Verteces.push_back(_Vertex);
}

void BSR::Renderer::VertexBuffer::Erase(const size_t _Index)
{
	Verteces.erase(Verteces.begin() + _Index);
}

void BSR::Renderer::VertexBuffer::Clear()
{
	Verteces.clear();
}

const size_t BSR::Renderer::VertexBuffer::GetSize() const
{
	return Verteces.size();
}

float* BSR::Renderer::VertexBuffer::GetData()
{
	return (float*)(Verteces.data());
}

const float* BSR::Renderer::VertexBuffer::GetData() const
{
	return (const float*)(Verteces.data());
}

BSR::Renderer::VertexData& BSR::Renderer::VertexBuffer::operator[] (const size_t _Index)
{
	return Verteces[_Index];
}

const BSR::Renderer::VertexData& BSR::Renderer::VertexBuffer::operator[] (const size_t _Index) const
{
	return Verteces[_Index];
}

void BSR::Renderer::VertexBuffer::operator= (const VertexBuffer& _Other)
{
	Verteces = _Other.Verteces;
}

void BSR::Renderer::VertexBuffer::operator= (VertexBuffer&& _Other) noexcept
{
	Verteces = std::move(_Other.Verteces);
}



BSR::Renderer::IndexBuffer::IndexBuffer() : Indexes()
{

}

BSR::Renderer::IndexBuffer::IndexBuffer(const IndexBuffer& _Other) : Indexes(_Other.Indexes)
{

}

BSR::Renderer::IndexBuffer::IndexBuffer(IndexBuffer&& _Other) noexcept : Indexes(std::move(_Other.Indexes))
{

}

BSR::Renderer::IndexBuffer::~IndexBuffer()
{

}

void BSR::Renderer::IndexBuffer::PushBack(const IndexData& _Index)
{
	Indexes.push_back(_Index);
}

void BSR::Renderer::IndexBuffer::Erase(const size_t _Index)
{
	Indexes.erase(Indexes.begin() + _Index);
}

void BSR::Renderer::IndexBuffer::Clear()
{
	Indexes.clear();
}

const size_t BSR::Renderer::IndexBuffer::GetSize() const
{
	return Indexes.size();
}

size_t* BSR::Renderer::IndexBuffer::GetData()
{
	return (size_t*)(Indexes.data());
}

const size_t* BSR::Renderer::IndexBuffer::GetData() const
{
	return (const size_t*)(Indexes.data());
}

BSR::Renderer::IndexData& BSR::Renderer::IndexBuffer::operator[] (const size_t _Index)
{
	return Indexes[_Index];
}

const BSR::Renderer::IndexData& BSR::Renderer::IndexBuffer::operator[] (const size_t _Index) const
{
	return Indexes[_Index];
}

void BSR::Renderer::IndexBuffer::operator= (const IndexBuffer& _Other)
{
	Indexes = _Other.Indexes;
}

void BSR::Renderer::IndexBuffer::operator= (IndexBuffer&& _Other) noexcept
{
	Indexes = std::move(_Other.Indexes);
}



void BSR::Renderer::Mesh::GenerateCube(Mesh& _Mesh)
{
	_Mesh.VBO.Clear();
	_Mesh.IBO.Clear();

	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, -0.5f, 0.5f), BSR::Math::Vec3f(-1.0f, 0.0f, 0.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, 0.5f, 0.5f), BSR::Math::Vec3f(-1.0f, 0.0f, 0.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(1.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, 0.5f, -0.5f), BSR::Math::Vec3f(-1.0f, 0.0f, 0.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, -0.5f, 0.5f), BSR::Math::Vec3f(-1.0f, 0.0f, 0.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, 0.5f, -0.5f), BSR::Math::Vec3f(-1.0f, 0.0f, 0.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, -0.5f, -0.5f), BSR::Math::Vec3f(-1.0f, 0.0f, 0.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(0.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, -0.5f, -0.5f), BSR::Math::Vec3f(0.0f, 0.0f, -1.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, 0.5f, -0.5f), BSR::Math::Vec3f(0.0f, 0.0f, -1.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(1.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, 0.5f, -0.5f), BSR::Math::Vec3f(0.0f, 0.0f, -1.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, -0.5f, -0.5f), BSR::Math::Vec3f(0.0f, 0.0f, -1.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, 0.5f, -0.5f), BSR::Math::Vec3f(0.0f, 0.0f, -1.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, -0.5f, -0.5f), BSR::Math::Vec3f(0.0f, 0.0f, -1.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(0.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, -0.5f, -0.5f), BSR::Math::Vec3f(1.0f, 0.0f, 0.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, 0.5f, -0.5f), BSR::Math::Vec3f(1.0f, 0.0f, 0.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(1.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, 0.5f, 0.5f), BSR::Math::Vec3f(1.0f, 0.0f, 0.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, -0.5f, -0.5f), BSR::Math::Vec3f(1.0f, 0.0f, 0.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, 0.5f, 0.5f), BSR::Math::Vec3f(1.0f, 0.0f, 0.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, -0.5f, 0.5f), BSR::Math::Vec3f(1.0f, 0.0f, 0.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(0.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, -0.5f, 0.5f), BSR::Math::Vec3f(0.0f, 0.0f, 1.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, 0.5f, 0.5f), BSR::Math::Vec3f(0.0f, 0.0f, 1.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(1.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, 0.5f, 0.5f), BSR::Math::Vec3f(0.0f, 0.0f, 1.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, -0.5f, 0.5f), BSR::Math::Vec3f(0.0f, 0.0f, 1.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, 0.5f, 0.5f), BSR::Math::Vec3f(0.0f, 0.0f, 1.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, -0.5f, 0.5f), BSR::Math::Vec3f(0.0f, 0.0f, 1.0f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec2f(0.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, -0.5f, -0.5f), BSR::Math::Vec3f(0.0f, -1.0f, 0.0f), BSR::Math::Vec3f(1.0f, 0.0f, 0.0f), BSR::Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, -0.5f, -0.5f), BSR::Math::Vec3f(0.0f, -1.0f, 0.0f), BSR::Math::Vec3f(1.0f, 0.0f, 0.0f), BSR::Math::Vec2f(1.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, -0.5f, 0.5f), BSR::Math::Vec3f(0.0f, -1.0f, 0.0f), BSR::Math::Vec3f(1.0f, 0.0f, 0.0f), BSR::Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, -0.5f, -0.5f), BSR::Math::Vec3f(0.0f, -1.0f, 0.0f), BSR::Math::Vec3f(1.0f, 0.0f, 0.0f), BSR::Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, -0.5f, 0.5f), BSR::Math::Vec3f(0.0f, -1.0f, 0.0f), BSR::Math::Vec3f(1.0f, 0.0f, 0.0f), BSR::Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, -0.5f, 0.5f), BSR::Math::Vec3f(0.0f, -1.0f, 0.0f), BSR::Math::Vec3f(1.0f, 0.0f, 0.0f), BSR::Math::Vec2f(0.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, 0.5f, -0.5f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec3f(-1.0f, 0.0f, 0.0f), BSR::Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, 0.5f, -0.5f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec3f(-1.0f, 0.0f, 0.0f), BSR::Math::Vec2f(1.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, 0.5f, 0.5f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec3f(-1.0f, 0.0f, 0.0f), BSR::Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, 0.5f, -0.5f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec3f(-1.0f, 0.0f, 0.0f), BSR::Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, 0.5f, 0.5f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec3f(-1.0f, 0.0f, 0.0f), BSR::Math::Vec2f(1.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, 0.5f, 0.5f), BSR::Math::Vec3f(0.0f, 1.0f, 0.0f), BSR::Math::Vec3f(-1.0f, 0.0f, 0.0f), BSR::Math::Vec2f(0.0f, 1.0f)));

	_Mesh.IBO.PushBack(IndexData(0, 1, 2));
	_Mesh.IBO.PushBack(IndexData(3, 4, 5));
	_Mesh.IBO.PushBack(IndexData(6, 7, 8));
	_Mesh.IBO.PushBack(IndexData(9, 10, 11));
	_Mesh.IBO.PushBack(IndexData(12, 13, 14));
	_Mesh.IBO.PushBack(IndexData(15, 16, 17));
	_Mesh.IBO.PushBack(IndexData(18, 19, 20));
	_Mesh.IBO.PushBack(IndexData(21, 22, 23));
	_Mesh.IBO.PushBack(IndexData(24, 25, 26));
	_Mesh.IBO.PushBack(IndexData(27, 28, 29));
	_Mesh.IBO.PushBack(IndexData(30, 31, 32));
	_Mesh.IBO.PushBack(IndexData(33, 34, 35));
}

void BSR::Renderer::Mesh::GenerateQuad(Mesh& _Mesh)
{
	_Mesh.VBO.Clear();
	_Mesh.IBO.Clear();

	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, -0.5f, 0.0f), BSR::Math::Vec3f(0.0f, 0.0f, 1.0f), BSR::Math::Vec3f(1.0f, 0.0f, 0.0f), BSR::Math::Vec2f(0.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(-0.5f, 0.5f, 0.0f), BSR::Math::Vec3f(0.0f, 0.0f, 1.0f), BSR::Math::Vec3f(1.0f, 0.0f, 0.0f), BSR::Math::Vec2f(0.0f, 1.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, -0.5f, 0.0f), BSR::Math::Vec3f(0.0f, 0.0f, 1.0f), BSR::Math::Vec3f(1.0f, 0.0f, 0.0f), BSR::Math::Vec2f(1.0f, 0.0f)));
	_Mesh.VBO.PushBack(VertexData(BSR::Math::Vec3f(0.5f, 0.5f, 0.0f), BSR::Math::Vec3f(0.0f, 0.0f, 1.0f), BSR::Math::Vec3f(1.0f, 0.0f, 0.0f), BSR::Math::Vec2f(1.0f, 1.0f)));

	_Mesh.IBO.PushBack(IndexData(0, 2, 1));
	_Mesh.IBO.PushBack(IndexData(1, 2, 3));
}



BSR::Renderer::Model::Model() : Meshes()
{

}

BSR::Renderer::Model::Model(Model&& _Other) noexcept : Meshes(std::move(_Other.Meshes))
{

}

BSR::Renderer::Model::~Model()
{
	for (size_t _Index = 0; _Index < Meshes.size(); _Index++)
	{
		delete[] Meshes[_Index].Name;
	}
}

bool BSR::Renderer::Model::Load(const wchar_t* _Path)
{
	struct MeshFileData
	{
		wchar_t* Name = nullptr;
		size_t FacesStart = (size_t)(-1);
		size_t FacesEnd = (size_t)(-1);
	};

	struct FaceVertex
	{
		size_t Position = (size_t)(-1);
		size_t Normal = (size_t)(-1);
		size_t TextureCoords = (size_t)(-1);
	};

	Clear();

	if (!_Path)
	{
		return false;
	}

	std::wifstream _fIn;

	_fIn.open(_Path);

	if (!_fIn.is_open())
	{
		return false;
	}

	std::vector<MeshFileData> _Meshes;

	std::vector<BSR::Math::Vec3f> _Positions;
	std::vector<BSR::Math::Vec3f> _Normals;
	std::vector<BSR::Math::Vec2f> _TextureCoords;

	std::vector<std::vector<FaceVertex>> _Faces;

	wchar_t _Line[BSR_OBJ_MAX_LINE_LEN + 1];
	wchar_t _OriginalLine[BSR_OBJ_MAX_LINE_LEN + 1];

	while (!_fIn.eof())
	{
		_fIn.getline(_OriginalLine, BSR_OBJ_MAX_LINE_LEN);

		for (size_t _Index = 0; _Index < BSR_OBJ_MAX_LINE_LEN + 1; _Index++)
		{
			_Line[_Index] = _OriginalLine[_Index];
		}

		std::vector<const wchar_t*> _Tokens;

		{
			const wchar_t* _Token = String::TokenizeWhiteSpace(_Line);

			while (_Token)
			{
				_Tokens.push_back(_Token);

				_Token = String::TokenizeWhiteSpace(nullptr);
			}
		}

		if (!_Tokens.size())
		{
			continue;
		}

		if (_Tokens[0][0] == L'#')
		{
			continue;
		}

		if (String::TheSame(_Tokens[0], L"o"))
		{
			if (_Tokens.size() != 2)
			{
				_fIn.close();
				for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
				{
					delete[] _Meshes[_IndexDelete].Name;
				}
				return false;
			}

			for (size_t _Index = 0; _Index < _Meshes.size(); _Index++)
			{
				if (String::TheSame(_Tokens[1], _Meshes[_Index].Name))
				{
					_fIn.close();
					for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
					{
						delete[] _Meshes[_IndexDelete].Name;
					}
					return false;
				}
			}

			MeshFileData _MeshFileData;

			size_t _NameLen = String::Length(_Tokens[1]);

			_MeshFileData.Name = new wchar_t[_NameLen + 1];

			if (!_MeshFileData.Name)
			{
				_fIn.close();
				for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
				{
					delete[] _Meshes[_IndexDelete].Name;
				}
				return false;
			}

			for (size_t _Index = 0; _Index < _NameLen + 1; _Index++)
			{
				_MeshFileData.Name[_Index] = _Tokens[1][_Index];
			}

			_MeshFileData.FacesStart = _Faces.size();

			if (_Meshes.size())
			{
				_Meshes[_Meshes.size() - 1].FacesEnd = _Faces.size();
			}

			_Meshes.push_back(_MeshFileData);

			continue;
		}

		if (String::TheSame(_Tokens[0], L"v"))
		{
			if (_Tokens.size() != 4)
			{
				_fIn.close();
				for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
				{
					delete[] _Meshes[_IndexDelete].Name;
				}
				return false;
			}

			_Positions.push_back(BSR::Math::Vec3f(std::stof(_Tokens[1]), std::stof(_Tokens[2]), std::stof(_Tokens[3])));

			continue;
		}

		if (String::TheSame(_Tokens[0], L"vn"))
		{
			if (_Tokens.size() != 4)
			{
				_fIn.close();
				for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
				{
					delete[] _Meshes[_IndexDelete].Name;
				}
				return false;
			}

			_Normals.push_back((BSR::Math::Vec3f(std::stof(_Tokens[1]), std::stof(_Tokens[2]), std::stof(_Tokens[3]))));

			continue;
		}

		if (String::TheSame(_Tokens[0], L"vt"))
		{
			if (_Tokens.size() != 3)
			{
				_fIn.close();
				for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
				{
					delete[] _Meshes[_IndexDelete].Name;
				}
				return false;
			}

			_TextureCoords.push_back((BSR::Math::Vec2f(std::stof(_Tokens[1]), std::stof(_Tokens[2]))));

			continue;
		}

		if (String::TheSame(_Tokens[0], L"f"))
		{
			if (_Tokens.size() < 4)
			{
				_fIn.close();
				for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
				{
					delete[] _Meshes[_IndexDelete].Name;
				}
				return false;
			}

			_Faces.push_back(std::vector<FaceVertex>());

			std::vector<FaceVertex>& _CurrentFace = _Faces[_Faces.size() - 1];

			for (size_t _Index = 1; _Index < _Tokens.size(); _Index++)
			{
				size_t _TokenLen = String::Length(_Tokens[_Index]);

				if (_Tokens[_Index][0] == L'/' || _Tokens[_Index][_TokenLen - 1] == L'/')
				{
					_fIn.close();
					for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
					{
						delete[] _Meshes[_IndexDelete].Name;
					}
					return false;
				}

				FaceVertex _FaceVertex;

				size_t _CountSlash = String::Count(_Tokens[_Index], L'/');

				switch (_CountSlash)
				{
				case 0:
				{
					BSR_X64_CALL(_FaceVertex.Position = std::stoull(_Tokens[_Index]) - 1);
					BSR_X86_CALL(_FaceVertex.Position = std::stoul(_Tokens[_Index]) - 1);

					if (_FaceVertex.Position >= _Positions.size())
					{
						_fIn.close();
						for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
						{
							delete[] _Meshes[_IndexDelete].Name;
						}
						return false;
					}

					break;
				}
				case 1:
				{
					wchar_t _FaceVertexText[BSR_OBJ_MAX_LINE_LEN + 1];

					for (size_t _IndexCopy = 0; _IndexCopy < _TokenLen + 1; _IndexCopy++)
					{
						_FaceVertexText[_IndexCopy] = _Tokens[_Index][_IndexCopy];
					}

					std::vector<const wchar_t*> _FaceVertexTokens;

					{
						const wchar_t* _Token = String::TokenizeSlashes(_FaceVertexText);

						while (_Token)
						{
							_FaceVertexTokens.push_back(_Token);

							_Token = String::TokenizeSlashes(nullptr);
						}
					}

					BSR_X64_CALL(_FaceVertex.Position = std::stoull(_FaceVertexTokens[0]) - 1);
					BSR_X86_CALL(_FaceVertex.Position = std::stoul(_FaceVertexTokens[0]) - 1);

					if (_FaceVertex.Position >= _Positions.size())
					{
						_fIn.close();
						for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
						{
							delete[] _Meshes[_IndexDelete].Name;
						}
						return false;
					}

					BSR_X64_CALL(_FaceVertex.TextureCoords = std::stoull(_FaceVertexTokens[1]) - 1);
					BSR_X86_CALL(_FaceVertex.TextureCoords = std::stoul(_FaceVertexTokens[1]) - 1);

					if (_FaceVertex.TextureCoords >= _TextureCoords.size())
					{
						_fIn.close();
						for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
						{
							delete[] _Meshes[_IndexDelete].Name;
						}
						return false;
					}

					break;
				}
				case 2:
				{
					wchar_t _FaceVertexText[BSR_OBJ_MAX_LINE_LEN + 1];

					for (size_t _IndexCopy = 0; _IndexCopy < _TokenLen + 1; _IndexCopy++)
					{
						_FaceVertexText[_IndexCopy] = _Tokens[_Index][_IndexCopy];
					}

					std::vector<const wchar_t*> _FaceVertexTokens;

					{
						const wchar_t* _Token = String::TokenizeSlashes(_FaceVertexText);

						while (_Token)
						{
							_FaceVertexTokens.push_back(_Token);

							_Token = String::TokenizeSlashes(nullptr);
						}
					}

					if (_FaceVertexTokens.size() == 2)
					{
						BSR_X64_CALL(_FaceVertex.Position = std::stoull(_FaceVertexTokens[0]) - 1);
						BSR_X86_CALL(_FaceVertex.Position = std::stoul(_FaceVertexTokens[0]) - 1);

						if (_FaceVertex.Position >= _Positions.size())
						{
							_fIn.close();
							for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
							{
								delete[] _Meshes[_IndexDelete].Name;
							}
							return false;
						}

						BSR_X64_CALL(_FaceVertex.Normal = std::stoull(_FaceVertexTokens[1]) - 1);
						BSR_X86_CALL(_FaceVertex.Normal = std::stoul(_FaceVertexTokens[1]) - 1);

						if (_FaceVertex.Normal >= _Normals.size())
						{
							_fIn.close();
							for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
							{
								delete[] _Meshes[_IndexDelete].Name;
							}
							return false;
						}
					}
					else
					{
						BSR_X64_CALL(_FaceVertex.Position = std::stoull(_FaceVertexTokens[0]) - 1);
						BSR_X86_CALL(_FaceVertex.Position = std::stoul(_FaceVertexTokens[0]) - 1);

						if (_FaceVertex.Position >= _Positions.size())
						{
							_fIn.close();
							for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
							{
								delete[] _Meshes[_IndexDelete].Name;
							}
							return false;
						}

						BSR_X64_CALL(_FaceVertex.TextureCoords = std::stoull(_FaceVertexTokens[1]) - 1);
						BSR_X86_CALL(_FaceVertex.TextureCoords = std::stoul(_FaceVertexTokens[1]) - 1);

						if (_FaceVertex.TextureCoords >= _TextureCoords.size())
						{
							_fIn.close();
							for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
							{
								delete[] _Meshes[_IndexDelete].Name;
							}
							return false;
						}

						BSR_X64_CALL(_FaceVertex.Normal = std::stoull(_FaceVertexTokens[2]) - 1);
						BSR_X86_CALL(_FaceVertex.Normal = std::stoul(_FaceVertexTokens[2]) - 1);

						if (_FaceVertex.Normal >= _Normals.size())
						{
							_fIn.close();
							for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
							{
								delete[] _Meshes[_IndexDelete].Name;
							}
							return false;
						}
					}

					break;
				}
				default:
				{
					_fIn.close();
					for (size_t _IndexDelete = 0; _IndexDelete < _Meshes.size(); _IndexDelete++)
					{
						delete[] _Meshes[_IndexDelete].Name;
					}
					return false;
				}
				}

				_CurrentFace.push_back(_FaceVertex);
			}

			continue;
		}
	}

	if (!_Meshes.size())
	{
		MeshFileData _DefaultMesh;

		_DefaultMesh.Name = new wchar_t[lstrlenW(L"default_name") + 1];

		if (!_DefaultMesh.Name)
		{
			_fIn.close();
			return false;
		}

		lstrcpyW(_DefaultMesh.Name, L"default_name");
		_DefaultMesh.FacesStart = 0;

		_Meshes.push_back(_DefaultMesh);
	}

	_Meshes[_Meshes.size() - 1].FacesEnd = _Faces.size();

	for (size_t _IndexMesh = 0; _IndexMesh < _Meshes.size(); _IndexMesh++)
	{
		MeshFileData& _CurrentMesh = _Meshes[_IndexMesh];

		Mesh _MeshTemp;

		_MeshTemp.Name = _CurrentMesh.Name;

		for (size_t _IndexFace = _CurrentMesh.FacesStart; _IndexFace < _CurrentMesh.FacesEnd; _IndexFace++)
		{
			std::vector<FaceVertex>& _CurrentFace = _Faces[_IndexFace];

			for (size_t _IndexFaceVertex = 1; _IndexFaceVertex < _CurrentFace.size() - 1; _IndexFaceVertex++)
			{
				VertexData _VertA;
				VertexData _VertB;
				VertexData _VertC;

				_VertA.Position = _Positions[_CurrentFace[0].Position];

				_VertB.Position = _Positions[_CurrentFace[_IndexFaceVertex].Position];

				_VertC.Position = _Positions[_CurrentFace[_IndexFaceVertex + 1].Position];

				if (_CurrentFace[0].Normal != (size_t)(-1))
				{
					_VertA.Normal = _Normals[_CurrentFace[0].Normal];
				}
				else
				{
					_VertA.Normal = BSR::Math::Vec3f::Cross(_VertB.Position - _VertA.Position, _VertC.Position - _VertA.Position);
				}
				if (_CurrentFace[0].TextureCoords != (size_t)(-1))
				{
					_VertA.TextureCoords = _TextureCoords[_CurrentFace[0].TextureCoords];
				}
				else
				{
					_VertA.TextureCoords = BSR::Math::Vec2f(0.0f, 0.0f);
				}

				if (_CurrentFace[_IndexFaceVertex].Normal != (size_t)(-1))
				{
					_VertB.Normal = _Normals[_CurrentFace[_IndexFaceVertex].Normal];
				}
				else
				{
					_VertB.Normal = BSR::Math::Vec3f::Cross(_VertB.Position - _VertA.Position, _VertC.Position - _VertA.Position);
				}
				if (_CurrentFace[_IndexFaceVertex].TextureCoords != (size_t)(-1))
				{
					_VertB.TextureCoords = _TextureCoords[_CurrentFace[_IndexFaceVertex].TextureCoords];
				}
				else
				{
					_VertB.TextureCoords = BSR::Math::Vec2f(0.0f, 0.0f);
				}

				if (_CurrentFace[_IndexFaceVertex + 1].Normal != (size_t)(-1))
				{
					_VertC.Normal = _Normals[_CurrentFace[_IndexFaceVertex + 1].Normal];
				}
				else
				{
					_VertC.Normal = BSR::Math::Vec3f::Cross(_VertB.Position - _VertA.Position, _VertC.Position - _VertA.Position);
				}
				if (_CurrentFace[_IndexFaceVertex + 1].TextureCoords != (size_t)(-1))
				{
					_VertC.TextureCoords = _TextureCoords[_CurrentFace[_IndexFaceVertex + 1].TextureCoords];
				}
				else
				{
					_VertC.TextureCoords = BSR::Math::Vec2f(0.0f, 0.0f);
				}

				if (_CurrentFace[0].TextureCoords != (size_t)(-1) && _CurrentFace[_IndexFaceVertex].TextureCoords != (size_t)(-1) && _CurrentFace[_IndexFaceVertex + 1].TextureCoords != (size_t)(-1))
				{
					if (_VertA.TextureCoords != _VertB.TextureCoords && _VertB.TextureCoords != _VertC.TextureCoords && _VertC.TextureCoords != _VertA.TextureCoords)
					{
						BSR::Math::Vec3f _Edge1 = _VertB.Position - _VertA.Position;
						BSR::Math::Vec3f _Edge2 = _VertC.Position - _VertA.Position;

						BSR::Math::Vec2f _Delta1 = _VertB.TextureCoords - _VertA.TextureCoords;
						BSR::Math::Vec2f _Delta2 = _VertC.TextureCoords - _VertA.TextureCoords;

						float _Factor = 1.0f / (_Delta1.x * _Delta2.y - _Delta2.x * _Delta1.y);

						_VertA.Tangent.x = _Factor * (_Delta2.y * _Edge1.x - _Delta1.y * _Edge2.x);
						_VertA.Tangent.y = _Factor * (_Delta2.y * _Edge1.y - _Delta1.y * _Edge2.y);
						_VertA.Tangent.z = _Factor * (_Delta2.y * _Edge1.z - _Delta1.y * _Edge2.z);

						_VertA.Tangent /= _VertA.Tangent.Magnitude();

						_VertB.Tangent.x = _VertA.Tangent.x;
						_VertB.Tangent.y = _VertA.Tangent.y;
						_VertB.Tangent.z = _VertA.Tangent.z;

						_VertC.Tangent.x = _VertA.Tangent.x;
						_VertC.Tangent.y = _VertA.Tangent.y;
						_VertC.Tangent.z = _VertA.Tangent.z;
					}
					else
					{
						_VertA.Tangent = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
						_VertB.Tangent = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
						_VertC.Tangent = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
					}
				}
				else
				{
					_VertA.Tangent = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
					_VertB.Tangent = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
					_VertC.Tangent = BSR::Math::Vec3f(0.0f, 0.0f, 0.0f);
				}

				IndexData _IndexData;

				_IndexData.IndexA = _MeshTemp.VBO.GetSize();
				_IndexData.IndexB = _MeshTemp.VBO.GetSize() + 1;
				_IndexData.IndexC = _MeshTemp.VBO.GetSize() + 2;

				_MeshTemp.VBO.PushBack(_VertA);
				_MeshTemp.VBO.PushBack(_VertB);
				_MeshTemp.VBO.PushBack(_VertC);

				_MeshTemp.IBO.PushBack(_IndexData);
			}
		}

		Mesh _MeshTrue;

		_MeshTrue.Name = _CurrentMesh.Name;

		for (size_t _IndexTemp = 0; _IndexTemp < _MeshTemp.VBO.GetSize(); _IndexTemp++)
		{
			bool _Found = false;

			for (size_t _IndexTrue = 0; _IndexTrue < _MeshTrue.VBO.GetSize(); _IndexTrue++)
			{
				if (_MeshTemp.VBO[_IndexTemp].Position == _MeshTrue.VBO[_IndexTrue].Position && _MeshTemp.VBO[_IndexTemp].Normal == _MeshTrue.VBO[_IndexTrue].Normal && _MeshTemp.VBO[_IndexTemp].Tangent == _MeshTrue.VBO[_IndexTrue].Tangent && _MeshTemp.VBO[_IndexTemp].TextureCoords == _MeshTrue.VBO[_IndexTrue].TextureCoords)
				{
					_Found = true;
					break;
				}
			}

			if (_Found)
			{
				continue;
			}

			_MeshTrue.VBO.PushBack(_MeshTemp.VBO[_IndexTemp]);
		}

		for (size_t _IndexTemp = 0; _IndexTemp < _MeshTemp.IBO.GetSize(); _IndexTemp++)
		{
			IndexData _IndexDataTrue;

			for (size_t _IndexTrue = 0; _IndexTrue < _MeshTrue.VBO.GetSize(); _IndexTrue++)
			{
				if (_MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexA].Position == _MeshTrue.VBO[_IndexTrue].Position && _MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexA].Normal == _MeshTrue.VBO[_IndexTrue].Normal && _MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexA].Tangent == _MeshTrue.VBO[_IndexTrue].Tangent && _MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexA].TextureCoords == _MeshTrue.VBO[_IndexTrue].TextureCoords)
				{
					_IndexDataTrue.IndexA = _IndexTrue;
					break;
				}
			}

			for (size_t _IndexTrue = 0; _IndexTrue < _MeshTrue.VBO.GetSize(); _IndexTrue++)
			{
				if (_MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexB].Position == _MeshTrue.VBO[_IndexTrue].Position && _MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexB].Normal == _MeshTrue.VBO[_IndexTrue].Normal && _MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexB].Tangent == _MeshTrue.VBO[_IndexTrue].Tangent && _MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexB].TextureCoords == _MeshTrue.VBO[_IndexTrue].TextureCoords)
				{
					_IndexDataTrue.IndexB = _IndexTrue;
					break;
				}
			}

			for (size_t _IndexTrue = 0; _IndexTrue < _MeshTrue.VBO.GetSize(); _IndexTrue++)
			{
				if (_MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexC].Position == _MeshTrue.VBO[_IndexTrue].Position && _MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexC].Normal == _MeshTrue.VBO[_IndexTrue].Normal && _MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexC].Tangent == _MeshTrue.VBO[_IndexTrue].Tangent && _MeshTemp.VBO[_MeshTemp.IBO[_IndexTemp].IndexC].TextureCoords == _MeshTrue.VBO[_IndexTrue].TextureCoords)
				{
					_IndexDataTrue.IndexC = _IndexTrue;
					break;
				}
			}

			_MeshTrue.IBO.PushBack(_IndexDataTrue);
		}

		Meshes.emplace_back(std::move(_MeshTrue));
	}

	_fIn.close();

	return true;
}

const bool BSR::Renderer::Model::Save(const wchar_t* _Path) const
{
	std::wofstream _fOut;

	_fOut.open(_Path);

	if (!_fOut.is_open())
	{
		return false;
	}

	size_t _CurrentSize = 1;

	for (size_t _IndexMesh = 0; _IndexMesh < Meshes.size(); _IndexMesh++)
	{
		const Mesh& _CurrentMesh = Meshes[_IndexMesh];

		if (!_CurrentMesh.Name)
		{
			_fOut.close();
			_wremove(_Path);
			return false;
		}

		_fOut << L'o' << L' ' << _CurrentMesh.Name << L'\n';

		for (size_t _IndexVertex = 0; _IndexVertex < _CurrentMesh.VBO.GetSize(); _IndexVertex++)
		{
			const VertexData& _CurrentVertex = _CurrentMesh.VBO[_IndexVertex];

			_fOut << L'v' << L' ' << _CurrentVertex.Position.x << L' ' << _CurrentVertex.Position.y << L' ' << _CurrentVertex.Position.z << L'\n';
		}

		for (size_t _IndexVertex = 0; _IndexVertex < _CurrentMesh.VBO.GetSize(); _IndexVertex++)
		{
			const VertexData& _CurrentVertex = _CurrentMesh.VBO[_IndexVertex];

			_fOut << L'v' << L't' << L' ' << _CurrentVertex.TextureCoords.x << L' ' << _CurrentVertex.TextureCoords.y << L'\n';
		}

		for (size_t _IndexVertex = 0; _IndexVertex < _CurrentMesh.VBO.GetSize(); _IndexVertex++)
		{
			const VertexData& _CurrentVertex = _CurrentMesh.VBO[_IndexVertex];

			_fOut << L'v' << L'n' << L' ' << _CurrentVertex.Normal.x << L' ' << _CurrentVertex.Normal.y << L' ' << _CurrentVertex.Normal.z << L'\n';
		}

		for (size_t _IndexTriangle = 0; _IndexTriangle < _CurrentMesh.IBO.GetSize(); _IndexTriangle++)
		{
			const IndexData& _CurrentTriangle = _CurrentMesh.IBO[_IndexTriangle];

			_fOut << L'f' << L' ';
			_fOut << _CurrentTriangle.IndexA + _CurrentSize << L'/' << _CurrentTriangle.IndexA + _CurrentSize << L'/' << _CurrentTriangle.IndexA + _CurrentSize << L' ';
			_fOut << _CurrentTriangle.IndexB + _CurrentSize << L'/' << _CurrentTriangle.IndexB + _CurrentSize << L'/' << _CurrentTriangle.IndexB + _CurrentSize << L' ';
			_fOut << _CurrentTriangle.IndexC + _CurrentSize << L'/' << _CurrentTriangle.IndexC + _CurrentSize << L'/' << _CurrentTriangle.IndexC + _CurrentSize << L'\n';
		}

		_CurrentSize += _CurrentMesh.VBO.GetSize();
	}

	_fOut.close();

	return true;
}

void BSR::Renderer::Model::PushBack(const Mesh& _Mesh)
{
	for (size_t _Index = 0; _Index < Meshes.size(); _Index++)
	{
		if (String::TheSame(Meshes[_Index].Name, _Mesh.Name))
		{
			BSR_DEBUG_BREAK();
			return;
		}
	}

	Mesh _NewMesh;

	if (_Mesh.Name)
	{
		size_t _StrLen = String::Length(_Mesh.Name);

		_NewMesh.Name = new wchar_t[_StrLen + 1];

		if (!_NewMesh.Name)
		{
			BSR_DEBUG_BREAK();
			return;
		}

		wcscpy_s(_NewMesh.Name, _StrLen + 1, _Mesh.Name);
	}

	_NewMesh.VBO = _Mesh.VBO;
	_NewMesh.IBO = _Mesh.IBO;

	Meshes.emplace_back(_NewMesh);
}

void BSR::Renderer::Model::EmplaceBack(Mesh&& _Mesh) noexcept
{
	for (size_t _Index = 0; _Index < Meshes.size(); _Index++)
	{
		if (String::TheSame(Meshes[_Index].Name, _Mesh.Name))
		{
			BSR_DEBUG_BREAK();
			return;
		}
	}

	Meshes.emplace_back(std::move(_Mesh));
}

void BSR::Renderer::Model::Erase(const size_t _Index)
{
	delete[] Meshes[_Index].Name;
	Meshes.erase(Meshes.begin() + _Index);
}

void BSR::Renderer::Model::Clear()
{
	for (size_t _Index = 0; _Index < Meshes.size(); _Index++)
	{
		delete[] Meshes[_Index].Name;
	}

	Meshes.clear();
}

const size_t BSR::Renderer::Model::GetSize() const
{
	return Meshes.size();
}

BSR::Renderer::Mesh& BSR::Renderer::Model::operator[] (const size_t _Index)
{
	return Meshes[_Index];
}

const BSR::Renderer::Mesh& BSR::Renderer::Model::operator[] (const size_t _Index) const
{
	return Meshes[_Index];
}

void BSR::Renderer::Model::operator= (Model&& _Other) noexcept
{
	Clear();

	Meshes = std::move(_Other.Meshes);
}



const bool BSR::Renderer::FrameBuffer::Valid() const
{
	return
		Width != 0 &&
		Height != 0 &&
		Albedo != nullptr &&
		Metalness != nullptr &&
		Roughness != nullptr &&
		AmbientOcclusion != nullptr &&
		NormalMap != nullptr &&
		Emission != nullptr &&
		Position != nullptr &&
		Depth != nullptr &&
		Stencil != nullptr &&
		Result != nullptr;
}

const bool BSR::Renderer::FrameBuffer::operator== (const FrameBuffer& _Other) const
{
	return
		Width == _Other.Width &&
		Height == _Other.Height &&
		Albedo == _Other.Albedo &&
		Metalness == _Other.Metalness &&
		Roughness == _Other.Roughness &&
		AmbientOcclusion == _Other.AmbientOcclusion &&
		NormalMap == _Other.NormalMap &&
		Emission == _Other.Emission &&
		Position == _Other.Position &&
		Depth == _Other.Depth &&
		Stencil == _Other.Stencil &&
		Result == _Other.Result;
}
