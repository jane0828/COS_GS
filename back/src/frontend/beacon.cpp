#ifndef _SGS_INCLUDE_H_
#include <sgs/include.h>
#include <sgs/global.h>
#endif


int BeaconHandler::Save2File()
{   
    if(autopilot->state)
        autopilot->BeaconCounter++;
    char beaconline[64];
    char beaconname[64];
    char binarybuf[1024];
    time_t tmtime = time(0);
    struct tm * local = localtime(&tmtime);
    sprintf(beaconname, "./data/beacon/Beacon--%04d-%02d-%02d-%02d-%02d-%02d--", local->tm_year+1900, local->tm_mon+1, local->tm_mday,local->tm_hour, local->tm_min, local->tm_sec);
    // sprintf(binarybuf, )
    FILE * beacon_fp;
    beacon_fp = fopen(beaconname, "wb");
    for(int i = 0 ; i < 5; i++)
        beaconline[i] = bec->CallSign[i];
    beaconline[6] = 0;
    // fprintf(beaconline);
    fprintf(beacon_fp, "Call Sign : %s\n", beaconline);
    fprintf(beacon_fp, "Message ID : 0x%X\n", csp_ntoh16(bec ->CCMessage_ID));
    fprintf(beacon_fp, "Sequence : 0x%X\n", csp_ntoh16(bec ->CCSequence));
    fprintf(beacon_fp, "Length : %" PRIu16 "\n", csp_ntoh16(bec ->CCLength) + 7);
    fprintf(beacon_fp, "Time Code : %X %X %X %X %X %X\n", bec ->CCTime_code[0], bec ->CCTime_code[1], bec ->CCTime_code[2], bec ->CCTime_code[3], bec ->CCTime_code[4], bec ->CCTime_code[5]);
    fprintf(beacon_fp, "Spacecraft Time : %" PRIu32 "\n", bec ->FMSpaceTime);
    fprintf(beacon_fp, "Reboot Count : %" PRIu16 "\n", bec ->FMRebootCount);
    fprintf(beacon_fp, "Current Mode : %" PRIu8 "\n", bec ->FMCurMode);
    fprintf(beacon_fp, "Current Submode : %" PRIu8 "\n", bec ->FmCurSubMode);
    fprintf(beacon_fp, "Previous Mode : %" PRIu8 "\n", bec ->FMPreviousMode);
    fprintf(beacon_fp, "Previous SubMode : %" PRIu8 "\n", bec ->FMPreviousSubMode);
    fprintf(beacon_fp, "Mode : %" PRIu8 "\n", bec ->SYMode);
    fprintf(beacon_fp, "Reset Cause : %" PRIu8 "\n", bec ->SYResetCause);
    fprintf(beacon_fp, "Reset Count : %" PRIu16 "\n", bec ->SYResetcount);
    fprintf(beacon_fp, "Battery Status : 0x%X\n", bec ->BAtteryStatus);
    fprintf(beacon_fp, "Battery IN Voltage : %" PRId16 "\n", bec ->BAtteryInV);
    fprintf(beacon_fp, "Baterry In Current : %" PRId16 "\n", bec ->BAtteryInI);
    fprintf(beacon_fp, "Battery Temperature : %" PRId16 "\n", bec ->BAtteryTemp);
    fprintf(beacon_fp, "Mboard MPPT In V : %" PRId16 " %" PRId16 " %" PRId16 "\n", bec ->MOMPPTInV[0], bec ->MOMPPTInV[1], bec ->MOMPPTInV[2]);
    fprintf(beacon_fp, "Mboard MPPT In I : %" PRId16 " %" PRId16 " %" PRId16 "\n", bec ->MOMPPTInI[0], bec ->MOMPPTInI[1], bec ->MOMPPTInI[2]);
    fprintf(beacon_fp, "Mboard Channel I 3V : %" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 "\n", bec ->MOChancur3V[0], bec ->MOChancur3V[1], bec ->MOChancur3V[2], bec ->MOChancur3V[3]);
    fprintf(beacon_fp, "Mboard Channel I 5V : %" PRId16 " %" PRId16 " %" PRId16 " %" PRId16 "\n", bec ->MOChancur5V[0], bec ->MOChancur5V[1], bec ->MOChancur5V[2], bec ->MOChancur5V[3]);
    fprintf(beacon_fp, "Mboard Supply V : %" PRId16 "\n", bec ->MOBoardSupV);
    fprintf(beacon_fp, "Mboard Temperature : %" PRId16 "\n", bec ->MOBoardTemp);
    fprintf(beacon_fp, "Dboard MPPT In V : %" PRId16 " %" PRId16 "\n", bec ->DAMPPTInV[0], bec ->DAMPPTInV[1]);
    fprintf(beacon_fp, "Dboard MPPT In I : %" PRId16 " %" PRId16 "\n", bec ->DAMPPTInI[0], bec ->DAMPPTInI[1]);
    fprintf(beacon_fp, "Dboard Channel I 5V : %" PRId16 "\n", bec ->DAChancur5V);
    fprintf(beacon_fp, "Dboard Channel I 12V : %" PRId16 "\n", bec ->DAChancur12V);
    fprintf(beacon_fp, "Temperature : %.2f\n", bec ->IMTemp);
    fprintf(beacon_fp, "Angular Velocity : %.2f %.2f %.2f\n", bec ->IMAngV[0], bec ->IMAngV[1], bec ->IMAngV[2]);
    fprintf(beacon_fp, "RW Error Count : %" PRIu16 "\n", bec ->IMRWErrcount);
    fprintf(beacon_fp, "Total RX Bytes : %" PRIu32 "\n", bec ->UXTotRXByte);
    fprintf(beacon_fp, "Boot Count : %" PRIu16 "\n", bec ->UXBootCount);
    fprintf(beacon_fp, "Active Conf : %" PRIu8 "\n", bec ->UXActiveConf);
    fprintf(beacon_fp, "\n\n\n\nBinary : \n");
    // fprintf(beacon_fp, )
    char BeaconBuf[sizeof(Beacon)];
    memcpy(BeaconBuf, bec, sizeof(Beacon));
    for(int i = 0; i < sizeof(BeaconBuf) ; i++)
        fprintf(beacon_fp, "%x\t", BeaconBuf[i]);
    fprintf(beacon_fp, "\n");
    fflush(beacon_fp);
    fclose(beacon_fp);
    return sizeof(this->bec);
    RSSI_Monitoring();
}

