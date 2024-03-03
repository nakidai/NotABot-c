#include <string.h>
#include <concord/discord.h>
#include <concord/log.h>

#include "config.h"

static u64snowflake g_app_id;

void on_ready(
    struct discord *client,
    const struct discord_ready *event
) {
    log_info("NotABot succesfully started!");
    g_app_id = event->application->id;
}

void on_interaction(
    struct discord *client,
    const struct discord_interaction *event
) {
    if (event->type != DISCORD_INTERACTION_APPLICATION_COMMAND)
        return; /* return if interaction isn't a slash command */
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
