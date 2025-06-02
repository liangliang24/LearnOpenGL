#include "pch.h"
#include "WindowsWindow.h"
#include "Event/ApplicationEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"

namespace LearnOpenGL
{
    WindowsWindow::WindowsWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	    m_Window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
        if (m_Window == NULL)
        {
            spdlog::error("Failed to create GLFW window");
            glfwTerminate();
        }
        spdlog::info("Create GLFW window successfully!");
        glfwMakeContextCurrent(m_Window);
    }

    WindowsWindow::~WindowsWindow()
    {
        glfwDestroyWindow(m_Window);
    }

    void WindowsWindow::Init()
    {
        glfwSetWindowUserPointer(m_Window, &m_Data);
        glfwGetCursorPos(m_Window, &m_Data.MouseX, &m_Data.MouseY);
        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                WindowData windowData = *(WindowData*)glfwGetWindowUserPointer(window);

                WindowResizeEvent event(width, height);
                windowData.EventCallback(event);
            });
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
            {
                WindowData windowData = *(WindowData*)glfwGetWindowUserPointer(window);

                MouseMoveEvent event(xPos, yPos, xPos - windowData.MouseX, yPos - windowData.MouseY);
                windowData.MouseX = xPos;
                windowData.MouseY = yPos;
                windowData.EventCallback(event);
            });
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
                default:
                    break;
                }
            });
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                MouseScrolledEvent event((float)xOffset, (float)yOffset);
                data.EventCallback(event);
            });
        glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t keycode)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                KeyTypedEvent event(keycode);
                data.EventCallback(event);
            });
        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                }
                    break;
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                }
                    break;
                }
            });
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                WindowCloseEvent event;
                data.EventCallback(event);
            });
    }
    void WindowsWindow::OnUpdate()
    {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(m_Window);
        glfwGetCursorPos(m_Window, &data.MouseX, &data.MouseY);

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}


