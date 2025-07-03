#include <Teddy.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class ExampleLayer : public Teddy::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), 
		m_CameraPosition(0.0f),
		m_SquarePosition(0.0f)
	{
		m_VertexArray.reset(Teddy::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.2f, 0.5f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 1.0f, 0.3f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.3f, 0.2f, 1.0f, 1.0f
		};

		Teddy::Ref<Teddy::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Teddy::VertexBuffer::Create(vertices, sizeof(vertices)));

		Teddy::BufferLayout layout = {
			{ Teddy::ShaderDataType::Float3, "a_Position" },
			{ Teddy::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Teddy::Ref<Teddy::IndexBuffer> indexBuffer;
		indexBuffer.reset(Teddy::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Teddy::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		Teddy::Ref<Teddy::VertexBuffer> squareVB;
		squareVB.reset(Teddy::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Teddy::ShaderDataType::Float3, "a_Position" },
			{ Teddy::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Teddy::Ref<Teddy::IndexBuffer> squareIB;
		squareIB.reset(Teddy::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(

			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);		
			}
		)";
		std::string fragmentSrc = R"(

			#version 330 core
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(Teddy::Shader::Create(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);		
			}
		)";
		std::string blueShaderFragmentSrc = R"(

			#version 330 core
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
	
			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_BlueShader.reset(Teddy::Shader::Create(blueShaderVertexSrc, blueShaderFragmentSrc));

		m_TextureShader.reset(Teddy::Shader::Create("assets/shaders/Texture.glsl"));

		m_Texture = Teddy::Texture2D::Create("assets/textures/piano/pedal/sus.png");

		std::dynamic_pointer_cast<Teddy::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Teddy::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Teddy::Timestep ts) override
	{

		//TED_TRACE("Delta time: {0}s {1}ms", ts.GetSeconds(), ts.GetMilliseconds());

		if (Teddy::Input::IsKeyPressed(TED_KEY_LEFT)) {
			m_CameraPosition.x -= 5.0f * ts.GetSeconds();
		} else if (Teddy::Input::IsKeyPressed(TED_KEY_RIGHT)) {
			m_CameraPosition.x += 5.0f * ts.GetSeconds();
		}

		if (Teddy::Input::IsKeyPressed(TED_KEY_A)) {
			m_SquarePosition.x -= 2.5f * ts.GetSeconds();
		}
		else if (Teddy::Input::IsKeyPressed(TED_KEY_D)) {
			m_SquarePosition.x += 2.5f * ts.GetSeconds();
		}

		Teddy::RenderCommand::SetClearColor({ 0.9f, 0.1f, 0.1f, 1 });
		Teddy::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		//m_Camera.SetRotation(45.0f);

		Teddy::Renderer::BeginScene(m_Camera);

		glm::vec3 whiteColor(1.0f, 1.0f, 1.0f);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Teddy::OpenGLShader>(m_BlueShader)->Bind();

		for (int y = -20; y < 20; y++)
		{
			for (int x = -20; x < 20; x++)
			{
				glm::vec3 pos(x * 0.1f, y * 0.1f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				if ((x+y)% 2 == 0) {
					std::dynamic_pointer_cast<Teddy::OpenGLShader>(m_BlueShader)->UploadUniformFloat3("u_Color", m_SquareColor);
				}
				else {
					std::dynamic_pointer_cast<Teddy::OpenGLShader>(m_BlueShader)->UploadUniformFloat3("u_Color", whiteColor);
				}

				Teddy::Renderer::Submit(m_BlueShader, m_SquareVA, transform);
			}
		}

		m_Texture->Bind();
		Teddy::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// triangle and square
		glm::mat4 squareTransform = glm::translate(glm::mat4(1.0f), m_SquarePosition);
		Teddy::Renderer::Submit(m_BlueShader, m_SquareVA, squareTransform);
		//Teddy::Renderer::Submit(m_Shader, m_VertexArray);

		Teddy::Renderer::EndScene();
	}

	void OnEvent(Teddy::Event& event) override
	{
		Teddy::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Teddy::KeyPressedEvent>(TED_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Teddy::KeyPressedEvent& event)
	{
		return false; 
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("ChangeColor");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

private:

	Teddy::Ref<Teddy::Shader> m_Shader;
	Teddy::Ref<Teddy::VertexArray> m_VertexArray;

	Teddy::Ref<Teddy::Shader> m_BlueShader, m_TextureShader;
	Teddy::Ref<Teddy::VertexArray> m_SquareVA;

	Teddy::Ref<Teddy::Texture2D> m_Texture;

	Teddy::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;

	glm::vec3 m_SquareColor = {0.2f, 0.1f, 1.0f};
	glm::vec3 m_SquarePosition;
};

class KeyStar : public Teddy::Application {
public:
	KeyStar() {
		PushLayer(new ExampleLayer());
	}
	~KeyStar() {
		// Cleanup code can go here
	}
};


Teddy::Application* Teddy::CreateApplication() {
	return new KeyStar();
} // CreateApplication