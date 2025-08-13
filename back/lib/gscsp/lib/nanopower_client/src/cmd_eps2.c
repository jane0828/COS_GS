/* Copyright (c) 2013-2019 GomSpace A/S. All rights reserved. */
/**
 * NanoPower firmware
 */

#include <inttypes.h>

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <io/nanopower2.h>
#include <csp/csp.h>
#include <csp/csp_endian.h>

#include <util/console.h>
#include <util/log.h>

#ifdef NANOPOWER_ENABLE_CONFIG_COMMANDS
eps_config_t nanopower_config;
eps_config2_t nanopower_config2;
eps_config3_t nanopower_config3;

static uint8_t conf_valid = 0;
static uint8_t conf2_valid = 0;
static uint8_t conf3_valid = 0;

static int config_edit_value(int signed_val, int * edited, int value)
{
    int ret __attribute__((unused));

    printf(" %d > ", value);
    fflush(stdout);
    if (signed_val) {
        int scan;
        if ((ret = scanf("%d%*[^\r]", &scan)) == 1) {
            value = scan;
        }
    } else {
        unsigned int scan;
        if ((ret = scanf("%u%*[^\r]", &scan)) == 1) {
            value = scan;
        }
    }
    if (edited) {
        *edited = ret;
    }
    printf(" [%d] > %d\r\n", ret, value);
    ret = scanf("%*c");
    // cppcheck-suppress fflushOnInputStream
    fflush(stdin);

    return value;
}
#endif

int cmd_eps_node(struct command_context *ctx)
{
    if (ctx->argc != 2) {
        return CMD_ERROR_SYNTAX;
    }
    eps_set_node(atoi(ctx->argv[1]));
    return CMD_ERROR_NONE;
}

int cmd_eps_hk_get(struct command_context *ctx)
{
    eps_hk_t hk;
    memset(&hk, 0, sizeof(hk));
    if (eps_hk_get(&hk) > 0) {
        eps_hk_print(&hk);
    } else {
        return CMD_ERROR_FAIL;
    }
    return CMD_ERROR_NONE;
}

int cmd_eps_hk_vi_get(struct command_context *ctx)
{
    eps_hk_vi_t hk;
    memset(&hk, 0, sizeof(hk));
    if (eps_hk_vi_get(&hk) > 0) {
        eps_hk_vi_print_text(&hk);
    } else {
        return CMD_ERROR_FAIL;
    }
    return CMD_ERROR_NONE;
}

int cmd_eps_hk_out_get(struct command_context *ctx)
{
    eps_hk_out_t hk;
    memset(&hk, 0, sizeof(hk));
    if (eps_hk_out_get(&hk) > 0) {
        eps_hk_out_print_text(&hk);
    } else {
        return CMD_ERROR_FAIL;
    }
    return CMD_ERROR_NONE;
}

int cmd_eps_hk_wdt_get(struct command_context *ctx)
{
    eps_hk_wdt_t hk;
    memset(&hk, 0, sizeof(hk));
    if (eps_hk_wdt_get(&hk) > 0) {
        eps_hk_wdt_print_text(&hk);
    } else {
        return CMD_ERROR_FAIL;
    }
    return CMD_ERROR_NONE;
}

int cmd_eps_hk_basic_get(struct command_context *ctx)
{
    eps_hk_basic_t hk;
    memset(&hk, 0, sizeof(hk));
    if (eps_hk_basic_get(&hk) > 0) {
        eps_hk_basic_print_text(&hk);
    } else {
        return CMD_ERROR_FAIL;
    }
    return CMD_ERROR_NONE;
}

#ifdef NANOPOWER_ENABLE_CONFIG_COMMANDS
int cmd_eps_config_get(struct command_context *ctx)
{
    if (eps_config_get(&nanopower_config) > 0) {
        eps_config_print(&nanopower_config);
        conf_valid = 1;
    } else {
        return CMD_ERROR_FAIL;
    }
    return CMD_ERROR_NONE;
}

