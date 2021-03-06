#pragma once

#include <optional>
#include <variant>
#include <chrono>
#include "types.h"
#include "message/received_message.h"
#include "../utils/variant_wrapper.h"
#include "../utils/json_extensions.h"

namespace mirai
{
    /**
     * \brief Event for receiving a group message
     */
    struct GroupMessage final
    {
        ReceivedMessage message; ///< The message
        Member sender; ///< Sender of the message
    };

    /**
     * \brief Event for receiving a friend message
     */
    struct FriendMessage final
    {
        ReceivedMessage message; ///< The messgae
        Friend sender; ///< Sender of the message
    };

    /**
     * \brief Event for receiving a temporary message
     */
    struct TempMessage final
    {
        ReceivedMessage message; ///< The messgae
        Member sender; ///< Sender of the message
    };

    /**
     * \brief Event received when the bot goes online
     */
    struct BotOnlineEvent final
    {
        uid_t qq; ///< QQ of the bot
    };

    /**
     * \brief Event received when the bot gets offline actively
     */
    struct BotOfflineEventActive final
    {
        uid_t qq; ///< QQ of the bot
    };

    /**
     * \brief Event received when the bot is forced offline
     */
    struct BotOfflineEventForce final
    {
        uid_t qq; ///< QQ of the bot
    };

    /**
     * \brief Event received when the bot is disconnected
     */
    struct BotOfflineEventDropped final
    {
        uid_t qq; ///< QQ of the bot
    };

    /**
     * \brief Event received when the bot actively re-login
     */
    struct BotReloginEvent final
    {
        uid_t qq; ///< QQ of the bot
    };

    /**
     * \brief Event received when a group message is recalled by someone
     */
    struct GroupRecallEvent final
    {
        uid_t author_id; ///< The sender of the recalled message
        msgid_t message_id; ///< The ID of the message
        int32_t time = 0; ///< Timestamp when the message is sent
        Group group; ///< The group in which the message is recalled
        std::optional<Member> operator_; ///< The operator who recalled the message, null if it's the bot
    };

    /**
     * \brief Event received when a friend message is recalled by someone
     */
    struct FriendRecallEvent final
    {
        uid_t author_id; ///< The sender of the recalled message
        msgid_t message_id; ///< The ID of the message
        int32_t time = 0; ///< Timestamp when the message is sent
        uid_t operator_; ///< QQ of the operator who recalled the message
    };

    /**
     * \brief Event received when the bot's permission is changed in some group
     * \remarks The operator can only be the group owner
     */
    struct BotGroupPermissionChangeEvent final
    {
        Permission origin{}; ///< The original permission
        Permission current{}; ///< The permission now
        Group group; ///< The group in which the bot's permission is changed
    };

    /**
     * \brief Event received when the bot is muted in some group
     */
    struct BotMuteEvent final
    {
        std::chrono::seconds duration{}; ///< The duration of the mute
        Member operator_; ///< The operator who muted the bot
    };

    /**
     * \brief Event received when the bot is unmuted in some group
     */
    struct BotUnmuteEvent final
    {
        Member operator_; ///< The operator who unmuted the bot
    };

    /**
     * \brief Event received when the bot joins any group
     */
    struct BotJoinGroupEvent final
    {
        Group group; ///< The group that the bot joined
    };

    /**
     * \brief Event received when the bot quits a group actively
     */
    struct BotLeaveEventActive final
    {
        Group group; ///< The group that the bot quitted
    };

    /**
     * \brief Event received when the bot gets kicked out of a group
     */
    struct BotLeaveEventKick final
    {
        Group group; ///< The group that the bot got kicked out of
    };

    /**
     * \brief Event received when some group's name is changed
     */
    struct GroupNameChangeEvent final
    {
        std::string origin; ///< The original group name
        std::string current; ///< The group name now
        Group group; ///< The group of which name is changed
        std::optional<Member> operator_; ///< The operator who changed the group name, null if it's the bot
    };

    /**
     * \brief Event received when the group entrance announcement in some group is changed
     */
    struct GroupEntranceAnnouncementChangeEvent final
    {
        std::string origin; ///< The original announcement
        std::string current; ///< The announcement now
        Group group; ///< The group of which announcement is changed
        std::optional<Member> operator_; ///< The operator who changed the announcement, null if it's the bot
    };

    /**
     * \brief Event received when in some group the mute-all state is changed
     */
    struct GroupMuteAllEvent final
    {
        bool origin = false; ///< The original state
        bool current = false; ///< The state now
        Group group; ///< The group in which the state is changed
        std::optional<Member> operator_; ///< The operator who changed the state, null if it's the bot
    };

    /**
     * \brief Event received when in some group the allow anonymous chat state is changed
     */
    struct GroupAllowAnonymousChatEvent final
    {
        bool origin = false; ///< The original state
        bool current = false; ///< The state now
        Group group; ///< The group in which the state is changed
        std::optional<Member> operator_; ///< The operator who changed the state, null if it's the bot
    };

