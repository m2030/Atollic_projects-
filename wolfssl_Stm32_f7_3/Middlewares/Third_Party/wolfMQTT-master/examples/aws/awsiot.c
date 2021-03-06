/* awsiot.c
 *
 * Copyright (C) 2006-2021 wolfSSL Inc.
 *
 * This file is part of wolfMQTT.
 *
 * wolfMQTT is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wolfMQTT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 */

/* Include the autoconf generated config.h */
#ifdef HAVE_CONFIG_H
    #include <config.h>
#endif
  #include <main.h>
#define ENABLE_AWSIOT_EXAMPLE

#include "wolfmqtt/mqtt_client.h"


/* This example only works with ENABLE_MQTT_TLS (wolfSSL library) */
/* Notes:
 *
 *  The "wc_GetTime" API was added in 3.9.1 and if not present you'll need to implement
 *  your own version of this to get current UTC seconds or update your wolfSSL library
*/

/* This example requires features in wolfSSL 3.9.1 or later */
#if defined(ENABLE_MQTT_TLS)
    #if !defined(WOLFSSL_USER_SETTINGS) && !defined(USE_WINDOWS_API)
        #include <wolfssl/options.h>
    #endif
    #include <wolfssl/wolfcrypt/settings.h>
    #include <wolfssl/version.h>

    #if defined(LIBWOLFSSL_VERSION_HEX) && \
        LIBWOLFSSL_VERSION_HEX >= 0x03009001
        #undef ENABLE_AWSIOT_EXAMPLE
        #define ENABLE_AWSIOT_EXAMPLE
    #endif
#endif


#ifdef ENABLE_AWSIOT_EXAMPLE

#include <wolfssl/ssl.h>
#include <wolfssl/wolfcrypt/asn_public.h>
#include <wolfssl/wolfcrypt/coding.h>
#include <wolfssl/wolfcrypt/hmac.h>

#include "awsiot.h"
#include "examples/mqttexample.h"
#include "examples/mqttnet.h"
#include <wolfmqtt/version.h>

/* Locals */
static int mStopRead = 0;
char buf[70];
extern UART_HandleTypeDef huart3;

/* Configuration */

//#define AWSIOT_PUBLISH_MSG_SZ   400

/* Demo Certificates */
static const char* root_ca =
"-----BEGIN CERTIFICATE-----\n"
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n"
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n"
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n"
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n"
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n"
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n"
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n"
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n"
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n"
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n"
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n"
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n"
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n"
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n"
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n"
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n"
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n"
"rqXRfboQnoZsG4q5WTP468SQvvG5\n"
"-----END CERTIFICATE-----";

#if 0
static const char* device_pub_key =
"-----BEGIN PUBLIC KEY-----\n"
"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAqsAKVhbfQEWblC8Pvgub\n"
"qpJasVoCEsSfvLF4b5DIAsoMeieP26y6Vyd3njRyuigSQ6jP+mo3GyqSfeCbqfJ2\n"
"dx3BNICEk7P46Bu37ewoI24pScnT+5Rcfw//dh8WHDq0d0933i4vOKEKA6ft51Ax\n"
"Y6KzvNrwxo6bml0Vi2DBx3WLGw+MDiHXk2L0geSYSNjFz/u9dkgEUPhEGbNZuw2y\n"
"xhwbNfaiPe2ld5Fir6iUybuj93xfWqqNls77V6Qj7mI8pamdGFtQnkP+6l2XTa6J\n"
"bunCqZo1PURtUXch5db6rMq/6rRZrlJM7NngPI1vv8jF4T3G4mjyT8I4KxQK1s90\n"
"9QIDAQAB\n"
"-----END PUBLIC KEY-----";
#endif

