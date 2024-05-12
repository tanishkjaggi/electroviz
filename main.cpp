#include <iostream>
#include <Windows.h>
#include <GL/gl.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>

const int WINDOW_WIDTH = 800;   // Width of the window
const int WINDOW_HEIGHT = 600;  // Height of the window
const int NUM_ELECTRONS = 5;    // Number of electrons
const int ELECTRON_RADIUS = 10; // Radius of electron representation
const int LOG_FREQUENCY = 10;   // Log data every 10 frames

// Struct to represent an electron
struct Electron
{
    float x, y;   // Position
    float vx, vy; // Velocity
    char label;   // Label of the electron
};

std::vector<Electron> electrons(NUM_ELECTRONS); // Vector to store electrons
std::ofstream logfile("electron_data.txt");     // File stream for logging electron data

// Function to initialize electrons with random positions, velocities, and labels
void initElectrons()
{
    srand(static_cast<unsigned int>(time(nullptr))); // Seed the random number generator
    char label = 'A';                                // Start label from 'A'
    for (int i = 0; i < NUM_ELECTRONS; ++i)
    {
        // Random position within the window
        electrons[i].x = static_cast<float>(rand()) / RAND_MAX * WINDOW_WIDTH;
        electrons[i].y = static_cast<float>(rand()) / RAND_MAX * WINDOW_HEIGHT;
        // Random velocity between -1 and 1 in both x and y directions
        electrons[i].vx = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f;
        electrons[i].vy = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 2.0f;
        electrons[i].label = label++; // Assign label
    }
}

// Function to update electron positions and log data
void updateElectrons()
{
    for (int i = 0; i < NUM_ELECTRONS; ++i)
    {
        electrons[i].x += electrons[i].vx; // Update x position
        electrons[i].y += electrons[i].vy; // Update y position

        // Reflect from edges if out of bounds
        if (electrons[i].x < 0 || electrons[i].x >= WINDOW_WIDTH)
        {
            electrons[i].vx *= -1; // Invert x velocity
        }
        if (electrons[i].y < 0 || electrons[i].y >= WINDOW_HEIGHT)
        {
            electrons[i].vy *= -1; // Invert y velocity
        }

        // Log data periodically
        if (GetTickCount() % LOG_FREQUENCY == 0)
        {
            logfile << electrons[i].x << " " << electrons[i].y << " " << electrons[i].vx << " " << electrons[i].vy << std::endl;
        }
    }
}

// Function to draw electrons on the window
void drawElectrons()
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer
    glBegin(GL_POINTS);           // Start drawing points
    for (int i = 0; i < NUM_ELECTRONS; ++i)
    {
        glColor3ub(0, 0, 255);                      // Set color to blue
        glVertex2f(electrons[i].x, electrons[i].y); // Draw electron as a point
    }
    glEnd(); // End drawing points

    // Draw circles around electrons
    glBegin(GL_LINE_LOOP); // Start drawing lines
    for (int i = 0; i < NUM_ELECTRONS; ++i)
    {
        glColor3ub(0, 0, 255);        // Set color to blue
        for (int j = 0; j < 360; ++j) // Draw circle with 360 points
        {
            float angle = j * 3.1415926535 / 180;                         // Convert degrees to radians
            float x = electrons[i].x + ELECTRON_RADIUS * std::cos(angle); // Calculate x coordinate of circle point
            float y = electrons[i].y + ELECTRON_RADIUS * std::sin(angle); // Calculate y coordinate of circle point
            glVertex2f(x, y);                                             // Draw circle point
        }
    }
    glEnd(); // End drawing lines

    // Draw labels for electrons
    HDC hdc = wglGetCurrentDC();       // Get device context
    SetTextColor(hdc, RGB(0, 0, 255)); // Set text color to blue
    SetBkMode(hdc, TRANSPARENT);       // Set background mode to transparent
    for (int i = 0; i < NUM_ELECTRONS; ++i)
    {
        TextOut(hdc, electrons[i].x - 5, electrons[i].y - 5, &electrons[i].label, 1); // Draw label
    }
    glFlush(); // Flush OpenGL commands
}

// Window message handler
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0); // Post quit message
        return 0;
    case WM_PAINT:
        drawElectrons(); // Draw electrons on window paint message
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam); // Default window message processing
}

// Main entry point of the application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const char *CLASS_NAME = "Electron Window Class"; // Window class name

    // Define window class
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;   // Set window procedure
    wc.hInstance = hInstance;      // Set instance handle
    wc.lpszClassName = CLASS_NAME; // Set class name

    RegisterClass(&wc); // Register window class

    // Create window
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Electron Simulation",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        return 0; // Return if window creation failed
    }

    // Initialize OpenGL
    HDC hdc = GetDC(hwnd);                                                    // Get device context
    PIXELFORMATDESCRIPTOR pfd = {0};                                          // Pixel format descriptor
    pfd.nSize = sizeof(pfd);                                                  // Size of pixel format descriptor
    pfd.nVersion = 1;                                                         // Version number
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; // Drawing capabilities
    pfd.iPixelType = PFD_TYPE_RGBA;                                           // RGBA pixel type
    pfd.cColorBits = 32;                                                      // Color depth
    pfd.cDepthBits = 24;                                                      // Depth buffer size
    pfd.iLayerType = PFD_MAIN_PLANE;                                          // Main drawing layer

    int pixelFormat = ChoosePixelFormat(hdc, &pfd); // Choose pixel format
    SetPixelFormat(hdc, pixelFormat, &pfd);         // Set pixel format

    HGLRC hglrc = wglCreateContext(hdc); // Create OpenGL rendering context
    wglMakeCurrent(hdc, hglrc);          // Make OpenGL rendering context current

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set clear color

    // Initialize electrons
    initElectrons();

    ShowWindow(hwnd, nCmdShow); // Show window

    // Main message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg); // Translate virtual-key messages into character messages
        DispatchMessage(&msg);  // Dispatch message to window procedure

        // Update electrons and redraw window
        updateElectrons();
        drawElectrons();
        SwapBuffers(hdc); // Swap front and back buffers
    }

    wglMakeCurrent(NULL, NULL); // Unbind OpenGL rendering context
    wglDeleteContext(hglrc);    // Delete OpenGL rendering context
    ReleaseDC(hwnd, hdc);       // Release device context

    logfile.close(); // Close the log file
    return 0;        // Return success
}
