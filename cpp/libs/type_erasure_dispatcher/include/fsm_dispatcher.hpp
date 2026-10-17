#include <typeindex>
#include <functional>
#include <memory>
#include <optional>
#include <map>
#include <cstdio>

#include "fsm_defs.hpp"

using OperationId = std::type_index;

struct IOperationParam {};

template<typename Param>
struct OperationParam : IOperationParam {
    Param param;

    OperationParam() : param{} {
        // DO NOTHING
    }

    OperationParam(Param&& param) : param(std::move(param)) {
        // DO NOTHING
    }

    OperationParam(const Param& param) : param{param} {
        // DO NOTHING
    }
};

template<typename Operation>
const Operation& extract_operation_from_i_operation_param(const IOperationParam* i_op_param) {
    return static_cast<const OperationParam<Operation>*>(i_op_param)->param;
}

template<typename Operation>
std::unique_ptr<IOperationParam> generate_i_operation_param_from_operation(const Operation& operation) {
    return std::make_unique<IOperationParam>(OperationParam<Operation>(operation));
}

struct TranisitonInput {
    FsmState state;
    OperationId operation_id;

    bool operator<(const TranisitonInput& other) const {
        bool is_state_smaller = static_cast<uint32_t>(state) < static_cast<uint32_t>(other.state);
        bool is_op_id_smaller = operation_id < other.operation_id;
        return is_state_smaller || is_op_id_smaller;
    }

    std::string to_string() const {
        return std::string(fsm_state_to_str(state)) + "-" + operation_id.name();
    }
};

using TransitionCb = std::function<void(FsmState& new_state,
                                        const IOperationParam* i_op_param)>;
using TransitionInputToCb = std::map<TranisitonInput, TransitionCb>;


#define REGISTER_TRANSITION(dispatcher, from, Operation, callback)                                          \
    do {                                                                                                    \
        TransitionCb cb = [this](FsmState& new_state,                                                       \
                                 const IOperationParam* i_op_param) {                                       \
            const Operation& operation = extract_operation_from_i_operation_param<Operation>(i_op_param);   \
            callback(new_state, operation);                                                                 \
        };                                                                                                  \
        dispatcher.register_transition<Operation>(from, std::move(cb));                                     \
    } while (0)

class FsmDispatcher {
public:
    FsmDispatcher() = default;

    template<typename Operation>
    void register_transition(const FsmState& from, TransitionCb&& callback) {
        TranisitonInput input = _generate_input<Operation>(from);
        _transition_input_to_cb[input] = std::move(callback);
    }

    template<typename Operation>
    void transition(const Operation& operation) {
        TranisitonInput input = _generate_input<Operation>(std::nullopt);
        if (!_transition_input_to_cb.contains(input)) {
            printf("no state + operation exist\n");
            return;
        }

        FsmState new_state;
        auto& callback = _transition_input_to_cb.at(input);
        auto i_op_param_ptr = generate_i_operation_param_from_operation(operation);
        callback(new_state, i_op_param_ptr.get());
        printf("transition from %s to %s\n", fsm_state_to_str(_active_state), fsm_state_to_str(new_state));
        _active_state = new_state;
    }

    void print_transition_input_to_cb() {
        printf("num_entries=[%zu]\n", _transition_input_to_cb.size());
        for (const auto& [input, _]: _transition_input_to_cb) {
            printf("input=[%s]\n", input.to_string().c_str());
        }
    }

private:
    TransitionInputToCb _transition_input_to_cb{};
    FsmState _active_state{FsmState::A};

    template<typename Operation>
    TranisitonInput _generate_input(std::optional<FsmState> opt_state) {
        OperationId operation_id(typeid(Operation));
        if (opt_state.has_value()) {
            return {*opt_state, operation_id};
        }

        return {_active_state, operation_id};
    }
};