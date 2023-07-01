#pragma once

class Rect {
public:
    Rect() : _w{0}, _h{0} {}
    Rect(int w, int h) : _w{w}, _h{h} {}
    Rect(const Rect& lhs) = default;
    Rect(Rect&& rhs) = default;
    Rect& operator=(const Rect& lhs) = default;
    Rect& operator=(Rect&& rhs) = default;
    int get_w() const { return _w; }
    int get_h() const { return _h; }

private:
    int _w;
    int _h;
};
