#pragma once

#include <string>
#include <vector>

#include <stdint.h>

#include "priv/rl.h"
#include "priv/maths.h"

#define GETSET(v, type, name) \
    void set##name(type new_##name) { v = new_##name; } \
    type get##name() const { return v; }

#define CGETSET(v, type, name) GETSET(v, const type &, name)

namespace sf {
    using int8  = int8_t;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;
    
    using Uint8  = uint8_t;
    using Uint16 = uint16_t;
    using Uint32 = uint32_t;
    using Uint64 = uint64_t;

    using uchar = unsigned char;
    using uint = unsigned int;
    using FloatRect = rectf;

    using Transform = Matrix;

    constexpr double pi = 3.14159265358979323846;
    constexpr float deg2rad = (float)(pi/180.0);

    enum PrimitiveType { Quads };

    struct View {
        View() { setZero(); }
        View(const rectf &r) { setZero(); m_camera.target = r.centre(); m_size = r.size; }
        View(vec2 c, vec2 s) { setZero(); m_camera.target = c; m_size = s; }

        void setZero() { memset(this, 0, sizeof(*this)); }

        void setSize(vec2 new_size);
        vec2 getSize() const { return m_size; }

        void setCenter(vec2 new_center);
        vec2 getCenter() const { return m_camera.target; }

        void setViewport(rectf new_viewport);
        rectf getViewport() const { return m_viewport; }

        GETSET(m_camera, rl::Camera2D, Camera);

        void begin();
        void end();
        
        vec2 m_size;
        rectf m_viewport;
        rl::Camera2D m_camera;
    };

    struct Color {
        Color() = default;
        Color(rl::Color c) : m_color(c) {}
        Color(uchar r, uchar g, uchar b, uchar a = 255)
            : m_color({r, g, b, a}) {}

        bool operator==(Color c) const { return m_col32 == c.m_col32; }
        bool operator!=(Color c) const { return m_col32 != c.m_col32; }

        operator rl::Color() const { return m_color; }

        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Yellow;
        static const Color Magenta;
        static const Color Black;
        static const Color White;
        static const Color Transparent;

        union {
            rl::Color m_color = rl::WHITE;
            struct { uchar r, g, b, a; };
            uint32_t m_col32;
        };
    };

    struct RectangleShape {
        RectangleShape() = default;
        RectangleShape(vec2 size) : m_size(size) {}
        RectangleShape(vec2 pos, vec2 size) : m_pos(pos), m_size(size) {}
        RectangleShape(rectf rec) : m_pos(rec.pos), m_size(rec.size) {}

        GETSET(m_pos, vec2, Position);
        GETSET(m_size, vec2, Size);
        GETSET(m_outline, Color, OutlineColor);
        GETSET(m_fill, Color, FillColor);
        GETSET(m_thick, float, OutlineThickness);

        vec2 m_pos;
        vec2 m_size;
        Color m_outline = Color::Black;
        Color m_fill = Color::Transparent;
        float m_thick = 1.f;
    };

    struct Shader {
        Shader() { memset(this, 0, sizeof(*this)); }
        enum Type { Vertex, Fragment };
        bool loadFromFile(const std::string &filename, Type type);

        operator rl::Shader() const { return m_shd; }

        rl::Shader m_shd;
    };

    struct Image {
        Image() { memset(this, 0, sizeof(*this)); }
        Image(const rl::Image &img) : m_img(img) {}

        bool loadFromFile(const std::string &filename);

        vec2u getSize() const { return { (uint)m_img.width, (uint)m_img.height }; }

        rl::Image m_img;
    };

    struct Texture {
        Texture() { memset(this, 0, sizeof(*this)); }
        Texture(const rl::Texture &t) : m_tex(t) {}

        bool loadFromFile(const std::string &filename);
        bool loadFromImage(const Image &img);
        void takeScreenshot();

        vec2i getSize() const { return { m_tex.width, m_tex.height }; }
        void setSmooth(bool smooth) { (void)smooth; }

        operator bool() const { return m_tex.id != 0; }
        operator rl::Texture() const { return m_tex; }

        rl::Texture m_tex;
    };

    struct Sprite {
        void setTexture(Texture new_texture);
        Texture getTexture() const { return m_tex; }

        GETSET(m_col, Color, Color);
        GETSET(m_tex_rect, recti, TextureRect);
        GETSET(m_pos, vec2, Position);
        GETSET(m_origin, vec2, Origin);
        GETSET(m_angle, float, Rotation);

