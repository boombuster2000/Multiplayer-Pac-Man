#include "OverlayLayer.h"
#include "raylib.h"

OverlayLayer::OverlayLayer() = default;

void OverlayLayer::OnRender()
{
    ClearBackground(WHITE);
    DrawFPS(GetScreenWidth() - 100, 10);
}
