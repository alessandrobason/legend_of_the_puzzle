#pragma once

#include "Graphics.hpp"

#include <assert.h>

namespace rl {
    #include <rlgl.h>
} // namespace rl

using namespace sf;

const Color Color::Red         = rl::RED;
const Color Color::Green       = rl::GREEN;
const Color Color::Blue        = rl::BLUE;
const Color Color::Yellow      = rl::YELLOW;
const Color Color::Magenta     = rl::MAGENTA;
const Color Color::Black       = rl::BLACK;
const Color Color::White       = rl::WHITE;
const Color Color::Transparent = rl::BLANK;

const Time Time::Zero = 0.f;

void View::setSize(vec2 new_size) {
    m_size = new_size;
    vec2 screen_size = vec2i(rl::GetScreenWidth(), rl::GetScreenHeight());
    
    vec2 rel = screen_size / m_size;
    assert(fabsf(rel.x - rel.y) < 0.1f);
    m_camera.zoom = rel.x;
    // m_size : ss = 1.f : zoom
    // m_camera.zoom
}

void View::setCenter(vec2 new_center) {
    // m_camera.offset = m_camera.target = new_center;
    m_camera.zoom = 1.f;
    m_camera.target = new_center;
    m_camera.offset = (vec2)m_camera.target * m_camera.zoom;
}

void View::setViewport(rectf new_viewport) {
    m_viewport = new_viewport;
}

void View::begin() {
    rl::BeginMode2D(m_camera);
}

void View::end() {
    rl::EndMode2D();

    vec2i p = (vec2)m_camera.target - m_size / 2.f;
    vec2i s = m_size;
    rl::DrawRectangleLines(p.x, p.y, s.x, s.y, rl::RED);
}

bool Shader::loadFromFile(const std::string &filename, Type type) {
    // todo
    return true;
}

bool Image::loadFromFile(const std::string &filename) {
    m_img = rl::LoadImage(filename.c_str());
    return m_img.data != nullptr;
}

bool Texture::loadFromFile(const std::string &filename) {
    m_tex = rl::LoadTexture(filename.c_str());
    return m_tex.id != 0;
}

bool Texture::loadFromImage(const Image &img) {
    m_tex = rl::LoadTextureFromImage(img.m_img);
    return m_tex.id != 0;
}

void Texture::takeScreenshot() {
    loadFromImage(rl::LoadImageFromScreen());
}

void Sprite::setTexture(Texture new_texture) {
    if (m_tex.m_tex.id == 0 && m_tex_rect == rectf(0, 0, 0, 0)) {
        m_tex_rect.size = new_texture.getSize();
    }
    
    m_tex = new_texture;
}

Transform Sprite::getTransform() const {
    Matrix tran, rot;
    tran.translate(m_pos);
    rot.rotate(m_angle);

    float a  = -(m_angle * deg2rad);
    float c  = cosf(a);
    float s  = sinf(a);
    float tx = -m_origin.x * c - m_origin.y * s + m_pos.x;
    float ty =  m_origin.x * s - m_origin.y * c + m_pos.y;

    // clang-format off
    return Matrix({c,  s,  tx, 0,
                  -s,  c,  ty, 0,
                   0,  0,  1,  0,
                   0,  0,  0,  1});
}

Time::Time() {
    m_sec = (float)rl::GetTime();
}

Time Clock::restart() {
    Time t = m_start;
    m_start = Time();
    return t;
}

Time Clock::getElapsedTime() {
    return m_start;
}

bool SoundBuffer::loadFromFile(const std::string &filename) {
    // todo
    return true;
}

void Sound::play() {
    // todo
}

void Sound::pause() {
    // todo
}
        
bool Music::openFromFile(const std::string &filename) {
    // todo
    return true;
}

void Music::play() {
    // todo
}

void Music::pause() {
    // todo
}

void Music::setPlayingOffset(Time t) {
    // todo
}

bool Font::loadFromFile(const std::string &filename) {
    m_font = rl::LoadFont(filename.c_str());
    return m_font.texture.id != 0;
}

void RenderTarget::draw(const Text &text) {
    int font_size = (int)text.m_chsize;
    int spacing = font_size/10;

    vec2 off = rl::MeasureTextEx(text.m_font, text.m_str.c_str(), (float)font_size, (float)spacing);
    vec2 pos = text.m_pos;
    pos.x -= off.x / 2.f;
    pos.y -= off.y / 4.f;
    
    rl::DrawTextEx(
        text.m_font,
        text.m_str.c_str(),
        pos,
        (float)text.m_chsize,
        (float)spacing,
        text.m_fill
    );
}

void RenderTarget::draw(const Sprite &spr) {
    rl::DrawTexturePro(
        spr.m_tex,
        spr.m_tex_rect, 
        rectf{ spr.m_pos, spr.m_tex.getSize() },
        spr.m_origin,
        spr.m_angle,
        spr.m_col
    );
}

