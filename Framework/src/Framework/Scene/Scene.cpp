#include "precompile.h"
#include "Scene.h"

#include <glm/glm.hpp>

#include "Framework/Renderer/Renderer2D.h"
#include "Components.h"
#include "Entity.h"

namespace Framework
{
    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    Entity Scene::CreateEntity(const std::string& aName)
    {
        Entity entity = { myRegistry.create(), this };
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = aName.empty() ? "Entity" : aName;
        return entity;
    }

    void Scene::DestroyEntity(Entity anEntity)
    {
        myRegistry.destroy(anEntity);
    }

    void Scene::OnUpdateRuntime(Timestep aTimestep)
    {
        
        //Update scripts
        {
            myRegistry.view<NativeScriptComponent>().each([=](auto aEntity, auto& aNsc) 
            {
                //TODO: Move to Scene::OnScenePlay
                if (aNsc.Instance == nullptr)
                {
                    aNsc.Instance = aNsc.InstantiateScript();
                    aNsc.Instance->myEntity = { aEntity, this };
                    aNsc.Instance->OnCreate();
                }

                aNsc.Instance->OnUpdate(aTimestep);
            });
        }

        //Render 2D
        {
            Camera* mainCamera = nullptr;
            glm::mat4 mainCameraTransform;
            {
                auto view = myRegistry.view<TransformComponent, CameraComponent>();
                for (auto entity : view)
                {
                    auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
                    if (camera.Primary)
                    {
                        mainCamera = &camera.Camera;
                        mainCameraTransform = transform.GetTransform();
                        break;
                    }
                }
            }

            if (mainCamera)
            {
                Renderer2D::BeginScene(*mainCamera, mainCameraTransform);

                {
                    auto group = myRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

                    for (auto entity : group)
                    {
                        auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                        Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
                    }
                }

                Renderer2D::EndScene();
            }
        }

    }

    void Scene::OnUpdateEditor(Timestep aTimestep, EditorCamera& camera)
    {

        Renderer2D::BeginScene(camera);
        {
            auto group = myRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

            for (auto entity : group)
            {
                auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
            }
        }

        Renderer2D::EndScene();
    }

    void Scene::OnViewportResize(const uint32_t aWidth, const uint32_t aHeight)
    {
        myViewportWidth = aWidth;
        myViewportHeight = aHeight;

        //Resize our non-FixedAspectRatio camera
        auto view = myRegistry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (cameraComponent.FixedAspectRatio == false)
            {
                cameraComponent.Camera.SetViewportSize(aWidth, aHeight);
            }
        }
    }

    Entity Scene::GetPrimaryCameraEntity()
    {
        auto view = myRegistry.view<CameraComponent>();
        for (auto entity : view)
        {
            const auto& camera = view.get<CameraComponent>(entity);
            if (camera.Primary)
            {
                return Entity{ entity, this };
            }
        }
        return {};
    }

    template<typename T>
    void Scene::OnComponentAdded(Entity anEntity, T& aComponent)
    {
        static_assert(false);
    }

    template<>
    void Scene::OnComponentAdded<TransformComponent>(Entity anEntity, TransformComponent& aComponent)
    {   
    }

    template<>
    void Scene::OnComponentAdded<CameraComponent>(Entity anEntity, CameraComponent& aComponent)
    {
        aComponent.Camera.SetViewportSize(myViewportWidth, myViewportHeight);
    }

    template<>
    void Scene::OnComponentAdded<TagComponent>(Entity anEntity, TagComponent& aComponent)
    {
    }

    template<>
    void Scene::OnComponentAdded<SpriteRendererComponent>(Entity anEntity, SpriteRendererComponent& aComponent)
    {
    }

    template<>
    void Scene::OnComponentAdded<NativeScriptComponent>(Entity anEntity, NativeScriptComponent& aComponent)
    {
    }
}