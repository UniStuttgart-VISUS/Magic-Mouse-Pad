// <copyright file="messages.h" company="Visualisierungsinstitut der Universit�t Stuttgart">
// Copyright � 2020 Visualisierungsinstitut der Universit�t Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph M�ller</author>

#pragma once

#include <cinttypes>

#include "MagicMousePad/byteorder.h"
#include "MagicMousePad/MouseButton.h"


namespace MagicMousePad {

#pragma pack(push, 2)
    /// <summary>
    /// The header uses for all messages of the magic mouse pad.
    /// <summary>
    struct Header {
        /// <summary>
        /// The fourcc code of the magic mouse pad.
        /// </summary>
        std::uint32_t Tag;

        /// <summary>
        /// The overall length of the message (including this header) in bytes.
        /// </summary>
        std::uint16_t Length;

        /// <summary>
        /// The ID of the message in the body.
        /// </summary>
        std::uint16_t ID;

        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        inline Header(void) : Tag(static_cast<uint32_t>('MMPD')),
            Length(0), ID(0) { }
    };

    static_assert(sizeof(Header) == 8, "The header must comprise 8 non-padded "
        "bytes.");

    /// <summary>
    /// Convert the data in <paramref name="header" /> from network byte order
    /// to host byte order.
    /// </summary>
    inline void ToHostOrder(Header &header) {
        header.Tag = ToHostOrder(header.Tag);
        header.Length = ToHostOrder(header.Length);
        header.ID = ToHostOrder(header.ID);
    }

    /// <summary>
    /// Convert the data in <paramref name="header" /> from host byte order
    /// to network byte order.
    /// </summary>
    inline void ToNetworkOrder(Header &header) {
        header.Tag = ToNetworkOrder(header.Tag);
        header.Length = ToNetworkOrder(header.Length);
        header.ID = ToNetworkOrder(header.ID);
    }


    /// <summary>
    /// The ID of a <see cref="SubscriptionMessage" />.
    /// </summary>
    static const std::uint32_t SubscriptionMessageID = 0x00000001;

    /// <summary>
    /// This message is sent from the mouse subscriber to the magic mouse pad in
    /// order to request mouse update messages.
    /// </summary>
    struct SubscriptionMessage {

        /// <summary>
        /// The message header.
        /// </summary>
        Header Header;

        /// <summary>
        /// The left offset of the subscriber wrt the global coordinates of the
        /// tiled display.
        /// </summary>
        /// <remarks>
        /// <para>This information is used by the magic mouse pad to transform
        /// the mouse coordinates into the local system of the subscriber.
        /// </para>
        /// <para>Set the area of the subscriber to an empty rectangle to
        /// subscribe for global coordinates.</para>
        /// </remarks>
        std::uint32_t Left;
        
        /// <summary>
        /// The top offset of the subscriber wrt the global coordinates of the
        /// tiled display.
        /// </summary>
        /// <remarks>
        /// <para>This information is used by the magic mouse pad to transform
        /// the mouse coordinates into the local system of the subscriber.
        /// </para>
        /// <para>Set the area of the subscriber to an empty rectangle to
        /// subscribe for global coordinates.</para>
        /// </remarks>
        std::uint32_t Top;

        /// <summary>
        /// The width of the area covered by the subscriber wrt the global
        /// coordinates of the tiled display.
        /// </summary>
        /// <remarks>
        /// <para>This information is used by the magic mouse pad to transform
        /// the mouse coordinates into the local system of the subscriber.
        /// </para>
        /// <para>Set the area of the subscriber to an empty rectangle to
        /// subscribe for global coordinates.</para>
        /// </remarks>
        std::uint32_t Width;

        /// <summary>
        /// The height of the area covered by the subscriber wrt the global
        /// coordinates of the tiled display.
        /// </summary>
        /// <remarks>
        /// <para>This information is used by the magic mouse pad to transform
        /// the mouse coordinates into the local system of the subscriber.
        /// </para>
        /// <para>Set the area of the subscriber to an empty rectangle to
        /// subscribe for global coordinates.</para>
        /// </remarks>
        std::uint32_t Height;

        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        inline SubscriptionMessage(void) : Left(0), Top(0), Width(0),
                Height(0) {
            this->Header.ID = SubscriptionMessageID;
            this->Header.Length = sizeof(*this);
        }
    };

    /// <summary>
    /// Convert the data in <paramref name="msg" /> from network byte order
    /// to host byte order.
    /// </summary>
    inline void ToHostOrder(SubscriptionMessage &msg) {
        ToHostOrder(msg.Header);
        msg.Left = ToHostOrder(msg.Left);
        msg.Top = ToHostOrder(msg.Top);
        msg.Width = ToHostOrder(msg.Width);
        msg.Height = ToHostOrder(msg.Height);
    }