int cmd_eps_config_print(struct command_context *ctx)
{
    eps_config_print(&nanopower_config);
    return CMD_ERROR_NONE;
}

int cmd_eps_config_set(struct command_context *ctx)
{
    if (!conf_valid) {
        return CMD_ERROR_FAIL;
    }
    eps_config_t sendconfig;
    memcpy(&sendconfig, &nanopower_config,sizeof(eps_config_t));
    if (eps_config_set(&sendconfig) < 0) {
        return CMD_ERROR_FAIL;
    }
    return CMD_ERROR_NONE;
}

int cmd_eps_config_restore(struct command_context *ctx)
{
    eps_config_cmd(1);
    return CMD_ERROR_NONE;
}

int cmd_eps_config_confirm(struct command_context *ctx)
{
    if (!conf_valid) {
        return CMD_ERROR_FAIL;
    }
    eps_config_cmd(2);
    return CMD_ERROR_NONE;
}

int cmd_eps_config_edit(struct command_context *ctx)
{
    int i;
    int ret __attribute__((unused));

    printf("Edit EPS configuration\r\n");
    printf("Press <.> to skip\r\n");

    printf("PPT mode: %u > ", nanopower_config.ppt_mode);
    fflush(stdout);
    {
        unsigned int scan;
        if ((ret = scanf("%u%*[^\r]", &scan)) == 1) {
            nanopower_config.ppt_mode = scan;
        }
    }
    printf(" [%d] > %u\r\n", ret, nanopower_config.ppt_mode);
    ret = scanf("%*c");

    printf("Battheater mode: %u > ", nanopower_config.battheater_mode);
    fflush(stdout);
    {
        unsigned int scan;
        if ((ret = scanf("%u%*[^\r]", &scan)) == 1) {
            nanopower_config.battheater_mode = scan;
        }
    }
    printf(" [%d] > %u\r\n", ret, nanopower_config.battheater_mode);
    ret = scanf("%*c");

    printf("Battheater LOW: %d > ", nanopower_config.battheater_low);
    fflush(stdout);
    {
        int scan;
        if ((ret = scanf("%d%*[^\r]", &scan)) == 1) {
            nanopower_config.battheater_low = scan;
        }
    }
    printf(" [%d] > %d\r\n", ret, nanopower_config.battheater_low);
    ret = scanf("%*c");

    /* PPT mode */
    printf("Battheater HIGH: %d > ", nanopower_config.battheater_high);
    fflush(stdout);
    {
        int scan;
        if ((ret = scanf("%d%*[^\r]", &scan)) == 1) {
            nanopower_config.battheater_high = scan;
        }
    }
    printf(" [%d] > %d\r\n", ret, nanopower_config.battheater_high);
    ret = scanf("%*c");

    /* VBOOST [3] */
    printf("VBOOST voltage[3]\r\n");
    for (i = 0; i < 3; i++) {
        printf("vboost[i] %u > ", nanopower_config.vboost[i]);
        fflush(stdout);
        unsigned int scan;
        if ((ret = scanf("%u%*[^\r]", &scan)) == 1) {
            nanopower_config.vboost[i] = scan;
        }
        printf(" [%d] > %u\r\n", ret, nanopower_config.vboost[i]);
        ret = scanf("%*c");
    }

    /* OUTPUT[8] */
    for (i = 0; i < 8; i++) {
        printf("OUTPUT[%d]\r\n", i);

        /* Normal Mode */
        printf("Startup value[%d] %u > ", i, nanopower_config.output_normal_value[i]);
        fflush(stdout);
        unsigned int scan;
        if ((ret = scanf("%u%*[^\r]", &scan)) == 1) {
            nanopower_config.output_normal_value[i] = scan;
        }
        printf(" [%d] > %u\r\n", ret, nanopower_config.output_normal_value[i]);
        ret = scanf("%*c");

        /* Safe Mode */
        printf("Safe value[%d] %u > ", i, nanopower_config.output_safe_value[i]);
        fflush(stdout);
        if ((ret = scanf("%u%*[^\r]", &scan)) == 1) {
            nanopower_config.output_safe_value[i] = scan;
        }
        printf(" [%d] > %u\r\n", ret, nanopower_config.output_safe_value[i]);
        ret = scanf("%*c");

        /* ON delay */
        printf("ON-delay[%d] %u > ", i, nanopower_config.output_initial_on_delay[i]);
        fflush(stdout);
        if ((ret = scanf("%u%*[^\r]", &scan)) == 1) {
            nanopower_config.output_initial_on_delay[i] = scan;
        }
        printf(" [%d] > %u\r\n", ret, nanopower_config.output_initial_on_delay[i]);
        ret = scanf("%*c");

        /* OFF delay */
        printf("OFF-delay[%d] %u > ", i, nanopower_config.output_initial_off_delay[i]);
        fflush(stdout);
        if ((ret = scanf("%u%*[^\r]", &scan)) == 1) {
            nanopower_config.output_initial_off_delay[i] = scan;
        }
        printf(" [%d] > %u\r\n\r\n", ret, nanopower_config.output_initial_off_delay[i]);
        ret = scanf("%*c");
    }

    conf_valid = 1;
    eps_config_print(&nanopower_config);

    return CMD_ERROR_NONE;
}

