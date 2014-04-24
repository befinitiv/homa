#this script calculates the average current of a node which periodically wakes up, sends a beacon and listens for a reply 
#typical application would be a battery powered actuator waiting for commands.

# the idea is that the beacon is used for a central system to synchronize with the node.
# this allows the central system to determine the point of time where the node is in receive mode.


#basic operation:
# - uc wakes up
# - radio is initialized
# - beacon is sent out
# - a specific time is being waited for the other side to prepare the command packet
# - radio goes into receive mode and listens for a command packet
# - after the command has been received, further communication can be established

#packet format:
# beacon:		<preamble><broadcast addr><node id><crc>
# command:	<preable><node addr><command><crc>
#
# node id is 1 byte (0x03 for example)
# command is 1 byte
# broadcast address is 3 bytes (0xA7A7A7 for example)
# node address is 3 bytes, from which the first two bytes are fixed and the third equals the node id (0xA7A703 for example)



battery_capacity = 225e-3 #in Ah (225mAh is the capacity of a CR2032 cell)
wakeup_period = 1.0 #in s (we assume the active time is neglectable compared to the sleep time. thus wakeup_period = sleep_time)
beacon_message_length = 1 + 3 + 1 + 1 #message (from node to central) length in bytes (preamble, broadcast address, node id, crc). should be kept as small as possible.
command_message_length = 1 + 3 + 1 + 1 #message length (answer from central) in bytes (preamble, dest address, command, crc). should be kept as small as possible.
bitrate = 250.0e3


#currents
uc_standby_c = 3.0e-6 #LSI enabled (LSE would be 2.7e-6)
uc_running_c = 4.4e-3 #HSI 8MHz flash execution typical value
uc_sleep_c = 1.5e-3 #HSI, most peripherals turned off

rf_pwr_down_c = 0.9e-6
rf_stdby1_c = 26.0e-6
rf_stdby2_c = 320.0e-6
rf_crystal_startup_1ms5_c = 400.0e-6 #oscillator startup current
rf_rx_250kbps_c = 12.6e-3 #rx current
rf_tx_c = 11.3e-3 #transmit current at 0dBm
rf_rx_avg_settling_130us_c = 8.9e-3 #average current during rx startup and change from tx to rx

#radio transition times
rf_pwr_down_to_stdby_t = 1.5e-3
rf_stdby_to_rx_tx_t = 130.0e-6



#----------------------------





#how much time needs the message on the air
beacon_message_time = beacon_message_length * 8 / bitrate
command_message_time = command_message_length * 8 / bitrate


#this current is drawn in lowest power mode. we assume that this is always present as it is neglectable compared to other modes comsumption
quiescent_current = uc_standby_c + rf_pwr_down_c



transmit_time = (beacon_message_time)
pause_time = 100e-6
receive_time = (command_message_time) * 1.1 #10% guard time for jitter


#unit of this is Ah
radio_mode_change_charge = rf_rx_avg_settling_130us_c * rf_stdby_to_rx_tx_t
active_charge_radio = rf_crystal_startup_1ms5_c * rf_pwr_down_to_stdby_t + radio_mode_change_charge + transmit_time * rf_tx_c + pause_time * rf_stdby1_c + radio_mode_change_charge + receive_time * rf_rx_250kbps_c;
active_charge_uc = (uc_running_c * 0.1 + uc_sleep_c*0.9) * (rf_pwr_down_to_stdby_t + rf_stdby_to_rx_tx_t*2 + transmit_time + pause_time + receive_time) #the assumption is that during active phase, the uc is 10% of the time powered on and 90% in sleep mode

#in A
average_current = (active_charge_radio + active_charge_uc) / wakeup_period + quiescent_current

#in h
battery_lifetime = battery_capacity / average_current




print "Average current: %f mA\nBattery lifetime: %d Days (%.2f years)" % (average_current * 1e3, battery_lifetime / 24, battery_lifetime / 24 / 365)