int BeaconHandler::Save2DB()
{
    return sizeof(this->bec);
}

int BeaconHandler::File2DB()
{

    /*-----------------------------------------------------------*/
/*---------------------Connect mysql--------------------*/

    MYSQL_RES *res;
    MYSQL_ROW row;
    MYSQL_FIELD *field_name;

    mysql_init(this->sql);

    if (!mysql_real_connect(this->sql,NULL,"root","qwe123",NULL,3306,(char*)NULL,0)) {
        printf("%s\n",mysql_error(this->sql));
        exit(1);
    }

    // later, rivise query sentence. "use {DB name}"
    if (mysql_query(this->sql,"use test_db")) {
        printf("%s\n",mysql_error(this->sql));
        exit(1);
    }

    /*--------------------Read beacon file----------------------*/
    FILE* bec_fp;
    char data[34][64];
    char str[1024];
    bec_fp = fopen(this->BeaconFileName, "r"); // modify beaconname later

    int cnt = 0;
        
    while (fgets(str, sizeof(str), bec_fp) != NULL) {
        //printf("%s\n", str);
        strcpy(data[cnt], str);
        cnt++;
    }

    fclose(bec_fp);

     /*------------------------Store data-----------------------------*/

    char *names[34]; //store field(=column) name  ex) Call sign 
    char *values[34]; //store data value
    int i;
    for (i = 0; i < 34; i++) {
        names[i] = strtok(data[i], ":");
        values[i] = strtok(NULL, ":");
    }

    /*-----------Making query sentence - for whole data except Binary.-----------*/         
    /* Mysql query Grammer : 
        insert into [table name](column1, column2...) values(value1, value2...) */
    
    // append integer data : 4,6,7,8,9,10,12,13,14,16,17,18,23,24,27,28,31,32,33,34
    char query1[1000];
    sprintf(query1,"insert into beacon(`Length`,`Spacecraft Time`,`Reboot Count`,`Current Mode`,`Current Submode`,`Previous Mode`,`Mode`,`Reset Cause`,`Reset Count`,`Battery IN Voltage`,`Battery In Current`,`Battery Temperature`,`Mboard Supply V`,`Mboard Temperature`,`Dboard Channel I 5V`,`Dboard Channel I 12V`,`RW Error Count`,`Total RX Bytes`,`Boot Count`,`Active Conf`) values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d)",
    atoi(values[3]),atoi(values[5]),atoi(values[6]),atoi(values[7]),atoi(values[8]),atoi(values[9]),
    atoi(values[11]),atoi(values[12]),atoi(values[13]),atoi(values[15]),atoi(values[16]),atoi(values[17]),
    atoi(values[22]),atoi(values[23]),atoi(values[26]),atoi(values[27]),atoi(values[30]),atoi(values[31]),
    atoi(values[32]),atoi(values[33]));
    // Get row number - used in 'update' query :query2,3,4,5
    if (mysql_query(this->sql,"select count(*) from beacon")) {
        printf("%s\n",mysql_error(this->sql));
        exit(1);
    }
    res=mysql_use_result(this->sql);
    row=mysql_fetch_row(res);
    printf("%d\n",atoi(row[0])); // row[0] is char. use atoi to convert int & used for loading check

    // append char type data : 1,11
    char query2[100];
    sprintf(query2,
    "update beacon set `Call Sign`='MIMAN',`Previous SubMode`= '%s' where No=%d",
    values[10],atoi(row[0]));

    // append hex type data : 2,3,15
    char query3[200];
    sprintf(query3,
    "update beacon set `Message ID` = %ld,`Sequence` = %ld,`Battery Status` = %ld where No=%d",
    strtol(values[1],NULL,16),strtol(values[2],NULL,16),strtol(values[14],NULL,16),atoi(row[0]));

    // append float & multi value data : 29 & 5, 19, 20, 21, 22, 25, 26, 30
    char query4[500];
    sprintf(query4,
    "update beacon set `Temperature`= %f, `Time Code`=\"%s\", `Mboard MPPT In V`=\"%s\", `Mboard MPPT In I`=\"%s\", `Mboard Channel I 3V`=\"%s\", `Mboard Channel I 5V`=\"%s\", `Dboard MPPT In V`=\"%s\", `Dboard MPPT In I`=\"%s\", `Angular Velocity`=\"%s\" where No=%d",
    strtof(values[28],NULL),values[4],values[18],values[19],values[20],values[21],values[24],values[25],values[29],atoi(row[0]));

    mysql_free_result(res);

    /*---------------------------Main query - insert data----------------------------*/

    // insert beacon data into table 'beacon_data'
    if (mysql_query(this->sql,query1)) {   
        printf("%s\n",mysql_error(this->sql));
        exit(1);
    }
    if (mysql_query(this->sql,query2)) {
        printf("%s\n", mysql_error(this->sql));
        exit(1);
    }
    if (mysql_query(this->sql,query3)) {
        printf("%s\n",mysql_error(this->sql));
        exit(1);
    }
    if (mysql_query(this->sql,query4)) {
        printf("%s\n",mysql_error(this->sql));
        exit(1);
    }

    mysql_close(this->sql);
/*-----------------------------------------------------------*/
    return sizeof(bec);
}