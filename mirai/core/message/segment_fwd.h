#pragma once

#include <type_traits>
#include <tuple>
#include <optional>

// Forward declarations of segment structs,
// used for Message::match_types SFINAE

namespace mirai
{
    namespace msg
    {
        struct Source;
        struct Quote;
        struct At;
        struct AtAll;
        struct Face;
        struct Plain;
        struct Image;
        struct FlashImage;
        struct Xml;
        struct Json;
        struct App;
        struct Poke;

        namespace detail
        {
            template <typename... Ts> struct tlist {};

            using MsgTuple = std::tuple<Source, Quote, At, AtAll, Face,
                Plain, Image, FlashImage, Xml, Json, App, Poke>;

            template <typename TList, typename T> struct contains {};
            template <typename... Ts, typename T> struct contains<std::tuple<Ts...>, T> :
                std::bool_constant<(std::is_same_v<Ts, T> || ...)> {};
        }

        template <typename T> constexpr bool is_segment_type = detail::contains<detail::MsgTuple, T>::value;
    }

    enum class SegmentType;
    class Segment;

    namespace detail
    {
        template <SegmentType T>
        using segment_type = std::tuple_element_t<static_cast<size_t>(T), msg::detail::MsgTuple>;

        template <typename... Ts>
        using match_result = std::optional<std::tuple<const Ts&...>>;

        template <typename... Ts>
        using enable_match = std::enable_if_t<(msg::is_segment_type<Ts> && ...)>;

        template <SegmentType... Ts>
        using match_result_enum = match_result<segment_type<Ts>...>;
    }
}
