#ifndef __COMMANDS_GETMSG_H__
#define __COMMANDS_GETMSG_H__

#include <concord/discord.h>

struct discord_interaction_response command_getmsg(
    struct discord *client,
    const struct discord_interaction *event
);

#endif /* __COMMANDS_GETMSG_H__ */