    /**
     * \brief Event received when in some group the allow confess talk state is changed
     */
    struct GroupAllowConfessTalkEvent final
    {
        bool origin = false; ///< The original state
        bool current = false; ///< The state now
        Group group; ///< The group in which the state is changed
        bool is_by_bot = false; ///< Whether the state change is by the bot
    };

    /**
     * \brief Event received when in some group the allow member invitation state is changed
     */
    struct GroupAllowMemberInviteEvent final
    {
        bool origin = false; ///< The original state
        bool current = false; ///< The state now
        Group group; ///< The group in which the state is changed
        std::optional<Member> operator_; ///< The operator who changed the state, null if it's the bot
    };

    /**
     * \brief Event received when someone (not the bot) joins a group
     */
    struct MemberJoinEvent final
    {
        Member member; ///< The new group member
    };

    /**
     * \brief Event received when someone (not the bot) is kicked out of some group
     */
    struct MemberLeaveEventKick final
    {
        Member member; ///< The kicked group member
        std::optional<Member> operator_; ///< The operator who kicked the member out, null if it's the bot
    };

    /**
     * \brief Event received when someone (not the bot) leaves some group
     */
    struct MemberLeaveEventQuit final
    {
        Member member; ///< The group member who has left the group
    };

    /**
     * \brief Event received when someone's group member card is changed
     */
    struct MemberCardChangeEvent final
    {
        std::string origin; ///< The original member card
        std::string current; ///< The member card now
        Member member; ///< The member whose card got changed
        std::optional<Member> operator_; ///< The operator who changed, null if it's the bot
    };

    /**
     * \brief Event received when someone's special title is changed
     */
    struct MemberSpecialTitleChangeEvent final
    {
        std::string origin; ///< The original special title
        std::string current; ///< The special title now
        Member member; ///< The member whose special title got changed
    };

    /**
     * \brief Event received when someone's group permission is changed
     */
    struct MemberPermissionChangeEvent final
    {
        Permission origin{}; ///< The original permission
        Permission current{}; ///< The permission now
        Member member; ///< The member whose permission got changed
    };

    /**
     * \brief Event received when someone is muted in some group
     */
    struct MemberMuteEvent final
    {
        std::chrono::seconds duration{}; ///< The duration of the mute
        Member member; ///< The member who has got muted
        std::optional<Member> operator_; ///< The operator who muted the group member, null if it's the bot
    };

    /**
     * \brief Event received when someone is unmuted in some group
     */
    struct MemberUnmuteEvent final
    {
        Member member; ///< The member who has got unmuted
        std::optional<Member> operator_; ///< The operator who unmuted the group member, null if it's the bot
    };

    /**
     * \brief Event received when someone requested to add friend
     */
    struct NewFriendRequestEvent final
    {
        int64_t event_id = 0; ///< The identifier of the event for future respond
        uid_t from_id; ///< QQ of the user who started this request
        std::optional<gid_t> group_id; ///< If the request is started from a group then this is the group id
        std::string nick; ///< The nickname or group card
    };

    /**
     * \brief Response type for new friend request event
     */
    enum class NewFriendResponseType
    {
        approve, disapprove, blacklist
    };

    /**
     * \brief Event received when someone requested to join a group
     */
    struct MemberJoinRequestEvent final
    {
        int64_t event_id = 0; ///< The identifier of the event for future respond
        uid_t from_id; ///< QQ of the user who started this request
        gid_t group_id; ///< The group ID
        std::string group_name; ///< Name of the group
        std::string nick; ///< The nickname
    };

    /**
     * \brief Response type for member join request event
     */
    enum class MemberJoinResponseType
    {
        approve, disapprove, ignore,
        disapprove_blacklist, ignore_blacklist
    };