static const char* device_priv_key =
"-----BEGIN RSA PRIVATE KEY-----\n"
"MIIEpAIBAAKCAQEA6XNpsWVLaLRktfCj+WyRnID1BYzL+see1/ZZG3K0NJiNGcoZ\n"
"xy61DyA7lYvnatI1H+ptAP9o4UsiIfM/02M5JA4MoUT7HZGYppn7iR17p7GRLnRr\n"
"zGAnYAX9UBrD9HpDdxEm1E6uI7BFVGpUZyaoJQKTeAlVg2dxmzSetT8XC8F23RIo\n"
"dC9zbaCca/eJ8lW526UyIHqZzIfTUcuyHqBHA0EdMKseerALByOq1TuycUdaEfm1\n"
"KlRN31qMwZr/oqdOQPZhConhvHwNfby6Z3C3aXczWlYcXumjo5TnLzf0dK5CQRbW\n"
"xYkyZx8reFtet0Iw3e6gtfuGkPOUHJVH6rGgrQIDAQABAoIBAEa514iOBrWXCXfx\n"
"iswTL14514Gx8U5t0drd4IsnSh8xJHZuuzaUGPiXLFYXR6oeO0gLDrEhsBWpXJLq\n"
"jiitw+7/bqYhLu77YEMBHaqhywe4DSME1G/bpbI6vk7ZfQqBTBUzlg72M13OloKH\n"
"oECL/p8P07unIEgqO8novDhu/71ocEH1BoON6K3xSDo1GzBfVdTCboJC9Noj7Xjq\n"
"eUy2xP6060awKJhrN6GU1IU5ZGJx1KjD9tRk6mM3qwGeba6p/C/lBAVBkfYYiVQr\n"
"/S1QgDhXSRIzBXgjzITfL8Az4VoWwaTMQktgDJH+YZ+qCiyMtUP3er+K774lBjsX\n"
"lQHPmoUCgYEA+RlJdqO27BUJgA4umJ2MVvcR+rKhUDNhi+8wn8p27PRUwq7avy4+\n"
"GY3E9l1dshxRSFfaPnZiqQZ3e8TgYiPbmz4bOWeaxwKTaXQnzJQM1tUnKV/bdDld\n"
"DmBQ0Ee+y1eTtAd/YXOVCqLHX3yKjVthly5qDVSc2OkuZ305k0LBGncCgYEA7+sk\n"
"48gwEHhY768jcyZCnvz3VLbR4pqYsAsmZU8Ku8jjEgm0on3yEKqAQ+7p7syBuuUJ\n"
"iX85zHRKlc/n13aS7mMdvONKZnrNOUh/cahbGlGBSWh44b1hGQaEBSuvPdjHoM9q\n"
"Q/SqPNwyTrkSDEmR89dMK6BR4mN9RoLXG55GQvsCgYEA8ug5vzt+kVEiZoFt5t0b\n"
"kWJ1Yeb3b2Hraa3h2LBysDbCwOYJ4gyR+hFJVIVPbPQ4PzeMVwTSyMjJkyQ1Ov6E\n"
"lCpIGf5x7EcSwO7IMLLeolBRVANp2ThnCjHaHIEaCOhp3FDDYqB3YlvB19d2tMm7\n"
"rNVSDZXJLayV181Ba9zYuZkCgYAJeHPFVhKno8iG9ZflpL3+fqfYvTq9wjKCyH3k\n"
"g+E9lml4Pc/Wljgm9kQH6ndriMEaOo5M0iva6UBEd6eENtcH9/uliCdYskJFn5oA\n"
"R5AJEuySlEApUQtzLHGrrpTFUThYHbfpJNBPhLWrq9yNhZ3wTTBhjHuO/zRjwyhw\n"
"m6fUXwKBgQCrvjyXOanbh1XDCY6ZFZOg7qbg0zou67onmmnWlt++UI+uv+iWjoui\n"
"1kcsLGt9JtFjbjR7QXwlfd4Bg5BPo8Z4gK9+fF/wikdF0EFXC4JgP4gzkx0PEjhX\n"
"wIDP58KgE2UjX3j8uchscddhjL/OxMJZ9L/sJbCD/RuCE1MYQ/7/yw==\n"
"-----END RSA PRIVATE KEY-----";