int cmd_eps_config_heater(struct command_context *ctx)
{
    if (!conf_valid) {
        printf("Must be called after eps conf get or edit\r\n");
        return CMD_ERROR_FAIL;
    }
    if (ctx->argc != 4) {
        return CMD_ERROR_SYNTAX;
    }
    nanopower_config.battheater_mode = atoi(ctx->argv[1]);
    nanopower_config.battheater_low = atoi(ctx->argv[2]);
    nanopower_config.battheater_high = atoi(ctx->argv[3]);
    eps_config_print(&nanopower_config);
    return CMD_ERROR_NONE;
}

int cmd_eps_config_output(struct command_context *ctx)
{
    if (!conf_valid) {
        printf("Must be called after eps conf get or edit\r\n");
        return CMD_ERROR_FAIL;
    }
    if (ctx->argc != 6) {
        return CMD_ERROR_SYNTAX;
    }
    unsigned int chan = atoi(ctx->argv[1]);
    if (chan <= 7) {
        nanopower_config.output_normal_value[chan] = atoi(ctx->argv[2]);
        nanopower_config.output_safe_value[chan] = atoi(ctx->argv[3]);
        nanopower_config.output_initial_on_delay[chan] = atoi(ctx->argv[4]);
        nanopower_config.output_initial_off_delay[chan] = atoi(ctx->argv[5]);
        eps_config_print(&nanopower_config);
        return CMD_ERROR_NONE;
    } else {
        return CMD_ERROR_SYNTAX;
    }
}

int cmd_eps_config2_get(struct command_context *ctx)
{
    if (eps_config2_get(&nanopower_config2) > 0) {
        eps_config2_print(&nanopower_config2);
        conf2_valid = 1;
    } else {
        return CMD_ERROR_FAIL;
    }
    return CMD_ERROR_NONE;
}

int cmd_eps_config2_print(struct command_context *ctx)
{
    eps_config2_print(&nanopower_config2);
    return CMD_ERROR_NONE;
}

int cmd_eps_config2_set(struct command_context *ctx)
{
    if (!conf2_valid) {
        return CMD_ERROR_FAIL;
    }
    eps_config2_t sendconfig2;
    memcpy(&sendconfig2, &nanopower_config2, sizeof(eps_config2_t));
    if (eps_config2_set(&nanopower_config2) < 0) {
        return CMD_ERROR_FAIL;
    }
    return CMD_ERROR_NONE;
}

int cmd_eps_config2_restore(struct command_context *ctx)
{
    eps_config2_cmd(1);
    return CMD_ERROR_NONE;
}

