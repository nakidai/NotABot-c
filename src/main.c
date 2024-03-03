#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <concord/discord.h>
#include <concord/log.h>

#include "config.h"

#define REG_COMMAND(PARAMS)                                        \
    discord_create_global_application_command(                     \
        client, event->application->id,                            \
        &PARAMS,                                                   \
        NULL                                                       \
    )

#define COMMAND(NAME, FUNCTION)                                    \
    if (!strcmp(event->data->name, NAME))                          \
    {                                                              \
        response = FUNCTION(client, event);                        \
        discord_create_interaction_response(                       \
            client, event->id, event->token,                       \
            &response, NULL                                        \
        );                                                         \
        discord_interaction_response_cleanup(&response);           \
    }

/* Commands */
#include "commands.h"

void on_ready(
    struct discord *client,
    const struct discord_ready *event
) {
    log_info("Starting...");


    log_info("Creating commands...");
    struct discord_create_global_application_command params;

    params = (struct discord_create_global_application_command)
    {
        .name = "getmsg",
        .description = "Get message",
        .options = &(struct discord_application_command_options)
        {
            .size = 2,
            .array = (struct discord_application_command_option[])
            {
                {
                    .name = "message_id",
                    .description = "Message ID",
                    .type = DISCORD_APPLICATION_OPTION_STRING,
                    .required = true
                },
                {
                    .name = "channel_id",
                    .description = "Channel ID (current if not specified)",
                    .type = DISCORD_APPLICATION_OPTION_STRING,
                    .required = false
                }
            }
        }
    };
    REG_COMMAND(params);


    log_info("NotABot succesfully started!");
}

void on_interaction(
    struct discord *client,
    const struct discord_interaction *event
) {
    if (event->type != DISCORD_INTERACTION_APPLICATION_COMMAND)
        return;

    struct discord_interaction_response response;
    COMMAND("getmsg", command_getmsg)
}

int main(void)
{
    ccord_global_init();
    struct discord *client = discord_config_init(CONFIG_PATH);

    discord_set_on_ready(client, &on_ready);
    discord_set_on_interaction_create(client, &on_interaction);

    discord_run(client);

    discord_cleanup(client);
    ccord_global_cleanup();
}
