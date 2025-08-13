#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif

// void * RadInitialize(void * args)
// {
//     Setup * setups = (Setup *) args;
//     // rparam_set_quiet(RPARAM_NOT_QUIET);
//     // rparam_query_reset();
   

//     Load_Paramtable0(NULL);
//     Load_Paramtable1(NULL);
//     Load_Paramtable5(NULL);
//     set_guard_spec_micsec(AX100_5_baud, AX100_1_baud, AX100_5_guard, AX100_1_guard, rAX100_5_guard, rAX100_1_guard);
//     return NULL;
// }

void SGS_RADIAL::RSSI_Monitoring()
{
    gs_rparam_get(this->node, 4, 0x0004, GS_PARAM_INT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &this->ptable4.last_rssi, sizeof(this->ptable4.last_rssi));
}

static void * Load_Paramtable0(void * param)
{
    SGS_RADIAL * rad = (SGS_RADIAL *)param;
    if(!rad)
        return NULL;
    int ErrCnt = 0;

    //Get rssi_offset
    if(gs_rparam_get(rad->node, 0, AX100_0_RSSI_OFFSET_ADDR, GS_PARAM_INT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.rssi_offset, sizeof(rad->ptable0.rssi_offset)) != GS_OK)
        ErrCnt++;
    
    //Get max_temp
    if (gs_rparam_get(rad->node, 0, AX100_0_MAX_TEMP_ADDR, GS_PARAM_INT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.max_temp, sizeof(rad->ptable0.max_temp)) != GS_OK)
        ErrCnt++;
    //Get bgndrssi_ema
    if (gs_rparam_get(rad->node, 0, AX100_0_BGNDRSSL_EMA_ADDR, GS_PARAM_FLOAT, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.bgndrssl_ema, sizeof(rad->ptable0.bgndrssl_ema)) != GS_OK)
        ErrCnt++;

    //Get csp_node
    if (gs_rparam_get(rad->node, 0, AX100_0_CSP_NODE_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.csp_node, sizeof(rad->ptable0.csp_node)) != GS_OK)
        ErrCnt++;

    //Get i2c_en
    if (gs_rparam_get(rad->node, 0, AX100_0_I2C_EN_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.i2c_en, sizeof(rad->ptable0.i2c_en)) != GS_OK)
        ErrCnt++;

    //Get can_en
    if (gs_rparam_get(rad->node, 0, AX100_0_CAN_EN_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.can_en, sizeof(rad->ptable0.can_en)) != GS_OK)
        ErrCnt++;

    //Get extptt_en
    if (gs_rparam_get(rad->node, 0, AX100_0_EXTPTT_EN_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.extptt_en, sizeof(rad->ptable0.extptt_en)) != GS_OK)
        ErrCnt++;

    //Get led_en
    if (gs_rparam_get(rad->node, 0, AX100_0_LED_EN_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.led_en, sizeof(rad->ptable0.led_en)) != GS_OK)
        ErrCnt++;
    
    //Get kiss_usart
    if (gs_rparam_get(rad->node, 0, AX100_0_KISS_USART_ADDR, GS_PARAM_INT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.kiss_usart, sizeof(rad->ptable0.kiss_usart)) != GS_OK)
        ErrCnt++;
    
    //Get gosh_usart
    if (gs_rparam_get(rad->node, 0, AX100_0_GOSH_USART_ADDR, GS_PARAM_INT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.gosh_usart, sizeof(rad->ptable0.gosh_usart)) != GS_OK)
        ErrCnt++;

    //Get i2c_addr
    if (gs_rparam_get(rad->node, 0, AX100_0_I2C_ADDR_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.i2c_addr, sizeof(rad->ptable0.i2c_addr)) != GS_OK)
        ErrCnt++;

    //Get i2c_khz
    if (gs_rparam_get(rad->node, 0, AX100_0_I2C_KHZ_ADDR, GS_PARAM_UINT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.i2c_khz, sizeof(rad->ptable0.i2c_khz)) != GS_OK)
        ErrCnt++;

    //Get can_khz
    if (gs_rparam_get(rad->node, 0, AX100_0_CAN_KHZ_ADDR, GS_PARAM_UINT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.can_khz, sizeof(rad->ptable0.can_khz)) != GS_OK)
        ErrCnt++;

    //Get reboot_in
    if (gs_rparam_get(rad->node, 0, AX100_0_REBOOT_IN_ADDR, GS_PARAM_UINT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.reboot_in, sizeof(rad->ptable0.reboot_in)) != GS_OK)
        ErrCnt++;

    //Get tx_inhibit
    if (gs_rparam_get(rad->node, 0, AX100_0_TX_INHIBIT_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.tx_inhibit, sizeof(rad->ptable0.tx_inhibit)) != GS_OK)
        ErrCnt++;

    //Get log_store
    if (gs_rparam_get(rad->node, 0, AX100_0_LOG_STORE_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.log_store, sizeof(rad->ptable0.log_store)) != GS_OK)
        ErrCnt++;

    //Get tx_pwr
    if (gs_rparam_get(rad->node, 0, AX100_0_TX_PWR_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.tx_pwr, sizeof(rad->ptable0.tx_pwr)) != GS_OK)
        ErrCnt++;

    //Get max_tx_time
    if (gs_rparam_get(rad->node, 0, AX100_0_MAX_TX_TIME_ADDR, GS_PARAM_UINT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.max_tx_time, sizeof(rad->ptable0.max_tx_time)) != GS_OK)
        ErrCnt++;

    //Get max_idle_time
    if (gs_rparam_get(rad->node, 0, AX100_0_MAX_IDLE_TIME_ADDR, GS_PARAM_UINT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.max_idle_time, sizeof(rad->ptable0.max_idle_time)) != GS_OK)
        ErrCnt++;

    //Get max_idle_time
    if (gs_rparam_get(rad->node, 0, AX100_0_CSP_RTABLE_ADDR, GS_PARAM_STRING, AX100_CKSUM, AX100_LOCAL_TIMEOUT, rad->ptable0.csp_rtable, sizeof(rad->ptable0.csp_rtable)) != GS_OK)
        ErrCnt++;

   
    if (ErrCnt)
        rad->callback(SGS_CONMOD_ERR, "Unable to load %d parameters of Table 0.", ErrCnt);
    else
        rad->callback(SGS_CONMOD_OK, "Table 0 Load Success.");
    return NULL;
}