int cmd_eps_config2_confirm(struct command_context *ctx)
{
    if (!conf2_valid) {
        return CMD_ERROR_FAIL;
    }
    eps_config2_cmd(2);
    return CMD_ERROR_NONE;
}

int cmd_eps_config2_edit(struct command_context *ctx)
{
    unsigned int scan;
    int ret __attribute__((unused));

    if (ctx->argc == 5) {
        unsigned int voltage;

        voltage = atoi(ctx->argv[1]);
        nanopower_config2.batt_maxvoltage = voltage;

        voltage = atoi(ctx->argv[2]);
        nanopower_config2.batt_normalvoltage = voltage;

        voltage = atoi(ctx->argv[3]);
        nanopower_config2.batt_safevoltage = voltage;

        voltage = atoi(ctx->argv[4]);
        nanopower_config2.batt_criticalvoltage = voltage;
    } else {
        printf("Edit EPS configuration 2\r\n");
        printf("Press <.> to skip\r\n");
        printf("You are about to change battery level voltages.\r\n");
        printf("Make sure that: maxvolt > normalvolt > safevolt > criticalvolt \r\n\r\n");

        printf("Batt max volt: %u > ", nanopower_config2.batt_maxvoltage);
        fflush(stdout);
        if ((ret = scanf("%u%*[^\r]", &scan)) == 1) {
            nanopower_config2.batt_maxvoltage = scan;
        }
        printf(" [%d] > %u\r\n", ret, nanopower_config2.batt_maxvoltage);
        printf("You are trying to set battery protection maximum voltage to %u mV\r\n",nanopower_config2.batt_maxvoltage);
        printf("It is VERY important that you NEVER set the max voltage to more than what the installed battery can handle!!!!\r\n\r\n");

        ret = scanf("%*c");
        // cppcheck-suppress fflushOnInputStream
        fflush(stdin);
        printf("Batt normal volt: %u > ", nanopower_config2.batt_normalvoltage);
        fflush(stdout);
        if ((ret = scanf("%u%*[^\r]", &scan)) == 1) {
            nanopower_config2.batt_normalvoltage = scan;
        }
        printf(" [%d] > %u\r\n", ret, nanopower_config2.batt_normalvoltage);
        ret = scanf("%*c");
        // cppcheck-suppress fflushOnInputStream
        fflush(stdin);

        printf("Batt safe volt: %u > ", nanopower_config2.batt_safevoltage);
        fflush(stdout);
        if ((ret = scanf("%u%*[^\r]", &scan)) == 1) {
            nanopower_config2.batt_safevoltage = scan;
        }
        printf(" [%d] > %u\r\n", ret, nanopower_config2.batt_safevoltage);
        ret = scanf("%*c");
        // cppcheck-suppress fflushOnInputStream
        fflush(stdin);

        printf("Batt critical volt: %u > ", nanopower_config2.batt_criticalvoltage);
        fflush(stdout);
        if ((ret = scanf("%u%*[^\r]", &scan)) == 1) {
            nanopower_config2.batt_criticalvoltage = scan;
        }
        printf(" [%d] > %u\r\n", ret, nanopower_config2.batt_criticalvoltage);
        ret = scanf("%*c");
        // cppcheck-suppress fflushOnInputStream
        fflush(stdin);
    }
    if (nanopower_config2.batt_maxvoltage <= nanopower_config2.batt_normalvoltage) {
        printf("\r\nWarning, you  have set max volt to less that normal volt, please change this!\r\n");
        conf2_valid = 0;
    } else if (nanopower_config2.batt_normalvoltage <= nanopower_config2.batt_safevoltage) {
        printf("\r\nWarning, you  have set normal volt to less that safe volt, please change this!\r\n");
        conf2_valid = 0;
    } else if (nanopower_config2.batt_safevoltage <= nanopower_config2.batt_criticalvoltage) {
        printf("\r\nWarning, you  have set safe volt to less that critical volt, please change this!\r\n");
        conf2_valid = 0;
    } else {
        conf2_valid = 1;
    }
    eps_config2_print(&nanopower_config2);
    return CMD_ERROR_NONE;
}

