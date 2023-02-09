#include <stdio.h>
#include <ntcan.h>
#include <cstring>
#include <unistd.h>

/*
 * This example demonstrates how the NTCAN-API can be used to open a handle,
 * set a Classical CAN bitrate and transmit Classical CAN messages at a certain
 * point of time in the future using the Timestamped TX feature with a
 * non-blocking request. The example code acquires the current timestamp and
 * starts transmission of 10 frames after 1000 ms with a time difference of 100
 * ms between each frame. Finally all proper cleanup operations are performed
*/

void sched_test(uint16_t can_id, uint32_t time_interval_ms)
{
 NTCAN_HANDLE m_hCan;
 uint64_t timestampFreq;
 CMSG msg;
 CSCHED schedule;
 NTCAN_RESULT rc;
 /* Open CAN handle for net 42 */
 rc = canOpen(0, 0, 10, 10, 1000, 1000, &m_hCan);
 if (rc != NTCAN_SUCCESS) {
 return;
 }
 /* Request timestamp/tick frequency of interface */
 rc = canIoctl(m_hCan, NTCAN_IOCTL_GET_TIMESTAMP_FREQ, &timestampFreq);
 if (rc != NTCAN_SUCCESS) {
 printf("Gathering timestamp frequency failed with %d\n", rc);
 (void)canClose(m_hCan);
 return;
 }
 /* Set baudrate to 1 MBit/s */
 rc = canSetBaudrate(m_hCan, NTCAN_BAUD_1000);
 if (rc != NTCAN_SUCCESS) {
 printf("Configuration CAN bit rate failed with %d\n", rc);
 (void)canClose(m_hCan);
 return;
 }
 /*
 * Setup the Tx object with the given CAN-ID and initialize the message.
 */
 memset(&msg, 0, sizeof(CMSG));
 msg.id = (uint32_t)can_id;
 msg.len = 8;
 strcpy((char *)msg.data, "Hello");
 /*
 * Define a scheduling set for the given CAN-ID with a 16 bit
 * counter (little endian) at byte offset 6 which is incremented
 * with each transmission and counts from 0 up to 5 before it
 * start again with 0 with the given scheduling interval converted
 * from milliseconds into ticks.
 */
 memset(&schedule, 0, sizeof(CSCHED));
 schedule.id = (uint32_t)can_id;
 schedule.flags = NTCAN_SCHED_FLAG_EN | NTCAN_SCHED_FLAG_INC16 |
 NTCAN_SCHED_FLAG_OFS6;
 schedule.time_start = 0;
 schedule.time_interval = ((timestampFreq * time_interval_ms) / 1000ULL);
 schedule.count_start = 0x0;
 schedule.count_stop = 0x5;
 /* Create a Tx object */
 rc = canIoctl(m_hCan, NTCAN_IOCTL_TX_OBJ_CREATE, &msg);
 if (rc != NTCAN_SUCCESS) {
 printf("Creation of Tx object failed with error %d\n", rc);
 (void)canClose(m_hCan);
 return;
 }
/* Configure the scheduling for the Tx object */
 rc = canIoctl(m_hCan, NTCAN_IOCTL_TX_OBJ_SCHEDULE, &schedule);
 if (rc != NTCAN_SUCCESS) {
 printf("Configuration of scheduling failed with error %d\n", rc);
 (void)canClose(m_hCan);
 return;
 }
 /* Start the scheduling set */
 rc = canIoctl(m_hCan, NTCAN_IOCTL_TX_OBJ_SCHEDULE_START, NULL);
 if (rc != NTCAN_SUCCESS) {
 printf("Start of scheduling failed with error %d\n", rc);
 (void)canClose(m_hCan);
 return;
 }
 usleep(5000); /* OS specific delay for 5 seconds !!! */
 /* Update the Tx message */
 strcpy((char *)msg.data, "World");
 rc = canIoctl(m_hCan, NTCAN_IOCTL_TX_OBJ_UPDATE, &msg);
 if (rc != NTCAN_SUCCESS) {
 printf("Update of scheduling failed with error %d\n", rc);
 }
 usleep(5000); /* OS specific delay for 5 seconds !!! */
 /* Stop the scheduling */
 rc = canIoctl(m_hCan, NTCAN_IOCTL_TX_OBJ_SCHEDULE_STOP, NULL);
 if (rc != NTCAN_SUCCESS) {
 printf("Stop of scheduling failed with error %d\n", rc);
 }
 (void)canClose(m_hCan);
}

void timestamped_tx(uint16_t can_id)
{
 NTCAN_HANDLE m_hCan;
 uint64_t timestampFreq, timestamp;
 CMSG_T msgT[10];
 NTCAN_RESULT rc;
 int32_t len;
 int i;

 /* Open CAN handle for net 42 */
 rc = canOpen(42, NTCAN_MODE_TIMESTAMPED_TX, 100, 100, 1000, 1000, &m_hCan);
 if (rc != NTCAN_SUCCESS) {
 printf("Opening handle failed with %d\n", rc);
 return;
 }
 /* Request timestamp/tick frequency of interface */
 rc = canIoctl(m_hCan, NTCAN_IOCTL_GET_TIMESTAMP_FREQ, &timestampFreq);
 if (rc != NTCAN_SUCCESS) {
 printf("Gathering timestamp frequency failed with %d\n", rc);
 (void)canClose(m_hCan);
 return;
 }
 /* Set baudrate to 500KBit/s */
 rc = canSetBaudrate(m_hCan, NTCAN_BAUD_500);
 if (rc != NTCAN_SUCCESS) {
 printf("Configuration CAN bit rate failed with %d\n", rc);
 (void)canClose(m_hCan);
 return;
 }
 /* Request timestamp/tick frequency of interface */
 rc = canIoctl(m_hCan, NTCAN_IOCTL_GET_TIMESTAMP, &timestamp);
 if (rc != NTCAN_SUCCESS) {
 printf("Gathering timestamp failed with %d\n", rc);
 (void)canClose(m_hCan);
 return;
 }
 // Start transmission in one second from now
 timestamp += timestampFreq;
 /*
 * Setup the Tx object with the given CAN-ID and initialize the message.
 */
 memset(msgT, 0, sizeof(msgT));
 len = sizeof(msgT) / sizeof(*msgT);
 for (i = 0; i < len; i++) {
 msgT[i].id = (int32_t)(can_id + i);
 msgT[i].len = NTCAN_DATASIZE_TO_DLC(8);
 msgT[i].timestamp = timestamp;
 strcpy((char *)msgT[i].data, "Hello !!");
 timestamp += timestampFreq / 10; /* Next transmission in 100 ms */
 }

 /*
 * Non-blocking call to schedule message transmission.
 */
 rc = canSendT(m_hCan, msgT, &len);
 if (rc != NTCAN_SUCCESS) {
 printf("canSendT() failed with %d\n", rc);
 }
 /*
 * NOTE: All frames which are not transmitted before the handle is closed
 * will be aborted !!!
 */
 usleep(3000); /* OS specific delay for 3 seconds !!! */
 (void)canClose(m_hCan);
 return;
}

int main(void){
//timestamped_tx(32);

sched_test(23, 1000);
return 0;
}