static void * Load_Paramtable1(void * param)
{
    SGS_RADIAL * rad = (SGS_RADIAL *)param;
    if(!rad)
        return NULL;
    int ErrCnt = 0;

    //Get freq
    if (gs_rparam_get(rad->node, 1, AX100_1_FREQ_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.freq, sizeof(rad->ptable1.freq)) != GS_OK)
        ErrCnt++;

    //Get baud
    if (gs_rparam_get(rad->node, 1, AX100_1_BAUD_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.baud, sizeof(rad->ptable1.baud)) != GS_OK)
        ErrCnt++;

    //Get modindex
    if (gs_rparam_get(rad->node, 1, AX100_1_MODINDEX_ADDR, GS_PARAM_FLOAT, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.modindex, sizeof(rad->ptable1.modindex)) != GS_OK)
        ErrCnt++;

    //Get guard
    if (gs_rparam_get(rad->node, 1, AX100_1_GUARD_ADDR, GS_PARAM_UINT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.guard, sizeof(rad->ptable1.guard)) != GS_OK)
        ErrCnt++;

    //Get pllrang
    if (gs_rparam_get(rad->node, 1, AX100_1_PLLRANG_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.pllrang, sizeof(rad->ptable1.pllrang)) != GS_OK)
        ErrCnt++;

    //Get mode
    if (gs_rparam_get(rad->node, 1, AX100_1_MODE_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.mode, sizeof(rad->ptable1.mode)) != GS_OK)
        ErrCnt++;

    //Get csp_hmac
    if (gs_rparam_get(rad->node, 1, AX100_1_CSP_HMAC_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.csp_hmac, sizeof(rad->ptable1.csp_hmac)) != GS_OK)
        ErrCnt++;

    //Get csp_rs
    if (gs_rparam_get(rad->node, 1, AX100_1_CSP_RS_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.csp_rs, sizeof(rad->ptable1.csp_rs)) != GS_OK)
        ErrCnt++;

    //Get csp_crc
    if (gs_rparam_get(rad->node, 1, AX100_1_CSP_CRC_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.csp_crc, sizeof(rad->ptable1.csp_crc)) != GS_OK)
        ErrCnt++;

    //Get csp_rand
    if (gs_rparam_get(rad->node, 1, AX100_1_CSP_RAND_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.csp_rand, sizeof(rad->ptable1.csp_rand)) != GS_OK)
        ErrCnt++;

    //Get csp_hmac_key

    //Get ax25_call
    if (gs_rparam_get(rad->node, 1, AX100_1_AX25_CALL_ADDR, GS_PARAM_STRING, AX100_CKSUM, AX100_LOCAL_TIMEOUT, rad->ptable1.ax25_call, sizeof(rad->ptable1.ax25_call)) != GS_OK)
        ErrCnt++;

    //Get bw
    if (gs_rparam_get(rad->node, 1, AX100_1_BW_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.bw, sizeof(rad->ptable1.bw)) != GS_OK)
        ErrCnt++;

    //Get afcrange
    if (gs_rparam_get(rad->node, 1, AX100_1_AFCRANGE_ADDR, GS_PARAM_INT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.afcrange, sizeof(rad->ptable1.afcrange)) != GS_OK)
        ErrCnt++;

    if (ErrCnt)
        rad->callback(SGS_CONMOD_ERR, "Unable to load %d parameters of Table 0.", ErrCnt);
    else
        rad->callback(SGS_CONMOD_OK, "Table 0 Load Success.");
    return NULL;
}

