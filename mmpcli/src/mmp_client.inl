// <copyright file="mmp_client.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


/*
 * mmp_client::track_sequence_number
 */
template<class TMessage>
bool mmp_client::track_sequence_number(_In_ const TMessage *message) {
    assert(message != nullptr);
    const auto s = ::ntohl(message->sequence_number);
    auto e = this->_sequence_number.load(std::memory_order_acquire);
    MMP_TRACE(L"Received sequence number %u, current sequence number is %u.",
        s, e);
    return (s > e) && this->_sequence_number.compare_exchange_strong(
        e, s, std::memory_order_release, std::memory_order_relaxed);
}


/*
 * mmp_client::xform_position
 */
template<class TMessage>
std::pair<std::int32_t, std::int32_t> mmp_client::xform_position(
        _In_ const TMessage *message) {
    assert(message != nullptr);
    std::int32_t x = ::ntohl(message->x);
    std::int32_t y = ::ntohl(message->y);

    if ((this->_config.flags & mmp_flag_clip) != 0) {
        if (x < 0) {
            x = 0;
        } else if (x > static_cast<int32_t>(this->_config.width)) {
            x = this->_config.width;
        }

        if (y < 0) {
            y = 0;
        } else if (y > static_cast<int32_t>(this->_config.height)) {
            y = this->_config.height;
        }
    }

    if ((this->_config.flags & mmp_flag_local) != 0) {
        x -= this->_config.offset_x;
        y -= this->_config.offset_y;
        //// Clip against the local desktop size.
        //const auto screen_width = ::GetSystemMetrics(SM_CXSCREEN);
        //const auto screen_height = ::GetSystemMetrics(SM_CYSCREEN);
        //if (x < 0) {
        //    x = 0;
        //} else if (x > screen_width) {
        //    x = screen_width;
        //}
        //if (y < 0) {
        //    y = 0;
        //} else if (y > screen_height) {
        //    y = screen_height;
        //}
    }

    return std::make_pair(x, y);
}