static const char* device_cert =
"-----BEGIN CERTIFICATE-----\n"
"MIIDWjCCAkKgAwIBAgIVAKfeGqcV/0b+wajeBd3Fj3bOi+iHMA0GCSqGSIb3DQEB\n"
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n"
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMDA5MTAxNDEx\n"
"NDhaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n"
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDpc2mxZUtotGS18KP5\n"
"bJGcgPUFjMv6x57X9lkbcrQ0mI0ZyhnHLrUPIDuVi+dq0jUf6m0A/2jhSyIh8z/T\n"
"YzkkDgyhRPsdkZimmfuJHXunsZEudGvMYCdgBf1QGsP0ekN3ESbUTq4jsEVUalRn\n"
"JqglApN4CVWDZ3GbNJ61PxcLwXbdEih0L3NtoJxr94nyVbnbpTIgepnMh9NRy7Ie\n"
"oEcDQR0wqx56sAsHI6rVO7JxR1oR+bUqVE3fWozBmv+ip05A9mEKieG8fA19vLpn\n"
"cLdpdzNaVhxe6aOjlOcvN/R0rkJBFtbFiTJnHyt4W163QjDd7qC1+4aQ85QclUfq\n"
"saCtAgMBAAGjYDBeMB8GA1UdIwQYMBaAFAOfjIP1vbH3fx2BnF1ZrJFY17bVMB0G\n"
"A1UdDgQWBBTwW+n4EdrOttALQKsSLDeGOyxqnDAMBgNVHRMBAf8EAjAAMA4GA1Ud\n"
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAvAXIRCRN4MLQY4AxuTtWZHDL\n"
"Aa1L4iDbpY3sEBK3Y0B8ljlpiqKxHZ0WJcPipLy5LDlZu5y7J06ZBN8Gd+/5X27V\n"
"RpN0jklRQx+gCOIwjUR9S5iZY/PW8r2OM0l4p3b1h4CFf1QEpSTn9VQKrcmpWqFu\n"
"97VB37+W/RgmoI9Olx5bvufUJ4qPmGt9gHNEsHCCbT25s3Vg68wmLv0qmnxFUZxl\n"
"jORDq7Qw/2ifG0srNj/CIeB9dP1gEeaZqwKY0Cfk0W6JngVLqkKeEz0D/v7LhVwm\n"
"GBWqlC7BUGmhGVuTvtmu5rIzov7p6JPcvDQRUVMteYundiU4jzZrToPy15lwhw==\n"
"-----END CERTIFICATE-----";


static int mqtt_aws_tls_verify_cb(int preverify, WOLFSSL_X509_STORE_CTX* store)
{
    char buffer[WOLFSSL_MAX_ERROR_SZ];

    sprintf(buf,"MQTT TLS Verify Callback: PreVerify %d, Error %d (%s)", preverify,
        store->error, store->error != 0 ?
            wolfSSL_ERR_error_string(store->error, buffer) : "none");
    HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

    sprintf(buf,"  Subject's domain name is %s", store->domain);
    HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

    if (store->error != 0) {
        /* Allowing to continue */
        /* Should check certificate and return 0 if not okay */
        sprintf(buf,"  Allowing cert anyways");
        HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

    }

    return 1;
}