    /// <summary>
    /// Convert the data in <paramref name="msg" /> from host byte order
    /// to network byte order.
    /// </summarySubscriptionMessage
    inline void ToNetworkOrder(SubscriptionMessage &msg) {
        ToNetworkOrder(msg.Header);
        msg.Left = ToNetworkOrder(msg.Left);
        msg.Top = ToNetworkOrder(msg.Top);
        msg.Width = ToNetworkOrder(msg.Width);
        msg.Height = ToNetworkOrder(msg.Height);
    }


    /// <summary>
    /// The ID of a <see cref="MousePositionMessage" />.
    /// </summary>
    static const std::uint32_t MousePositionMessageID = 0x00000010;

    /// <summary>
    /// This message is sent from the magic mouse pad to the subscriber to
    /// inform about a new mouse position.
    /// </summary>
    struct MousePositionMessage {

        /// <summary>
        /// The message header.
        /// </summary>
        Header Header;

        /// <summary>
        /// The new x-position of the mouse.
        /// </summary>
        std::int32_t X;

        /// <summary>
        /// The new y-position of the mouse.
        /// </summary>
        std::int32_t Y;

        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        inline MousePositionMessage(void) : X(0), Y(0) {
            this->Header.ID = MousePositionMessageID;
            this->Header.Length = sizeof(*this);
        }
    };

    /// <summary>
    /// Convert the data in <paramref name="msg" /> from network byte order
    /// to host byte order.
    /// </summary>
    inline void ToHostOrder(MousePositionMessage &msg) {
        ToHostOrder(msg.Header);
        msg.X = ToHostOrder(msg.X);
        msg.Y = ToHostOrder(msg.Y);
    }

    /// <summary>
    /// Convert the data in <paramref name="msg" /> from host byte order
    /// to network byte order.
    /// </summarySubscriptionMessage
    inline void ToNetworkOrder(MousePositionMessage &msg) {
        ToNetworkOrder(msg.Header);
        msg.X = ToNetworkOrder(msg.X);
        msg.Y = ToNetworkOrder(msg.Y);
    }


    /// <summary>
    /// The ID of a <see cref="MouseDownMessage" />.
    /// </summary>
    static const std::uint32_t MouseDownMessageID = 0x00000011;

    /// <summary>
    /// This message is sent from the magic mouse pad to the subscriber to
    /// inform about a mouse button being pressed.
    /// </summary>
    struct MouseDownMessage {

        /// <summary>
        /// The message header.
        /// </summary>
        Header Header;

        /// <summary>
        /// The button that has been pressed.
        /// </summary>
        std::underlying_type<MouseButton>::type Button;

        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        inline MouseDownMessage(void) : Button(0) {
            this->Header.ID = MouseDownMessageID;
            this->Header.Length = sizeof(*this);
        }
    };

    /// <summary>
    /// Convert the data in <paramref name="msg" /> from network byte order
    /// to host byte order.
    /// </summary>
    inline void ToHostOrder(MouseDownMessage &msg) {
        ToHostOrder(msg.Header);
        msg.Button = ToHostOrder(std::underlying_type<MouseButton>::type(
            msg.Button));
    }

    /// <summary>
    /// Convert the data in <paramref name="msg" /> from host byte order
    /// to network byte order.
    /// </summarySubscriptionMessage
    inline void ToNetworkOrder(MouseDownMessage &msg) {
        ToNetworkOrder(msg.Header);
        msg.Button = ToNetworkOrder(std::underlying_type<MouseButton>::type(
            msg.Button));
    }


    /// <summary>
    /// The ID of a <see cref="MouseUpMessage" />.
    /// </summary>
    static const std::uint32_t MouseUpMessageID = 0x00000012;

    /// <summary>
    /// This message is sent from the magic mouse pad to the subscriber to
    /// inform about a mouse button being released.
    /// </summary>
    struct MouseUpMessage {

        /// <summary>
        /// The message header.
        /// </summary>
        Header Header;

        /// <summary>
        /// The button that has been released.
        /// </summary>
        std::underlying_type<MouseButton>::type Button;

        /// <summary>
        /// Initialises a new instance.
        /// </summary>
        inline MouseUpMessage(void) : Button(0) {
            this->Header.ID = MouseDownMessageID;
            this->Header.Length = sizeof(*this);
        }
    };

    /// <summary>
    /// Convert the data in <paramref name="msg" /> from network byte order
    /// to host byte order.
    /// </summary>
    inline void ToHostOrder(MouseUpMessage &msg) {
        ToHostOrder(msg.Header);
        msg.Button = ToHostOrder(std::underlying_type<MouseButton>::type(
            msg.Button));
    }

    /// <summary>
    /// Convert the data in <paramref name="msg" /> from host byte order
    /// to network byte order.
    /// </summarySubscriptionMessage
    inline void ToNetworkOrder(MouseUpMessage &msg) {
        ToNetworkOrder(msg.Header);
        msg.Button = ToNetworkOrder(std::underlying_type<MouseButton>::type(
            msg.Button));
    }

#pragma pack(pop)
}
