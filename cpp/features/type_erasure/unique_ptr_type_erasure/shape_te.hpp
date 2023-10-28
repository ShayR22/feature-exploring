#pragma once

#include <memory>
#include <span>
#include <cstdlib>

namespace up_shape_te {

template<typename T>
std::span<const uint8_t> default_stack_serializtion(const T& element) {
    const uint8_t* element_buffer = reinterpret_cast<const uint8_t*>(std::addressof(element));
    size_t element_size = sizeof(element);
    return std::span<const uint8_t>(element_buffer, element_size);
}

class Shape {
public:
    template<typename ShapeT, typename DrawStrategy, typename SerializeStrategy>
    Shape(ShapeT&& shape, DrawStrategy&& drawer, SerializeStrategy&& serializer) :
        _pimpl{std::make_unique<ShapeModel<ShapeT, DrawStrategy, SerializeStrategy>>(std::move(shape),
                                                                                     std::move(drawer),
                                                                                     std::move(serializer))} {
        // DO NOTHING
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
        _pimpl->draw();
    }

    std::span<const uint8_t> serialize() const {
        return _pimpl->serialize();
    }

private:
    struct ShapeConcept {
        virtual ~ShapeConcept() = default;
        virtual void draw() const = 0;
        virtual std::span<const uint8_t> serialize() const = 0;
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

        ShapeT _shape;
        DrawStrategy _drawer;
        SerializeStrategy _serializer;
    };

    std::unique_ptr<ShapeConcept> _pimpl{nullptr};
};

} // end namespace simple_up_shape_te