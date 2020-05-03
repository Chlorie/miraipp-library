#include "message.h"
#include <sstream>
#include "common.h"
#include "../common.h"
#include "../../utils/string.h"

namespace mirai
{
    Message::Message(MessageChain chain): chain_(std::move(chain))
    {
        combine_adjacent_text(chain_);
    }

    Message::Message(const Segment& segment): chain_{ segment } {}

    Message::Message(Segment&& segment): chain_{ std::move(segment) } {}

    Message::Message(const std::string_view plain_text):
        chain_({ msg::Plain{ std::string(plain_text) } }) {}

    Message& Message::operator=(MessageChain chain)
    {
        chain_ = std::move(chain);
        combine_adjacent_text(chain_);
        return *this;
    }

    Message& Message::operator=(const Segment& segment)
    {
        chain_ = MessageChain{ segment };
        return *this;
    }

    Message& Message::operator=(Segment&& segment)
    {
        chain_ = MessageChain{ std::move(segment) };
        return *this;
    }

    Message& Message::operator=(const std::string_view plain_text)
    {
        chain_ = { { msg::Plain{ std::string(plain_text) } } };
        return *this;
    }

    Message& Message::operator+=(const MessageChain& chain)
    {
        if (chain.empty()) return *this;
        operator+=(chain.front());
        chain_.insert(chain_.end(),
            std::next(chain.begin()),
            chain.end());
        return *this;
    }

    Message& Message::operator+=(MessageChain&& chain)
    {
        if (chain.empty()) return *this;
        operator+=(chain.front());
        chain_.insert(chain_.end(),
            std::make_move_iterator(std::next(chain.begin())),
            std::make_move_iterator(chain.end()));
        return *this;
    }

    Message& Message::operator+=(const Segment& segment)
    {
        if (is_plain(segment))
            return operator+=(get_plain(segment));
        chain_.emplace_back(segment);
        return *this;
    }

    Message& Message::operator+=(Segment&& segment)
    {
        if (is_plain(segment))
            return operator+=(get_plain(segment));
        chain_.emplace_back(std::move(segment));
        return *this;
    }

    Message& Message::operator+=(const std::string_view plain_text)
    {
        if (!empty() && is_plain(chain_.back()))
            get_plain(chain_.back()) += plain_text;
        else
            chain_.emplace_back(msg::Plain{ std::string(plain_text) });
        return *this;
    }

    bool operator==(const Message& lhs, const std::string_view rhs)
    {
        if (lhs.chain_.size() != 1 || !is_plain(lhs.chain_.front())) return false;
        return get_plain(lhs.chain_.front()) == rhs;
    }

    std::string Message::extract_text() const
    {
        std::string res;
        for (auto& node : chain_)
            node.dispatch([&res](const msg::Plain& plain) { res += plain.text; });
        return res;
    }

    std::string Message::stringify() const
    {
        std::ostringstream oss;
        for (const auto& segment : chain_)
            oss << segment.apply([](const auto& seg) { return seg.stringify(); });
        return oss.str();
    }

    bool Message::starts_with(const std::string_view text) const
    {
        if (chain_.empty() || !is_plain(chain_.front())) return false;
        return utils::starts_with(get_plain(chain_.front()), text);
    }

    bool Message::starts_with(const Segment& segment) const
    {
        if (is_plain(segment))
            return starts_with(get_plain(segment));
        if (chain_.empty())
            return false;
        return chain_.front() == segment;
    }

    bool Message::ends_with(const std::string_view text) const
    {
        if (chain_.empty() || !is_plain(chain_.back())) return false;
        return utils::ends_with(get_plain(chain_.front()), text);
    }

    bool Message::ends_with(const Segment& segment) const
    {
        if (is_plain(segment))
            return ends_with(get_plain(segment));
        if (chain_.empty())
            return false;
        return chain_.back() == segment;
    }

    bool Message::contains(const std::string_view text) const
    {
        return std::any_of(chain_.begin(), chain_.end(),
            [text](const Segment& seg)
            {
                return is_plain(seg)
                    && (get_plain(seg).find(text) != std::string::npos);
            });
    }

    bool Message::contains(const Segment& segment) const
    {
        if (is_plain(segment))
            return contains(get_plain(segment));
        return std::find(chain_.begin(), chain_.end(), segment) != chain_.end();
    }

    std::string Message::escape(const std::string_view unescaped)
    {
        size_t offset = 0;
        std::string result;
        result.reserve(unescaped.size());
        while (true)
        {
            const size_t pos = unescaped.find_first_of("\\[]{}", offset);
            if (pos == std::string::npos) break;
            const size_t length = pos - offset;
            result += unescaped.substr(offset, length);
            switch (unescaped[pos])
            {
                case '\\':
                    result += "\\\\";
                    break;
                case '[':
                    result += "\\[";
                    break;
                case ']':
                    result += "\\]";
                    break;
                case '{':
                    result += "[[";
                    break;
                case '}':
                    result += "]]";
                    break;
                default: break;
            }
            offset = pos + 1;
        }
        result += unescaped.substr(offset);
        return result;
    }

    std::string Message::unescape(const std::string_view escaped)
    {
        const char* error = "Erroneous escaped string";
        size_t offset = 0;
        std::string result;
        result.reserve(escaped.size());
        while (true)
        {
            const size_t pos = escaped.find_first_of("\\[", offset);
            if (pos == std::string::npos) break;
            const size_t length = pos - offset;
            if (length == escaped.size() - 1) throw RuntimeError(error);
            result += escaped.substr(offset, length);
            if (escaped[pos] == '[')
            {
                if (escaped[pos + 1] != '[') throw RuntimeError(error);
                result += '{';
            }
            else if (escaped[pos] == ']')
            {
                if (escaped[pos + 1] != ']') throw RuntimeError(error);
                result += '}';
            }
            else // escaped[pos] == '\\'
            {
                const char next = escaped[pos + 1];
                if (next != '\\' && next != '[' && next != ']') throw RuntimeError(error);
                result += next;
            }
            offset = pos + 2;
        }
        result += escaped.substr(offset);
        return result;
    }
}