    void from_json(const utils::json& json, GroupMessage& value);
    void from_json(const utils::json& json, FriendMessage& value);
    void from_json(const utils::json& json, TempMessage& value);
    void from_json(const utils::json& json, BotOnlineEvent& value);
    void from_json(const utils::json& json, BotOfflineEventActive& value);
    void from_json(const utils::json& json, BotOfflineEventForce& value);
    void from_json(const utils::json& json, BotOfflineEventDropped& value);
    void from_json(const utils::json& json, BotReloginEvent& value);
    void from_json(const utils::json& json, GroupRecallEvent& value);
    void from_json(const utils::json& json, FriendRecallEvent& value);
    void from_json(const utils::json& json, BotGroupPermissionChangeEvent& value);
    void from_json(const utils::json& json, BotMuteEvent& value);
    void from_json(const utils::json& json, BotUnmuteEvent& value);
    void from_json(const utils::json& json, BotLeaveEventActive& value);
    void from_json(const utils::json& json, BotLeaveEventKick& value);
    void from_json(const utils::json& json, BotJoinGroupEvent& value);
    void from_json(const utils::json& json, GroupNameChangeEvent& value);
    void from_json(const utils::json& json, GroupEntranceAnnouncementChangeEvent& value);
    void from_json(const utils::json& json, GroupMuteAllEvent& value);
    void from_json(const utils::json& json, GroupAllowAnonymousChatEvent& value);
    void from_json(const utils::json& json, GroupAllowConfessTalkEvent& value);
    void from_json(const utils::json& json, GroupAllowMemberInviteEvent& value);
    void from_json(const utils::json& json, MemberJoinEvent& value);
    void from_json(const utils::json& json, MemberLeaveEventKick& value);
    void from_json(const utils::json& json, MemberLeaveEventQuit& value);
    void from_json(const utils::json& json, MemberCardChangeEvent& value);
    void from_json(const utils::json& json, MemberSpecialTitleChangeEvent& value);
    void from_json(const utils::json& json, MemberPermissionChangeEvent& value);
    void from_json(const utils::json& json, MemberMuteEvent& value);
    void from_json(const utils::json& json, MemberUnmuteEvent& value);
    void from_json(const utils::json& json, NewFriendRequestEvent& value);
    void from_json(const utils::json& json, MemberJoinRequestEvent& value);

    using EventVariant = std::variant<
        GroupMessage, FriendMessage, TempMessage,
        BotOnlineEvent, BotOfflineEventActive, BotOfflineEventForce, BotOfflineEventDropped,
        BotReloginEvent, GroupRecallEvent, FriendRecallEvent, BotGroupPermissionChangeEvent,
        BotMuteEvent, BotUnmuteEvent, BotJoinGroupEvent, BotLeaveEventActive, BotLeaveEventKick,
        GroupNameChangeEvent, GroupEntranceAnnouncementChangeEvent, GroupMuteAllEvent,
        GroupAllowAnonymousChatEvent, GroupAllowConfessTalkEvent, GroupAllowMemberInviteEvent,
        MemberJoinEvent, MemberLeaveEventKick, MemberLeaveEventQuit, MemberCardChangeEvent,
        MemberSpecialTitleChangeEvent, MemberPermissionChangeEvent, MemberMuteEvent, MemberUnmuteEvent,
        NewFriendRequestEvent, MemberJoinRequestEvent
    >;

    /**
     * \brief Enum corresponding to every type of an event, including the two message events
     */
    enum class EventType
    {
        group_message, friend_message, temp_message,
        bot_online_event, bot_offline_event_active, bot_offline_event_force, bot_offline_event_dropped,
        bot_relogin_event, group_recall_event, friend_recall_event, bot_group_permission_change_event,
        bot_mute_event, bot_unmute_event, bot_join_group_event, bot_leave_event_active, bot_leave_event_kick,
        group_name_change_event, group_entrance_announcement_change_event, group_mute_all_event,
        group_allow_anonymous_chat_event, group_allow_confess_talk_event, group_allow_member_invite_event,
        member_join_event, member_leave_event_kick, member_leave_event_quit, member_card_change_event,
        member_special_title_change_event, member_permission_change_event, member_mute_event,
        member_unmute_event, new_friend_request_event, member_join_request_event, max_value
    };

    static_assert(std::variant_size_v<EventVariant> == static_cast<size_t>(EventType::max_value),
        "Mismatched enum and variant size (Event)");

    inline constexpr std::array<std::string_view, std::variant_size_v<EventVariant>> event_type_names
    {
        "GroupMessage", "FriendMessage", "TempMessage",
        "BotOnlineEvent", "BotOfflineEventActive", "BotOfflineEventForce", "BotOfflineEventDropped",
        "BotReloginEvent", "GroupRecallEvent", "FriendRecallEvent", "BotGroupPermissionChangeEvent",
        "BotMuteEvent", "BotUnmuteEvent", "BotJoinGroupEvent", "GroupNameChangeEvent",
        "GroupEntranceAnnouncementChangeEvent", "GroupMuteAllEvent", "GroupAllowAnonymousChatEvent",
        "GroupAllowConfessTalkEvent", "GroupAllowMemberInviteEvent", "MemberJoinEvent",
        "MemberLeaveEventKick", "MemberLeaveEventQuit", "MemberCardChangeEvent",
        "MemberSpecialTitleChangeEvent", "MemberPermissionChangeEvent", "MemberMuteEvent",
        "MemberUnmuteEvent", "NewFriendRequestEvent", "MemberJoinRequestEvent"
    };

    /**
     * \brief The event type containing every kind of event,
     * plus the two message receiving "events"
     */
    using Event = utils::VariantWrapper<EventVariant, EventType>;

    void from_json(const utils::json& json, Event& value);
}
