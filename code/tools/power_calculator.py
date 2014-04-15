#this script calculates the average current of a node which periodically wakes up and checks the channel for data
#typical application would be a battery powered actuator waiting for commands.


battery_capacity = 225e-3 #in Ah (225mAh is the capacity of a CR2032 cell)
wakeup_period = 1.0 #in s (we assume the active time is neglectable compared to the sleep time. thus wakeup_period = sleep_time)
message_length = 1 + 3 + 1 #message length in bytes (preamble, address, payload). should be kept as small as possible.
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
message_time = message_length * 8 / bitrate


#this current is drawn in lowest power mode. we assume that this is always present as it is neglectable compared to other modes comsumption
quiescent_current = uc_standby_c + rf_pwr_down_c



#the idea: the receiver (energy constrained) stays most of the time in sleep and wakes up shortly to check the channel for data
# if there is data for the node, it answers immediately and then further communication can take place.
# the assumption is, that new data for the node is a rare event.
# the transmitter needs to send out a comb of packets to wake up the receiver.
#transmitter operation:
# <- tx -><- rx -><- tx -><- rx ->...
# as receiver and transmitter are asynchronous, the receiver has to monitor the channel at least for the time of a <-tx-><-rx-><-tx-> triple plus some guard time to guarantee a full <-tx->capture

receive_time = (message_time+rf_stdby_to_rx_tx_t) * 3


#receiver operation:
#<- sleep -><- rx -><- sleep -><- rx ->...
#in more detail: <- sleep -><- uc wakeup -><- rf crystal startup -><- rx settling -><- active rx period -><- sleep ->...

#unit of this is Ah
active_charge_radio = rf_crystal_startup_1ms5_c * rf_pwr_down_to_stdby_t + rf_rx_avg_settling_130us_c * rf_stdby_to_rx_tx_t + receive_time * rf_rx_250kbps_c;
active_charge_uc = (uc_running_c * 0.1 + uc_sleep_c*0.9) * (rf_pwr_down_to_stdby_t + rf_stdby_to_rx_tx_t + receive_time) #the assumption is that during active phase, the uc is 10% of the time powered on and 90% in sleep mode

#in A
average_current = (active_charge_radio + active_charge_uc) / wakeup_period + quiescent_current

#in h
battery_lifetime = battery_capacity / average_current



print "Average current: %f mA\nBattery lifetime: %d Days (%.2f years)" % (average_current * 1e3, battery_lifetime / 24, battery_lifetime / 24 / 365)