static void * Load_Paramtable5(void * param)
{
    SGS_RADIAL * rad = (SGS_RADIAL *)param;
    if(!rad)
        return NULL;
    int ErrCnt = 0;

    //Get freq
    if (gs_rparam_get(rad->node, 5, AX100_5_FREQ_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.freq, sizeof(rad->ptable5.freq)) != GS_OK)
        ErrCnt++;

    //Get baud
    if (gs_rparam_get(rad->node, 5, AX100_5_BAUD_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.baud, sizeof(rad->ptable5.baud)) != GS_OK)
        ErrCnt++;

    //Get modindex
    if (gs_rparam_get(rad->node, 5, AX100_5_MODINDEX_ADDR, GS_PARAM_FLOAT, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.modindex, sizeof(rad->ptable5.modindex)) != GS_OK)
        ErrCnt++;

    //Get guard
    if (gs_rparam_get(rad->node, 5, AX100_5_GUARD_ADDR, GS_PARAM_UINT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.guard, sizeof(rad->ptable5.guard)) != GS_OK)
        ErrCnt++;

    //Get pllrang
    if (gs_rparam_get(rad->node, 5, AX100_5_PLLRANG_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.pllrang, sizeof(rad->ptable5.pllrang)) != GS_OK)
        ErrCnt++;

    //Get mode
    if (gs_rparam_get(rad->node, 5, AX100_5_MODE_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.mode, sizeof(rad->ptable5.mode)) != GS_OK)
        ErrCnt++;

    //Get csp_hmac
    if (gs_rparam_get(rad->node, 5, AX100_5_CSP_HMAC_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.csp_hmac, sizeof(rad->ptable5.csp_hmac)) != GS_OK)
        ErrCnt++;

    //Get csp_rs
    if (gs_rparam_get(rad->node, 5, AX100_5_CSP_RS_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.csp_rs, sizeof(rad->ptable5.csp_rs)) != GS_OK)
        ErrCnt++;

    //Get csp_crc
    if (gs_rparam_get(rad->node, 5, AX100_5_CSP_CRC_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.csp_crc, sizeof(rad->ptable5.csp_crc)) != GS_OK)
        ErrCnt++;

    //Get csp_rand
    if (gs_rparam_get(rad->node, 5, AX100_5_CSP_RAND_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.csp_rand, sizeof(rad->ptable5.csp_rand)) != GS_OK)
        ErrCnt++;

    //Get csp_hmac_key

    //Get ax25_call
    if (gs_rparam_get(rad->node, 5, AX100_5_AX25_CALL_ADDR, GS_PARAM_STRING, AX100_CKSUM, AX100_LOCAL_TIMEOUT, rad->ptable5.ax25_call, sizeof(rad->ptable5.ax25_call)) != GS_OK)
        ErrCnt++;

    //Get preamblen
    if (gs_rparam_get(rad->node, 5, AX100_5_PREAMBLEN_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.preamblen, sizeof(rad->ptable5.preamblen)) != GS_OK)
        ErrCnt++;

    //Get preambflags
    if (gs_rparam_get(rad->node, 5, AX100_5_PREAMBFLAGS_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.preamblen, sizeof(rad->ptable5.preamblen)) != GS_OK)
        ErrCnt++;

    //Get intfrmlen
    if (gs_rparam_get(rad->node, 5, AX100_5_INTFRMLEN_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.preamblen, sizeof(rad->ptable5.preamblen)) != GS_OK)
        ErrCnt++;

    //Get intfrmflags
    if (gs_rparam_get(rad->node, 5, AX100_5_INTFRMFLAGS_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.intfrmlen, sizeof(rad->ptable5.intfrmlen)) != GS_OK)
        ErrCnt++;

    //Get rssibusy
    if (gs_rparam_get(rad->node, 5, AX100_5_RSSIBUSY_ADDR, GS_PARAM_INT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.intfrmflags, sizeof(rad->ptable5.intfrmflags)) != GS_OK)
        ErrCnt++;

    //Get kup_delay
    if (gs_rparam_get(rad->node, 5, AX100_5_KUP_DELAY_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.rssibusy, sizeof(rad->ptable5.rssibusy)) != GS_OK)
        ErrCnt++;

    //Get pa_level
    if (gs_rparam_get(rad->node, 5, AX100_5_PA_LEVEL_ADDR, GS_PARAM_INT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.pa_level, sizeof(rad->ptable5.pa_level)) != GS_OK)
        ErrCnt++;

    if (ErrCnt)
        rad->callback(SGS_CONMOD_ERR, "Unable to load %d parameters of Table 0.", ErrCnt);
    else
        rad->callback(SGS_CONMOD_OK, "Table 0 Load Success.");
    return NULL;
}

