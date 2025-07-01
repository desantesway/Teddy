#include <Teddy.h>

#include "imgui.h"

class ExampleLayer : public Teddy::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), 
		m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(Teddy::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.2f, 0.5f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 1.0f, 0.3f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.3f, 0.2f, 1.0f, 1.0f
		};

		std::shared_ptr<Teddy::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Teddy::VertexBuffer::Create(vertices, sizeof(vertices)));

		Teddy::BufferLayout layout = {
			{ Teddy::ShaderDataType::Float3, "a_Position" },
			{ Teddy::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Teddy::IndexBuffer> indexBuffer;
		indexBuffer.reset(Teddy::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Teddy::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		std::shared_ptr<Teddy::VertexBuffer> squareVB;
		squareVB.reset(Teddy::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Teddy::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Teddy::IndexBuffer> squareIB;
		squareIB.reset(Teddy::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(

			#version 330 core
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);		
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

		m_Shader.reset(new Teddy::Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);		
			}
		)";
		std::string blueShaderFragmentSrc = R"(

			#version 330 core
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_BlueShader.reset(new Teddy::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	void OnUpdate(Teddy::Timestep ts) override
	{

		TED_TRACE("Delta time: {0}s {1}ms", ts.GetSeconds(), ts.GetMilliseconds());

		if (Teddy::Input::IsKeyPressed(TED_KEY_LEFT)) {
			m_CameraPosition.x -= 5.0f * ts.GetSeconds();
		} else if (Teddy::Input::IsKeyPressed(TED_KEY_RIGHT)) {
			m_CameraPosition.x += 5.0f * ts.GetSeconds();
		}

		Teddy::RenderCommand::SetClearColor({ 0.9f, 0.1f, 0.1f, 1 });
		Teddy::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		//m_Camera.SetRotation(45.0f);

		Teddy::Renderer::BeginScene(m_Camera);

		Teddy::Renderer::Submit(m_BlueShader, m_SquareVA);
		Teddy::Renderer::Submit(m_Shader, m_VertexArray);

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
		ImGui::Begin("Example Layer");
		ImGui::Text("Hello, this is an example layer!");
		ImGui::End();
	}

private:

	std::shared_ptr<Teddy::Shader> m_Shader;
	std::shared_ptr<Teddy::VertexArray> m_VertexArray;

	std::shared_ptr<Teddy::Shader> m_BlueShader;
	std::shared_ptr<Teddy::VertexArray> m_SquareVA;

	Teddy::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
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