#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stddef.h>

#include <concord/log.h>
#include <concord/discord.h>

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

#include "commands/getmsg.h"
#include "other.h"

struct discord_interaction_response command_getmsg(
    struct discord *client,
    const struct discord_interaction *event
) {
    struct discord_interaction_response response = {0};
    struct discord_interaction_callback_data *data = malloc(
        sizeof(struct discord_interaction_callback_data)
    );
    memset(data, 0, sizeof(struct discord_interaction_callback_data));
    response.type = DISCORD_INTERACTION_CHANNEL_MESSAGE_WITH_SOURCE;
    response.data = data;

    u64snowflake message_id, channel_id; 

    errno = 0;
    message_id = strtoull(
        event->data->options->array[0].value,
        NULL,
        10
    );
    if (errno || !string_isnumber(event->data->options->array[0].value))
    {
        data->content = stralloc("Invalid MessageID");
        data->flags |= DISCORD_MESSAGE_EPHEMERAL;
        return response;
    }

    if (event->data->options->size == 1)
    {
        channel_id = event->channel_id;
    }
    else
    {
        errno = 0;
        channel_id = strtoull(
            event->data->options->array[1].value,
            NULL,
            10
        );
        if (errno || !string_isnumber(event->data->options->array[1].value))
        {
            data->content = stralloc("Invalid ChannelID");
            data->flags |= DISCORD_MESSAGE_EPHEMERAL;
            return response;
        }
    }

    struct discord_message message = {0};
    struct discord_ret_message ret = {.sync = &message};

    CCORDcode code = discord_get_channel_message(
        client, channel_id,
        message_id, &ret
    );
    if (code != CCORD_OK)
    {
        data->content = stralloc(discord_strerror(code, client));
        data->flags |= DISCORD_MESSAGE_EPHEMERAL;
        return response;
    }

    char *content_before = malloc(DISCORD_MAX_MESSAGE_LEN);
    data->content = malloc(DISCORD_MAX_MESSAGE_LEN);
    snprintf(
        data->content,
        DISCORD_MAX_MESSAGE_LEN,
        "User %s sent [message](https://discord.com/channels/%llu/%llu/%llu) <t:%llu:D><t:%llu:T>\n%s",
        message.author->username,
        message.guild_id, message.channel_id, message.id,
        message.timestamp / 1000, message.timestamp / 1000,
        message.content
    );

    int error;
    PCRE2_SIZE error_ofs;
    size_t message_len = DISCORD_MAX_MESSAGE_LEN;
    pcre2_code *re = pcre2_compile(
        "<@(\\d+)>",
        PCRE2_ZERO_TERMINATED,
        0,
        &error,
        &error_ofs,
        NULL
    );
    pcre2_substitute(
        re,
        data->content,
        PCRE2_ZERO_TERMINATED,
        0,
        PCRE2_SUBSTITUTE_GLOBAL,
        NULL,
        NULL,
        "<ping>",
        PCRE2_ZERO_TERMINATED,
        content_before,
        &message_len
    );
    pcre2_code_free(re);

    re = pcre2_compile(
        "<@&(\\d+)>",
        PCRE2_ZERO_TERMINATED,
        0,
        &error,
        &error_ofs,
        NULL
    );
    pcre2_substitute(
        re,
        content_before,
        PCRE2_ZERO_TERMINATED,
        0,
        PCRE2_SUBSTITUTE_GLOBAL,
        NULL,
        NULL,
        "<role ping>",
        PCRE2_ZERO_TERMINATED,
        data->content,
        &message_len
    );
    pcre2_code_free(re);

    discord_message_cleanup(&message);
    return response;
}
