#include <tuple>
#include <nlohmann/json.hpp>
#include <stdio.h>
#include <stdexcept>

template<typename... Args, std::size_t... Is>
std::tuple<Args...> from_json(const nlohmann::json & args, std::index_sequence<Is...>)
{
    return { args[Is].get<Args>()... };
}

// template<typename... Args>
// std::tuple<Args...> from_json(const nlohmann::json & args)
// {
//     std::tuple<Args...> rv;

//     size_t rv_size = std::tuple_size(rv);
    // for (int i = 0; i < rv_size; i++) {
    //     std::get<i>(rv_size) = i;
    // }

    // size_t rv_tuple_size = std::tuple_size(rv);
    // size_t keys_size = keys.size();
    // if (rv_tuple_size != keys_size) {
    //     std::string err = "size missmatch: " + std::to_string(rv_tuple_size) + " != " + std::to_string(keys_size) + "\n";
    //     throw std::runtime_error(err);
    // }

    // for (size_t i = 0; i < keys.size(); i++) {
    //     const auto& key = keys[i];
    //     std::get<i>(rv) = args[key];
    // }

//     return rv;
// }

int main(int argc, const char *argv[]) {
    nlohmann::json j = {
        {20, 40.5}
    };


    std::tuple<int, double> values = from_json(j, std::make_integer_sequence<int, 2>{});
    printf("j = %s\n", j.dump(4).c_str());


    return 0;
}