int cmd_eps_config3_get(struct command_context *ctx)
{
    if (eps_config3_get(&nanopower_config3) > 0) {
        eps_config3_print(&nanopower_config3);
        conf3_valid = 1;
    } else {
        return CMD_ERROR_FAIL;
    }
    return CMD_ERROR_NONE;
}

int cmd_eps_config3_print(struct command_context *ctx)
{
    eps_config3_print(&nanopower_config3);
    return CMD_ERROR_NONE;
}

int cmd_eps_config3_set(struct command_context *ctx)
{
    if (!conf3_valid) {
        return CMD_ERROR_FAIL;
    }
    eps_config3_t sendconfig3;
    memcpy(&sendconfig3, &nanopower_config3, sizeof(eps_config3_t));
    if (eps_config3_set(&nanopower_config3) < 0) {
        return CMD_ERROR_FAIL;
    }
    return CMD_ERROR_NONE;
}

int cmd_eps_config3_restore(struct command_context *ctx)
{
    eps_config3_cmd(EPS_CONFIG_CMD_RESTORE);
    return CMD_ERROR_NONE;
}

int cmd_eps_config3_confirm(struct command_context *ctx)
{
    if (!conf3_valid) {
        return CMD_ERROR_FAIL;
    }
    eps_config3_cmd(EPS_CONFIG_CMD_CONFIRM);
    return CMD_ERROR_NONE;
}

int cmd_eps_config3_edit(struct command_context *ctx)
{
    int ret;

    printf("Edit EPS configuration 3\r\n");
    printf("Press <.> to skip\r\n");
    for (int i = 0; i < 8; i++) {
        printf("CUR_LIM[%d] (0 - 2500 mA, 0 to disable)\r\n", i);
        printf("Current limit value[%d]", i);
        nanopower_config3.cur_lim[i] = config_edit_value(0, &ret, nanopower_config3.cur_lim[i]);
    }
    printf("Current EMA gain (10-90)");
    nanopower_config3.cur_ema_gain = config_edit_value(0, &ret, nanopower_config3.cur_ema_gain);

    for (int i = 0; i < 2; i++) {
        printf("CSP Wdt channel[%d] (0-5)", i);
        nanopower_config3.cspwdt_channel[i] = config_edit_value(0, &ret, nanopower_config3.cspwdt_channel[i]);
        printf("CSP Wdt address[%d] (0 to disable)", i);
        nanopower_config3.cspwdt_address[i] = config_edit_value(0, &ret, nanopower_config3.cspwdt_address[i]);
    }
    conf3_valid = 1;
    for (int i = 0; i < 8; i++) {
        if (nanopower_config3.cur_lim[i] > 2500) {
            printf("\r\nWarning, you  have set current limit[%d] > 2500 mA, please change this!\r\n", i);
            conf3_valid = 0;
        }
    }
    if ((nanopower_config3.cur_ema_gain < 10) || (nanopower_config3.cur_ema_gain > 90)) {
        printf("\r\nWarning, you  have set current EMA gain outside range [10 - 90], please change this!\r\n");
        conf3_valid = 0;
    }
    eps_config3_print(&nanopower_config3);
    return CMD_ERROR_NONE;
}

int cmd_eps_config3_current_limits(struct command_context *ctx)
{
    if (!conf3_valid) {
        printf("Must be called after eps conf3 get or edit\r\n");
        return CMD_ERROR_FAIL;
    }
    if (ctx->argc != 3) {
        printf ("ctx->argc : %d\r\n" , ctx->argc);
        return CMD_ERROR_SYNTAX;
    }

    unsigned int chan = atoi(ctx->argv[1]);
    unsigned int limit = atoi(ctx->argv[2]);
    if ((chan <= 7) && (limit <= 2500)) {
        nanopower_config3.cur_lim[chan] = limit;
        eps_config3_print(&nanopower_config3);
        return CMD_ERROR_NONE;
    } else {
        return CMD_ERROR_SYNTAX;
    }
}

