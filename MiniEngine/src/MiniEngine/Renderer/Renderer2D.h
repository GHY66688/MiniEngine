#pragma once


#include "OrthographicCamera.h"

#include "MiniEngine/Renderer/Texture.h"


namespace MG {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		//static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::mat4 rotation, const glm::vec4& color)
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& color);
	};
}
