#pragma once

#include "../utils/json_extensions.h"
#include "../utils/adaptor.h"

namespace mirai
{
    /**
     * \brief A type representing the QQ id of a user
     */
    struct uid_t final
    {
        int64_t id = 0;

        /**
         * \brief Construct a default user id with the value 0
         */
        constexpr uid_t() = default;

        /**
         * \brief Construct a user id
         * \param id The id
         */
        constexpr explicit uid_t(const int64_t id): id(id) {}

        /**
         * \brief Implicitly convert to int64_t
         */
        constexpr operator int64_t() const { return id; }
    };

    /**
     * \brief A type representing the id of a group
     */
    struct gid_t final
    {
        int64_t id = 0;

        /**
         * \brief Construct a default group id with the value 0
         */
        constexpr gid_t() = default;

        /**
         * \brief Construct a group id
         * \param id The id
         */
        constexpr explicit gid_t(const int64_t id): id(id) {}

        /**
         * \brief Implicitly convert to int64_t
         */
        constexpr operator int64_t() const { return id; }
    };

    /**
     * \brief A type representing the id of a message
     */
    struct msgid_t final
    {
        int32_t id = 0;

        /**
         * \brief Construct a message id
         * \param id The id
         */
        constexpr explicit msgid_t(const int32_t id = 0): id(id) {}

        /**
         * \brief Implicitly convert to int64_t
         */
        constexpr operator int32_t() const { return id; }
    };

    namespace literals
    {
        using namespace std::literals;

        /**
         * \brief Construct a user id
         * \param id The id
         * \return A user id object
         */
        constexpr uid_t operator""_uid(const unsigned long long id) { return uid_t(static_cast<int64_t>(id)); }

        /**
         * \brief Construct a group id
         * \param id The id
         * \return A group id object
         */
        constexpr gid_t operator""_gid(const unsigned long long id) { return gid_t(static_cast<int64_t>(id)); }
    }

    /**
     * \brief Execution policy of event processing
     */
    enum class ExecutionPolicy { single_thread, thread_pool };

    /**
     * \brief Whether the target is friend or group
     */
    enum class TargetType { friend_, group, temp };

    NLOHMANN_JSON_SERIALIZE_ENUM(TargetType, {
        { TargetType::friend_, "friend" },
        { TargetType::group, "group" },
        { TargetType::temp, "temp" }
        });

    /**
     * \brief Permission of a group member
     */
    enum class Permission { member, administrator, owner };

    NLOHMANN_JSON_SERIALIZE_ENUM(Permission, {
        { Permission::member, "MEMBER" },
        { Permission::administrator, "ADMINISTRATOR" },
        { Permission::owner, "OWNER" },
        });

    /**
     * \brief Information about a group
     */
    struct Group final
    {
        gid_t id; ///< ID of the group
        std::string name; ///< Name of the group
        Permission permission{}; ///< The permission of the bot in the group
    };

    /**
     * \brief Information about a group member
     */
    struct Member final
    {
        uid_t id; ///< ID of the member
        std::string member_name; ///< Name of the member
        Permission permission{}; ///< The permission of the group member
        Group group; ///< Information about the group
    };

    /**
     * \brief Information about a friend
     */
    struct Friend final
    {
        uid_t id; ///< ID of the friend
        std::string nickname; ///< Nickname of the friend
        std::string remark; ///< Remark of the friend
    };

    /**
     * \brief Group configuration
     * \remarks The optionals are for configuring, the results will always
     * have values
     */
    struct GroupConfig final
    {
        std::optional<std::string> name; ///< The group name
        std::optional<std::string> announcement; ///< The group entrance announcement
        std::optional<bool> confess_talk = false; ///< Whether to enable confess talk
        std::optional<bool> allow_member_invite = false; ///< Whether to allow inviting other people
        std::optional<bool> auto_approve = false; ///< Whether to approve join requests automatically
        std::optional<bool> anonymous_chat = false; ///< Whether to enable anonymous chat
    };

    /**
     * \brief Other information about a group member
     */
    struct MemberInfo final
    {
        std::string name; ///< Name of the member
        std::string special_title; ///< Special title of the member
    };

    /**
     * \brief Session configuration
     */
    struct SessionConfig final
    {
        size_t cache_size = 4096; ///< Cache size of the session
        bool enable_websocket = false; ///< Whether websocket is enabled for this session
    };

    inline void to_json(utils::json& json, const uid_t value) { json = value.id; }
    inline void from_json(const utils::json& json, uid_t& value) { json.get_to(value.id); }
    inline void to_json(utils::json& json, const gid_t value) { json = value.id; }
    inline void from_json(const utils::json& json, gid_t& value) { json.get_to(value.id); }
    inline void to_json(utils::json& json, const msgid_t value) { json = value.id; }
    inline void from_json(const utils::json& json, msgid_t& value) { json.get_to(value.id); }

    void from_json(const utils::json& json, Group& value);
    void from_json(const utils::json& json, Member& value);
    void from_json(const utils::json& json, Friend& value);

    void to_json(utils::json& json, const GroupConfig& value);
    void from_json(const utils::json& json, GroupConfig& value);
    void to_json(utils::json& json, const MemberInfo& value);
    void from_json(const utils::json& json, MemberInfo& value);
    void to_json(utils::json& json, const SessionConfig& value);
    void from_json(const utils::json& json, SessionConfig& value);
}