static void * Save_Paramtable0(void * param)
{
    SGS_RADIAL * rad = (SGS_RADIAL *)param;
    if(!rad)
        return NULL;
    int ErrCnt = 0;

    // rparam_set_quiet(RPARAM_NOT_QUIET);
    //set rssi_offset
    if (gs_rparam_set(rad->node, 0, AX100_0_RSSI_OFFSET_ADDR, GS_PARAM_INT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.rssi_offset, sizeof(rad->ptable0.rssi_offset)) != GS_OK)
        ErrCnt++;
    
    //set max_temp
    if (gs_rparam_set(rad->node, 0, AX100_0_MAX_TEMP_ADDR, GS_PARAM_INT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.max_temp, sizeof(rad->ptable0.max_temp)) != GS_OK)
        ErrCnt++;

    //set bgndrssi_ema
    if (gs_rparam_set(rad->node, 0, AX100_0_BGNDRSSL_EMA_ADDR, GS_PARAM_FLOAT, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.bgndrssl_ema, sizeof(rad->ptable0.bgndrssl_ema)) != GS_OK)
        ErrCnt++;

    //set csp_node
    if (gs_rparam_set(rad->node, 0, AX100_0_CSP_NODE_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.csp_node, sizeof(rad->ptable0.csp_node)) != GS_OK)
        ErrCnt++;

    //set i2c_en
    if (gs_rparam_set(rad->node, 0, AX100_0_I2C_EN_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.i2c_en, sizeof(rad->ptable0.i2c_en)) != GS_OK)
        ErrCnt++;

    //set can_en
    if (gs_rparam_set(rad->node, 0, AX100_0_CAN_EN_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.can_en, sizeof(rad->ptable0.can_en)) != GS_OK)
        ErrCnt++;

    //set extptt_en
    if (gs_rparam_set(rad->node, 0, AX100_0_EXTPTT_EN_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.extptt_en, sizeof(rad->ptable0.extptt_en)) != GS_OK)
        ErrCnt++;

    //set led_en
    if (gs_rparam_set(rad->node, 0, AX100_0_LED_EN_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.led_en, sizeof(rad->ptable0.led_en)) != GS_OK)
        ErrCnt++;
    
    //set kiss_usart
    if (gs_rparam_set(rad->node, 0, AX100_0_KISS_USART_ADDR, GS_PARAM_INT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.kiss_usart, sizeof(rad->ptable0.kiss_usart)) != GS_OK)
        ErrCnt++;
    
    //set gosh_usart
    if (gs_rparam_set(rad->node, 0, AX100_0_GOSH_USART_ADDR, GS_PARAM_INT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.gosh_usart, sizeof(rad->ptable0.gosh_usart)) != GS_OK)
        ErrCnt++;

    //set i2c_addr
    if (gs_rparam_set(rad->node, 0, AX100_0_I2C_ADDR_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.i2c_addr, sizeof(rad->ptable0.i2c_addr)) != GS_OK)
        ErrCnt++;

    //set i2c_khz
    if (gs_rparam_set(rad->node, 0, AX100_0_I2C_KHZ_ADDR, GS_PARAM_UINT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.i2c_khz, sizeof(rad->ptable0.i2c_khz)) != GS_OK)
        ErrCnt++;

    //set can_khz
    if (gs_rparam_set(rad->node, 0, AX100_0_CAN_KHZ_ADDR, GS_PARAM_UINT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.can_khz, sizeof(rad->ptable0.can_khz)) != GS_OK)
        ErrCnt++;

    //set reboot_in
    if (gs_rparam_set(rad->node, 0, AX100_0_REBOOT_IN_ADDR, GS_PARAM_UINT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.reboot_in, sizeof(rad->ptable0.reboot_in)) != GS_OK)
        ErrCnt++;

    //set tx_inhibit
    if (gs_rparam_set(rad->node, 0, AX100_0_TX_INHIBIT_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.tx_inhibit, sizeof(rad->ptable0.tx_inhibit)) != GS_OK)
        ErrCnt++;

    //set log_store
    if (gs_rparam_set(rad->node, 0, AX100_0_LOG_STORE_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.log_store, sizeof(rad->ptable0.log_store)) != GS_OK)
        ErrCnt++;

    //set tx_pwr
    if (gs_rparam_set(rad->node, 0, AX100_0_TX_PWR_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.tx_pwr, sizeof(rad->ptable0.tx_pwr)) != GS_OK)
        ErrCnt++;

    //set max_tx_time
    if (gs_rparam_set(rad->node, 0, AX100_0_MAX_TX_TIME_ADDR, GS_PARAM_UINT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.max_tx_time, sizeof(rad->ptable0.max_tx_time)) != GS_OK)
        ErrCnt++;

    //set max_idle_time
    if (gs_rparam_set(rad->node, 0, AX100_0_MAX_IDLE_TIME_ADDR, GS_PARAM_UINT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable0.max_idle_time, sizeof(rad->ptable0.max_idle_time)) != GS_OK)
        ErrCnt++;

    //set max_idle_time
    if (gs_rparam_set(rad->node, 0, AX100_0_CSP_RTABLE_ADDR, GS_PARAM_STRING, AX100_CKSUM, AX100_LOCAL_TIMEOUT, rad->ptable0.csp_rtable, sizeof(rad->ptable0.csp_rtable)) != GS_OK)
        ErrCnt++;

    if (ErrCnt)
        rad->callback(SGS_CONMOD_ERR, "Unable to load %d parameters of Table 0.", ErrCnt);
    else
        rad->callback(SGS_CONMOD_OK, "Table 0 Load Success.");
    return NULL;
}

