#pragma once

class Rect {
public:
    Rect(int w, int h) : _w{w}, _h{h} {}
    int get_w() const { return _w; }
    int get_h() const { return _h; }

private:
    int _w;
    int _h;
};
