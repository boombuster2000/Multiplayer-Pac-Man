#include "engine/ui/enums.h"
#include "engine/ui/text_box.h"
#include "raylib.h"
#include <unordered_map>

namespace ui
{
void TextBox::HandleKeyPresses()
{
    const std::unordered_map<KeyboardKey, char> keyToLetters = {
        {KEY_A, 'a'}, {KEY_B, 'b'}, {KEY_C, 'c'}, {KEY_D, 'd'}, {KEY_E, 'e'}, {KEY_F, 'f'}, {KEY_G, 'g'},
        {KEY_H, 'h'}, {KEY_I, 'i'}, {KEY_J, 'j'}, {KEY_K, 'k'}, {KEY_L, 'l'}, {KEY_M, 'm'}, {KEY_N, 'n'},
        {KEY_O, 'o'}, {KEY_P, 'p'}, {KEY_Q, 'q'}, {KEY_R, 'r'}, {KEY_S, 's'}, {KEY_T, 't'}, {KEY_U, 'u'},
        {KEY_V, 'v'}, {KEY_W, 'w'}, {KEY_X, 'x'}, {KEY_Y, 'y'}, {KEY_Z, 'z'}};

    const std::unordered_map<KeyboardKey, char> keyToNumbers = {
        {KEY_ZERO, '0'}, {KEY_ONE, '1'}, {KEY_TWO, '2'},   {KEY_THREE, '3'}, {KEY_FOUR, '4'},
        {KEY_FIVE, '5'}, {KEY_SIX, '6'}, {KEY_SEVEN, '7'}, {KEY_EIGHT, '8'}, {KEY_NINE, '9'}};

    bool shiftDown = IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);

    for (const auto& [key, letter] : keyToLetters)
    {
        if (IsKeyPressed(key))
        {
            m_text.SetText(m_text.GetText() + (shiftDown ? static_cast<char>(toupper(letter)) : letter));
        }
    }

    for (const auto& [key, number] : keyToNumbers)
    {
        if (IsKeyPressed(key))
        {
            m_text.SetText(m_text.GetText() + number);
        }
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !m_text.GetText().empty())
    {
        std::string currentText = m_text.GetText();
        currentText.pop_back();
        m_text.SetText(currentText);
    }
}

void TextBox::UpdateCursor()
{
    m_cursorTimer += GetFrameTime();
    if (m_cursorTimer >= 0.5f)
    {
        m_cursorTimer = 0.0f;
        m_cursorVisible = !m_cursorVisible;
    }
}

Vector2Ex<float> TextBox::CalculateTextPositionOffset(const Vector2Ex<float>& boxDimensions,
                                                      const Vector2Ex<float>& textDimensions) const
{
    float x = 0.0f;
    switch (m_textAlignment)
    {
    case Alignment::LEFT:
        x = m_padding;
        break;
    case Alignment::RIGHT:
        x = boxDimensions.x - textDimensions.x - m_padding;
        break;
    case Alignment::CENTER:
        x = (boxDimensions.x - textDimensions.x) / 2;
        break;
    default:
        break;
    }

    float y = (boxDimensions.y - textDimensions.y) / 2;
    return {x, y};
};

void TextBox::UpdateTextPosition()
{
    const Vector2Ex<float>& textBoxTopLeft = GetPositionAtAnchor(AnchorPoint::TOP_LEFT);
    const Vector2Ex<float>& textDimensions = m_text.GetDimensions();
    const Vector2Ex<float>& newTextPos = textBoxTopLeft + CalculateTextPositionOffset(m_dimensions, textDimensions);
    m_text.SetPosition(newTextPos);
}

TextBox::TextBox(Vector2Ex<float> position, Vector2Ex<float> dimensions, TextStyle typedTextStyle,
                 Alignment textAlignment, AnchorPoint anchorPoint, float padding, TextStyle backgroundTextStyle,
                 std::string backgroundText, bool visible)
    : RenderableObject(position, anchorPoint, visible), m_dimensions(dimensions), m_typedTextStyle(typedTextStyle),
      m_text("", typedTextStyle, position, AnchorPoint::TOP_LEFT, visible), m_textAlignment(textAlignment),
      m_padding(padding), m_backgroundTextStyle(backgroundTextStyle),
      m_backgroundText(backgroundText, backgroundTextStyle, position, AnchorPoint::TOP_LEFT, visible),
      m_cursorVisible(true), m_cursorTimer(0.0f)
{
    SetOrigin(anchorPoint);

    const Vector2Ex<float>& textBoxTopLeft = GetPositionAtAnchor(AnchorPoint::TOP_LEFT);
    const Vector2Ex<float>& backgroundTextSize = m_backgroundText.GetDimensions();
    const Vector2Ex<float>& backgroundTextPos =
        textBoxTopLeft + CalculateTextPositionOffset(m_dimensions, backgroundTextSize);
    m_backgroundText.SetPosition(backgroundTextPos);
}

void TextBox::Update()
{
    HandleKeyPresses();
    UpdateCursor();
    UpdateTextPosition();
}

Vector2Ex<float> TextBox::GetDimensions() const
{
    return m_dimensions;
}

void TextBox::Render(Vector2Ex<float> offset) const
{
    const Vector2Ex<float>& renderPos = GetPositionAtAnchor(AnchorPoint::TOP_LEFT) + offset;
    DrawRectangleV(renderPos, m_dimensions, LIGHTGRAY);
    DrawRectangleLinesEx({renderPos.x, renderPos.y, m_dimensions.x, m_dimensions.y}, 1, BLACK);

    if (m_text.GetText().empty())
    {
        m_backgroundText.Render(offset);
    }
    else
    {
        m_text.Render(offset);
        if (m_cursorVisible)
        {
            const Vector2Ex<float> textPos = m_text.GetPositionAtAnchor(AnchorPoint::TOP_RIGHT);
            Vector2Ex<float> cursorPosition = {textPos.x + 2, textPos.y};
            DrawCursor(cursorPosition + offset, m_text.GetFontSize(), BLACK);
        }
    }
}

void TextBox::DrawCursor(Vector2Ex<float> position, float fontSize, Color color) const
{
    DrawRectangleV(position, {2, fontSize}, color);
}
} // namespace ui