int cmd_eps_config3_ema(struct command_context *ctx)
{
    if (!conf3_valid) {
        printf("Must be called after eps conf3 get or edit\r\n");
        return CMD_ERROR_FAIL;
    }
    if (ctx->argc != 2) {
        printf ("ctx->argc : %d\r\n" , ctx->argc);
        return CMD_ERROR_SYNTAX;
    }
    unsigned int ema_gain = atoi(ctx->argv[1]);

    if ((ema_gain < 10) || (ema_gain > 90)) {
        return CMD_ERROR_SYNTAX;
    } else {
        nanopower_config3.cur_ema_gain = ema_gain;
        eps_config3_print(&nanopower_config3);
    }
    return CMD_ERROR_NONE;
}

int cmd_eps_config3_cspwdt(struct command_context *ctx)
{
    if (!conf3_valid) {
        printf("Must be called after eps conf3 get or edit\r\n");
        return CMD_ERROR_FAIL;
    }
    if (ctx->argc != 4) {
        printf ("ctx->argc : %d\r\n" , ctx->argc);
        return CMD_ERROR_SYNTAX;
    }

    unsigned int wdt = atoi(ctx->argv[1]);
    unsigned int chan = atoi(ctx->argv[2]);
    unsigned int address = atoi(ctx->argv[3]);
    if ((wdt <= 1) && (chan <= 5) && (address <= 31)) {
        nanopower_config3.cspwdt_channel[wdt] = chan;
        nanopower_config3.cspwdt_address[wdt] = address;
        eps_config3_print(&nanopower_config3);
        return CMD_ERROR_NONE;
    } else {
        return CMD_ERROR_SYNTAX;
    }
}
#endif

int cmd_eps_output(struct command_context *ctx)
{
    const char * args = command_args(ctx);
    unsigned int outputs;
    printf("console args: %s\r\n", args);
    if (sscanf(args, "%X", &outputs) != 1) {
        return CMD_ERROR_SYNTAX;
    }

    printf("Outputs 0x%02X\r\n", outputs);

    eps_output_set((uint8_t) outputs);

    return CMD_ERROR_NONE;
}

int cmd_eps_output_set_single(struct command_context *ctx)
{
    const char * args = command_args(ctx);
    unsigned int channel;
    unsigned int mode;
    int delay;
    printf("Input channel, mode (0=off, 1=on), and delay\r\n");
    if (sscanf(args, "%u %u %d", &channel,&mode,&delay) != 3) {
        return CMD_ERROR_SYNTAX;
    }
    printf("Channel %u is set to %u with delay %d\r\n", channel, mode, delay);

    eps_output_set_single((uint8_t) channel, (uint8_t) mode, (int16_t) delay);

    return CMD_ERROR_NONE;
}

int cmd_eps_heater(struct command_context *ctx)
{
    const char * args = command_args(ctx);
    unsigned int channel;
    unsigned int mode;
    uint8_t state[2];

    printf("Input channel, mode (0=off, 1=on)\r\n");
    if (sscanf(args, "%u %u", &channel,&mode) != 2) {
        return CMD_ERROR_SYNTAX;
    }
    printf("Setting heater %u to %u\r\n", channel, mode);

    eps_heater((uint8_t) channel, (uint8_t) mode, state);
    printf("Heater state is %u %u\r\n",(unsigned int) state[0],(unsigned int) state[1]);

    return CMD_ERROR_NONE;
}

int cmd_eps_counters_reset(struct command_context *ctx)
{
    eps_counters_reset();
    return CMD_ERROR_NONE;
}

int cmd_eps_hardreset(struct command_context *ctx)
{
    eps_hardreset();
    return CMD_ERROR_NONE;
}

int cmd_eps_gnd_reset(struct command_context *ctx)
{
    eps_wdt_gnd_reset();
    return CMD_ERROR_NONE;
}

