#pragma once

extern Framework::Application* Framework::CreateApplication();

int main(int argc, char** argv)
{
    Framework::Log::init();

    auto app = Framework::CreateApplication();

    app->Run();

    delete app;
}