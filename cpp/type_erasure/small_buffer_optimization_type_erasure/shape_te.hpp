#pragma once

#include <memory>
#include <span>
#include <cstdlib>
#include <array>
#include <cstddef>

namespace sbo_shape_te {

template<typename T>
std::span<const uint8_t> default_stack_serializtion(const T& element) {
    const uint8_t* element_buffer = reinterpret_cast<const uint8_t*>(std::addressof(element));
    size_t element_size = sizeof(element);
    return std::span<const uint8_t>(element_buffer, element_size);
}

template<size_t buffersize = 128U, size_t alignment = 16U>
class Shape {
public:
    template<typename ShapeT, typename DrawStrategy, typename SerializeStrategy>
    Shape(ShapeT&& shape, DrawStrategy&& drawer, SerializeStrategy&& serializer) {
        // ModelT (Model Templated) for more code readability
        using ModelT = ShapeModel<ShapeT, DrawStrategy, SerializeStrategy>;
        // Assert the templated new model is not too big or has to big of alignment
        static_assert(sizeof(ModelT) <= buffersize);
        static_assert(alignof(ModelT) <= alignment);
        ::new(_pimpl()) ModelT(std::move(shape), std::move(drawer), std::move(serializer));
    }

    Shape(const Shape& other) {
        // clone the other by giving pointer to myself to populate
        other._pimpl()->clone(_pimpl());
    }

    Shape& operator=(const Shape& other) {
        // create a clone of the other, swap the buffers (the pimpl) and return urself
        Shape tmp(other);
        _buffer.swap(other._buffer);
        return *this;
    }

    Shape(Shape&& other) {
        other._pimpl()->move(_pimpl());
    }

    Shape& operator=(Shape&& other) {
        Shape tmp(std::move(other));
        _buffer.swap(tmp._buffer);
        return *this;
    }

    ~Shape() {
        std::destroy_at(_pimpl());
    }

    template<typename ShapeT, typename DrawStrategy>
    Shape(ShapeT&& shape, DrawStrategy&& drawer) :
        Shape(std::move(shape),
              std::move(drawer),
              [](const ShapeT& shape) {
                return default_stack_serializtion(shape);
              }) {
        // DO NOTHING
    }

    void draw() const {
        _pimpl()->draw();
    }

    std::span<const uint8_t> serialize() const {
        return _pimpl()->serialize();
    }

private:
    struct ShapeConcept {
        virtual ~ShapeConcept() = default;
        virtual void draw() const = 0;
        virtual std::span<const uint8_t> serialize() const = 0;
        virtual void clone(ShapeConcept* other) const = 0;
        virtual void move(ShapeConcept* other) = 0;
    };

    template<typename ShapeT, typename DrawStrategy, typename SerializeStrategy>
    struct ShapeModel : public ShapeConcept {
        ShapeModel(ShapeT&& shape, DrawStrategy&& drawer, SerializeStrategy&& serializer) :
            _shape{std::move(shape)},
            _drawer{std::move(drawer)},
            _serializer{std::move(serializer)} {
            // DO NOTHING
        }

        void draw() const override {
            _drawer(_shape);
        }

        std::span<const uint8_t> serialize() const override {
            return _serializer(_shape);
        }

        void clone(ShapeConcept* other) const override {
            std::construct_at(static_cast<ShapeModel*>(other), *this);
        }

        void move(ShapeConcept* other) override {
            std::construct_at(static_cast<ShapeModel*>(other), std::move(*this));
        }

        ShapeT _shape;
        DrawStrategy _drawer;
        SerializeStrategy _serializer;
    };

    const ShapeConcept* _pimpl() const {
        return reinterpret_cast<const ShapeConcept*>(_buffer.data());
    }

    ShapeConcept* _pimpl() {
        return reinterpret_cast<ShapeConcept*>(_buffer.data());
    }


    // create an aligned buffer with fixed size
    alignas(alignment) std::array<std::byte, buffersize> _buffer;
};

} // end namespace sbo_shape_te