        void setOrigin(float x, float y) { m_origin = { x, y }; }
        void move(vec2 m) { m_pos += m; }
        Transform getTransform() const;

        vec2 m_pos;
        vec2 m_origin;
        float m_angle = 0.f;
        Texture m_tex;
        recti m_tex_rect;
        Color m_col;
    };

    struct RenderStates {
        Texture *texture = nullptr;
        Transform transform;
        Shader *shader = nullptr;
    };

    struct Drawable {
        virtual void draw(struct RenderTarget &target, RenderStates states) const = 0;
    };

    struct Transformable {
        GETSET(m_trans, Transform, Transform);

        Transform m_trans;
    };

    struct Time {
        Time();
        Time(float t) : m_sec(t) {}

        static const Time Zero;

        float asSeconds() const { return m_sec; }
        float m_sec = 0.f;
    };

    struct Clock {
        Clock() = default;
        Time restart();
        Time getElapsedTime();

        Time m_start;
    };

    struct SoundBuffer {
        bool loadFromFile(const std::string &filename);
    };

    struct Sound {
        void play();
        void pause();

        GETSET(m_volume, float, Volume);
        CGETSET(m_buf, SoundBuffer, Buffer);

        float m_volume = 1.f;
        SoundBuffer m_buf;
    };

    struct Music {
        bool openFromFile(const std::string &filename);
        void play();
        void pause();
        void setPlayingOffset(Time t);

        GETSET(m_volume, float, Volume);
        GETSET(m_looping, bool, Loop);

        float m_volume = 1.f;
        bool m_looping = true;
    };

    struct Vertex {
        vec2 position;
        Color color;
        vec2 texCoords;
    };

    struct VertexArray {
        void append(const Vertex &v) { m_vertices.emplace_back(v); }

        Vertex &operator[](size_t i) { return m_vertices[i]; }
        const Vertex &operator[](size_t i) const { return m_vertices[i]; }

        std::vector<Vertex> m_vertices;
    };

    struct Font {
        Font() { memset(this, 0, sizeof(*this)); }

        bool loadFromFile(const std::string &filename);

        GETSET(m_font.texture, Texture, Texture);

        operator rl::Font() const { return m_font; }

        rl::Font m_font;
    };

    struct Text {
        CGETSET(m_font, Font, Font);
        CGETSET(m_str, std::string, String);
        GETSET(m_chsize, uint, CharacterSize);
        GETSET(m_pos, vec2, Position);
        GETSET(m_fill, Color, FillColor);
        GETSET(m_outline, Color, OutlineColor);
        GETSET(m_outthick, float, OutlineThickness);
        GETSET(m_line_spacing, float, LineSpacing);

        // rectf getLocalBounds() const;
        void move(vec2 p) { m_pos += p; }

        Font m_font;
        std::string m_str;
        uint m_chsize;
        vec2 m_pos;
        Color m_fill;
        Color m_outline;
        float m_outthick = 1.f;
        float m_line_spacing = 1.f;
    };

    struct RenderTarget {
        void draw(const Text &text);
        void draw(const Sprite &spr);
        void draw(const RectangleShape &r);
        void draw(const Drawable &drawable);
        void draw(const Drawable &drawable, const RenderStates &states);
        void draw(const VertexArray &va, const RenderStates &states);
        void draw(const Vertex *vertices, size_t vcount, PrimitiveType type, const RenderStates &states = {});

        void clear(Color c = Color::Black);
        void display();

        CGETSET(m_view, View, View);

        View m_view;
    };

    struct RenderWindow : public RenderTarget {
        RenderWindow() = default;
        RenderWindow(vec2 size, const std::string &title);

        bool isOpen() const;
        void close();
        void setFullscreen(bool f);

        vec2 mapPixelToCoords(const vec2i &point) const { return mapPixelToCoords(point, m_view); }
        vec2 mapPixelToCoords(const vec2i &point, const View &view) const;

        void setMouseCursorVisible(bool visible);
        vec2 getSize() const;
    };

    struct Keyboard {
        enum Key { None, W, A, S, D, E, F1, Escape, KeyCount };

        static bool isKeyDown(Key key);
        static bool isKeyPressed(Key key);
    };

    struct Mouse {
        enum Button {
            Left, Right, Middle, ButtonCount
        };

        static bool isButtonPressed(Button button);
        static bool isButtonDown(Button button);
        static vec2i getPosition();
        static vec2i getPosition(const RenderWindow &relative_to);
        // static void setPosition(vec2i position);
    };
} // namespace sf