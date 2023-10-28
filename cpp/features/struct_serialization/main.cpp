#include <span>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

struct Serialize {
    std::span<const uint8_t> serialize() const {
        return {};
    }
    void deserialize(const std::span<const uint8_t>& data) {

    }
};

struct Data1 : public Serialize {
    struct Data {
        int x;
        int y;
    };
    Data _data;

    Data1() : _data{} {}
    Data1(int x, int y) : _data{x, y} {
        // DO NOTHING
    }

    std::span<const uint8_t> serialize() const {
        printf("_data size=[%zu]\n", sizeof(_data));
        return std::span<const uint8_t, sizeof(&_data)>(reinterpret_cast<const uint8_t*>(&_data), sizeof(_data));
    }

    void deserialize(const std::span<const uint8_t>& data) {
        memcpy(&_data, data.data(), data.size());
    }

    void print() {
        printf("x=%d y=%d\n", _data.x, _data.y);
    }
};

struct Data2 : public Serialize {
    std::vector<int> _num;
};

struct Data3 {
    int x;
    int y;
    int z;
};

struct Data4: public Serialize {
    int x;
    int y;
    int z;
    int w;
};

template <typename T>
void serialize(const T& t) {
    if constexpr (std::is_trivially_copyable<T>::value) {
        printf("stack allocation\n");
    } else {
        printf("not stack allocation\n");
    }
}


int main() {
    Data1 d1{1, 2};
    std::span<const uint8_t> buf = d1.serialize();
    printf("buf_size = %zu\n", buf.size());
    Data1 d2;
    d2.deserialize(buf);

    d1.print();
    d2.print();

    Data3 d3{};
    Data4 d4{};

    serialize(d1);
    serialize(d2);
    serialize(d3);
    serialize(d4);
    return 0;
}