static void * Save_Paramtable1(void * param)
{
    SGS_RADIAL * rad = (SGS_RADIAL *)param;
    if(!rad)
        return NULL;
    int ErrCnt = 0;

    //set freq
    if (gs_rparam_set(rad->node, 1, AX100_1_FREQ_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.freq, sizeof(rad->ptable1.freq)) != GS_OK)
        ErrCnt++;

    //set baud
    if (gs_rparam_set(rad->node, 1, AX100_1_BAUD_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.baud, sizeof(rad->ptable1.baud)) != GS_OK)
        ErrCnt++;

    //set modindex
    if (gs_rparam_set(rad->node, 1, AX100_1_MODINDEX_ADDR, GS_PARAM_FLOAT, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.modindex, sizeof(rad->ptable1.modindex)) != GS_OK)
        ErrCnt++;

    //set guard
    if (gs_rparam_set(rad->node, 1, AX100_1_GUARD_ADDR, GS_PARAM_UINT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.guard, sizeof(rad->ptable1.guard)) != GS_OK)
        ErrCnt++;

    //set pllrang
    if (gs_rparam_set(rad->node, 1, AX100_1_PLLRANG_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.pllrang, sizeof(rad->ptable1.pllrang)) != GS_OK)
        ErrCnt++;

    //set mode
    if (gs_rparam_set(rad->node, 1, AX100_1_MODE_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.mode, sizeof(rad->ptable1.mode)) != GS_OK)
        ErrCnt++;

    //set csp_hmac
    if (gs_rparam_set(rad->node, 1, AX100_1_CSP_HMAC_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.csp_hmac, sizeof(rad->ptable1.csp_hmac)) != GS_OK)
        ErrCnt++;

    //set csp_rs
    if (gs_rparam_set(rad->node, 1, AX100_1_CSP_RS_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.csp_rs, sizeof(rad->ptable1.csp_rs)) != GS_OK)
        ErrCnt++;

    //set csp_crc
    if (gs_rparam_set(rad->node, 1, AX100_1_CSP_CRC_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.csp_crc, sizeof(rad->ptable1.csp_crc)) != GS_OK)
        ErrCnt++;

    //set csp_rand
    if (gs_rparam_set(rad->node, 1, AX100_1_CSP_RAND_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.csp_rand, sizeof(rad->ptable1.csp_rand)) != GS_OK)
        ErrCnt++;

    //set csp_hmac_key

    //set ax25_call
    if (gs_rparam_set(rad->node, 1, AX100_1_AX25_CALL_ADDR, GS_PARAM_STRING, AX100_CKSUM, AX100_LOCAL_TIMEOUT, rad->ptable1.ax25_call, sizeof(rad->ptable1.ax25_call)) != GS_OK)
        ErrCnt++;

    //set bw
    if (gs_rparam_set(rad->node, 1, AX100_1_BW_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.bw, sizeof(rad->ptable1.bw)) != GS_OK)
        ErrCnt++;

    //set afcrange
    if (gs_rparam_set(rad->node, 1, AX100_1_AFCRANGE_ADDR, GS_PARAM_INT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable1.afcrange, sizeof(rad->ptable1.afcrange)) != GS_OK)
        ErrCnt++;
    
    if (ErrCnt)
        rad->callback(SGS_CONMOD_ERR, "Unable to load %d parameters of Table 0.", ErrCnt);
    else
        rad->callback(SGS_CONMOD_OK, "Table 0 Load Success.");
    return NULL;
}

