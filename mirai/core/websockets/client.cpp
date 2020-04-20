#include "client.h"
#include "../common.h"

namespace mirai::ws
{
    Client::Client()
    {
        client_.clear_access_channels(wspp::log::alevel::all);
        client_.clear_error_channels(wspp::log::alevel::all);
        client_.init_asio();
        client_.start_perpetual();
        thread_ = utils::Thread([&client = client_]()
        {
            try { client.run(); }
            catch (...) { error_logger(); }
        });
    }

    Client::~Client() noexcept
    {
        client_.stop_perpetual();
        for (const auto& connection : connections_)
        {
            try
            {
                if (!connection->ended())
                    close(*connection);
            }
            catch (...) {}
        }
    }

    Connection& Client::connect(const std::string& uri)
    {
        std::error_code error;
        const auto ptr = client_.get_connection(uri, error);
        if (error) throw RuntimeError(error.message());
        auto& connection = *connections_.emplace_back(
            std::make_unique<Connection>(ptr->get_handle(), uri));
        using Handle = wspp::connection_hdl;
        ptr->set_open_handler([&](const Handle hdl) { connection.on_open(client_, hdl); });
        ptr->set_fail_handler([&](const Handle hdl) { connection.on_fail(client_, hdl); });
        ptr->set_close_handler([&](const Handle hdl) { connection.on_close(client_, hdl); });
        ptr->set_message_handler([&](const Handle hdl, const AsioClient::message_ptr msg)
        {
            connection.on_message(hdl, msg);
        });
        client_.connect(ptr);
        return connection;
    }

    void Client::close(Connection& connection)
    {
        std::error_code error;
        client_.close(connection.handle(),
            wspp::close::status::going_away, {}, error);
        if (error) throw RuntimeError(error.message());
    }
}
