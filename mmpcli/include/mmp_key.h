// <copyright file="mmp_key.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2025 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#if !defined(_MMP_KEY_H)
#define _MMP_KEY_H
#pragma once


/// <summary>
/// Enumerates possible mouse buttons that the magic mouse pad can report to the
/// client.
/// </summary>
typedef enum mmp_key_t {
    /// <summary>
    /// No key.
    /// </summary>
    mmp_key_none = 0,

    mmp_key_lbutton = 0x01,
    mmp_key_rbutton = 0x02,
    mmp_key_cancel = 0x03,
    mmp_key_mbutton = 0x04,
#if (_WIN32_WINNT >= 0x0500)
    mmp_key_xbutton1 = 0x05,
    mmp_key_xbutton2 = 0x06,
#endif /* (_WIN32_WINNT >= 0x0500) */
    mmp_key_back = 0x08,
    mmp_key_tab = 0x09,
    mmp_key_clear = 0x0C,
    mmp_key_return = 0x0D,
    mmp_key_shift = 0x10,
    mmp_key_control = 0x11,
    mmp_key_menu = 0x12,
    mmp_key_pause = 0x13,
    mmp_key_capital = 0x14,
    mmp_key_kana = 0x15,
    mmp_key_hangul = 0x15,
    mmp_key_ime_on = 0x16,
    mmp_key_junja = 0x17,
    mmp_key_final = 0x18,
    mmp_key_hanja = 0x19,
    mmp_key_kanji = 0x19,
    mmp_key_ime_off = 0x1A,
    mmp_key_escape = 0x1B,
    mmp_key_convert = 0x1C,
    mmp_key_nonconvert = 0x1D,
    mmp_key_accept = 0x1E,
    mmp_key_modechange = 0x1F,
    mmp_key_space = 0x20,
    mmp_key_prior = 0x21,
    mmp_key_next = 0x22,
    mmp_key_end = 0x23,
    mmp_key_home = 0x24,
    mmp_key_left = 0x25,
    mmp_key_up = 0x26,
    mmp_key_right = 0x27,
    mmp_key_down = 0x28,
    mmp_key_select = 0x29,
    mmp_key_print = 0x2A,
    mmp_key_execute = 0x2B,
    mmp_key_snapshot = 0x2C,
    mmp_key_insert = 0x2D,
    mmp_key_delete = 0x2E,
    mmp_key_help = 0x2F,

    mmp_key_0 = 0x30,
    mmp_key_1 = 0x31,
    mmp_key_2 = 0x32,
    mmp_key_3 = 0x33,
    mmp_key_4 = 0x34,
    mmp_key_5 = 0x35,
    mmp_key_6 = 0x36,
    mmp_key_7 = 0x37,
    mmp_key_8 = 0x38,
    mmp_key_9 = 0x39,

    mmp_key_a = 0x41,
    mmp_key_b = 0x42,
    mmp_key_c = 0x43,
    mmp_key_d = 0x44,
    mmp_key_e = 0x45,
    mmp_key_f = 0x46,
    mmp_key_g = 0x47,
    mmp_key_h = 0x48,
    mmp_key_i = 0x49,
    mmp_key_j = 0x4A,
    mmp_key_k = 0x4B,
    mmp_key_l = 0x4C,
    mmp_key_m = 0x4D,
    mmp_key_n = 0x4E,
    mmp_key_o = 0x4F,
    mmp_key_p = 0x50,
    mmp_key_q = 0x51,
    mmp_key_r = 0x52,
    mmp_key_s = 0x53,
    mmp_key_t = 0x54,
    mmp_key_u = 0x55,
    mmp_key_v = 0x56,
    mmp_key_w = 0x57,
    mmp_key_x = 0x58,
    mmp_key_y = 0x59,
    mmp_key_z = 0x5A,

    mmp_key_lwin = 0x5B,
    mmp_key_rwin = 0x5C,
    mmp_key_apps = 0x5D,
    mmp_key_sleep = 0x5F,
    mmp_key_numpad0 = 0x60,
    mmp_key_numpad1 = 0x61,
    mmp_key_numpad2 = 0x62,
    mmp_key_numpad3 = 0x63,
    mmp_key_numpad4 = 0x64,
    mmp_key_numpad5 = 0x65,
    mmp_key_numpad6 = 0x66,
    mmp_key_numpad7 = 0x67,
    mmp_key_numpad8 = 0x68,
    mmp_key_numpad9 = 0x69,
    mmp_key_multiply = 0x6A,
    mmp_key_add = 0x6B,
    mmp_key_separator = 0x6C,
    mmp_key_subtract = 0x6D,
    mmp_key_decimal = 0x6E,
    mmp_key_divide = 0x6F,
    mmp_key_f1 = 0x70,
    mmp_key_f2 = 0x71,
    mmp_key_f3 = 0x72,
    mmp_key_f4 = 0x73,
    mmp_key_f5 = 0x74,
    mmp_key_f6 = 0x75,
    mmp_key_f7 = 0x76,
    mmp_key_f8 = 0x77,
    mmp_key_f9 = 0x78,
    mmp_key_f10 = 0x79,
    mmp_key_f11 = 0x7A,
    mmp_key_f12 = 0x7B,
    mmp_key_f13 = 0x7C,
    mmp_key_f14 = 0x7D,
    mmp_key_f15 = 0x7E,
    mmp_key_f16 = 0x7F,
    mmp_key_f17 = 0x80,
    mmp_key_f18 = 0x81,
    mmp_key_f19 = 0x82,
    mmp_key_f20 = 0x83,
    mmp_key_f21 = 0x84,
    mmp_key_f22 = 0x85,
    mmp_key_f23 = 0x86,
    mmp_key_f24 = 0x87,
#if(_WIN32_WINNT >= 0x0604)
    mmp_key_navigation_view = 0x88,
    mmp_key_navigation_menu = 0x89,
    mmp_key_navigation_up = 0x8A,
    mmp_key_navigation_down = 0x8B,
    mmp_key_navigation_left = 0x8C,
    mmp_key_navigation_right = 0x8D,
    mmp_key_navigation_accept = 0x8E,
    mmp_key_navigation_cancel = 0x8F,
#endif /* (_WIN32_WINNT >= 0x0604) */
    mmp_key_numlock = 0x90,
    mmp_key_scroll = 0x91,

    /// <summary>
    /// '=' key on numpad.
    /// </summary>
    mmp_key_oem_nec_equal = 0x92,

    /// <summary>
    /// // 'Dictionary' key
    /// </summary>
    mmp_key_oem_fj_jisho = 0x92,

    /// <summary>
    /// 'Unregister word' key
    /// </summary>
    mmp_key_oem_fj_masshou = 0x93,

    /// <summary>
    /// 'Register word' key
    /// </summary>
    mmp_key_oem_fj_touroku = 0x94,

    /// <summary>
    /// 'Left OYAYUBI' key
    /// </summary>
    mmp_key_oem_fj_loya = 0x95,

    /// <summary>
    /// 'Right OYAYUBI' key
    /// </summary>
    mmp_key_oem_fj_roya = 0x96,
    mmp_key_lshift = 0xA0,
    mmp_key_rshift = 0xA1,
    mmp_key_lcontrol = 0xA2,
    mmp_key_rcontrol = 0xA3,
    mmp_key_lmenu = 0xA4,
    mmp_key_rmenu = 0xA5,
#if (_WIN32_WINNT >= 0x0500)
    mmp_key_browser_back = 0xA6,
    mmp_key_browser_forward = 0xA7,
    mmp_key_browser_refresh = 0xA8,
    mmp_key_browser_stop = 0xA9,
    mmp_key_browser_search = 0xAA,
    mmp_key_browser_favorites = 0xAB,
    mmp_key_browser_home = 0xAC,
    mmp_key_volume_mute = 0xAD,
    mmp_key_volume_down = 0xAE,
    mmp_key_volume_up = 0xAF,
    mmp_key_media_next_track = 0xB0,
    mmp_key_media_prev_track = 0xB1,
    mmp_key_media_stop = 0xB2,
    mmp_key_media_play_pause = 0xB3,
    mmp_key_launch_mail = 0xB4,
    mmp_key_launch_media_select = 0xB5,
    mmp_key_launch_app1 = 0xB6,
    mmp_key_launch_app2 = 0xB7,
#endif /* (_WIN32_WINNT >= 0x0500) */

    /// <summary>
    /// ';:' for US
    /// </summary>
    mmp_key_oem_1 = 0xBA,

    /// <summary>
    /// '+' any country
    /// </summary>
    mmp_key_oem_plus = 0xBB,

    /// <summary>
    /// ',' any country
    /// </summary>
    mmp_key_oem_comma = 0xBC,

    /// <summary>
    /// '-' any country
    /// </summary>
    mmp_key_oem_minus = 0xBD,

    /// <summary>
    /// '.' any country
    /// </summary>
    mmp_key_oem_period = 0xBE,

    /// <summary>
    /// '/?' for US
    /// </summary>
    mmp_key_oem_2 = 0xBF,

    /// <summary>
    /// '`~' for US
    /// </summary>
    mmp_key_oem_3 = 0xC0,

#if (_WIN32_WINNT >= 0x0604)
    mmp_key_gamepad_a = 0xC3,
    mmp_key_gamepad_b = 0xC4,
    mmp_key_gamepad_x = 0xC5,
    mmp_key_gamepad_y = 0xC6,
    mmp_key_gamepad_right_shoulder = 0xC7,
    mmp_key_gamepad_left_shoulder = 0xC8,
    mmp_key_gamepad_left_trigger = 0xC9,
    mmp_key_gamepad_right_trigger = 0xCA,
    mmp_key_gamepad_dpad_up = 0xCB,
    mmp_key_gamepad_dpad_down = 0xCC,
    mmp_key_gamepad_dpad_left = 0xCD,
    mmp_key_gamepad_dpad_right = 0xCE,
    mmp_key_gamepad_menu = 0xCF,
    mmp_key_gamepad_view = 0xD0,
    mmp_key_gamepad_left_thumbstick_button = 0xD1,
    mmp_key_gamepad_right_thumbstick_button = 0xD2,
    mmp_key_gamepad_left_thumbstick_up = 0xD3,
    mmp_key_gamepad_left_thumbstick_down = 0xD4,
    mmp_key_gamepad_left_thumbstick_right = 0xD5,
    mmp_key_gamepad_left_thumbstick_left = 0xD6,
    mmp_key_gamepad_right_thumbstick_up = 0xD7,
    mmp_key_gamepad_right_thumbstick_down = 0xD8,
    mmp_key_gamepad_right_thumbstick_right = 0xD9,
    mmp_key_gamepad_right_thumbstick_left = 0xDA,
#endif /* (_WIN32_WINNT >= 0x0604) */

    /// <summary>
    /// '[{' for US
    /// </summary>
    mmp_key_oem_4 = 0xDB,

    /// <summary>
    /// '\|' for US
    /// </summary>
    mmp_key_oem_5 = 0xDC,

    /// <summary>
    /// ']}' for US
    /// </summary>
    mmp_key_oem_6 = 0xDD,

    /// <summary>
    /// ''"' for US
    /// </summary>
    mmp_key_oem_7 = 0xDE,

    mmp_key_oem_8 = 0xDF,

    /// <summary>
    /// 'AX' key on Japanese AX kbd
    /// </summary>
    mmp_key_oem_ax = 0xE1,

    /// <summary>
    /// "<>" or "\|" on RT 102-key kbd.
    /// </summary>
    mmp_key_oem_102 = 0xE2,

    /// <summary>
    /// Help key on ICO
    /// </summary>
    mmp_key_ico_help = 0xE3,

    /// <summary>
    /// 00 key on ICO
    /// </summary>
    mmp_key_ico_00 = 0xE4,

#if (WINVER >= 0x0400)
    mmp_key_processkey = 0xE5,
#endif /* (WINVER >= 0x0400) */
    mmp_key_ico_clear = 0xE6,
#if (_WIN32_WINNT >= 0x0500)
    mmp_key_packet = 0xE7,
#endif /* (_WIN32_WINNT >= 0x0500) */
    mmp_key_oem_reset = 0xE9,
    mmp_key_oem_jump = 0xEA,
    mmp_key_oem_pa1 = 0xEB,
    mmp_key_oem_pa2 = 0xEC,
    mmp_key_oem_pa3 = 0xED,
    mmp_key_oem_wsctrl = 0xEE,
    mmp_key_oem_cusel = 0xEF,
    mmp_key_oem_attn = 0xF0,
    mmp_key_oem_finish = 0xF1,
    mmp_key_oem_copy = 0xF2,
    mmp_key_oem_auto = 0xF3,
    mmp_key_oem_enlw = 0xF4,
    mmp_key_oem_backtab = 0xF5,
    mmp_key_attn = 0xF6,
    mmp_key_crsel = 0xF7,
    mmp_key_exsel = 0xF8,
    mmp_key_ereof = 0xF9,
    mmp_key_play = 0xFA,
    mmp_key_zoom = 0xFB,
    mmp_key_noname = 0xFC,
    mmp_key_pa1 = 0xFD,
    mmp_key_oem_clear = 0xFE,
} mmp_key;

#endif /* !defined(_MMP_KEY_H) */
