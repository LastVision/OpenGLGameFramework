#include "GameLayer.h"

#include "imgui/imgui.h"

GameLayer::GameLayer()
    : Layer("Game"), myCameraController(1280.0f / 720.0f)
{
}

void GameLayer::OnAttach()
{
	myCameraController.SetZoomLevel(5.0);
	
	//Initialize things here
}

void GameLayer::OnDeattach()
{
	//Destory things here
}

void GameLayer::OnUpdate(Framework::Timestep timestep)
{
	myCameraController.OnUpdate(timestep);

	//Update game loop happens here

	Framework::Renderer2D::ResetStats();
	Framework::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	Framework::RenderCommand::Clear();

	Framework::Renderer2D::BeginScene(myCameraController.GetCamera());

	//Rendering happens here

	Framework::Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender()
{
	//This can be useful to render things like console, debug stats, etc

    ImGui::Begin("Demo");
	auto stats = Framework::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.myDrawCalls);
	ImGui::Text("Quads: %d", stats.myQuadCounts);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    ImGui::End();
}

void GameLayer::OnEvent(Framework::Event& e)
{
	//here we can catch events like resizeing, keyboard and mouse input, etc.

	myCameraController.OnEvent(e);
}
