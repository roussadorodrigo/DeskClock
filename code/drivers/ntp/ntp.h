/**
 * @file ntp.h
 * @author Ricardo Duarte & Rodrigo Viegas
 * @brief Header file for the ntp module
 * @version 0.1
 * @date 2025-10-09
 */


#ifndef _NTP_H_
#define _NTP_H_

/*Defines*/
#define MAX_RETRY_NUMBER 3


/**
 * @name ntc_init
 * @brief Initializes the ntc module
 * @arg ssid - wifi network name
 *      password - wifi network password
 * @return void
 */
void ntc_init(const char* ssid, const char* password);


#endif