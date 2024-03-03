#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <concord/log.h>
#include <concord/discord.h>

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
        (char **)NULL,
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
        /* TODO: write hadnler for second argument */
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

    data->content = malloc(DISCORD_MAX_MESSAGE_LEN);
    snprintf(
        data->content,
        DISCORD_MAX_MESSAGE_LEN,
        "User %s sent [message](https://discord.com/channels/%llu/%llu/%llu) <t:%llu:D><t:%llu:T>\n%s",
        message.author->username,
        message.guild_id, message.channel_id, message.id,
        message.timestamp, message.timestamp,
        message.content
    );

    discord_message_cleanup(&message);
    return response;
}
