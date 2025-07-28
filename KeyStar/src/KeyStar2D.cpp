#include "KeyStar2D.h"

KeyStar2D::KeyStar2D() 
	: Layer("Example"), m_CameraController(1920.0f / 1080.0f, true)
{

}

void KeyStar2D::OnAttach()
{

	m_SquareVA = Teddy::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Teddy::Ref<Teddy::VertexBuffer> squareVB;
	squareVB.reset(Teddy::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout({
		{ Teddy::ShaderDataType::Float3, "a_Position" }
		});
	m_SquareVA->AddVertexBuffer(squareVB);
	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Teddy::Ref<Teddy::IndexBuffer> squareIB;
	squareIB.reset(Teddy::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_FlatColorShader = Teddy::Shader::Create("assets/shaders/FlatColor.glsl");
}

void KeyStar2D::OnDetach()
{
}

void KeyStar2D::OnUpdate(Teddy::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Teddy::RenderCommand::SetClearColor({ 0.9f, 0.1f, 0.1f, 1 });
	Teddy::RenderCommand::Clear();

	Teddy::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Teddy::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Teddy::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Teddy::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Teddy::Renderer::EndScene();
}

void KeyStar2D::OnEvent(Teddy::Event& event)
{
	m_CameraController.OnEvent(event);

}
void KeyStar2D::OnImGuiRender() 
{
	ImGui::Begin("Settings");
	ImGui::Text("ChangeColor");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}