static void * Save_Paramtable5(void * param)
{
    SGS_RADIAL * rad = (SGS_RADIAL *)param;
    if(!rad)
        return NULL;

    int ErrCnt = 0;

    //set freq
    if (gs_rparam_set(rad->node, 5, AX100_5_FREQ_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.freq, sizeof(rad->ptable5.freq)) != GS_OK)
        ErrCnt++;

    //set baud
    if (gs_rparam_set(rad->node, 5, AX100_5_BAUD_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.baud, sizeof(rad->ptable5.baud)) != GS_OK)
        ErrCnt++;

    //set modindex
    if (gs_rparam_set(rad->node, 5, AX100_5_MODINDEX_ADDR, GS_PARAM_FLOAT, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.modindex, sizeof(rad->ptable5.modindex)) != GS_OK)
        ErrCnt++;

    //set guard
    if (gs_rparam_set(rad->node, 5, AX100_5_GUARD_ADDR, GS_PARAM_UINT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.guard, sizeof(rad->ptable5.guard)) != GS_OK)
        ErrCnt++;

    //set pllrang
    if (gs_rparam_set(rad->node, 5, AX100_5_PLLRANG_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.pllrang, sizeof(rad->ptable5.pllrang)) != GS_OK)
        ErrCnt++;

    //set mode
    if (gs_rparam_set(rad->node, 5, AX100_5_MODE_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.mode, sizeof(rad->ptable5.mode)) != GS_OK)
        ErrCnt++;

    //set csp_hmac
    if (gs_rparam_set(rad->node, 5, AX100_5_CSP_HMAC_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.csp_hmac, sizeof(rad->ptable5.csp_hmac)) != GS_OK)
        ErrCnt++;

    //set csp_rs
    if (gs_rparam_set(rad->node, 5, AX100_5_CSP_RS_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.csp_rs, sizeof(rad->ptable5.csp_rs)) != GS_OK)
        ErrCnt++;

    //set csp_crc
    if (gs_rparam_set(rad->node, 5, AX100_5_CSP_CRC_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.csp_crc, sizeof(rad->ptable5.csp_crc)) != GS_OK)
        ErrCnt++;

    //set csp_rand
    if (gs_rparam_set(rad->node, 5, AX100_5_CSP_RAND_ADDR, GS_PARAM_BOOL, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.csp_rand, sizeof(rad->ptable5.csp_rand)) != GS_OK)
        ErrCnt++;

    //set csp_hmac_key

    //set ax25_call
    if (gs_rparam_set(rad->node, 5, AX100_5_AX25_CALL_ADDR, GS_PARAM_STRING, AX100_CKSUM, AX100_LOCAL_TIMEOUT, rad->ptable5.ax25_call, sizeof(rad->ptable5.ax25_call)) != GS_OK)
        ErrCnt++;

    //set preamblen
    if (gs_rparam_set(rad->node, 5, AX100_5_PREAMBLEN_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.preamblen, sizeof(rad->ptable5.preamblen)) != GS_OK)
        ErrCnt++;

    //set preambflags
    if (gs_rparam_set(rad->node, 5, AX100_5_PREAMBFLAGS_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.preambflags, sizeof(rad->ptable5.preambflags)) != GS_OK)
        ErrCnt++;

    //set intfrmlen
    if (gs_rparam_set(rad->node, 5, AX100_5_INTFRMLEN_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.intfrmlen, sizeof(rad->ptable5.intfrmlen)) != GS_OK)
        ErrCnt++;

    //set intfrmflags
    if (gs_rparam_set(rad->node, 5, AX100_5_INTFRMFLAGS_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.intfrmflags, sizeof(rad->ptable5.intfrmflags)) != GS_OK)
        ErrCnt++;

    //set rssibusy
    if (gs_rparam_set(rad->node, 5, AX100_5_RSSIBUSY_ADDR, GS_PARAM_INT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.rssibusy, sizeof(rad->ptable5.rssibusy)) != GS_OK)
        ErrCnt++;

    //set kup_delay
    if (gs_rparam_set(rad->node, 5, AX100_5_KUP_DELAY_ADDR, GS_PARAM_UINT8, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.kup_delay, sizeof(rad->ptable5.kup_delay)) != GS_OK)
        ErrCnt++;

    //set pa_level
    if (gs_rparam_set(rad->node, 5, AX100_5_PA_LEVEL_ADDR, GS_PARAM_INT16, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &rad->ptable5.pa_level, sizeof(rad->ptable5.pa_level)) != GS_OK)
        ErrCnt++;

    if (ErrCnt)
        rad->callback(SGS_CONMOD_ERR, "Unable to load %d parameters of Table 0.", ErrCnt);
    else
        rad->callback(SGS_CONMOD_OK, "Table 0 Load Success.");
    return NULL;
}

