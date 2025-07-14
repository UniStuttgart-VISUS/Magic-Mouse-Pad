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
    // TODO: implement reordering here.
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
    const auto clip = ((this->_config.flags & mmp_flag_clip) != 0);
    const auto hide = ((this->_config.flags & mmp_flag_hide_remote) != 0);
    std::int32_t x = ::ntohl(message->x);
    std::int32_t y = ::ntohl(message->y);

    if (clip) {
        mmp_client::clip(x, y, this->_config.width, this->_config.height);
    }

    if ((this->_config.flags & mmp_flag_local) != 0) {
        x -= this->_config.offset_x;
        y -= this->_config.offset_y;

        if (clip) {
            // If requested, clip the position to the local screen. Furthermore,
            // hide the cursor if it is outside the local screen if that was
            // requested, too.
            const auto w = ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
            const auto h = ::GetSystemMetrics(SM_CYVIRTUALSCREEN);

            const auto clipped = mmp_client::clip(x, y, w, h);

            if (hide) {
                ::ShowCursor(!clipped);
            }
        }
    }

    return std::make_pair(x, y);
}