#ifdef NANOPOWER_ENABLE_CONFIG_COMMANDS
int cmd_eps_ppt_set(struct command_context *ctx)
{
    if (ctx->argc != 2) {
        return CMD_ERROR_SYNTAX;
    }
    unsigned char pptmode = atoi(ctx->argv[1]);
    eps_pptmode_set(pptmode);

    if (conf_valid == 1) {
        nanopower_config.ppt_mode = pptmode;

        printf("Local config settings:\r\n");
        eps_config_print(&nanopower_config);
    }

    return CMD_ERROR_NONE;
}

int cmd_eps_vboost_set(struct command_context *ctx)
{
    if (ctx->argc != 4) {
        return CMD_ERROR_SYNTAX;
    }
    unsigned int vboost1 = atoi(ctx->argv[1]);
    unsigned int vboost2 = atoi(ctx->argv[2]);
    unsigned int vboost3 = atoi(ctx->argv[3]);
    eps_vboost_set(vboost1, vboost2, vboost3);

    if (conf_valid == 1) {
        for (int i=0;i<3;i++) {
            unsigned int vboost = atoi(ctx->argv[i+1]);
            nanopower_config.vboost[i] = vboost;
        }
        printf("Local config settings:\r\n");
        eps_config_print(&nanopower_config);
    }

    return CMD_ERROR_NONE;
}

static const gs_command_t GS_COMMAND_SUB eps_config_subcommands[] = {
    {
        .name = "get",
        .help = "Conf get",
        .handler = cmd_eps_config_get,
    },{
        .name = "set",
        .help = "Conf set",
        .handler = cmd_eps_config_set,
    },{
        .name = "edit",
        .help = "Edit local config",
        .handler = cmd_eps_config_edit,
    },{
        .name = "print",
        .help = "Print local config",
        .handler = cmd_eps_config_print,
    },{
        .name = "restore",
        .help = "Restore config from default",
        .handler = cmd_eps_config_restore,
    },{
        .name = "confirm",
        .help = "Confirm config",
        .handler = cmd_eps_config_confirm,
    },{
        .name = "heater",
        .help = "edit batt heater settings",
        .usage = "<mode (0=off)> <LOW temp> <HIGH temp>",
        .handler = cmd_eps_config_heater,
    },{
        .name = "output",
        .help = "edit output channel settings",
        .usage = "<power channel (0-7)> <Startup value> <Safe value> <ON-delay> <OFF-delay>",
        .handler = cmd_eps_config_output,
    }
};

static const gs_command_t GS_COMMAND_SUB eps_config2_subcommands[] = {
    {
        .name = "get",
        .help = "Conf 2 get",
        .handler = cmd_eps_config2_get,
    },{
        .name = "set",
        .help = "Conf 2 set",
        .handler = cmd_eps_config2_set,
    },{
        .name = "edit",
        .help = "Edit local config 2",
        .usage = "[<max_voltage> <normal_voltage> <safe_voltage> <critical_voltage>]",
        .handler = cmd_eps_config2_edit,
    },{
        .name = "print",
        .help = "Print local config 2",
        .handler = cmd_eps_config2_print,
    },{
        .name = "restore",
        .help = "Restore config 2 from default",
        .handler = cmd_eps_config2_restore,
    },{
        .name = "confirm",
        .help = "Confirm config 2",
        .handler = cmd_eps_config2_confirm,
    }
};

