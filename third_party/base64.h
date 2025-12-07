#pragma once

#include "../core/header.h"
#include <openssl/ssl.h>

//
// encode/decode by using openssl:
//    std::string base64Decode (const std::string& strData)
//    std::string base64Encode (const std::string & strData)
// encode/decode without third-party lib:
//    std::string base64_encode (const std::string& decoded_string)
//    std::string base64_decode (const std::string& encoded_string)
//

/*
 *
 *      base64 encode/decode by using openssl lib
 *
 * */

std::string base64Decode(const std::string& strData)
{
    std::string decodedString;
    BIO *       b64 = nullptr, *bmem = nullptr;
    try {
        size_t len = strData.length();
        b64        = BIO_new(BIO_f_base64());
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
        bmem = BIO_new_mem_buf((void*)strData.data(), len);
        bmem = BIO_push(b64, bmem);

        char inbuf[512];
        int  inlen;
        while ((inlen = BIO_read(bmem, inbuf, sizeof(inbuf))) > 0) {
            decodedString += std::string(inbuf, inlen);
        }
    } catch (...) {
        if (bmem) {
            BIO_free_all(bmem);
        }
        throw;
    }
    BIO_free_all(bmem);
    return decodedString;
}

std::string base64Encode(const std::string& strData)
{
    std::string result;

    BIO *    bmem, *b64;
    BUF_MEM* bptr;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bmem = BIO_new(BIO_s_mem());
    b64  = BIO_push(b64, bmem);
    BIO_write(b64, strData.data(), strData.size());
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    result = std::string(bptr->data, bptr->length);

    BIO_free_all(b64);

    return result;
}

/*
 *
 *      base64 encode/decode without using third-party lib
 *      http://www.cplusplus.com/forum/beginner/51572/
 * */

static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                        "abcdefghijklmnopqrstuvwxyz"
                                        "0123456789+/";

static bool is_base64(unsigned char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(const std::string& decoded_string)
{
    size_t        in_len  = decoded_string.size();
    const size_t  ori_len = in_len;
    std::string   ret;
    int           i = 0;
    int           j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = decoded_string.at(ori_len - in_len - 1);

        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';
    }

    return ret;
}

std::string base64_decode(const std::string& encoded_string)
{
    size_t        in_len = encoded_string.size();
    size_t        i      = 0;
    size_t        j      = 0;
    int           in_    = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string   ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = static_cast<unsigned char>(base64_chars.find(char_array_4[i]));

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++)
            ret += char_array_3[j];
    }

    return ret;
}