/* Use this callback to setup TLS certificates and verify callbacks */
static int mqtt_aws_tls_cb(MqttClient* client)
{
    int rc = WOLFSSL_FAILURE;

    client->tls.ctx = wolfSSL_CTX_new(wolfTLSv1_2_client_method());
    if (client->tls.ctx) {
        wolfSSL_CTX_set_verify(client->tls.ctx, WOLFSSL_VERIFY_PEER,
                               mqtt_aws_tls_verify_cb);

        /* Load CA certificate buffer */
        rc = wolfSSL_CTX_load_verify_buffer(client->tls.ctx,
            (const byte*)root_ca, (long)XSTRLEN(root_ca), WOLFSSL_FILETYPE_PEM);

        /* Load Client Cert */
        if (rc == WOLFSSL_SUCCESS)
            rc = wolfSSL_CTX_use_certificate_buffer(client->tls.ctx,
                (const byte*)device_cert, (long)XSTRLEN(device_cert),
                WOLFSSL_FILETYPE_PEM);

        /* Load Private Key */
        if (rc == WOLFSSL_SUCCESS)
            rc = wolfSSL_CTX_use_PrivateKey_buffer(client->tls.ctx,
                (const byte*)device_priv_key, (long)XSTRLEN(device_priv_key),
                WOLFSSL_FILETYPE_PEM);
    }

    sprintf(buf,"MQTT TLS Setup (%d)", rc);
    HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

    return rc;
}

static int mqtt_message_cb(MqttClient *client, MqttMessage *msg,
    byte msg_new, byte msg_done)
{
    MQTTCtx* mqttCtx = (MQTTCtx*)client->ctx;
    byte buf[PRINT_BUFFER_SIZE+1];
    word32 len;

    (void)mqttCtx;

    if (msg_new) {
        /* Determine min size to dump */
        len = msg->topic_name_len;
        if (len > PRINT_BUFFER_SIZE) {
            len = PRINT_BUFFER_SIZE;
        }
        XMEMCPY(buf, msg->topic_name, len);
        buf[len] = '\0'; /* Make sure its null terminated */

        /* Print incoming message */
        sprintf(buf,"MQTT Message: Topic %s, Qos %d, Len %u",
            buf, msg->qos, msg->total_len);
        HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

    }

    /* Print message payload */
    len = msg->buffer_len;
    if (len > PRINT_BUFFER_SIZE) {
        len = PRINT_BUFFER_SIZE;
    }
    XMEMCPY(buf, msg->buffer, len);
    buf[len] = '\0'; /* Make sure its null terminated */
    sprintf(buf,"Payload (%d - %d): %s",
        msg->buffer_pos, msg->buffer_pos + len, buf);
    HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

    if (msg_done) {
        sprintf(buf,"MQTT Message: Done");
        HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

    }

    /* Return negative to terminate publish processing */
    return MQTT_CODE_SUCCESS;
}