void SGS_RADIAL::Save2FRAM(int8_t table)
{
    uint8_t node = (uint8_t)this->node;
    uint8_t port = AX100_PORT_RPARAM;
    if(gs_rparam_save(node, AX100_LOCAL_TIMEOUT, table, table) == GS_OK);
    //set_guard_spec_micsec(this->ptable5.baud, this->ptable1.baud, this->ptable5.baud, this->ptable1.baud, rAX100_5_guard, rAX100_1_guard);
    return;
}

void SGS_RADIAL::SetRxFreq(uint32_t freq)
{
    //Set freq
    ptable1.freq = freq;
    gs_rparam_set(this->node, 1, AX100_1_FREQ_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &this->ptable1.freq, sizeof(uint32_t));
}

void SGS_RADIAL::SetTxFreq(uint32_t freq)
{
    //Set freq
    ptable5.freq = freq;
    gs_rparam_set(this->node, 5, AX100_5_FREQ_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &this->ptable5.freq, sizeof(uint32_t));
}

void SGS_RADIAL::SetRxBaud(uint32_t baud)
{
    //Set Baud
    gs_rparam_set(this->node, 1, AX100_1_BAUD_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &this->ptable1.baud, sizeof(uint32_t));
}

void SGS_RADIAL::SetTxBaud(uint32_t baud)
{
    //Set Baud
    gs_rparam_set(this->node, 5, AX100_5_BAUD_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &this->ptable5.baud, sizeof(uint32_t));
}

void SGS_RADIAL::UpdateFreq()
{
    gs_rparam_get(this->node, 1, AX100_1_FREQ_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &this->ptable1.freq, sizeof(uint32_t));
    gs_rparam_get(this->node, 5, AX100_5_FREQ_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &this->ptable5.freq, sizeof(uint32_t));
}

void SGS_RADIAL::UpdateBaud()
{
    gs_rparam_get(this->node, 1, AX100_1_BAUD_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &this->ptable1.baud, sizeof(uint32_t));
    gs_rparam_get(this->node, 5, AX100_5_BAUD_ADDR, GS_PARAM_UINT32, AX100_CKSUM, AX100_LOCAL_TIMEOUT, &this->ptable5.baud, sizeof(uint32_t));
}
