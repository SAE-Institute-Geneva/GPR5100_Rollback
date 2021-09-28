#include <network/full_debug_app.h>

namespace game
{
    SimulationDebugApp::SimulationDebugApp() : server_(clients_)
    {
        for (int i = 0; i < clients_.size(); i++)
        {
            clients_[i] = std::make_unique<SimulationClient>(server_);
        }
    }

    void SimulationDebugApp::OnEvent(const sf::Event& event)
    {
    }

    void SimulationDebugApp::Init()
    {
        /*
        Job initJob = Job([this]()
            {
                quad_.Init();
                //client shader init
                const auto& config = BasicEngine::GetInstance()->GetConfig();
                clientShader_.LoadFromFile(config.dataRootPath + "shaders/comp_net/client.vert",
                    config.dataRootPath + "shaders/comp_net/client.frag");
                windowSize_ = config.windowSize;
                for (auto& framebuffer : clientsFramebuffer_)
                {
                    framebuffer.SetSize(windowSize_ / Vec2u(2, 1));
                    framebuffer.Create();
                }
                for (auto& client : clients_)
                {
                    client->SetWindowSize(windowSize_ / Vec2u(2, 1));
                }
                for (auto& client : clients_)
                {
                    client->Init();
                }
                server_.Init();
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            });
        BasicEngine::GetInstance()->ScheduleJob(&initJob, JobThreadType::RENDER_THREAD);
        initJob.Join();
        */
    }

    void SimulationDebugApp::Update(sf::Time dt)
    {
        /*
        //Checking if keys are down
        const Uint8* keys = SDL_GetKeyboardState(nullptr);
        net::PlayerInput clientInput1 = 0;
        clientInput1 = clientInput1 | (keys[SDL_SCANCODE_LEFT] ? asteroid::PlayerInput::LEFT : 0u);
        clientInput1 = clientInput1 | (keys[SDL_SCANCODE_RIGHT] ? asteroid::PlayerInput::RIGHT : 0u);
        clientInput1 = clientInput1 | (keys[SDL_SCANCODE_UP] ? asteroid::PlayerInput::UP : 0u);
        clientInput1 = clientInput1 | (keys[SDL_SCANCODE_DOWN] ? asteroid::PlayerInput::DOWN : 0u);
        clientInput1 = clientInput1 | (keys[SDL_SCANCODE_RCTRL] ? asteroid::PlayerInput::SHOOT : 0u);

        clients_[0]->SetPlayerInput(clientInput1);

        net::PlayerInput clientInput2 = 0;
        clientInput2 = clientInput2 | (keys[SDL_SCANCODE_A] ? asteroid::PlayerInput::LEFT : 0u);
        clientInput2 = clientInput2 | (keys[SDL_SCANCODE_D] ? asteroid::PlayerInput::RIGHT : 0u);
        clientInput2 = clientInput2 | (keys[SDL_SCANCODE_W] ? asteroid::PlayerInput::UP : 0u);
        clientInput2 = clientInput2 | (keys[SDL_SCANCODE_S] ? asteroid::PlayerInput::DOWN : 0u);
        clientInput2 = clientInput2 | (keys[SDL_SCANCODE_SPACE] ? asteroid::PlayerInput::SHOOT : 0u);

        clients_[1]->SetPlayerInput(clientInput2);
        server_.Update(dt);
        for (auto& client : clients_)
        {
            client->Update(dt);
        }
        RendererLocator::get().Render(this);
        */
    }

    void SimulationDebugApp::Destroy()
    {
        for (auto& client : clients_)
        {
            client->Destroy();
        }
        server_.Destroy();
    }

    void SimulationDebugApp::DrawImGui()
    {
        server_.DrawImGui();
        for (auto& client : clients_)
        {
            client->DrawImGui();
        }
    }

    void SimulationDebugApp::Draw(sf::RenderTarget& renderTarget)
    {
        /*
        const auto& config = BasicEngine::GetInstance()->GetConfig();
        if (config.windowSize != windowSize_)
        {
            logDebug(fmt::format(
                "Resize Simulation Debug App Window framebuffers! with new resolution {}", config.windowSize.ToString()));
            for (auto& framebuffer : clientsFramebuffer_)
            {
                framebuffer.SetSize(config.windowSize / Vec2u(2, 1));
                framebuffer.Reload();
            }

            for (auto& client : clients_)
            {
                client->SetWindowSize(config.windowSize / Vec2u(2, 1));
            }
            windowSize_ = config.windowSize;
        }
        for (PlayerNumber playerNumber = 0; playerNumber < asteroid::maxPlayerNmb; playerNumber++)
        {
            clientsFramebuffer_[playerNumber].Bind();
            clientsFramebuffer_[playerNumber].Clear(Color::black);
            clients_[playerNumber]->Render();
        }

        gl::Framebuffer::Unbind();
        glViewport(0, 0, windowSize_.x, windowSize_.y);
        clientShader_.Bind();
        auto transform = Mat4f::Identity;
        transform = Transform3d::Scale(transform, Vec3f(0.5f, 1.0f, 1.0f));
        transform = Transform3d::Translate(transform, Vec3f(-0.5f, 0.0f, 0.0f));
        clientShader_.SetMat4("transform", transform);
        clientShader_.SetTexture("texture", clientsFramebuffer_[0].GetColorTexture());
        quad_.Draw();

        transform = Mat4f::Identity;
        transform = Transform3d::Scale(transform, Vec3f(0.5f, 1.0f, 1.0f));
        transform = Transform3d::Translate(transform, Vec3f(0.5f, 0.0f, 0.0f));
        clientShader_.SetMat4("transform", transform);
        clientShader_.SetTexture("texture", clientsFramebuffer_[1].GetColorTexture());
        quad_.Draw();
        */

    }
}