int awsiot_test(MQTTCtx *mqttCtx)
{
    int rc = MQTT_CODE_SUCCESS, i;

    switch (mqttCtx->stat)
    {
        case WMQ_BEGIN:
        {
            sprintf(buf,"AwsIoT Client: QoS %d, Use TLS %d", mqttCtx->qos, mqttCtx->use_tls);
            HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

            /* Aws IoT requires TLS */
            if (!mqttCtx->use_tls) {
                return MQTT_CODE_ERROR_BAD_ARG;
            }

            FALL_THROUGH;
        }

        case WMQ_NET_INIT:
        {
            mqttCtx->stat = WMQ_NET_INIT;

            /* Initialize Network */
            rc = MqttClientNet_Init(&mqttCtx->net, mqttCtx);
            if (rc == MQTT_CODE_CONTINUE) {
                return rc;
            }
            sprintf(buf,"MQTT Net Init: %s (%d)",
               MqttClient_ReturnCodeToString(rc), rc);
            HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

            if (rc != MQTT_CODE_SUCCESS) {
                goto exit;
            }

            /* setup tx/rx buffers */
            mqttCtx->tx_buf = (byte*)WOLFMQTT_MALLOC(MAX_BUFFER_SIZE);
            mqttCtx->rx_buf = (byte*)WOLFMQTT_MALLOC(MAX_BUFFER_SIZE);

            FALL_THROUGH;
        }

        case WMQ_INIT:
        {
            mqttCtx->stat = WMQ_INIT;

            /* Initialize MqttClient structure */
            rc = MqttClient_Init(&mqttCtx->client, &mqttCtx->net, mqtt_message_cb,
                mqttCtx->tx_buf, MAX_BUFFER_SIZE, mqttCtx->rx_buf, MAX_BUFFER_SIZE,
                mqttCtx->cmd_timeout_ms);
            if (rc == MQTT_CODE_CONTINUE) {
                return rc;
            }
            sprintf(buf,"MQTT Init: %s (%d)",
                MqttClient_ReturnCodeToString(rc), rc);
            HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

            if (rc != MQTT_CODE_SUCCESS) {
                goto exit;
            }
            mqttCtx->client.ctx = mqttCtx;

        #ifdef WOLFMQTT_V5
            /* AWS broker only supports v3.1.1 client */
            mqttCtx->client.protocol_level = MQTT_CONNECT_PROTOCOL_LEVEL_4;
        #endif


            FALL_THROUGH;
        }

        case WMQ_TCP_CONN:
        {
            mqttCtx->stat = WMQ_TCP_CONN;

            /* Connect to broker */
            rc = MqttClient_NetConnect(&mqttCtx->client, mqttCtx->host, mqttCtx->port,
                DEFAULT_CON_TIMEOUT_MS, mqttCtx->use_tls, mqtt_aws_tls_cb);
            if (rc == MQTT_CODE_CONTINUE) {
                return rc;
            }
            sprintf(buf,"MQTT Socket Connect: %s (%d)",
                MqttClient_ReturnCodeToString(rc), rc);
            HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

            if (rc != MQTT_CODE_SUCCESS) {
                goto exit;
            }

            /* Build connect packet */
            XMEMSET(&mqttCtx->connect, 0, sizeof(MqttConnect));
            mqttCtx->connect.keep_alive_sec = mqttCtx->keep_alive_sec;
            mqttCtx->connect.clean_session = mqttCtx->clean_session;
            mqttCtx->connect.client_id = mqttCtx->client_id;

            /* Last will and testament sent by broker to subscribers
                of topic when broker connection is lost */
            XMEMSET(&mqttCtx->lwt_msg, 0, sizeof(mqttCtx->lwt_msg));
            mqttCtx->connect.lwt_msg = &mqttCtx->lwt_msg;
            mqttCtx->connect.enable_lwt = mqttCtx->enable_lwt;
            if (mqttCtx->enable_lwt) {
                /* Send client id in LWT payload */
                mqttCtx->lwt_msg.qos = mqttCtx->qos;
                mqttCtx->lwt_msg.retain = 0;
                mqttCtx->lwt_msg.topic_name = AWSIOT_PUBLISH_TOPIC"lwt";
                mqttCtx->lwt_msg.buffer = (byte*)mqttCtx->client_id;
                mqttCtx->lwt_msg.total_len = (word16)XSTRLEN(mqttCtx->client_id);
            }
            /* Optional authentication */
            mqttCtx->connect.username = mqttCtx->username;
            mqttCtx->connect.password = mqttCtx->password;

            FALL_THROUGH;
        }

        case WMQ_MQTT_CONN:
        {
            mqttCtx->stat = WMQ_MQTT_CONN;

            /* Send Connect and wait for Connect Ack */
            rc = MqttClient_Connect(&mqttCtx->client, &mqttCtx->connect);
            if (rc == MQTT_CODE_CONTINUE) {
                return rc;
            }
            sprintf(buf,"MQTT Connect: Proto (%s), %s (%d)",
                MqttClient_GetProtocolVersionString(&mqttCtx->client),
                MqttClient_ReturnCodeToString(rc), rc);
            HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

            if (rc != MQTT_CODE_SUCCESS) {
                goto disconn;
            }

            /* Validate Connect Ack info */
            sprintf(buf,"MQTT Connect Ack: Return Code %u, Session Present %d",
                mqttCtx->connect.ack.return_code,
                (mqttCtx->connect.ack.flags &
                    MQTT_CONNECT_ACK_FLAG_SESSION_PRESENT) ?
                    1 : 0
            );
            HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

            /* Build list of topics */
            mqttCtx->topics[0].topic_filter = mqttCtx->topic_name;
            mqttCtx->topics[0].qos = mqttCtx->qos;

            /* Subscribe Topic */
            XMEMSET(&mqttCtx->subscribe, 0, sizeof(MqttSubscribe));
            mqttCtx->subscribe.packet_id = mqtt_get_packetid();
            mqttCtx->subscribe.topic_count = sizeof(mqttCtx->topics)/sizeof(MqttTopic);
            mqttCtx->subscribe.topics = mqttCtx->topics;

            FALL_THROUGH;
        }

        case WMQ_SUB:
        {
            mqttCtx->stat = WMQ_SUB;

            rc = MqttClient_Subscribe(&mqttCtx->client, &mqttCtx->subscribe);
            if (rc == MQTT_CODE_CONTINUE) {
                return rc;
            }
            sprintf(buf,"MQTT Subscribe: %s (%d)",
                MqttClient_ReturnCodeToString(rc), rc);
            HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

            if (rc != MQTT_CODE_SUCCESS) {
                goto disconn;
            }

            /* show subscribe results */
            for (i = 0; i < mqttCtx->subscribe.topic_count; i++) {
                MqttTopic *topic = &mqttCtx->subscribe.topics[i];
                sprintf(buf,"  Topic %s, Qos %u, Return Code %u",
                    topic->topic_filter,
                    topic->qos, topic->return_code);
                HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

            }

            /* Publish Topic */
            XSNPRINTF((char*)mqttCtx->app_ctx, AWSIOT_PUBLISH_MSG_SZ,
                "{\"state\":{\"reported\":{\"hardware\":{\"type\":\"%s\",\"firmware_version\":\"%s\"}}}}",
                APP_HARDWARE, APP_FIRMWARE_VERSION);

            XMEMSET(&mqttCtx->publish, 0, sizeof(MqttPublish));
            mqttCtx->publish.retain = 0;
            mqttCtx->publish.qos = mqttCtx->qos;
            mqttCtx->publish.duplicate = 0;
            mqttCtx->publish.topic_name = AWSIOT_PUBLISH_TOPIC;
            mqttCtx->publish.packet_id = mqtt_get_packetid();
            mqttCtx->publish.buffer = (byte*)mqttCtx->app_ctx;
            mqttCtx->publish.total_len = (word32)XSTRLEN((char*)mqttCtx->app_ctx);

            FALL_THROUGH;
        }

        case WMQ_PUB:
        {
            mqttCtx->stat = WMQ_PUB;

            rc = MqttClient_Publish(&mqttCtx->client, &mqttCtx->publish);
            if (rc == MQTT_CODE_CONTINUE) {
                return rc;
            }
            sprintf(buf,"MQTT Publish: Topic %s, %s (%d)",
                mqttCtx->publish.topic_name, MqttClient_ReturnCodeToString(rc), rc);
            HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

            if (rc != MQTT_CODE_SUCCESS) {
                goto disconn;
            }

            /* Read Loop */
            sprintf(buf,"MQTT Waiting for message...");
            HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);


            FALL_THROUGH;
        }

        case WMQ_WAIT_MSG:
        {
            mqttCtx->stat = WMQ_WAIT_MSG;

            do {
                /* check for test mode or stop */
                if (mStopRead || mqttCtx->test_mode) {
                    rc = MQTT_CODE_SUCCESS;
                    sprintf(buf,"MQTT Exiting...");
                    HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

                    break;
                }

                /* Try and read packet */
                rc = MqttClient_WaitMessage(&mqttCtx->client, mqttCtx->cmd_timeout_ms);

            #ifdef WOLFMQTT_NONBLOCK
                /* Track elapsed time with no activity and trigger timeout */
                rc = mqtt_check_timeout(rc, &mqttCtx->start_sec,
                    mqttCtx->cmd_timeout_ms/1000);
            #endif

                /* check return code */
                if (rc == MQTT_CODE_CONTINUE) {
                    return rc;
                }
            #ifdef WOLFMQTT_ENABLE_STDIN_CAP
                else if (rc == MQTT_CODE_STDIN_WAKE) {
                    /* Get data from STDIO */
                    XMEMSET(mqttCtx->rx_buf, 0, MAX_BUFFER_SIZE);
                    if (XFGETS((char*)mqttCtx->rx_buf, MAX_BUFFER_SIZE - 1, stdin) != NULL) {
                        /* rc = (int)XSTRLEN((char*)mqttCtx->rx_buf); */

                        /* Publish Topic */
                        XSNPRINTF((char*)mqttCtx->app_ctx, AWSIOT_PUBLISH_MSG_SZ,
                            "{\"state\":{\"reported\":{\"msg\":\"%s\"}}}",
                            mqttCtx->rx_buf);
                        mqttCtx->stat = WMQ_PUB;
                        XMEMSET(&mqttCtx->publish, 0, sizeof(MqttPublish));
                        mqttCtx->publish.retain = 0;
                        mqttCtx->publish.qos = mqttCtx->qos;
                        mqttCtx->publish.duplicate = 0;
                        mqttCtx->publish.topic_name = AWSIOT_PUBLISH_TOPIC;
                        mqttCtx->publish.packet_id = mqtt_get_packetid();
                        mqttCtx->publish.buffer = (byte*)mqttCtx->app_ctx;
                        mqttCtx->publish.total_len = (word32)XSTRLEN((char*)mqttCtx->app_ctx);
                        rc = MqttClient_Publish(&mqttCtx->client, &mqttCtx->publish);
                        PRINTF("MQTT Publish: Topic %s, %s (%d)",
                            mqttCtx->publish.topic_name,
                            MqttClient_ReturnCodeToString(rc), rc);
                    }
                }
            #endif
                else if (rc == MQTT_CODE_ERROR_TIMEOUT) {
                    /* Keep Alive */
                    sprintf(buf,"Keep-alive timeout, sending ping");
                    HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

                    rc = MqttClient_Ping_ex(&mqttCtx->client, &mqttCtx->ping);
                    if (rc == MQTT_CODE_CONTINUE) {
                        return rc;
                    }
                    else if (rc != MQTT_CODE_SUCCESS) {
                        sprintf(buf,"MQTT Ping Keep Alive Error: %s (%d)",
                            MqttClient_ReturnCodeToString(rc), rc);
                        HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

                        break;
                    }
                }
                else if (rc != MQTT_CODE_SUCCESS) {
                    /* There was an error */
                    sprintf(buf,"MQTT Message Wait: %s (%d)",
                        MqttClient_ReturnCodeToString(rc), rc);
                    HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

                    break;
                }
            } while (1);

            /* Check for error */
            if (rc != MQTT_CODE_SUCCESS) {
                goto disconn;
            }

            FALL_THROUGH;
        }

        case WMQ_DISCONNECT:
        {
            /* Disconnect */
            rc = MqttClient_Disconnect(&mqttCtx->client);
            if (rc == MQTT_CODE_CONTINUE) {
                return rc;
            }
            sprintf(buf,"MQTT Disconnect: %s (%d)",
                MqttClient_ReturnCodeToString(rc), rc);
            HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);

            if (rc != MQTT_CODE_SUCCESS) {
                goto disconn;
            }

            FALL_THROUGH;
        }

        case WMQ_NET_DISCONNECT:
        {
            mqttCtx->stat = WMQ_NET_DISCONNECT;

            rc = MqttClient_NetDisconnect(&mqttCtx->client);
            if (rc == MQTT_CODE_CONTINUE) {
                return rc;
            }
            sprintf(buf,"MQTT Socket Disconnect: %s (%d)",
                MqttClient_ReturnCodeToString(rc), rc);
            HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 0xFFFF);


            FALL_THROUGH;
        }

        case WMQ_DONE:
        {
            mqttCtx->stat = WMQ_DONE;
            rc = mqttCtx->return_code;
            goto exit;
        }

        case WMQ_UNSUB: /* not used */
        default:
            rc = MQTT_CODE_ERROR_STAT;
            goto exit;
    } /* switch */

