#pragma once

#include <memory>
#include <span>
#include <cstdlib>

namespace simple_up_shape_te {

class Shape {
public:
    template<typename ShapeT, typename DrawStrategy>
    Shape(ShapeT&& shape, DrawStrategy&& drawer) :
        _pimpl{std::make_unique<ShapeModel<ShapeT, DrawStrategy>>(std::move(shape), std::move(drawer))} {
        // DO NOTHING
    }

    void draw() const {
        _pimpl->draw();
    }

private:
    struct ShapeConcept {
        virtual ~ShapeConcept() = default;
        virtual void draw() const = 0;
    };

    template<typename ShapeT, typename DrawStrategy>
    struct ShapeModel : public ShapeConcept {
        ShapeModel(ShapeT&& shape, DrawStrategy&& drawer) :
            _shape{std::move(shape)},
            _drawer{std::move(drawer)} {
            // DO NOTHING
        }

        void draw() const override {
            _drawer(_shape);
        }

        ShapeT _shape;
        DrawStrategy _drawer;
    };

    std::unique_ptr<ShapeConcept> _pimpl{nullptr};
};

}// end namespace simple_up_shape_te {