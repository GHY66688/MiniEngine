#pragma once


namespace MG {
	
	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case MG::ShaderDataType::Float:			return 4;
		case MG::ShaderDataType::Float2:		return 4 * 2;
		case MG::ShaderDataType::Float3:		return 4 * 3;
		case MG::ShaderDataType::Float4:		return 4 * 4;
		case MG::ShaderDataType::Mat3:			return 4 * 3 * 3;
		case MG::ShaderDataType::Mat4:			return 4 * 4 * 4;
		case MG::ShaderDataType::Int:			return 4;
		case MG::ShaderDataType::Int2:			return 4 * 2;
		case MG::ShaderDataType::Int3:			return 4 * 3;
		case MG::ShaderDataType::Int4:			return 4 * 4;
		case MG::ShaderDataType::Bool:			return 4;
		}

		MG_CORE_ASSERT(false, "Unknown Shader Data Type! [ShaderDataTypeSize]");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement()
		{

		}

		BufferElement(const std::string& name, ShaderDataType type, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{

		}


		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:			return 1;
			case ShaderDataType::Float2:		return 2;
			case ShaderDataType::Float3:		return 3;
			case ShaderDataType::Float4:		return 4;
			case ShaderDataType::Mat3:			return 3 * 3;
			case ShaderDataType::Mat4:			return 4 * 4;
			case ShaderDataType::Int:			return 1;
			case ShaderDataType::Int2:			return 2;
			case ShaderDataType::Int3:			return 3;
			case ShaderDataType::Int4:			return 4;
			case ShaderDataType::Bool:			return 1;
			}

			MG_CORE_ASSERT(false, "Unknown Shader Data Type! [GetComponentCount]");
			return 0;
		}
	};



	class BufferLayout
	{
	public:
		//ʹ�ö��element���г�ʼ��ʱ�������ȹ���vector�ٹ���BufferLayout

		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		inline const uint32_t GetStride() const { return m_Stride; }


		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	
	private:
		void CalculateOffsetAndStride()
		{
			//����һ��������Ҫ�����Ĵ�С
			uint32_t offset = 0;
			//����һ����ͬ������Ҫ�����Ĳ���
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}

		}
	
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};



	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);

	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t size);

	};

}	//end MG