static const gs_command_t GS_COMMAND_SUB eps_config3_subcommands[] = {
    {
        .name = "get",
        .help = "Conf 3 get",
        .handler = cmd_eps_config3_get,
    },{
        .name = "set",
        .help = "Conf 3 set",
        .handler = cmd_eps_config3_set,
    },{
        .name = "edit",
        .help = "Edit local config 3",
        .handler = cmd_eps_config3_edit,
    },{
        .name = "print",
        .help = "Print local config 3",
        .handler = cmd_eps_config3_print,
    },{
        .name = "restore",
        .help = "Restore config 3 from default",
        .handler = cmd_eps_config3_restore,
    },{
        .name = "confirm",
        .help = "Confirm config 3",
        .handler = cmd_eps_config3_confirm,
    },{
        .name = "currlim",
        .help = "edit current limits (max 2500mA)",
        .usage = "<power channel (0-7)> <current limit (0-2500)mA>",
        .handler = cmd_eps_config3_current_limits,
    },{
        .name = "ema",
        .help = "edit EMA gain setting",
        .usage = "<EMA gain (10-90)>",
        .handler = cmd_eps_config3_ema,
    },{
        .name = "cspwdt",
        .help = "edit CSP WDT settings",
        .usage = "<WDT channel (0/1)> <channel (0-5)> <CSP address (0-31, 0=off)>",
        .handler = cmd_eps_config3_cspwdt,
    }
};
#endif

static const gs_command_t GS_COMMAND_SUB eps_hk_subcommands[] = {
    {
        .name = "vi",
        .help = "Get Voltages and Currents",
        .handler = cmd_eps_hk_vi_get,
    },{
        .name = "out",
        .help = "Get output switch data",
        .handler = cmd_eps_hk_out_get,
    },{
        .name = "wdt",
        .help = "Get wdt data",
        .handler = cmd_eps_hk_wdt_get,
    },{
        .name = "basic",
        .help = "Get basic housekeeping data",
        .handler = cmd_eps_hk_basic_get,
    }
};

static const gs_command_t GS_COMMAND_SUB eps_subcommands[] = {
    {
        .name = "node",
        .help = "Set EPS address in OBC host table",
        .usage = "<node>",
        .handler = cmd_eps_node,
    },{
        .name = "hk",
        .help = "Get HK",
        .handler = cmd_eps_hk_get,
    },{
        .name = "hksub",
        .help = "Get HK sub structs",
        .chain = INIT_CHAIN(eps_hk_subcommands),
#ifdef NANOPOWER_ENABLE_CONFIG_COMMANDS
    },{
        .name = "conf",
        .help = "Configuration",
        .chain = INIT_CHAIN(eps_config_subcommands),
    },{
        .name = "conf2",
        .help = "Configuration 2",
        .chain = INIT_CHAIN(eps_config2_subcommands),
    },{
        .name = "conf3",
        .help = "Configuration 3",
        .chain = INIT_CHAIN(eps_config3_subcommands),
#endif
    },{
        .name = "outputmask",
        .help = "Set on/off, argument hex value of output char",
        .usage = "<outputs>",
        .handler = cmd_eps_output,
    },{
        .name = "output",
        .help = "Set channel on/off, argument (channel) to (1 or 0)",
        .usage = "<channel> <mode> <delay>",
        .handler = cmd_eps_output_set_single,
    },{
        .name = "resetcounters",
        .help = "Reset all counters to zero",
        .handler = cmd_eps_counters_reset,
    },{
        .name = "resetwdt",
        .help = "Resets the WDT GND",
        .handler = cmd_eps_gnd_reset,
    },{
        .name = "hardreset",
        .help = "Completely powercycle EPS",
        .handler = cmd_eps_hardreset,
#ifdef NANOPOWER_ENABLE_CONFIG_COMMANDS
    },{
        .name = "ppt",
        .help = "change current PPT mode",
        .usage = "<mode>",
        .handler = cmd_eps_ppt_set,
    },{
        .name = "vboost",
        .help = "change current VBOOST voltage",
        .usage = "<vboost1> <vboost2> <vboost3>",
        .handler = cmd_eps_vboost_set,
#endif
    },{
        .name = "heater",
        .help = "get/set heater state",
        .usage = "<heater> <state>",
        .handler = cmd_eps_heater,
    }
};

static const gs_command_t GS_COMMAND_ROOT eps_commands[] = {
    {
        .name = "eps",
        .help = "NanoPower P31u client",
        .chain = INIT_CHAIN(eps_subcommands),
    }
};

void cmd_eps_setup(void)
{
    command_register(eps_commands);
}