void RenderTarget::draw(const RectangleShape &r) {
    rectf box = { r.m_pos, r.m_size };
    
    if (r.m_fill != Color::Transparent) {
        rl::DrawRectangleRec(box, r.m_fill);
    }
    rl::DrawRectangleLinesEx(box, r.m_thick, r.m_outline);
}

void RenderTarget::draw(const Drawable &drawable) {
    drawable.draw(*this, {});
}

void RenderTarget::draw(const Drawable &drawable, const RenderStates &states) {
    drawable.draw(*this, states);
}

void RenderTarget::draw(const VertexArray &va, const RenderStates &states) {
    draw(va.m_vertices.data(), va.m_vertices.size(), PrimitiveType::Quads, states);
}

void RenderTarget::draw(const Vertex *vertices, size_t vcount, PrimitiveType type, const RenderStates &states) {
    vec2 tex_size = vec2(1, 1);
    
    if (states.shader) rl::BeginShaderMode(states.shader->m_shd);
    if (states.texture) {
        rl::rlSetTexture(states.texture->m_tex.id);
        tex_size = states.texture->getSize();
    }
    
    rl::rlBegin(RL_QUADS);
        rl::rlNormal3f(0.f, 0.f, 1.f);

        for (size_t i = 0; i < vcount; ++i) {
            const Vertex &v = vertices[i];
            vec2 p = states.transform.transformPoint(v.position);
            vec2 tc = v.texCoords / tex_size;

            rl::rlColor4ub(v.color.r, v.color.g, v.color.b, v.color.a);
            rl::rlTexCoord2f(tc.x, tc.y);
            rl::rlVertex2f(p.x, p.y);
        }

    rl::rlEnd();

    rl::rlSetTexture(0);
    if (states.shader) rl::EndShaderMode();
}

void RenderTarget::clear(Color c) {
    rl::BeginDrawing();
    rl::ClearBackground(c.m_color);
}

void RenderTarget::display() {
    rl::EndDrawing();
}

RenderWindow::RenderWindow(vec2 size, const std::string &title) {
    rl::InitWindow((int)size.x, (int)size.y, title.c_str());
    rl::SetTargetFPS(60);
    // not ideal but there's no way i'm fixing this lol
    rl::rlDisableBackfaceCulling();
}

bool RenderWindow::isOpen() const {
    return !rl::WindowShouldClose();
}

void RenderWindow::close() {
    rl::CloseWindow();
}

void RenderWindow::setFullscreen(bool f) {
    if (rl::IsWindowFullscreen() != f) {
        rl::ToggleFullscreen();
    }
}

vec2 RenderWindow::mapPixelToCoords(const vec2i &point, const View &view) const {
    return rl::GetScreenToWorld2D((vec2)point, view.getCamera());
}

void RenderWindow::setMouseCursorVisible(bool visible) {
    visible ? rl::ShowCursor() : rl::HideCursor();
}

vec2 RenderWindow::getSize() const {
    return vec2i(rl::GetScreenWidth(), rl::GetScreenHeight());
}

static int sf_to_rl_key(sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::W:      return rl::KEY_W;
        case sf::Keyboard::A:      return rl::KEY_A;
        case sf::Keyboard::S:      return rl::KEY_S;
        case sf::Keyboard::D:      return rl::KEY_D;
        case sf::Keyboard::E:      return rl::KEY_E;
        case sf::Keyboard::F1:     return rl::KEY_F1;
        case sf::Keyboard::Escape: return rl::KEY_ESCAPE;
    }
    return rl::KEY_NULL;
}

static int sf_to_rl_mouse(sf::Mouse::Button btn) {
    switch (btn) {
    case sf::Mouse::Left:   return rl::MOUSE_BUTTON_LEFT;
    case sf::Mouse::Right:  return rl::MOUSE_BUTTON_RIGHT;
    case sf::Mouse::Middle: return rl::MOUSE_BUTTON_MIDDLE;
    }
    return -1;
}

bool Keyboard::isKeyDown(Key key) {
    return rl::IsKeyDown(sf_to_rl_key(key));
}

bool Keyboard::isKeyPressed(Key key) {
    return rl::IsKeyPressed(sf_to_rl_key(key));
}

bool Mouse::isButtonDown(Button button) {
    int btn = sf_to_rl_mouse(button);
    return btn < 0 ? false : rl::IsMouseButtonDown(btn);
}

bool Mouse::isButtonPressed(Button button) {
    int btn = sf_to_rl_mouse(button);
    return btn < 0 ? false : rl::IsMouseButtonPressed(btn);
}

vec2i Mouse::getPosition() {
    return rl::GetMousePosition();
}

vec2i Mouse::getPosition(const RenderWindow &relative_to) {
    return relative_to.mapPixelToCoords(rl::GetMousePosition());
}