disconn:
    mqttCtx->stat = WMQ_NET_DISCONNECT;
    mqttCtx->return_code = rc;
    rc = MQTT_CODE_CONTINUE;

exit:

    if (rc != MQTT_CODE_CONTINUE) {
        /* Free resources */
        if (mqttCtx->tx_buf) WOLFMQTT_FREE(mqttCtx->tx_buf);
        if (mqttCtx->rx_buf) WOLFMQTT_FREE(mqttCtx->rx_buf);

        /* Cleanup network */
        MqttClientNet_DeInit(&mqttCtx->net);

        MqttClient_DeInit(&mqttCtx->client);
    }

    return rc;
}
#endif /* ENABLE_AWSIOT_EXAMPLE */


/* so overall tests can pull in test function */
#ifndef NO_MAIN_DRIVER
    #ifdef USE_WINDOWS_API
        #include <windows.h> /* for ctrl handler */

        static BOOL CtrlHandler(DWORD fdwCtrlType)
        {
            if (fdwCtrlType == CTRL_C_EVENT) {
            #ifdef ENABLE_AWSIOT_EXAMPLE
                mStopRead = 1;
            #endif
                PRINTF("Received Ctrl+c");
                return TRUE;
            }
            return FALSE;
        }
    #elif HAVE_SIGNAL
        #include <signal.h>
        static void sig_handler(int signo)
        {
            if (signo == SIGINT) {
            #ifdef ENABLE_AWSIOT_EXAMPLE
                mStopRead = 1;
            #endif
                PRINTF("Received SIGINT");
            }
        }
    #endif



  int main(int argc, char** argv)
    {
        int rc;
    #ifdef ENABLE_AWSIOT_EXAMPLE
        MQTTCtx mqttCtx;
        char pubMsg[AWSIOT_PUBLISH_MSG_SZ] = {0};

        mqtt_init_ctx(&mqttCtx);
        mqttCtx.app_name = "awsiot";
        mqttCtx.host = AWSIOT_HOST;
        mqttCtx.qos = AWSIOT_QOS;
        mqttCtx.keep_alive_sec = AWSIOT_KEEP_ALIVE_SEC;
        mqttCtx.client_id = AWSIOT_DEVICE_ID;
        mqttCtx.topic_name = AWSIOT_SUBSCRIBE_TOPIC;
        mqttCtx.cmd_timeout_ms = AWSIOT_CMD_TIMEOUT_MS;
        mqttCtx.use_tls = 1;
        mqttCtx.app_ctx = pubMsg;


        rc = mqtt_parse_args(&mqttCtx, argc, argv);
        if (rc != 0) {
            return rc;
        }
    #endif

    #ifdef USE_WINDOWS_API
        if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE) == FALSE) {
            PRINTF("Error setting Ctrl Handler! Error %d", (int)GetLastError());
        }
    #elif HAVE_SIGNAL
        if (signal(SIGINT, sig_handler) == SIG_ERR) {
            PRINTF("Can't catch SIGINT");
        }
    #endif

    #ifdef ENABLE_AWSIOT_EXAMPLE
        do {
            rc = awsiot_test(&mqttCtx);
        } while (rc == MQTT_CODE_CONTINUE);

        mqtt_free_ctx(&mqttCtx);
    #else
        (void)argc;
        (void)argv;

        PRINTF("Example not compiled in!");
        rc = 0;
    #endif

        return (rc == 0) ? 0 : EXIT_FAILURE;
    }

#endif /* NO_MAIN_DRIVER */
